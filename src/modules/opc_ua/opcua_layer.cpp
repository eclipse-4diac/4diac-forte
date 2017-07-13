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



//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-parameter"
//#include <src_generated/ua_namespaceinit_generated.h>
//#pragma GCC diagnostic pop
#include <criticalregion.h>
#include "opcua_layer.h"
#include "opcua_handler.h"

using namespace forte::com_infra;

// method call timeout in seconds. Within this time the method call has to return a value
#define METHOD_CALL_TIMEOUT 4

struct AsyncCallPayload {
	UA_Variant *variants;
	unsigned int variantsSize;
};

#if _MSC_VER < 1900 
#define snprintf _snprintf
#endif

COPC_UA_Layer::COPC_UA_Layer(CComLayer *pa_poUpperLayer, CCommFB *pa_poComFB) : CComLayerAsync(pa_poUpperLayer, pa_poComFB),
																				clientSdConverter(NULL), clientRdConverter(NULL),
																				mInterruptResp(e_Nothing), fbNodeId(NULL), fbNodeIdParent(NULL),
																				methodNodeId(NULL),	sendDataNodeIds(NULL), readDataNodeIds(NULL),
																				clientEndpointUrl(NULL), clientMethodPath(NULL),
																				clientMutex(NULL),
																				mutexServerMethodCall(), serverMethodCallResultReady(false) {
	// constructor list initialization

}


COPC_UA_Layer::~COPC_UA_Layer() {
	// all the stuff is cleaned up in closeConnection()
}

EComResponse COPC_UA_Layer::openConnection(char *paLayerParameter) {

	if (fbNodeId != NULL) {
		DEVLOG_WARNING("OPC UA: Already connected. Nothing to do.\n");
		return e_InitTerminated;
	}

	// Create all the nodes up to the given node (ID parameter of the FB)
	if (getCommFB()->getComServiceType() != e_Client)
	{
		UA_StatusCode retVal;
		if ((retVal = COPC_UA_Handler::getInstance().getNodeForPath(&fbNodeId, paLayerParameter, true)) != UA_STATUSCODE_GOOD) {
			DEVLOG_ERROR("OPC UA: Could not get node for path: '%s': %s\n", paLayerParameter, UA_StatusCode_explanation(retVal));
			return e_InitTerminated;
		}
	}


	switch (getCommFB()->getComServiceType()) {
		case e_Publisher:
			// Make sure all the nodes exist and have the corresponding variable
			DEVLOG_DEBUG("OPC UA: Creating OPC UA Nodes for publisher\n");
			return this->createPubSubNodes(&this->sendDataNodeIds, getCommFB()->getNumSD(), true);
		case e_Subscriber:
			DEVLOG_DEBUG("OPC UA: Creating OPC UA Nodes for subscriber\n");
			return this->createPubSubNodes(&this->readDataNodeIds, getCommFB()->getNumRD(), false);
		case e_Server:
			DEVLOG_DEBUG("OPC UA: Creating OPC UA Method for server\n");
			return this->createMethodNode();
		case e_Client:
			DEVLOG_DEBUG("OPC UA: Creating OPC UA Client\n");
			return this->createClient(paLayerParameter);
		default:
			DEVLOG_WARNING("OPC UA: Invalid Comm Service Type for Function Block\n");
	}
	return e_InitOk;
}

void COPC_UA_Layer::closeConnection() {
	if (fbNodeId != NULL) {
		UA_NodeId_delete(fbNodeId);
		fbNodeId = NULL;
	}
	if (methodNodeId != NULL) {
		UA_NodeId_delete(methodNodeId);
		methodNodeId = NULL;
	}
	this->deleteNodeIds(&sendDataNodeIds, getCommFB()->getNumSD());
	this->deleteNodeIds(&readDataNodeIds, getCommFB()->getNumRD());
	if (clientSdConverter) {
		forte_free(clientSdConverter);
		clientSdConverter = NULL;
	}
	if (clientRdConverter) {
		forte_free(clientRdConverter);
		clientRdConverter = NULL;
	}
	if (clientEndpointUrl) {
		forte_free(clientEndpointUrl);
		clientEndpointUrl = NULL;
	}
	if (clientMethodPath) {
		forte_free(clientMethodPath);
		clientMethodPath = NULL;
	}
	if (fbNodeIdParent) {
		UA_NodeId_delete(fbNodeIdParent);
		fbNodeIdParent = NULL;
	}
}

/**
 * This method is required to inline initialize the connection point.
 */
static const SConnectionPoint& getFirstListEntry(const CSinglyLinkedList<SConnectionPoint>& list) {
	CSinglyLinkedList<SConnectionPoint>::Iterator it = list.begin();
	return *it;
}

void COPC_UA_Layer::deleteNodeIds(struct FB_NodeIds **nodeIds, unsigned int currentSize) {
	if (*nodeIds == NULL)
		return;
	for (unsigned int j = 0; j < currentSize; j++) {
		if ((*nodeIds)[j].functionBlockId != NULL)
			UA_NodeId_delete((*nodeIds)[j].functionBlockId);
		if ((*nodeIds)[j].functionBlockId != NULL)
			UA_NodeId_delete((*nodeIds)[j].variableId);
	}
	delete[](*nodeIds);
	(*nodeIds) = NULL;
}

bool COPC_UA_Layer::getPortConnectionInfo(unsigned int portIndex, bool isSD, const CFunctionBlock **connectedToFb, const char **connectedToPortName,
										  const UA_TypeConvert **convert) const {
	int portId = portIndex + 2;
	/* Retrieve Publisher, Connection and Signals Source Function Block Information */
	const SFBInterfaceSpec *interfaceSpec = getCommFB()->getFBInterfaceSpec();
	const CStringDictionary::TStringId portNameId = isSD ? interfaceSpec->m_aunDINames[portId] : interfaceSpec->m_aunDONames[portId];

	DEVLOG_DEBUG("OPC UA: Processing %s signal %s at port %i.\n", isSD ? "publish" : "subscribe", CStringDictionary::getInstance().get(portNameId), portId);

	const CDataConnection *portConnection = isSD ? getCommFB()->getDIConnection(portNameId) : getCommFB()->getDOConnection(portNameId);
	if (portConnection == NULL) {
		DEVLOG_ERROR("OPC UA: Got invalid port connection at port %d\n", portId);
		return false;
	}


	//TODO for now we assume that the subscriber connection only has one destination. Needs fix!

	if (!isSD && portConnection->getDestinationList().isEmpty()) {
		DEVLOG_WARNING("OPC UA: Subscriber does not have any connection.\n");
		return false;
	}


	const SConnectionPoint remoteConnectionPoint = isSD ? portConnection->getSourceId() : getFirstListEntry(portConnection->getDestinationList());
	if ((*connectedToFb = remoteConnectionPoint.mFB) == NULL) {
		return false;
	}
	const SFBInterfaceSpec *sourceInterfaceSpec = (*connectedToFb)->getFBInterfaceSpec();
	const CStringDictionary::TStringId sourceNameId = isSD ? sourceInterfaceSpec->m_aunDONames[remoteConnectionPoint.mPortId]
														   : sourceInterfaceSpec->m_aunDINames[remoteConnectionPoint.mPortId];
	*connectedToPortName = CStringDictionary::getInstance().get(sourceNameId);


	const CIEC_ANY *remotePort = isSD ? (*connectedToFb)->getDOFromPortId(remoteConnectionPoint.mPortId)
									  : (*connectedToFb)->getDIFromPortId(remoteConnectionPoint.mPortId);

	if (!COPC_UA_Helper::isTypeIdValid(remotePort->getDataTypeID())) {
		const CStringDictionary::TStringId connectedToType = isSD ? sourceInterfaceSpec->m_aunDODataTypeNames[remoteConnectionPoint.mPortId]
																  : sourceInterfaceSpec->m_aunDIDataTypeNames[remoteConnectionPoint.mPortId];
		DEVLOG_ERROR("OPC UA: Mapping of type %s to OPC UA not defined.\n", CStringDictionary::getInstance().get(connectedToType));
		return false;
	}
	*convert = &COPC_UA_Helper::mapForteTypeIdToOpcUa[remotePort->getDataTypeID()];

	return true;
}

forte::com_infra::EComResponse COPC_UA_Layer::createPubSubNodes(struct FB_NodeIds **nodeIds, unsigned int numPorts, bool isSD) {
	if (*nodeIds != NULL) {
		DEVLOG_ERROR("OPC UA: Publish/Subscribe Nodes already initialized.\n");
		return e_InitInvalidId;
	}

	if (numPorts == 0) {
		DEVLOG_ERROR("OPC UA: OPC UA Publisher/Subscriber without SD/RD Signal, pure event handling\n");
		return e_InitInvalidId;
	}

	*nodeIds = (struct FB_NodeIds *)forte_malloc(sizeof(struct FB_NodeIds)*numPorts);

	for (unsigned int i = 0; i < numPorts; i++) {

		const CFunctionBlock *connectedToFb = NULL;
		const char *connectedToName = NULL;
		const UA_TypeConvert *conv;

		if (!getPortConnectionInfo(i, isSD, &connectedToFb, &connectedToName, &conv)) {
			deleteNodeIds(nodeIds, i);
			return e_InitInvalidId;
		}


		UA_StatusCode retVal;

		{
			// create/get node for connected FB
			size_t len = strlen(connectedToFb->getInstanceName()) + 2; // include slash and nullbyte
			char *fbBrowseName = (char*)forte_malloc(sizeof(char)*len);
			snprintf(fbBrowseName, len, "/%s", connectedToFb->getInstanceName());
			UA_NodeId newNodeType = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE);
			retVal = COPC_UA_Handler::getInstance().getNodeForPath(&(*nodeIds)[i].functionBlockId, fbBrowseName, true, fbNodeId, &newNodeType);
			forte_free(fbBrowseName);
		}

		if (retVal == UA_STATUSCODE_GOOD) {
			// create/get variable node for port on connected FB
			size_t len = strlen(connectedToName) + 2; // include slash and nullbyte
			char *sourceVarBrowseName = (char*)forte_malloc(sizeof(char)*len);
			snprintf(sourceVarBrowseName, len, "/%s", connectedToName);
			retVal = COPC_UA_Handler::getInstance().getNodeForPath(&(*nodeIds)[i].variableId, sourceVarBrowseName, false,
																   (*nodeIds)[i].functionBlockId);
			forte_free(sourceVarBrowseName);
			if (retVal == UA_STATUSCODE_GOOD && (*nodeIds)[i].variableId == NULL) {
				// we need to create the variable

				(*nodeIds)[i].convert = conv;
				void *varValue = UA_new(conv->type);
				UA_init(varValue, conv->type);
				(*nodeIds)[i].variableId = UA_NodeId_new();
				retVal = COPC_UA_Handler::getInstance().createVariableNode((*nodeIds)[i].functionBlockId, connectedToName, conv->type,
																		   varValue, (*nodeIds)[i].variableId, !isSD);
				UA_delete(varValue, conv->type);
				if (retVal == UA_STATUSCODE_GOOD && !isSD) {
					COPC_UA_Handler::getInstance().registerNodeCallBack((*nodeIds)[i].variableId, this, conv, i);
				}
			} // else if retVal = UA_STATUSCODE_GOOD the node already exists
		}

		if (retVal != UA_STATUSCODE_GOOD) {
			deleteNodeIds(nodeIds, i + 1);
			return e_InitInvalidId;
		}
	}
	return e_InitOk;
}


forte::com_infra::EComResponse COPC_UA_Layer::createMethodNode() {

	DEVLOG_DEBUG("OPC UA: OPC UA creating method for %s\n", getCommFB()->getInstanceName());

	if (sendDataNodeIds != NULL || readDataNodeIds != NULL) {
		DEVLOG_ERROR("OPC UA: Method node already initialized.\n");
		return e_InitInvalidId;
	}

	if (this->getCommFB()->getNumRD() == 0 && this->getCommFB()->getNumSD() == 0) {
		DEVLOG_INFO("OPC UA: OPC UA Method without SD/RD Signal, pure event handling\n");
	}

	// create the list of input arguments of the method which corresponds to the RD ports (i.e. output of the FB)
	UA_Argument *inputArguments;
	if (createMethodArguments(&inputArguments, getCommFB()->getNumRD(), false) != e_InitOk) {
		return e_InitInvalidId;
	}

	UA_Argument *outputArguments;
	if (createMethodArguments(&outputArguments, getCommFB()->getNumSD(), true) != e_InitOk) {
		UA_Array_delete(inputArguments, getCommFB()->getNumRD(), &UA_TYPES[UA_TYPES_ARGUMENT]);
		return e_InitInvalidId;
	}

	forte::com_infra::EComResponse result = e_InitOk;

	UA_StatusCode retVal;
	if ((retVal = COPC_UA_Handler::getInstance().createMethodNode(fbNodeId, 1, getCommFB()->getInstanceName(), COPC_UA_Layer::onServerMethodCall, this,
																  getCommFB()->getNumRD(),
																  inputArguments, getCommFB()->getNumSD(), outputArguments, methodNodeId)) !=
		UA_STATUSCODE_GOOD) {
		DEVLOG_ERROR("OPC UA: OPC UA could not create method node: %s - %s\n", UA_StatusCode_name(retVal), UA_StatusCode_description(retVal));
		result = e_InitInvalidId;
	}
	UA_Array_delete(inputArguments, getCommFB()->getNumRD(), &UA_TYPES[UA_TYPES_ARGUMENT]);
	UA_Array_delete(outputArguments, getCommFB()->getNumSD(), &UA_TYPES[UA_TYPES_ARGUMENT]);

	return result;
}


forte::com_infra::EComResponse COPC_UA_Layer::createMethodArguments(UA_Argument **arguments, unsigned int numPorts, bool isSD) {
	*arguments = static_cast<UA_Argument *>(UA_Array_new(numPorts, &UA_TYPES[UA_TYPES_ARGUMENT]));

	for (unsigned int i = 0; i < numPorts; i++) {

		UA_Argument *arg = &(*arguments)[i];
		UA_Argument_init(arg);
		arg->arrayDimensionsSize = 0;
		arg->arrayDimensions = NULL;

		const CFunctionBlock *connectedToFb = NULL;
		const char *connectedToName = NULL;
		const UA_TypeConvert *conv;

		if (!getPortConnectionInfo(i, isSD, &connectedToFb, &connectedToName, &conv)) {
			UA_Array_delete(*arguments, numPorts, &UA_TYPES[UA_TYPES_ARGUMENT]);
			return e_InitInvalidId;
		}

		arg->dataType = conv->type->typeId;
		arg->description = UA_LOCALIZEDTEXT_ALLOC("en_US", "Method parameter");
		arg->name = UA_STRING_ALLOC(connectedToFb->getInstanceName());
		arg->valueRank = -1;

	}

	return e_InitOk;
}

forte::com_infra::EComResponse COPC_UA_Layer::clientCreateConverter(const UA_TypeConvert **converterList[], unsigned int numPorts, bool isSD) {
	if (*converterList != NULL) {
		DEVLOG_ERROR("OPC UA: Converter list already initialized.\n");
		return e_InitInvalidId;
	}

	*converterList = (const UA_TypeConvert **)forte_malloc(sizeof(UA_TypeConvert*)*numPorts);

	for (unsigned int i = 0; i < numPorts; i++) {

		const CFunctionBlock *connectedToFb = NULL;
		const char *connectedToName = NULL;

		if (!getPortConnectionInfo(i, isSD, &connectedToFb, &connectedToName, &(*converterList)[i])) {
			forte_free(*converterList);
			return e_InitInvalidId;
		}
	}
	return e_InitOk;
}

forte::com_infra::EComResponse COPC_UA_Layer::clientInit() {
	if (fbNodeId != NULL || fbNodeIdParent != NULL)
		return e_InitOk;

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
	// other thready may currently create nodes for the same path, thus mutex
	CCriticalRegion criticalRegion(*this->clientMutex);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

	if (UA_Client_getState(uaClient) != UA_CLIENTSTATE_CONNECTED) {
		UA_StatusCode retVal = UA_Client_connect(uaClient, clientEndpointUrl);

		if (retVal != UA_STATUSCODE_GOOD) {
			DEVLOG_ERROR("OPC UA: Could not connect client to endpoint %s. Error: %s\n", clientEndpointUrl, UA_StatusCode_description(retVal));
			UA_Client_delete(uaClient);
			return e_InitTerminated;
		}
	}

	UA_StatusCode retVal = COPC_UA_Handler::getInstance().getNodeForPath(&fbNodeId, clientMethodPath, false, NULL, NULL, &fbNodeIdParent, this->uaClient);
	if (retVal != UA_STATUSCODE_GOOD) {
		DEVLOG_ERROR("OPC UA: Could not get node for path from server '%s': '%s'. %s\n", clientEndpointUrl, clientMethodPath, UA_StatusCode_explanation(retVal));
		return e_InitTerminated;
	}

	if (fbNodeId == NULL) {
		DEVLOG_ERROR("OPC UA: Could not find the method node on the server '%s': '%s'\n", clientEndpointUrl, clientMethodPath);
		return e_InitTerminated;
	}

	if (fbNodeIdParent == NULL) {
		DEVLOG_ERROR("OPC UA: Could not find the parent node of the method on the server '%s': '%s'\n", clientEndpointUrl, clientMethodPath);
		return e_InitTerminated;
	}

	return e_InitOk;
}

forte::com_infra::EComResponse COPC_UA_Layer::createClient(const char* paLayerParameter) {
	// split the ID parameter:
	// opc.tcp://10.100.1.0:4840#/Objects/1:Adder
	// between the hash sign

	if (clientEndpointUrl != NULL || clientMethodPath != NULL) {
		DEVLOG_ERROR("OPC UA: Client already initialized.");
		return e_InitTerminated;
	}

	// start the async call thread
	this->start();

	char *idStr = strdup(paLayerParameter);

	char *endpointUrl;
	char *nodePath;

	endpointUrl = strtok(idStr, "#");
	if (!endpointUrl) {
		DEVLOG_ERROR("OPC UA: Invalid client identifier. Endpoint URL must be separated by hash '#' from the node path. Given: %s\n", paLayerParameter);
		forte_free(idStr);
		return e_InitTerminated;
	}
	nodePath = strtok(NULL, "#");
	if (!nodePath) {
		DEVLOG_ERROR("OPC UA: Invalid client identifier. Node path must be given after the hash '#'. Given: %s\n", paLayerParameter);
		forte_free(idStr);
		return e_InitTerminated;
	}

	this->uaClient = COPC_UA_Handler::getInstance().getClientForEndpoint(endpointUrl, true, &clientMutex);
	if (!this->uaClient)
		return e_InitTerminated;

	// construct node path which includes the method name, i.e., the FB name

	size_t nodePathLen = strlen(nodePath);
	// remove tailing slash
	while (nodePathLen && nodePath[nodePathLen - 1] == '/') {
		nodePathLen--;
	}

	// additional nullbyte in length
	char *fullNodePath = (char*)forte_malloc(sizeof(char) * (nodePathLen+2));

	snprintf(fullNodePath,nodePathLen+1,"%.*s", (int)nodePathLen, nodePath);

	this->clientMethodPath = fullNodePath;
	this->clientEndpointUrl = strdup(endpointUrl);

	forte_free(idStr);

	forte::com_infra::EComResponse resp = clientCreateConverter(&clientRdConverter, getCommFB()->getNumRD(), false);
	if (resp != e_InitOk)
		return resp;

	return clientCreateConverter(&clientSdConverter, getCommFB()->getNumSD(), true);

}

EComResponse COPC_UA_Layer::sendData(void *paData, unsigned int paSize) {

	EComResponse retVal = e_ProcessDataOk;

	if (getCommFB()->getComServiceType() == e_Server) {
		// continue method call
		serverMethodCallResultReady = true;
		return retVal;
	}

	if (getCommFB()->getComServiceType() == e_Client) {
		return this->clientCallMethod(paSize ? static_cast<CIEC_ANY *>(paData) : NULL, paSize);
	}

	if (paSize == 0) {
	} else {
		const CIEC_ANY *SDs(static_cast<CIEC_ANY *>(paData));

		for (unsigned int i = 0; i < paSize; i++) {
			FB_NodeIds *ni = &this->sendDataNodeIds[i];
			if (COPC_UA_Handler::getInstance().updateNodeValue(ni->variableId, &SDs[i], ni->convert) != UA_STATUSCODE_GOOD) {
				DEVLOG_ERROR("OPC UA: Could not convert publisher value for port %d to OPC UA.\n", i);
				retVal = e_ProcessDataDataTypeError;
			}
		}
	}
	return retVal;
}


EComResponse COPC_UA_Layer::recvData(const void *pa_pvData, unsigned int) {
	mInterruptResp = e_ProcessDataRecvFaild;

	struct recvData_handle {
		const struct UA_TypeConvert *convert;
		unsigned int portIndex;
		const UA_Variant *data;
	};
	const struct recvData_handle *handleRecv = static_cast<const recvData_handle *>(pa_pvData);


	if (getCommFB()->getNumRD() == 0) {
		mInterruptResp = e_ProcessDataOk;
	} else {
		if (UA_Variant_isScalar(handleRecv->data) && handleRecv->data->type == handleRecv->convert->type && handleRecv->data->data) {
			if (handleRecv->convert->set(handleRecv->data->data, &getCommFB()->getRDs()[handleRecv->portIndex])) {
				mInterruptResp = e_ProcessDataOk;
				getCommFB()->interruptCommFB(this);
			}
		}
	}

	return mInterruptResp;
}

forte::com_infra::EComResponse COPC_UA_Layer::clientCallMethod(const CIEC_ANY *sd, unsigned int sdSize) {

	UA_Variant *inputs = (UA_Variant *)UA_Array_new(sdSize, &UA_TYPES[UA_TYPES_VARIANT]);

	for (unsigned int i=0; i<sdSize; i++) {
		UA_Variant_init(&inputs[i]);
		void *varValue = UA_new(clientSdConverter[i]->type);
		UA_init(varValue, clientSdConverter[i]->type);
		if (!clientSdConverter[i]->get(&sd[i], varValue)) {
			DEVLOG_ERROR("OPC UA: Client could not convert input SD_%d to OPC UA.\n", i+1);
			UA_delete(varValue, clientSdConverter[i]->type);
			UA_Array_delete(inputs, sdSize, &UA_TYPES[UA_TYPES_VARIANT]);
			return e_ProcessDataDataTypeError;
		}
		UA_Variant_setScalarCopy(&inputs[i], varValue, clientSdConverter[i]->type);
		UA_delete(varValue, clientSdConverter[i]->type);
	}

	struct AsyncCallPayload *payload = (struct AsyncCallPayload*)forte_malloc(sizeof(AsyncCallPayload));
	payload->variants = inputs;
	payload->variantsSize = sdSize;
	if (callAsync(payload) == 0)
		return e_ProcessDataSendFailed;
	else
		return e_Nothing;
}


EComResponse COPC_UA_Layer::processInterruptChild() {
	return mInterruptResp;
}


#ifdef FORTE_COM_OPC_UA_VERSION_0_2
UA_StatusCode COPC_UA_Layer::onServerMethodCall(void *methodHandle, const UA_NodeId,
						size_t inputSize, const UA_Variant *input,
						size_t outputSize, UA_Variant *output) {
#else
UA_StatusCode COPC_UA_Layer::onServerMethodCall(void *methodHandle, const UA_NodeId *,
						const UA_NodeId *, void *,
						size_t inputSize, const UA_Variant *input,
						size_t outputSize, UA_Variant *output) {
#endif
	DEVLOG_DEBUG("OPC UA: OPC UA Server method call start.\n");
	COPC_UA_Layer *self = static_cast<COPC_UA_Layer *>(methodHandle);

#ifndef VXWORKS
#ifdef __GNUC__
#pragma GCC diagnostic push //TODO: are these pragmas really necessary?
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
#endif
	// other thready may currently create nodes for the same path, thus mutex
	CCriticalRegion criticalRegion(self->mutexServerMethodCall);
#ifndef VXWORKS
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
#endif
	self->serverMethodCallResultReady = false;

	if (inputSize != self->getCommFB()->getNumRD() || outputSize != self->getCommFB()->getNumSD()) {
		DEVLOG_ERROR("OPC UA: method call got invalid number of arguments. In: %d==%d, Out: %d==%d\n", self->getCommFB()->getNumRD(), inputSize,
					 self->getCommFB()->getNumSD(), outputSize);
		return UA_STATUSCODE_BADINVALIDARGUMENT;
	}


	self->mInterruptResp = e_ProcessDataOk;

	// put the input values on the wire, i.e. on the RD ports
	for (unsigned int i = 0; i < self->getCommFB()->getNumRD(); i++) {
		if (UA_Variant_isScalar(&input[i]) && input[i].data != NULL) {
			if (!COPC_UA_Helper::convertFromUa_typeId(self->getCommFB()->getRDs()[i].getDataTypeID(), input[i].data, &self->getCommFB()->getRDs()[i])) {
				self->mInterruptResp = e_ProcessDataRecvFaild;
				DEVLOG_ERROR("OPC UA: can not convert method inputArgument at idx %d to forte type\n", i);
				break;
			}
		}
	}


	if (self->mInterruptResp == e_ProcessDataOk) {
		self->getCommFB()->interruptCommFB(self);
		COPC_UA_Handler::getInstance().forceEventHandling(self);
	} else {
		return UA_STATUSCODE_BADINVALIDARGUMENT;
	}

	DEVLOG_DEBUG("OPC UA: server method call waiting for RSP event.\n");

	// wait until result is ready
	CIEC_DATE_AND_TIME startTime;
	startTime.setCurrentTime();
	CIEC_DATE_AND_TIME currentTime;
	currentTime.setCurrentTime();
	// TODO uses busy waiting. Is there a better way?
	while (!self->serverMethodCallResultReady && currentTime.getMilliSeconds() - startTime.getMilliSeconds() < METHOD_CALL_TIMEOUT * 1000) {
		currentTime.setCurrentTime();
	}
	if (currentTime.getMilliSeconds() - startTime.getMilliSeconds() >= METHOD_CALL_TIMEOUT * 1000) {
		DEVLOG_ERROR("OPC UA: method call did not get result values within timeout.\n");
		return UA_STATUSCODE_BADTIMEOUT;
	}

	DEVLOG_DEBUG("OPC UA: Server method call got RSP event.\n");
	self->serverMethodCallResultReady = false;

	// copy SD values to output
	for (unsigned int i = 0; i < self->getCommFB()->getNumSD(); i++) {


		if (!COPC_UA_Helper::isTypeIdValid(self->getCommFB()->getSDs()[i].getDataTypeID())) {
			DEVLOG_ERROR("OPC UA: can not convert forte type to method outputArgument at idx %d\n", i);
			break;
		}

		const UA_TypeConvert *convert = &COPC_UA_Helper::mapForteTypeIdToOpcUa[self->getCommFB()->getSDs()[i].getDataTypeID()];

		void *varValue = UA_new(convert->type);
		if (!convert->get(&self->getCommFB()->getSDs()[i], varValue)) {
			self->mInterruptResp = e_ProcessDataRecvFaild;
			DEVLOG_ERROR("OPC UA: can not convert forte type to method outputArgument at idx %d\n", i);
		} else {
			UA_Variant_setScalarCopy(&output[i], varValue, convert->type);
		}

		UA_delete(varValue, convert->type);

		if (self->mInterruptResp != e_ProcessDataOk)
			break;
	}

	DEVLOG_DEBUG("OPC UA: Server method call end.\n");

	return self->mInterruptResp == e_ProcessDataOk ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADUNEXPECTEDERROR;
}

void *COPC_UA_Layer::handleAsyncCall(const unsigned int /*callId*/, void *payload) {


	DEVLOG_DEBUG("OPC UA: Client connecting\n");

	if (this->clientInit() != e_InitOk)
		return NULL;

	struct AsyncCallPayload *inputData = (struct AsyncCallPayload*)payload;

	size_t outputSize;
	UA_Variant *output;
	clientMutex->lock();

	DEVLOG_DEBUG("OPC UA: Calling method\n");

	UA_StatusCode retval = UA_Client_call(this->uaClient, *this->fbNodeIdParent,
										  *this->fbNodeId, inputData->variantsSize, inputData->variants, &outputSize, &output);
	clientMutex->unlock();
	if(retval == UA_STATUSCODE_GOOD) {
		DEVLOG_DEBUG("OPC UA: Method call was successfull, and %lu returned values available.\n",
					 (unsigned long)outputSize);

		if (getCommFB()->getNumRD() != outputSize) {
			DEVLOG_ERROR("OPC UA: The number of RD connectors of the client does not match the number of returned values from the method call.\n");
		} else {

			struct AsyncCallPayload *outputData = (struct AsyncCallPayload *)forte_malloc(sizeof(struct AsyncCallPayload));

			outputData->variants = output;
			outputData->variantsSize = (unsigned int) outputSize;

			return outputData;
		}

	}

	DEVLOG_ERROR("OPC UA: Could not call method. Error: %s - %s\n", UA_StatusCode_name(retval), UA_StatusCode_description(retval));
	return NULL;
}

void COPC_UA_Layer::handleAsyncCallResult(const unsigned int /*callId*/, void *payload, void *result) {

	struct AsyncCallPayload *inputData = (struct AsyncCallPayload*)payload;
	UA_Array_delete(inputData->variants, inputData->variantsSize, &UA_TYPES[UA_TYPES_VARIANT]);
	forte_free(payload);

	if (result == NULL)
		return;

	struct AsyncCallPayload *outputData = (struct AsyncCallPayload*)result;
	bool failed = false;
	for (unsigned int i=0; i<outputData->variantsSize; i++) {
		if (!clientRdConverter[i]->set(outputData->variants[i].data, &getCommFB()->getRDs()[i])) {
			DEVLOG_ERROR("OPC UA: Client could not convert returned data from OPC UA to RD_%d.\n", i+1);
			failed = true;
			break;
		}
	}
	UA_Array_delete(outputData->variants, outputData->variantsSize, &UA_TYPES[UA_TYPES_VARIANT]);
	forte_free(result);
	if (!failed) {
		mInterruptResp = e_ProcessDataOk;
		getCommFB()->interruptCommFB(this);
	}

}

void COPC_UA_Layer::handleAsyncEvent() {
	getCommFB()->interruptCommFB(this);
	COPC_UA_Handler::getInstance().forceEventHandling(this);
}





