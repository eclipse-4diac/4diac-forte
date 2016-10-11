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
#include <commfb.h>
#include <devexec.h>
#include "../../core/utils/criticalregion.h"

using namespace forte::com_infra;

DEFINE_SINGLETON(COPC_UA_Handler);

#define FORTE_COM_OPC_UA_PORT 4840

void COPC_UA_Handler::configureUAServer(TForteUInt16 UAServerPort) {
	m_server_config = UA_ServerConfig_standard;
	m_server_config.enableUsernamePasswordLogin = false;
	m_server_config.networkLayersSize = 1;
	m_server_config.logger = UA_Log_Stdout;

	m_server_networklayer = UA_ServerNetworkLayerTCP(UA_ConnectionConfig_standard, UAServerPort);
	m_server_config.networkLayers = &m_server_networklayer;
}

COPC_UA_Handler::COPC_UA_Handler() : m_server_config(), m_server_networklayer(), getNodeForPathMutex(){
	configureUAServer(FORTE_COM_OPC_UA_PORT); 	// configure a standard server
	mOPCUAServer = UA_Server_new(m_server_config);

	setServerRunning();		// set server loop flag

	if(!isAlive()){
		start();
	}
}

COPC_UA_Handler::~COPC_UA_Handler() {
	stopServerRunning();
	UA_Server_delete(mOPCUAServer);
	m_server_networklayer.deleteMembers(&m_server_networklayer);
}

void COPC_UA_Handler::run(){
	UA_StatusCode retVal = UA_Server_run(mOPCUAServer, mbServerRunning);	// server keeps iterating as long as running is true;
	DEVLOG_INFO("UA_Server run status code %s", retVal);
}

void COPC_UA_Handler::enableHandler(void){
	start();
}

void COPC_UA_Handler::disableHandler(void){
	COPC_UA_Handler::stopServerRunning();
	end();
}

void COPC_UA_Handler::setPriority(int){

	//currently we are doing nothing here.
	//TODO We should adjust the thread priority.
}

int COPC_UA_Handler::getPriority(void) const{
	//the same as for setPriority
	return 0;
}

void COPC_UA_Handler::setServerRunning(){
	*mbServerRunning = UA_TRUE;
}

void COPC_UA_Handler::stopServerRunning(){
	*mbServerRunning = UA_FALSE;
}

UA_StatusCode COPC_UA_Handler::getNodeForPath(UA_NodeId **foundNodeId, char* nodePath, bool createIfNotFound, const UA_NodeId *startingNode, const UA_NodeId *newNodeType) {

	*foundNodeId = NULL;
	// remove tailing slash
	size_t pathLen = strlen(nodePath);
	while (pathLen && nodePath[pathLen-1] == '/') {
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
	char *tok = strtok (nodePath,"/");
	if (startingNode == NULL || (startingNode->namespaceIndex == 0 && startingNode->identifier.numeric == UA_NS0ID_OBJECTSFOLDER)) {
		if (strcmp(tok, "Objects") != 0 && strcmp(tok, "0:Objects") != 0) {
			DEVLOG_ERROR("Node path '%s' has to start with '/Objects'", fullPath);
			forte_free(fullPath);
			return UA_STATUSCODE_BADINVALIDARGUMENT;
		}
		folderCnt--; //remaining count without Objects folder
		tok = strtok(NULL,"/"); // skip objects folder
	}

	// create a client for requesting the nodes
	UA_Client *client = UA_Client_new(UA_ClientConfig_standard);

	char localEndpoint[28];
	snprintf(localEndpoint,28, "opc.tcp://localhost:%d", FORTE_COM_OPC_UA_PORT);

	{
		UA_StatusCode retVal;
		if((retVal = UA_Client_connect(client, localEndpoint)) != UA_STATUSCODE_GOOD) {
			DEVLOG_ERROR("Could not connect to local OPC UA Server");
			UA_Client_delete(client);
			forte_free(fullPath);
			return retVal;
		}
	}


	// for every folder (which is a BrowsePath) we want to get the node id
	UA_BrowsePath *browsePaths = static_cast<UA_BrowsePath*>(forte_malloc(sizeof(UA_BrowsePath) * folderCnt));

	for (unsigned int i=0; i<folderCnt; i++) {
		UA_BrowsePath_init(&browsePaths[i]);
		if (startingNode != NULL)
			browsePaths[i].startingNode = *startingNode;
		else
			browsePaths[i].startingNode = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
		browsePaths[i].relativePath.elementsSize = i+1;
		browsePaths[i].relativePath.elements = static_cast<UA_RelativePathElement*>(forte_malloc(sizeof(UA_RelativePathElement)*(i+1)));
		for (unsigned int j=0; j<=i; j++) {

			if (j<i) {
				// just copy from before
				UA_RelativePathElement_copy(&browsePaths[i-1].relativePath.elements[j], &browsePaths[i].relativePath.elements[j]);
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
		tok = strtok(NULL,"/");
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
			DEVLOG_ERROR("Could not translate browse paths for '%s' to node IDs. Service returned: 0x%08x", fullPath, response.responseHeader.serviceResult);
			UA_TranslateBrowsePathsToNodeIdsRequest_deleteMembers(&request);
			UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&response);
			UA_Client_disconnect(client);
			UA_Client_delete(client);
			forte_free(fullPath);
			return retVal;
		}

		if (response.resultsSize != folderCnt) {
			DEVLOG_ERROR("Could not translate browse paths for '%s' to node IDs. resultSize (%d) != expected count (%d)", fullPath, response.resultsSize,
						 folderCnt);
			UA_TranslateBrowsePathsToNodeIdsRequest_deleteMembers(&request);
			UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&response);
			UA_Client_disconnect(client);
			UA_Client_delete(client);
			forte_free(fullPath);
			return UA_STATUSCODE_BADUNEXPECTEDERROR;
		}

		*foundNodeId = nullptr;
		UA_StatusCode retVal = UA_STATUSCODE_GOOD;

		if (response.results[folderCnt-1].statusCode == UA_STATUSCODE_GOOD) {
			*foundNodeId = UA_NodeId_new();
			UA_NodeId_copy(&response.results[folderCnt-1].targets[0].targetId.nodeId, *foundNodeId);
		} else if (createIfNotFound) {
			// last node does not exist, and we should create the path
			// skip last node because we already know that it doesn't exist

			UA_NodeId creationType;

			if (newNodeType == NULL)
				creationType = UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE);
			else
				creationType = *newNodeType;

			*foundNodeId = UA_NodeId_new();
			int i;
			for (i = folderCnt - 2; i >= 0; i--) {
				if (response.results[i].statusCode != UA_STATUSCODE_GOOD) {
					// find first existing node
					continue;
				}
				// now we found the first existing node
				if (response.results[i].targetsSize == 0) {
					DEVLOG_ERROR("Could not translate browse paths for '%s' to node IDs. target size is 0.", fullPath);
					break;
				}
				if (response.results[i].targetsSize > 1) {
					DEVLOG_WARNING("The given browse path '%s' has multiple results for the same path. Taking the first result.", fullPath);
				}

				// foundNodeId contains the ID of the parent which exists
				UA_NodeId_copy(&response.results[i].targets[0].targetId.nodeId, *foundNodeId);
				break;
			}

			if (i==-1) {
				// no node of the path exists, thus parent is Objects folder
				UA_NodeId_copy(&browsePaths[0].startingNode, *foundNodeId);
			}
			i++;

			// create all the nodes on the way
			for (unsigned int j=(unsigned int)i; j<folderCnt; j++) {

				// the last browse path contains all relativePath elements.
				UA_QualifiedName *targetName = &request.browsePaths[folderCnt-1].relativePath.elements[j].targetName;

				UA_ObjectAttributes oAttr;
				UA_ObjectAttributes_init(&oAttr);
				char locale[] = "en_US";
				char *nodeName = static_cast<char*>(forte_malloc(sizeof(char)*targetName->name.length+1));
				memcpy(nodeName, targetName->name.data, targetName->name.length);
				nodeName[targetName->name.length] = 0;
				oAttr.description = UA_LOCALIZEDTEXT(locale, nodeName);
				oAttr.displayName = UA_LOCALIZEDTEXT(locale, nodeName);
				if ((retVal = UA_Server_addObjectNode(mOPCUAServer, UA_NODEID_NUMERIC(1, 0),
										**foundNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
										*targetName, creationType, oAttr, NULL, *foundNodeId)) != UA_STATUSCODE_GOOD) {
					DEVLOG_ERROR("Could not addObjectNode. Status: 0x%08x", retVal );
					forte_free(*foundNodeId);
					*foundNodeId = nullptr;
					break;
				}
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

UA_StatusCode COPC_UA_Handler::createUAVariableNode(const UA_NodeId *parentNode, const char* varName, const UA_DataType* varType, void* varValue, UA_NodeId * returnVarNodeId){


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
	UA_Variant_setScalar(&var_attr.value, varValue, varType);

	UA_NodeId * returnNodeId = UA_NodeId_new();

	// add UA Variable Node to the server address space
	UA_StatusCode retVal = UA_Server_addVariableNode(
			mOPCUAServer,                 // server
			UA_NODEID_NUMERIC(1,0),              	  // requestedNewNodeId
			*parentNode,                 // parentNodeId
			UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),        	  // referenceTypeId   Reference to the type definition for the variable node
			varBrowseName,                // browseName
			typeDefinition,               // typeDefinition
			var_attr,                     // Variable attributes
			NULL,                 // instantiation callback
			returnNodeId);			  	  // return Node Id


	if(retVal == UA_STATUSCODE_GOOD){
		DEVLOG_INFO("UA-Server AddressSpace: New Variable Node - %s added.\n", varName);
		retVal = UA_NodeId_copy(returnNodeId, returnVarNodeId);
	}else{
		DEVLOG_ERROR("UA-Server AddressSpace: Adding Variable Node %s failed. Status: 0x%08x\n", varName, retVal);
	}
	return retVal;
}

/*
 * Update UA Address Space node value given by the data pointer to an IEC61499 data object.
 * Mapping of IEC61499 to OPC-UA types is performed by scmUADataTypeMapping array.
 */
UA_StatusCode COPC_UA_Handler::updateNodeValue(const UA_NodeId *nodeId, const CIEC_ANY *data, const UA_TypeConvert *convert){
	UA_Variant* nodeValue = UA_Variant_new();
	UA_Variant_init(nodeValue);

	void *varValue = UA_new(convert->type);
	if (!convert->get(data, varValue)) {
		UA_delete(varValue, convert->type);
		return UA_STATUSCODE_BADUNEXPECTEDERROR;
	}

	UA_Variant_setScalarCopy(nodeValue, varValue, convert->type);
	UA_StatusCode retVal = UA_Server_writeValue(mOPCUAServer, *nodeId, *nodeValue);
	UA_delete(varValue, convert->type);
	return retVal;
}

struct UA_NodeCallback_Handle {
	forte::com_infra::CComLayer *comLayer;
	const struct UA_TypeConvert* convert;
	unsigned int portIndex;
};

UA_StatusCode COPC_UA_Handler::registerNodeCallBack(const UA_NodeId *nodeId, forte::com_infra::CComLayer *comLayer, const struct UA_TypeConvert* convert,
													unsigned int portIndex){
	struct UA_NodeCallback_Handle handle = {
		comLayer: comLayer,
		convert: convert,
		portIndex: portIndex
	};

	UA_ValueCallback callback = {static_cast<void *>(&handle), NULL, COPC_UA_Handler().getInstance().onWrite};
	return UA_Server_setVariableNode_valueCallback(mOPCUAServer, *nodeId, callback);
}


void COPC_UA_Handler::onWrite(void *handleRaw, const UA_NodeId nodeid, const UA_Variant *data, const UA_NumericRange *range){

	struct UA_NodeCallback_Handle *handle = static_cast<struct UA_NodeCallback_Handle *>(handleRaw);

	CComLayer *layer = handle->comLayer;

	EComResponse retVal;

	struct recvData_handle {
		const struct UA_TypeConvert* convert;
		unsigned int portIndex;
		const UA_Variant *data;
	} handleRecv;

	handleRecv.data = data;
	handleRecv.portIndex = handle->portIndex;
	handleRecv.convert = handle->convert;

	retVal = layer->recvData(static_cast<const void *>(&handleRecv), 0);	//TODO: add multidimensional data handling with 'range'.

	/* Handle return of receive data */
	if(e_ProcessDataOk == retVal){
		//only update data in item if data could be read
	}

	if(e_Nothing != retVal){
		getInstance().startNewEventChain(layer->getCommFB());
	}

}







