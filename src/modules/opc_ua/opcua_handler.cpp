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
#include <stdarg.h>

using namespace forte::com_infra;

DEFINE_SINGLETON(COPC_UA_Handler);

#define FORTE_COM_OPC_UA_PORT 4840

const char *LogsLevelNames[6] = {"trace", "debug", "info", "warning", "error", "fatal"};
const char *LogsCategoryNames[6] = {"network", "channel", "session", "server", "client", "userland"};

void UA_Log_Forte(UA_LogLevel level, UA_LogCategory category, const char *msg, va_list args) {
	char tmpStr[400];
	snprintf(tmpStr, 400, "[OPC UA] %s/%s\t", LogsLevelNames[level], LogsCategoryNames[category]);
	char *start = &tmpStr[strlen(tmpStr)];

	vsprintf(start, msg, args);

	size_t len = strlen(tmpStr);
	tmpStr[len] = '\n';
	tmpStr[len+1] = '\0';

	switch (level) {
		case UA_LOGLEVEL_TRACE:
		case UA_LOGLEVEL_DEBUG:
			DEVLOG_DEBUG(tmpStr_);
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
	uaServerConfig = UA_ServerConfig_standard;
	uaServerConfig.networkLayersSize = 1;
	uaServerConfig.logger = UA_Log_Forte;

	// TODO make sure the URI is unique, e.g. by using the application name or something else.
	uaServerConfig.applicationDescription.applicationUri = UA_String_fromChars("org.eclipse.4diac.forte");
	uaServerConfig.applicationDescription.applicationName.locale = UA_String_fromChars("EN");
	uaServerConfig.applicationDescription.applicationName.text = UA_String_fromChars("FORTE");

	// TODO set server capabilities
	// See http://www.opcfoundation.org/UA/schemas/1.03/ServerCapabilities.csv
	//config.serverCapabilitiesSize = 1;
	//UA_String caps = UA_String_fromChars("LDS");
	//config.serverCapabilities = &caps;

	uaServerNetworkLayer = UA_ServerNetworkLayerTCP(UA_ConnectionConfig_standard, UAServerPort);
	uaServerConfig.networkLayers = &uaServerNetworkLayer;
}

COPC_UA_Handler::COPC_UA_Handler() : uaServerConfig(), uaServerNetworkLayer(), getNodeForPathMutex(), nodeCallbackHandles() {
	configureUAServer(FORTE_COM_OPC_UA_PORT);
	uaServerRunningFlag = new UA_Boolean(UA_TRUE);
	uaServer = UA_Server_new(uaServerConfig);

	setServerRunning();        // set server loop flag

	if (!isAlive()) {
		start();
	}
}

COPC_UA_Handler::~COPC_UA_Handler() {
	stopServerRunning();
	end();
	UA_String_deleteMembers(&uaServerConfig.applicationDescription.applicationUri);
	UA_LocalizedText_deleteMembers(&uaServerConfig.applicationDescription.applicationName);
	UA_Server_delete(uaServer);
	uaServerNetworkLayer.deleteMembers(&uaServerNetworkLayer);

	for (CSinglyLinkedList<struct UA_NodeCallback_Handle *>::Iterator iter = nodeCallbackHandles.begin(); iter != nodeCallbackHandles.end(); ++iter)
		delete (*iter);
	nodeCallbackHandles.clearAll();
}

void COPC_UA_Handler::run() {
	DEVLOG_INFO("Starting OPC UA Server: opc.tcp://localhost:%d\n", FORTE_COM_OPC_UA_PORT);
	UA_StatusCode retVal = UA_Server_run(uaServer, uaServerRunningFlag);    // server keeps iterating as long as running is true;
	if (retVal != UA_STATUSCODE_GOOD) {
		DEVLOG_ERROR("OPC UA Server exited with error: %s - %s\n", UA_StatusCode_name(retVal), UA_StatusCode_description(retVal));
	} else {
		DEVLOG_INFO("OPC UA Server successfully stopped\n");
	}
}

void COPC_UA_Handler::enableHandler(void) {
	start();
}

void COPC_UA_Handler::disableHandler(void) {
	COPC_UA_Handler::stopServerRunning();
	end();
}

void COPC_UA_Handler::setPriority(int) {

	//currently we are doing nothing here.
}

int COPC_UA_Handler::getPriority(void) const {
	//the same as for setPriority
	return 0;
}

void COPC_UA_Handler::setServerRunning() {
	*uaServerRunningFlag = UA_TRUE;
}

void COPC_UA_Handler::stopServerRunning() {
	*uaServerRunningFlag = UA_FALSE;
}

UA_StatusCode
COPC_UA_Handler::getNodeForPath(UA_NodeId **foundNodeId, char *nodePath, bool createIfNotFound, const UA_NodeId *startingNode, const UA_NodeId *newNodeType) {

	*foundNodeId = NULL;
	// remove tailing slash
	size_t pathLen = strlen(nodePath);
	while (pathLen && nodePath[pathLen - 1] == '/') {
		nodePath[pathLen - 1] = 0;
		pathLen--;
	}
	if (pathLen == 0)
		return UA_STATUSCODE_BADINVALIDARGUMENT;

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
			DEVLOG_ERROR("Node path '%s' has to start with '/Objects'\n", fullPath);
			forte_free(fullPath);
			return UA_STATUSCODE_BADINVALIDARGUMENT;
		}
		folderCnt--; //remaining count without Objects folder
		tok = strtok(NULL, "/"); // skip objects folder
	}

	// create a client for requesting the nodes
	UA_Client *client = UA_Client_new(UA_ClientConfig_standard);

	char localEndpoint[28];
	snprintf(localEndpoint, 28, "opc.tcp://localhost:%d", FORTE_COM_OPC_UA_PORT);

	{
		UA_StatusCode retVal;
		if ((retVal = UA_Client_connect(client, localEndpoint)) != UA_STATUSCODE_GOOD) {
			DEVLOG_ERROR("Could not connect to local OPC UA Server: %s - %s\n", UA_StatusCode_name(retVal), UA_StatusCode_description(retVal));
			UA_Client_delete(client);
			forte_free(fullPath);
			return retVal;
		}
	}


	// for every folder (which is a BrowsePath) we want to get the node id
	UA_BrowsePath *browsePaths = new UA_BrowsePath[folderCnt];

	for (unsigned int i = 0; i < folderCnt; i++) {
		UA_BrowsePath_init(&browsePaths[i]);
		if (startingNode != NULL)
			browsePaths[i].startingNode = *startingNode;
		else
			browsePaths[i].startingNode = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
		browsePaths[i].relativePath.elementsSize = i + 1;
		browsePaths[i].relativePath.elements = new UA_RelativePathElement[i + 1];
		for (unsigned int j = 0; j <= i; j++) {

			if (j < i) {
				// just copy from before
				UA_RelativePathElement_copy(&browsePaths[i - 1].relativePath.elements[j], &browsePaths[i].relativePath.elements[j]);
				continue;
			}

			// the last element is a new one

			UA_RelativePathElement_init(&browsePaths[i].relativePath.elements[j]);
			browsePaths[i].relativePath.elements[j].isInverse = UA_TRUE;

			// split the qualified name
			char *splitPos = tok;
			while (*splitPos && *splitPos != ':') {
				splitPos++;
			}

			// default namespace is 0
			UA_UInt16 ns = 0;
			char *targetName = tok;

			if (*splitPos) {
				// namespace given
				ns = static_cast<UA_UInt16>(atoi(tok));
				targetName = ++splitPos;
			}
			browsePaths[i].relativePath.elements[j].targetName = UA_QUALIFIEDNAME(ns, strdup(targetName));
		}
		tok = strtok(NULL, "/");
	}

	UA_TranslateBrowsePathsToNodeIdsRequest request;
	UA_TranslateBrowsePathsToNodeIdsRequest_init(&request);
	request.browsePaths = browsePaths;
	request.browsePathsSize = folderCnt;

	{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
		// other thready may currently create nodes for the same path, thus mutex
		CCriticalRegion criticalRegion(this->getNodeForPathMutex);
#pragma GCC diagnostic pop
		UA_TranslateBrowsePathsToNodeIdsResponse response = UA_Client_Service_translateBrowsePathsToNodeIds(client, request);

		if (response.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
			UA_StatusCode retVal = response.responseHeader.serviceResult;
			DEVLOG_ERROR("Could not translate browse paths for '%s' to node IDs. Error: %s - %s\n", fullPath, UA_StatusCode_name(retVal),
						 UA_StatusCode_description(retVal));
			UA_TranslateBrowsePathsToNodeIdsRequest_deleteMembers(&request);
			UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&response);
			UA_Client_disconnect(client);
			UA_Client_delete(client);
			forte_free(fullPath);
			return retVal;
		}

		if (response.resultsSize != folderCnt) {
			DEVLOG_ERROR("Could not translate browse paths for '%s' to node IDs. resultSize (%d) != expected count (%d)\n", fullPath, response.resultsSize,
						 folderCnt);
			UA_TranslateBrowsePathsToNodeIdsRequest_deleteMembers(&request);
			UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&response);
			UA_Client_disconnect(client);
			UA_Client_delete(client);
			forte_free(fullPath);
			return UA_STATUSCODE_BADUNEXPECTEDERROR;
		}

		*foundNodeId = NULL;
		UA_StatusCode retVal = UA_STATUSCODE_GOOD;

		if (response.results[folderCnt - 1].statusCode == UA_STATUSCODE_GOOD) {
			*foundNodeId = UA_NodeId_new();
			UA_NodeId_copy(&response.results[folderCnt - 1].targets[0].targetId.nodeId, *foundNodeId);
		} else if (createIfNotFound) {
			// last node does not exist, and we should create the path
			// skip last node because we already know that it doesn't exist

			*foundNodeId = UA_NodeId_new();
			int i;
			for (i = folderCnt - 2; i >= 0; i--) {
				if (response.results[i].statusCode != UA_STATUSCODE_GOOD) {
					// find first existing node
					continue;
				}
				// now we found the first existing node
				if (response.results[i].targetsSize == 0) {
					DEVLOG_ERROR("Could not translate browse paths for '%s' to node IDs. target size is 0.\n", fullPath);
					break;
				}
				if (response.results[i].targetsSize > 1) {
					DEVLOG_WARNING("The given browse path '%s' has multiple results for the same path. Taking the first result.\n", fullPath);
				}

				// foundNodeId contains the ID of the parent which exists
				UA_NodeId_copy(&response.results[i].targets[0].targetId.nodeId, *foundNodeId);
				break;
			}

			if (i == -1) {
				// no node of the path exists, thus parent is Objects folder
				UA_NodeId_copy(&browsePaths[0].startingNode, *foundNodeId);
			}
			i++;

			// create all the nodes on the way
			for (unsigned int j = (unsigned int) i; j < folderCnt; j++) {

				// the last browse path contains all relativePath elements.
				UA_QualifiedName *targetName = &request.browsePaths[folderCnt - 1].relativePath.elements[j].targetName;

				UA_ObjectAttributes oAttr;
				UA_ObjectAttributes_init(&oAttr);
				char locale[] = "en_US";
				char *nodeName = new char[targetName->name.length + 1];
				memcpy(nodeName, targetName->name.data, targetName->name.length);
				nodeName[targetName->name.length] = 0;
				oAttr.description = UA_LOCALIZEDTEXT(locale, nodeName);
				oAttr.displayName = UA_LOCALIZEDTEXT(locale, nodeName);

				UA_NodeId creationType;
				if (newNodeType == NULL || j < folderCnt - 1)
					// the newNodeType is only used for the last node
					creationType = UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE);
				else
					creationType = *newNodeType;

				if ((retVal = UA_Server_addObjectNode(uaServer, UA_NODEID_NUMERIC(1, 0),
													  **foundNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
													  *targetName, creationType, oAttr, NULL, *foundNodeId)) != UA_STATUSCODE_GOOD) {
					DEVLOG_ERROR("Could not addObjectNode. Error: %s - %s\n", UA_StatusCode_name(retVal), UA_StatusCode_description(retVal));
					forte_free(*foundNodeId);
					delete[](nodeName);
					*foundNodeId = NULL;
					break;
				}
				delete[](nodeName);
			}
		}

		UA_TranslateBrowsePathsToNodeIdsRequest_deleteMembers(&request);
		UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&response);
		UA_Client_disconnect(client);
		UA_Client_delete(client);
		forte_free(fullPath);
		return retVal;
	}
}

UA_StatusCode COPC_UA_Handler::createVariableNode(const UA_NodeId *parentNode, const char *varName, const UA_DataType *varType, void *varValue,
												  UA_NodeId *returnVarNodeId, bool allowWrite) {


	// set UA NodeId attributes
	UA_QualifiedName varBrowseName = UA_QUALIFIEDNAME_ALLOC(1, varName);

	UA_NodeId typeDefinition = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

	// create variable attributes
	UA_VariableAttributes var_attr;
	UA_VariableAttributes_init(&var_attr);

	char locale[] = "en_US";
	char description[] = "Digital port of Function Block";

	var_attr.displayName = UA_LOCALIZEDTEXT_ALLOC(locale, varName);
	var_attr.description = UA_LOCALIZEDTEXT(locale, description);
	var_attr.userAccessLevel = UA_ACCESSLEVELMASK_READ;
	if (allowWrite)
		var_attr.userAccessLevel |= UA_ACCESSLEVELMASK_WRITE;
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
		DEVLOG_ERROR("UA-Server AddressSpace: Adding Variable Node %s failed. Error: %s - %s\n", varName, UA_StatusCode_name(retVal),
					 UA_StatusCode_description(retVal));
	}
	UA_NodeId_delete(returnNodeId);
	return retVal;
}

UA_StatusCode COPC_UA_Handler::createMethodNode(const UA_NodeId *parentNode, UA_UInt16 namespaceIdx, const char *methodName, UA_MethodCallback callback,
												void *callbackHandle, unsigned int inputArgumentsSize, const UA_Argument *inputArguments,
												unsigned int outputArgumentsSize, const UA_Argument *outputArguments, UA_NodeId *returnNodeId) {

	UA_MethodAttributes methodAttributes;
	UA_MethodAttributes_init(&methodAttributes);
	methodAttributes.description = UA_LOCALIZEDTEXT_ALLOC("en_US", "Method which can be called");
	methodAttributes.displayName = UA_LOCALIZEDTEXT_ALLOC("en_US", methodName);
	methodAttributes.executable = true;
	methodAttributes.userExecutable = true;
	UA_QualifiedName browseName = UA_QUALIFIEDNAME_ALLOC(namespaceIdx, methodName);
	UA_StatusCode retVal = UA_Server_addMethodNode(uaServer,
												   UA_NODEID_NUMERIC(1, 0),
												   *parentNode,
												   UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
												   browseName,
												   methodAttributes, callback, callbackHandle,
												   inputArgumentsSize, inputArguments,
												   outputArgumentsSize, outputArguments, returnNodeId);
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
	return retVal;
}


UA_StatusCode COPC_UA_Handler::registerNodeCallBack(const UA_NodeId *nodeId, forte::com_infra::CComLayer *comLayer, const struct UA_TypeConvert *convert,
													unsigned int portIndex) {
	// needs new, otherwise it will be removed as soon as registerNodecallBack exits, and thus handle is not valid in the callback
	struct UA_NodeCallback_Handle *handle = new struct UA_NodeCallback_Handle;

	handle->convert = convert;
	handle->comLayer = comLayer;
	handle->portIndex = portIndex;
	// store it in the list so we can delete it to avoid mem leaks
	nodeCallbackHandles.push_back(handle);

	UA_ValueCallback callback = {static_cast<void *>(handle), NULL, COPC_UA_Handler::getInstance().onWrite};
	return UA_Server_setVariableNode_valueCallback(uaServer, *nodeId, callback);
}


void COPC_UA_Handler::onWrite(void *handleRaw, const UA_NodeId, const UA_Variant *data, const UA_NumericRange *) {

	struct UA_NodeCallback_Handle *handle = static_cast<struct UA_NodeCallback_Handle *>(handleRaw);

	CComLayer *layer = handle->comLayer;

	EComResponse retVal;

	struct {
		const struct UA_TypeConvert *convert;
		unsigned int portIndex;
		const UA_Variant *data;
	} handleRecv;

	handleRecv.data = data;
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







