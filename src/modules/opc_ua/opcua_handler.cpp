/*******************************************************************************
 * Copyright (c) 2015-2016 Florian Froschermeier <florian.froschermeier@tum.de>,
 * 							fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Florian Froschermeier
 *      - initial integration of the OPC-UA protocol
 *    Stefan Profanter
 *      - refactoring and adaption to new concept
 *******************************************************************************/

#include "opcua_handler.h"
#include <devexec.h>
#include "criticalregion.h"
#include <forte_printer.h>


#ifndef FORTE_COM_OPC_UA_CUSTOM_HOSTNAME
#include <sockhand.h>
#endif

using namespace forte::com_infra;

DEFINE_SINGLETON(COPC_UA_Handler);

const char *LogsLevelNames[6] = {"trace", "debug", "info", "warning", "error", "fatal"};
const char *LogsCategoryNames[6] = {"network", "channel", "session", "server", "client", "userland"};

struct UA_ClientEndpointMap {
	UA_Client *client;
	char *endpointUrl;
	CSyncObject *clientMutex;
};

void UA_Log_Forte(UA_LogLevel level, UA_LogCategory category, const char *msg, va_list args) {
	char tmpStr[400];
	forte_snprintf(tmpStr, 400, "[OPC UA] %s/%s\t", LogsLevelNames[level], LogsCategoryNames[category]);
	char *start = &tmpStr[strlen(tmpStr)];

	vsprintf(start, msg, args);

	size_t len = strlen(tmpStr);
	tmpStr[len] = '\n';
	tmpStr[len + 1] = '\0';

	switch (level) {
		case UA_LOGLEVEL_TRACE:
		case UA_LOGLEVEL_DEBUG:
			DEVLOG_DEBUG(tmpStr);
			break;
		case UA_LOGLEVEL_INFO:
			DEVLOG_INFO(tmpStr);
			break;
		case UA_LOGLEVEL_WARNING:
			DEVLOG_WARNING(tmpStr);
			break;
		case UA_LOGLEVEL_ERROR:
		case UA_LOGLEVEL_FATAL:
			DEVLOG_ERROR(tmpStr);
			break;
	}
}

void COPC_UA_Handler::configureUAServer(TForteUInt16 UAServerPort) {

	char name[255];
	forte_snprintf(name, 255, "forte_%d", FORTE_COM_OPC_UA_PORT);

	uaServerConfig = UA_ServerConfig_new_minimal(UAServerPort, NULL);
	uaServerConfig->logger = UA_Log_Forte;

# ifdef FORTE_COM_OPC_UA_MULTICAST
	uaServerConfig->applicationDescription.applicationType = UA_APPLICATIONTYPE_DISCOVERYSERVER;
	// hostname will be added by mdns library
	UA_String_deleteMembers(&uaServerConfig->mdnsServerName);
	uaServerConfig->mdnsServerName = UA_String_fromChars(name);
# endif

	char hostname[256];
#ifdef FORTE_COM_OPC_UA_CUSTOM_HOSTNAME
	// cppcheck-suppress ConfigurationNotChecked
	forte_snprintf(hostname, 255, "%s-%s", FORTE_COM_OPC_UA_CUSTOM_HOSTNAME, name);
#else
	if(gethostname(hostname, 255) == 0) {
		size_t offset = strlen(hostname);
		size_t nameLen = strlen(name);
		if (offset + nameLen +1 > 255) {
			offset = MAX(255-nameLen-1, (size_t)0);
		}
		forte_snprintf(hostname+offset, 255-offset, "-%s", name);
	}
#endif

	char uri[255];
	forte_snprintf(uri, 255, "org.eclipse.4diac.%s", hostname);

	// delete pre-initialized values
	UA_String_deleteMembers(&uaServerConfig->applicationDescription.applicationUri);
	UA_LocalizedText_deleteMembers(&uaServerConfig->applicationDescription.applicationName);

	uaServerConfig->applicationDescription.applicationUri = UA_String_fromChars(uri);
	uaServerConfig->applicationDescription.applicationName.locale = UA_STRING_NULL;
	uaServerConfig->applicationDescription.applicationName.text = UA_String_fromChars(hostname);

	for (size_t i=0; i<uaServerConfig->endpointsSize; i++) {
		UA_String_deleteMembers(&uaServerConfig->endpoints[i].endpointDescription.server.applicationUri);
		UA_LocalizedText_deleteMembers(&uaServerConfig->endpoints[i].endpointDescription.server.applicationName);

		UA_String_copy(&uaServerConfig->applicationDescription.applicationUri,
					   &uaServerConfig->endpoints[i].endpointDescription.server.applicationUri);

		UA_LocalizedText_copy(&uaServerConfig->applicationDescription.applicationName,
							  &uaServerConfig->endpoints[i].endpointDescription.server.applicationName);
	}

	// TODO set server capabilities
	// See http://www.opcfoundation.org/UA/schemas/1.03/ServerCapabilities.csv
	//config.serverCapabilitiesSize = 1;
	//UA_String caps = UA_String_fromChars("LDS");
	//config.serverCapabilities = &caps;
}

#ifdef FORTE_COM_OPC_UA_MULTICAST

static void serverOnNetworkCallback(const UA_ServerOnNetwork *serverOnNetwork, UA_Boolean isServerAnnounce, UA_Boolean isTxtReceived, void* data) {
	COPC_UA_Handler* handler = static_cast<COPC_UA_Handler*>(data);

	const UA_String ownDiscoverUrl = handler->getDiscoveryUrl();

	if (UA_String_equal(&serverOnNetwork->discoveryUrl, &ownDiscoverUrl)) {
		// skip self
		return;
	}

	if (!isTxtReceived)
		return; // we wait until the corresponding TXT record is announced.

	DEVLOG_DEBUG("OPC UA: mDNS %s '%.*s' with url '%.*s'\n",isServerAnnounce?"announce":"remove", serverOnNetwork->serverName.length, serverOnNetwork->serverName.data,
			serverOnNetwork->discoveryUrl.length, serverOnNetwork->discoveryUrl.data);

	// check if server is LDS, and then register
	UA_String ldsStr = UA_String_fromChars("LDS");
	for (unsigned int i=0; i<serverOnNetwork->serverCapabilitiesSize; i++) {
		if (UA_String_equal(&serverOnNetwork->serverCapabilities[i], &ldsStr)) {
			if (isServerAnnounce)
				handler->registerWithLds(&serverOnNetwork->discoveryUrl);
			else
				handler->removeLdsRegister(&serverOnNetwork->discoveryUrl);
			break;
		}
	}
	UA_String_deleteMembers(&ldsStr);
}

void COPC_UA_Handler::registerWithLds(const UA_String *discoveryUrl) {
	// check if already registered with the given LDS
	for (CSinglyLinkedList<const char*>::Iterator iter = registeredWithLds.begin(); iter != registeredWithLds.end(); ++iter) {
		if (strncmp((char*)discoveryUrl->data, *iter, discoveryUrl->length) == 0)
			return;
	}

	// will be freed when removed from list
	char *discoveryUrlChar = (char*)malloc(sizeof(char)*discoveryUrl->length+1);
	memcpy(discoveryUrlChar, discoveryUrl->data, discoveryUrl->length);
	discoveryUrlChar[discoveryUrl->length] = 0;

	registeredWithLds.push_front(discoveryUrlChar);

	DEVLOG_INFO("OPC UA: Registering with LDS '%.*s'\n", discoveryUrl->length, discoveryUrl->data);
	UA_StatusCode retVal = UA_Server_addPeriodicServerRegisterCallback(uaServer, discoveryUrlChar , 10 * 60 * 1000, 500, NULL);
	if (retVal != UA_STATUSCODE_GOOD) {
		DEVLOG_ERROR("OPC UA: Could not register with LDS. Error: %s\n", UA_StatusCode_name(retVal));
	}
}

void COPC_UA_Handler::removeLdsRegister(const UA_String *discoveryUrl) {
	// check if already registered with the given LDS
	CSinglyLinkedList<const char*>::Iterator itRunner = registeredWithLds.begin();
	CSinglyLinkedList<const char*>::Iterator itRefNode = registeredWithLds.end();

	// remove entry from list
	while(itRunner != registeredWithLds.end()){
		if (strncmp((char*)discoveryUrl->data, *itRunner, discoveryUrl->length) == 0) {
			if(itRefNode == registeredWithLds.end()){
				registeredWithLds.pop_front();
			}
			else{
				registeredWithLds.eraseAfter(itRefNode);
			}
			// dirty cast to remove const
			forte_free((void *)(*itRunner));
			break;
		}
		itRefNode = itRunner;
		++itRunner;
	}

}

#endif

COPC_UA_Handler::COPC_UA_Handler() : uaServerConfig(),	getNodeForPathMutex(), nodeCallbackHandles(), clients(),
#ifdef FORTE_COM_OPC_UA_MULTICAST
	registeredWithLds(),
#endif
	nodeLayerReferences()
{

}

COPC_UA_Handler::~COPC_UA_Handler() {
	stopServer();
	end();
	UA_ServerConfig_delete(uaServerConfig);
	UA_Server_delete(uaServer);

	for (CSinglyLinkedList<struct UA_ClientEndpointMap *>::Iterator iter = clients.begin(); iter != clients.end(); ++iter) {
		UA_Client_disconnect((*iter)->client);
		UA_Client_delete((*iter)->client);
		forte_free((*iter)->endpointUrl);
		forte_free((*iter)->clientMutex);
		forte_free((*iter));
	}
	clients.clearAll();

	for (CSinglyLinkedList<struct UA_NodeCallback_Handle *>::Iterator iter = nodeCallbackHandles.begin(); iter != nodeCallbackHandles.end(); ++iter)
		forte_free(*iter);
	nodeCallbackHandles.clearAll();

	for (CSinglyLinkedList<struct ReferencedNodeByLayer *>::Iterator iter = nodeLayerReferences.begin(); iter != nodeLayerReferences.end(); ++iter) {
		// dirty cast to remove const
		UA_NodeId_delete((UA_NodeId*)(void *)((*iter)->nodeId));
		forte_free((*iter));
	}
	nodeLayerReferences.clearAll();

#ifdef FORTE_COM_OPC_UA_MULTICAST
	for (CSinglyLinkedList<const char *>::Iterator iter = registeredWithLds.begin(); iter != registeredWithLds.end(); ++iter) {
		// dirty cast to remove const
		forte_free((void *)(*iter));
	}
	registeredWithLds.clearAll();
#endif
}

void COPC_UA_Handler::run() {
	DEVLOG_INFO("OPC UA: Starting OPC UA Server: opc.tcp://localhost:%d\n", FORTE_COM_OPC_UA_PORT);
	UA_StatusCode retVal = UA_Server_run(uaServer, &uaServerRunningFlag);    // server keeps iterating as long as running is true;
	if (retVal != UA_STATUSCODE_GOOD) {
		DEVLOG_ERROR("OPC UA: Server exited with error: %s\n", UA_StatusCode_name(retVal));
	} else {
		DEVLOG_INFO("OPC UA: Server successfully stopped\n");
	}
}

void COPC_UA_Handler::enableHandler(void) {
	startServer();
}

void COPC_UA_Handler::disableHandler(void) {
	COPC_UA_Handler::stopServer();
	end();
}

void COPC_UA_Handler::setPriority(int) {

	//currently we are doing nothing here.
}

int COPC_UA_Handler::getPriority(void) const {
	//the same as for setPriority
	return 0;
}

void COPC_UA_Handler::startServer() {
	if (uaServerRunningFlag)
		return;
	uaServerRunningFlag = UA_TRUE;

	if (uaServerConfig == NULL) {
		configureUAServer(FORTE_COM_OPC_UA_PORT);
		uaServer = UA_Server_new(uaServerConfig);
	}


#ifdef FORTE_COM_OPC_UA_MULTICAST
#   ifndef UA_ENABLE_DISCOVERY_MULTICAST
#       error open62541 needs to be built with UA_ENABLE_DISCOVERY_MULTICAST=ON
#   else
	UA_Server_setServerOnNetworkCallback(uaServer, serverOnNetworkCallback, this);
#   endif
#endif

	if (!isAlive()) {
		start();
	}
}

void COPC_UA_Handler::stopServer() {
	uaServerRunningFlag = UA_FALSE;
}

UA_StatusCode
COPC_UA_Handler::getNodeForPath(UA_NodeId **foundNodeId, const char *nodePathConst, bool createIfNotFound,
								const UA_NodeId *startingNode, const UA_NodeId *newNodeType, UA_NodeId **parentNodeId,
								UA_Client *clientInitialized, CSinglyLinkedList<UA_NodeId *> *nodeIdsAlongPath) {

	*foundNodeId = NULL;
	char *nodePath = strdup(nodePathConst);
	// remove tailing slash
	size_t pathLen = strlen(nodePath);
	while (pathLen && nodePath[pathLen - 1] == '/') {
		nodePath[pathLen - 1] = 0;
		pathLen--;
	}
	if (pathLen == 0) {
		forte_free(nodePath);
		return UA_STATUSCODE_BADINVALIDARGUMENT;
	}

	// count number of folders in node path
	unsigned int folderCnt = 0;
	char *c = nodePath;
	while (*c) {
		if (*c == '/')
			folderCnt++;
		c++;
	}

	char *fullPath = strdup(nodePath);
	char *tok = strtok(nodePath, "/");
	if (startingNode == NULL || (startingNode->namespaceIndex == 0 && startingNode->identifier.numeric == UA_NS0ID_OBJECTSFOLDER)) {
		if (strcmp(tok, "Objects") != 0 && strcmp(tok, "0:Objects") != 0) {
			DEVLOG_ERROR("OPC UA: Node path '%s' has to start with '/Objects'\n", fullPath);
			forte_free(fullPath);
			forte_free(nodePath);
			return UA_STATUSCODE_BADINVALIDARGUMENT;
		}
		folderCnt--; //remaining count without Objects folder
		tok = strtok(NULL, "/"); // skip objects folder
	}

	// create a client for requesting the nodes
	UA_Client *client;

	if (clientInitialized) {
		client = clientInitialized;
	} else {
		// ensure local server is running
		startServer();

		client = UA_Client_new(UA_ClientConfig_default);

		char localEndpoint[28];
		forte_snprintf(localEndpoint, 28, "opc.tcp://localhost:%d", FORTE_COM_OPC_UA_PORT);

		{
			UA_StatusCode retVal;
			if ((retVal = UA_Client_connect(client, localEndpoint)) != UA_STATUSCODE_GOOD) {
				DEVLOG_ERROR("OPC UA: Could not connect to local OPC UA Server: %s\n", UA_StatusCode_name(retVal));
				UA_Client_delete(client);
				forte_free(fullPath);
				forte_free(nodePath);
				return retVal;
			}
		}
	}


	// for every folder (which is a BrowsePath) we want to get the node id
	UA_BrowsePath *browsePaths = static_cast<UA_BrowsePath *>(UA_Array_new(folderCnt*2, &UA_TYPES[UA_TYPES_BROWSEPATH]));

	for (unsigned int i = 0; i < folderCnt; i++) {
		UA_BrowsePath_init(&browsePaths[i]);
		if (startingNode != NULL)
			browsePaths[i].startingNode = *startingNode;
		else
			browsePaths[i].startingNode = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
		browsePaths[i].relativePath.elementsSize = i + 1;

		browsePaths[i].relativePath.elements = static_cast<UA_RelativePathElement *>(UA_Array_new(browsePaths[i].relativePath.elementsSize,
																					   &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]));

		for (unsigned int j = 0; j <= i; j++) {

			if (j < i) {
				// just copy from before
				UA_RelativePathElement_copy(&browsePaths[i - 1].relativePath.elements[j], &browsePaths[i].relativePath.elements[j]);
				continue;
			}

			// the last element is a new one

			UA_RelativePathElement_init(&browsePaths[i].relativePath.elements[j]);
			browsePaths[i].relativePath.elements[j].isInverse = UA_FALSE;

			// split the qualified name
			char *splitPos = tok;
			while (*splitPos && *splitPos != ':') {
				splitPos++;
			}

			// default namespace is 1
			UA_UInt16 ns = 1;
			char *targetName = tok;

			if (*splitPos) {
				// namespace given
				ns = static_cast<UA_UInt16>(atoi(tok));
				targetName = ++splitPos;
			}
			browsePaths[i].relativePath.elements[j].targetName = UA_QUALIFIEDNAME_ALLOC(ns, targetName);
		}
		tok = strtok(NULL, "/");
	}

	// same browse paths, but inverse reference, to check both directions
	for (unsigned int i = 0; i < folderCnt; i++) {
		UA_BrowsePath_copy(&browsePaths[i], &browsePaths[folderCnt+i]);
		for (unsigned int j = 0; j < browsePaths[folderCnt+i].relativePath.elementsSize; j++) {
			browsePaths[folderCnt+i].relativePath.elements[j].isInverse = !browsePaths[folderCnt+i].relativePath.elements[j].isInverse;
		}
	}

	UA_TranslateBrowsePathsToNodeIdsRequest request;
	UA_TranslateBrowsePathsToNodeIdsRequest_init(&request);
	request.browsePaths = browsePaths;
	request.browsePathsSize = folderCnt*2;

	{
		// other thread may currently create nodes for the same path, thus mutex
		CCriticalRegion criticalRegion(this->getNodeForPathMutex);

		UA_TranslateBrowsePathsToNodeIdsResponse response;

		if (clientInitialized) {

			CSyncObject *clientMutex = this->getMutexForClient(client);

			if (clientMutex) {
				clientMutex->lock();
			}
			response = UA_Client_Service_translateBrowsePathsToNodeIds(client, request);
			if (clientMutex) {
				clientMutex->unlock();
			}
		} else {
			response = UA_Client_Service_translateBrowsePathsToNodeIds(client, request);
			UA_Client_disconnect(client);
			UA_Client_delete(client);
		}

		if (response.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
			UA_StatusCode retVal = response.responseHeader.serviceResult;
			DEVLOG_ERROR("OPC UA: Could not translate browse paths for '%s' to node IDs. Error: %s\n", fullPath, UA_StatusCode_name(retVal));
			UA_TranslateBrowsePathsToNodeIdsRequest_deleteMembers(&request);
			UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&response);
			forte_free(fullPath);
			forte_free(nodePath);
			return retVal;
		}

		if (response.resultsSize != folderCnt*2) {
			DEVLOG_ERROR("OPC UA: Could not translate browse paths for '%s' to node IDs. resultSize (%d) != expected count (%d)\n", fullPath,
						 response.resultsSize, folderCnt);
			UA_TranslateBrowsePathsToNodeIdsRequest_deleteMembers(&request);
			UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&response);
			forte_free(fullPath);
			forte_free(nodePath);
			return UA_STATUSCODE_BADUNEXPECTEDERROR;
		}

		*foundNodeId = NULL;
		UA_StatusCode retVal = UA_STATUSCODE_GOOD;

		int foundFolderOffset = -1;
		if (response.results[folderCnt - 1].statusCode == UA_STATUSCODE_GOOD) {
			foundFolderOffset = 0;
		} else if (response.results[folderCnt*2 - 1].statusCode == UA_STATUSCODE_GOOD) {
			foundFolderOffset = folderCnt;
		}

		if (foundFolderOffset >= 0) {
			// all nodes exist, so just copy the node ids
			*foundNodeId = UA_NodeId_new();
			UA_NodeId_copy(&response.results[foundFolderOffset + folderCnt - 1].targets[0].targetId.nodeId, *foundNodeId);
			if (parentNodeId && folderCnt >= 2) {
				*parentNodeId = UA_NodeId_new();
				UA_NodeId_copy(&response.results[foundFolderOffset + folderCnt - 2].targets[0].targetId.nodeId, *parentNodeId);
			} else if (parentNodeId && !startingNode) {
				*parentNodeId = UA_NodeId_new();
				**parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
			}
			if (nodeIdsAlongPath) {
				for (unsigned int i = 0; i < folderCnt; i++) {
					UA_NodeId *tmp = UA_NodeId_new();
					UA_NodeId_copy(&response.results[foundFolderOffset + i].targets[0].targetId.nodeId, tmp);
					nodeIdsAlongPath->push_back(tmp);
				}
			}
		} else if (createIfNotFound) {
			// last node does not exist, and we should create the path
			// skip last node because we already know that it doesn't exist

			*foundNodeId = UA_NodeId_new();
			if (parentNodeId && folderCnt >= 2) {
				*parentNodeId = UA_NodeId_new();
			}
			foundFolderOffset = 0;
			// no unsigned, because we need to check for -1
			int i;
			for (i = folderCnt - 2; i >= 0; i--) {
				// find first existing node. Check for isInverse = TRUE
				if (response.results[i].statusCode == UA_STATUSCODE_GOOD) {
					foundFolderOffset = 0;
					break;
				}
				// and for isInverse = FALSE
				if (response.results[folderCnt + i].statusCode == UA_STATUSCODE_GOOD) {
					foundFolderOffset = folderCnt;
					break;
				}
			}
			for (; i >= 0; i--) {
				// now we found the first existing node
				if (response.results[foundFolderOffset+i].targetsSize == 0) {
					DEVLOG_ERROR("OPC UA: Could not translate browse paths for '%s' to node IDs. target size is 0.\n", fullPath);
					break;
				}
				if (response.results[foundFolderOffset+i].targetsSize > 1) {
					DEVLOG_WARNING("OPC UA: The given browse path '%s' has multiple results for the same path. Taking the first result.\n", fullPath);
				}

				// foundNodeId contains the ID of the parent which exists
				UA_NodeId_copy(&response.results[foundFolderOffset+i].targets[0].targetId.nodeId, *foundNodeId);
				if (parentNodeId && folderCnt >= 2) {
					UA_NodeId_copy(&response.results[foundFolderOffset+i].targets[0].targetId.nodeId, *parentNodeId);
				}
				break;
			}

			if (i == -1) {
				// no node of the path exists, thus parent is Objects folder
				UA_NodeId_copy(&browsePaths[0].startingNode, *foundNodeId);
				if (parentNodeId && folderCnt >= 2) {
					**parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
				}
			}
			i++;

			if (nodeIdsAlongPath) {
				for (int j = 0; j < i; j++) {
					UA_NodeId *tmp = UA_NodeId_new();
					UA_NodeId_copy(&response.results[foundFolderOffset+j].targets[0].targetId.nodeId, tmp);
					nodeIdsAlongPath->push_back(tmp);
				}
			}

			// create all the nodes on the way
			for (unsigned int j = (unsigned int) i; j < folderCnt; j++) {

				// the last browse path contains all relativePath elements.
				UA_QualifiedName *targetName = &request.browsePaths[folderCnt - 1].relativePath.elements[j].targetName;

				UA_ObjectAttributes oAttr;
				UA_ObjectAttributes_init(&oAttr);
				char locale[] = "";
				char *nodeName = static_cast<char *>(forte_malloc(sizeof(char) * (targetName->name.length + 1)));
				memcpy(nodeName, targetName->name.data, targetName->name.length);
				nodeName[targetName->name.length] = 0;
				oAttr.description = UA_LOCALIZEDTEXT_ALLOC(locale, nodeName);
				oAttr.displayName = UA_LOCALIZEDTEXT_ALLOC(locale, nodeName);
				forte_free(nodeName);

				UA_NodeId creationType;
				if (newNodeType == NULL || j < folderCnt - 1)
					// the newNodeType is only used for the last node
					creationType = UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE);
				else
					creationType = *newNodeType;

				if ((retVal = UA_Server_addObjectNode(uaServer, UA_NODEID_NUMERIC(targetName->namespaceIndex, 0),
													  **foundNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
													  *targetName, creationType, oAttr, NULL, *foundNodeId)) != UA_STATUSCODE_GOOD) {
					DEVLOG_ERROR("OPC UA: Could not addObjectNode. Error: %s\n", UA_StatusCode_name(retVal));
					UA_NodeId_delete(*foundNodeId);
					if (parentNodeId && folderCnt >= 2) {
						UA_NodeId_delete(*parentNodeId);
					}

					UA_ObjectAttributes_deleteMembers(&oAttr);
					*foundNodeId = NULL;
					break;
				}

				if (nodeIdsAlongPath) {
					UA_NodeId *tmp = UA_NodeId_new();
					UA_NodeId_copy(*foundNodeId, tmp);
					nodeIdsAlongPath->push_back(tmp);
				}

				if (j == folderCnt - 1 && parentNodeId) {
					// foundNodeId will be overwritten in next iteration
					UA_NodeId_copy(*foundNodeId, *parentNodeId);
				}


				UA_ObjectAttributes_deleteMembers(&oAttr);
			}
		}

		UA_TranslateBrowsePathsToNodeIdsRequest_deleteMembers(&request);
		UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&response);
		forte_free(fullPath);
		forte_free(nodePath);
		return retVal;
	}
}


CSyncObject *COPC_UA_Handler::getMutexForClient(const UA_Client *client) {
	for (CSinglyLinkedList<struct UA_ClientEndpointMap *>::Iterator iter = clients.begin(); iter != clients.end(); ++iter) {
		if ((*iter)->client == client)
			return (*iter)->clientMutex;
	}
	return NULL;
}

UA_Client *COPC_UA_Handler::getClientForEndpoint(const char *endpointUrl, bool createIfNotFound, CSyncObject **clientMutex) {

	// remove tailing slash
	size_t urlLen = strlen(endpointUrl);
	while (urlLen && endpointUrl[urlLen - 1] == '/') {
		urlLen--;
	}

	for (CSinglyLinkedList<struct UA_ClientEndpointMap *>::Iterator iter = clients.begin(); iter != clients.end(); ++iter) {
		if (strncmp((*iter)->endpointUrl, endpointUrl, urlLen) == 0) {
			if (clientMutex)
				*clientMutex = (*iter)->clientMutex;
			return (*iter)->client;
		}
	}

	if (!createIfNotFound)
		return NULL;

	struct UA_ClientEndpointMap *clientMap = static_cast<UA_ClientEndpointMap *>(forte_malloc(sizeof(struct UA_ClientEndpointMap)));

	UA_ClientConfig config = UA_ClientConfig_default;
	config.timeout = 8000;

	UA_Client *client = UA_Client_new(config);

	clientMap->client = client;
	clientMap->endpointUrl = static_cast<char *>(forte_malloc(sizeof(char) * (strlen(endpointUrl) + 1)));
	strcpy(clientMap->endpointUrl, endpointUrl);
	clientMap->clientMutex = static_cast<CSyncObject *>(forte_malloc(sizeof(CSyncObject)));
	*clientMap->clientMutex = CSyncObject();

	if (clientMutex)
		*clientMutex = clientMap->clientMutex;

	// store it in the list so we can delete it to avoid mem leaks
	clients.push_back(clientMap);

	return client;
}

UA_StatusCode COPC_UA_Handler::createVariableNode(const UA_NodeId *parentNode, const char *varName, const UA_DataType *varType, void *varValue,
												  UA_NodeId *returnVarNodeId, bool allowWrite) {


	// set UA NodeId attributes
	UA_QualifiedName varBrowseName = UA_QUALIFIEDNAME_ALLOC(1, varName);

	UA_NodeId typeDefinition = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

	// create variable attributes
	UA_VariableAttributes var_attr;
	UA_VariableAttributes_init(&var_attr);

	char locale[] = "en-US";
	char description[] = "Digital port of Function Block";

	var_attr.dataType = varType->typeId;
	var_attr.valueRank = -1; /* value is a scalar */
	var_attr.displayName = UA_LOCALIZEDTEXT_ALLOC(locale, varName);
	var_attr.description = UA_LOCALIZEDTEXT(locale, description);
	var_attr.userAccessLevel = UA_ACCESSLEVELMASK_READ;
	if (allowWrite)
		var_attr.userAccessLevel |= UA_ACCESSLEVELMASK_WRITE;
	var_attr.accessLevel = var_attr.userAccessLevel;
	UA_Variant_setScalar(&var_attr.value, varValue, varType);

	UA_NodeId *returnNodeId = UA_NodeId_new();

	// add UA Variable Node to the server address space
	UA_StatusCode retVal = UA_Server_addVariableNode(
			uaServer,                 // server
			UA_NODEID_NUMERIC(1, 0),                  // requestedNewNodeId
			*parentNode,                 // parentNodeId
			UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),              // referenceTypeId   Reference to the type definition for the variable node
			varBrowseName,                // browseName
			typeDefinition,               // typeDefinition
			var_attr,                     // Variable attributes
			NULL,                 // instantiation callback
			returnNodeId);                  // return Node Id

	UA_QualifiedName_deleteMembers(&varBrowseName);
	UA_LocalizedText_deleteMembers(&var_attr.displayName);

	if (retVal == UA_STATUSCODE_GOOD) {
		retVal = UA_NodeId_copy(returnNodeId, returnVarNodeId);
	} else {
		DEVLOG_ERROR("OPC UA: AddressSpace adding Variable Node %s failed. Error: %s\n", varName, UA_StatusCode_name(retVal));
	}
	UA_NodeId_delete(returnNodeId);
	return retVal;
}

UA_StatusCode COPC_UA_Handler::createMethodNode(const UA_NodeId *parentNode, UA_UInt16 namespaceIdx, const char *methodName, UA_MethodCallback callback,
												void *callbackHandle, unsigned int inputArgumentsSize, const UA_Argument *inputArguments,
												unsigned int outputArgumentsSize, const UA_Argument *outputArguments, UA_NodeId *returnNodeId) {

	UA_MethodAttributes methodAttributes;
	UA_MethodAttributes_init(&methodAttributes);
	methodAttributes.description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Method which can be called");
	methodAttributes.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", methodName);
	methodAttributes.executable = true;
	methodAttributes.userExecutable = true;
	UA_QualifiedName browseName = UA_QUALIFIEDNAME_ALLOC(namespaceIdx, methodName);
	UA_StatusCode retVal = UA_Server_addMethodNode(uaServer,
												   UA_NODEID_NUMERIC(1, 0),
												   *parentNode,
												   UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
												   browseName,
												   methodAttributes, callback,
												   inputArgumentsSize, inputArguments,
												   outputArgumentsSize, outputArguments, callbackHandle, returnNodeId);
	UA_QualifiedName_deleteMembers(&browseName);
	UA_LocalizedText_deleteMembers(&methodAttributes.description);
	UA_LocalizedText_deleteMembers(&methodAttributes.displayName);
	return retVal;
}

UA_StatusCode COPC_UA_Handler::updateNodeValue(const UA_NodeId *nodeId, const CIEC_ANY *data, const UA_TypeConvert *convert) {
	UA_Variant *nodeValue = UA_Variant_new();
	UA_Variant_init(nodeValue);

	void *varValue = UA_new(convert->type);
	if (!convert->get(data, varValue)) {
		UA_delete(varValue, convert->type);
		return UA_STATUSCODE_BADUNEXPECTEDERROR;
	}

	UA_Variant_setScalarCopy(nodeValue, varValue, convert->type);
	UA_StatusCode retVal = UA_Server_writeValue(uaServer, *nodeId, *nodeValue);
	UA_delete(varValue, convert->type);
	UA_Variant_delete(nodeValue);
	return retVal;
}


UA_StatusCode COPC_UA_Handler::registerNodeCallBack(const UA_NodeId *nodeId, forte::com_infra::CComLayer *comLayer, const struct UA_TypeConvert *convert,
													unsigned int portIndex) {
	// needs new, otherwise it will be removed as soon as registerNodecallBack exits, and thus handle is not valid in the callback
	struct UA_NodeCallback_Handle *handle = static_cast<UA_NodeCallback_Handle *>(forte_malloc(sizeof(struct UA_NodeCallback_Handle)));

	handle->convert = convert;
	handle->comLayer = comLayer;
	handle->portIndex = portIndex;
	// store it in the list so we can delete it to avoid mem leaks
	nodeCallbackHandles.push_back(handle);
	UA_ValueCallback callback = {NULL, COPC_UA_Handler::getInstance().onWrite};
	UA_Server_setNodeContext(uaServer, *nodeId, handle);
	return UA_Server_setVariableNode_valueCallback(uaServer, *nodeId, callback);
}

void COPC_UA_Handler::onWrite(UA_Server *, const UA_NodeId *,
							  void *, const UA_NodeId *,
							  void *nodeContext, const UA_NumericRange *,
							  const UA_DataValue *data) {

	struct UA_NodeCallback_Handle *handle = static_cast<struct UA_NodeCallback_Handle *>(nodeContext);

	CComLayer *layer = handle->comLayer;

	EComResponse retVal;

	struct {
		const struct UA_TypeConvert *convert;
		unsigned int portIndex;
		const UA_Variant *data;
	} handleRecv;

	handleRecv.data = data->hasValue ? &data->value : NULL;
	handleRecv.portIndex = handle->portIndex;
	handleRecv.convert = handle->convert;

	retVal = layer->recvData(static_cast<const void *>(&handleRecv), 0);    //TODO: add multidimensional data handling with 'range'.

	/* Handle return of receive data */
	if (e_ProcessDataOk == retVal) {
		//only update data in item if data could be read
	}

	if (e_Nothing != retVal) {
		getInstance().startNewEventChain(layer->getCommFB());
	}

}

void COPC_UA_Handler::referencedNodesIncrement(const CSinglyLinkedList<UA_NodeId *> *nodes, const COPC_UA_Layer *layer) {
	for (CSinglyLinkedList<UA_NodeId *>::Iterator iterNode = nodes->begin(); iterNode != nodes->end(); ++iterNode) {
		bool found = false;
		for (CSinglyLinkedList<struct ReferencedNodeByLayer *>::Iterator iterRef = nodeLayerReferences.begin(); iterRef != nodeLayerReferences.end(); ++iterRef) {
			if (UA_NodeId_equal((*iterRef)->nodeId, (*iterNode))) {
				found = true;
				(*iterRef)->referencedByLayer.push_front(layer);
				break;
			}
		}
		if (!found) {
			struct ReferencedNodeByLayer *newRef = static_cast<struct ReferencedNodeByLayer *>(forte_malloc(sizeof(struct ReferencedNodeByLayer)));
			UA_NodeId *newNode = UA_NodeId_new();
			UA_NodeId_copy((*iterNode), newNode);
			newRef->nodeId = newNode;
			newRef->referencedByLayer = CSinglyLinkedList<const COPC_UA_Layer*>();
			newRef->referencedByLayer.push_front(layer);
			nodeLayerReferences.push_front(newRef);
		}
	}
}

void COPC_UA_Handler::referencedNodesDecrement(const CSinglyLinkedList<UA_NodeId *> *nodes, const COPC_UA_Layer *layer, bool deleteIfLastReference) {
	for (CSinglyLinkedList<UA_NodeId *>::Iterator iterNode = nodes->begin(); iterNode != nodes->end(); ++iterNode) {
		for (CSinglyLinkedList<struct ReferencedNodeByLayer *>::Iterator iterRef = nodeLayerReferences.begin(); iterRef != nodeLayerReferences.end(); ++iterRef) {
			if (UA_NodeId_equal((*iterRef)->nodeId, (*iterNode))) {

				while ((*iterRef)->referencedByLayer.peek_front() == layer)
					(*iterRef)->referencedByLayer.pop_front();

				CSinglyLinkedList<const COPC_UA_Layer *>::Iterator iterLayerPrev = (*iterRef)->referencedByLayer.begin();
				for (CSinglyLinkedList<const COPC_UA_Layer *>::Iterator iterLayer = (*iterRef)->referencedByLayer.begin(); iterLayer != (*iterRef)->referencedByLayer.end(); ++iterLayer) {
					if ((*iterLayer) == layer) {
						(*iterRef)->referencedByLayer.eraseAfter(iterLayerPrev);
						// do not break, node may be referenced multiple times
						iterLayer = iterLayerPrev;
					}
				}

				if ((*iterRef)->referencedByLayer.isEmpty()) {
					if (deleteIfLastReference) {
						UA_Server_deleteNode(uaServer, *(*iterRef)->nodeId, UA_TRUE);
					}
				}
				break;
			}
		}
	}
}







