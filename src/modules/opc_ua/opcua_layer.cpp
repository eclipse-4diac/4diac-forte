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


COPC_UA_Layer::COPC_UA_Layer(CComLayer *pa_poUpperLayer, CCommFB *pa_poComFB) : CComLayer(pa_poUpperLayer, pa_poComFB),
																				mInterruptResp(e_Nothing), fbNodeId(NULL), methodNodeId(NULL),
																				sendDataNodeIds(NULL), readDataNodeIds(NULL),
																				mutexServerMethodCall(), serverMethodCallResultReady(false) {
	// constructor list initialization

}


COPC_UA_Layer::~COPC_UA_Layer() {
	// all the stuff is cleaned up in closeConnection()
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
}


EComResponse COPC_UA_Layer::openConnection(char *paLayerParameter) {

	if (fbNodeId != NULL) {
		DEVLOG_WARNING("Already connected. Nothing to do.\n");
		return e_InitTerminated;
	}

	// Create all the nodes up to the given node (ID parameter of the FB)
	{
		UA_StatusCode retVal;
		if ((retVal = COPC_UA_Handler::getInstance().getNodeForPath(&fbNodeId, paLayerParameter, true)) != UA_STATUSCODE_GOOD) {
			DEVLOG_ERROR("Could not get node for path: '%s': %s\n", paLayerParameter, UA_StatusCode_explanation(retVal));
			return e_InitTerminated;
		}
	}


	switch (getCommFB()->getComServiceType()) {
		case e_Publisher:
			// Make sure all the nodes exist and have the corresponding variable
			return this->createPubSubNodes(&this->sendDataNodeIds, getCommFB()->getNumSD(), true);
		case e_Subscriber:
			return this->createPubSubNodes(&this->readDataNodeIds, getCommFB()->getNumRD(), false);
		case e_Server:
			return this->createMethodNode();
		default:
			DEVLOG_WARNING("Invalid Comm Service Type for Function Block\n");
	}
	return e_InitOk;
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

	DEVLOG_DEBUG("Processing %s signal %s at port %i.\n", isSD ? "publish" : "subscribe", CStringDictionary::getInstance().get(portNameId), portId);

	const CDataConnection *portConnection = isSD ? getCommFB()->getDIConnection(portNameId) : getCommFB()->getDOConnection(portNameId);
	if (portConnection == NULL) {
		DEVLOG_ERROR("Got invalid port connection at port %d\n", portId);
		return false;
	}


	//TODO for now we assume that the subscriber connection only has one destination. Needs fix!

	if (!isSD && portConnection->getDestinationList().isEmpty()) {
		DEVLOG_WARNING("Subscriber does not have any connection.\n");
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
		DEVLOG_ERROR("Mapping of type %s to OPC UA not defined.\n", CStringDictionary::getInstance().get(connectedToType));
		return false;
	}
	*convert = &COPC_UA_Helper::mapForteTypeIdToOpcUa[remotePort->getDataTypeID()];

	return true;
}

forte::com_infra::EComResponse COPC_UA_Layer::createPubSubNodes(struct FB_NodeIds **nodeIds, unsigned int numPorts, bool isSD) {
	if (*nodeIds != NULL) {
		DEVLOG_ERROR("Publish/Subscribe Nodes already initialized.\n");
		return e_InitInvalidId;
	}

	if (numPorts == 0) {
		DEVLOG_ERROR("OPC UA Publisher/Subscriber without SD/RD Signal, pure event handling\n");
		return e_InitInvalidId;
	}

	*nodeIds = new struct FB_NodeIds[numPorts];

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
			char *fbBrowseName = new char[len];
			snprintf(fbBrowseName, len, "/%s", connectedToFb->getInstanceName());
			UA_NodeId newNodeType = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE);
			retVal = COPC_UA_Handler::getInstance().getNodeForPath(&(*nodeIds)[i].functionBlockId, fbBrowseName, true, fbNodeId, &newNodeType);
			delete[](fbBrowseName);
		}

		if (retVal == UA_STATUSCODE_GOOD) {
			// create/get variable node for port on connected FB
			size_t len = strlen(connectedToName) + 2; // include slash and nullbyte
			char *sourceVarBrowseName = new char[len];
			snprintf(sourceVarBrowseName, len, "/%s", connectedToName);
			retVal = COPC_UA_Handler::getInstance().getNodeForPath(&(*nodeIds)[i].variableId, sourceVarBrowseName, false,
																   (*nodeIds)[i].functionBlockId);
			delete[]sourceVarBrowseName;
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

	DEVLOG_DEBUG("OPC UA creating method for %s\n", getCommFB()->getInstanceName());

	if (sendDataNodeIds != NULL || readDataNodeIds != NULL) {
		DEVLOG_ERROR("Method node already initialized.\n");
		return e_InitInvalidId;
	}

	if (this->getCommFB()->getNumRD() == 0 && this->getCommFB()->getNumSD() == 0) {
		DEVLOG_ERROR("OPC UA Method without SD/RD Signal, pure event handling\n");
		return e_InitInvalidId;
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
		DEVLOG_ERROR("OPC UA could not create method node: %s - %s\n", UA_StatusCode_name(retVal), UA_StatusCode_description(retVal));
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

EComResponse COPC_UA_Layer::sendData(void *paData, unsigned int paSize) {

	EComResponse retVal = e_ProcessDataOk;

	if (getCommFB()->getComServiceType() == e_Server) {
		// continue method call
		serverMethodCallResultReady = true;
		return retVal;
	}

	if (paSize == 0) {
	} else {
		const CIEC_ANY *SDs(static_cast<CIEC_ANY *>(paData));

		for (unsigned int i = 0; i < paSize; i++) {
			FB_NodeIds *ni = &this->sendDataNodeIds[i];
			if (COPC_UA_Handler::getInstance().updateNodeValue(ni->variableId, &SDs[i], ni->convert) != UA_STATUSCODE_GOOD) {
				DEVLOG_ERROR("Could not convert publisher value for port %d to OPC UA.\n", i);
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


EComResponse COPC_UA_Layer::processInterrupt() {
	return mInterruptResp;
}

UA_StatusCode COPC_UA_Layer::onServerMethodCall(void *methodHandle, __attribute__((unused)) const UA_NodeId objectId, size_t inputSize, const UA_Variant *input,
												size_t outputSize,
												UA_Variant *output) {

	COPC_UA_Layer *self = static_cast<COPC_UA_Layer *>(methodHandle);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
	// other thready may currently create nodes for the same path, thus mutex
	CCriticalRegion criticalRegion(self->mutexServerMethodCall);
#pragma GCC diagnostic pop
	self->serverMethodCallResultReady = false;

	if (inputSize != self->getCommFB()->getNumRD() || outputSize != self->getCommFB()->getNumSD()) {
		DEVLOG_ERROR("OPC UA method call got invalid number of arguments. In: %d==%d, Out: %d==%d\n", self->getCommFB()->getNumRD(), inputSize,
					 self->getCommFB()->getNumSD(), outputSize);
		return UA_STATUSCODE_BADINVALIDARGUMENT;
	}


	self->mInterruptResp = e_ProcessDataOk;

	// put the input values on the wire, i.e. on the RD ports
	for (unsigned int i = 0; i < self->getCommFB()->getNumRD(); i++) {
		if (UA_Variant_isScalar(&input[i]) && input[i].data != NULL) {
			if (!COPC_UA_Helper::convertFromUa_typeId(self->getCommFB()->getRDs()[i].getDataTypeID(), input[i].data, &self->getCommFB()->getRDs()[i])) {
				self->mInterruptResp = e_ProcessDataRecvFaild;
				DEVLOG_ERROR("OPC UA can not convert method inputArgument at idx %d to forte type\n", i);
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

	// wait until result is ready
	{
		CIEC_DATE_AND_TIME startTime;
		startTime.setCurrentTime();
		CIEC_DATE_AND_TIME currentTime;
		currentTime.setCurrentTime();
		// TODO uses busy waiting. Is there a better way?
		while (!self->serverMethodCallResultReady && currentTime.getMilliSeconds() - startTime.getMilliSeconds() < METHOD_CALL_TIMEOUT * 1000) {
			currentTime.setCurrentTime();
		}
		if (currentTime.getMilliSeconds() - startTime.getMilliSeconds() >= METHOD_CALL_TIMEOUT * 1000) {
			DEVLOG_ERROR("OPC UA method call did not get result values within timeout.\n");
			return UA_STATUSCODE_BADTIMEOUT;
		}
	}
	self->serverMethodCallResultReady = false;

	// copy SD values to output
	for (unsigned int i = 0; i < self->getCommFB()->getNumSD(); i++) {


		if (!COPC_UA_Helper::isTypeIdValid(self->getCommFB()->getSDs()[i].getDataTypeID())) {
			DEVLOG_ERROR("OPC UA can not convert forte type to method outputArgument at idx %d\n", i);
			break;
		}

		const UA_TypeConvert *convert = &COPC_UA_Helper::mapForteTypeIdToOpcUa[self->getCommFB()->getSDs()[i].getDataTypeID()];

		void *varValue = UA_new(convert->type);
		if (!convert->get(&self->getCommFB()->getSDs()[i], varValue)) {
			self->mInterruptResp = e_ProcessDataRecvFaild;
			DEVLOG_ERROR("OPC UA can not convert forte type to method outputArgument at idx %d\n", i);
		} else {
			UA_Variant_setScalarCopy(&output[i], varValue, convert->type);
		}

		UA_delete(varValue, convert->type);

		if (self->mInterruptResp != e_ProcessDataOk)
			break;
	}

	return self->mInterruptResp == e_ProcessDataOk ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADUNEXPECTEDERROR;
}







