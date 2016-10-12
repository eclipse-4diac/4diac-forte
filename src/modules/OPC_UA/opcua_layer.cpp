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
#include "opcua_layer.h"
#include "opcua_handler.h"

using namespace forte::com_infra;


COPC_UA_Layer::COPC_UA_Layer(CComLayer * pa_poUpperLayer, CCommFB * pa_poComFB) : CComLayer(pa_poUpperLayer, pa_poComFB),
																				  mInterruptResp(e_Nothing), m_apUANodeId(0), fbNodeId(NULL),
																				  sendDataNodeIds(nullptr), readDataNodeIds(nullptr)
{
	// constructor list initialization

}


COPC_UA_Layer::~COPC_UA_Layer() {
	// TODO Auto-generated destructor stub
}


void COPC_UA_Layer::closeConnection(){
	if (fbNodeId != nullptr) {
		UA_NodeId_delete(fbNodeId);
		fbNodeId = nullptr;

		if(sendDataNodeIds != nullptr && getCommFB()->getComServiceType() == e_Publisher){
			for (unsigned int i=0; i<getCommFB()->getNumSD(); i++) {
				UA_NodeId_delete(sendDataNodeIds[i].functionBlockId);
				UA_NodeId_delete(sendDataNodeIds[i].variableId);
			}
			forte_free(sendDataNodeIds);
			sendDataNodeIds = nullptr;
		} else if(readDataNodeIds != nullptr && getCommFB()->getComServiceType() == e_Subscriber){
			for (unsigned int i=0; i<getCommFB()->getNumRD(); i++) {
				UA_NodeId_delete(readDataNodeIds[i].functionBlockId);
				UA_NodeId_delete(readDataNodeIds[i].variableId);
			}
			forte_free(readDataNodeIds);
			readDataNodeIds = nullptr;
		}
	}
}


EComResponse COPC_UA_Layer::openConnection(char * paLayerParameter){

	if (fbNodeId != nullptr) {
		DEVLOG_WARNING("Already connected. Nothing to do.\n");
		return e_InitTerminated;
	}

	if (COPC_UA_Handler::getInstance().getNodeForPath(&fbNodeId, paLayerParameter, true)!= UA_STATUSCODE_GOOD)
		return e_InitTerminated;


	switch(getCommFB()->getComServiceType()) {
		case e_Publisher:
			// Make sure all the nodes exist and have the corresponding variable
			return this->createPubSubNodes(&this->sendDataNodeIds, getCommFB()->getNumSD(), true);
		case e_Subscriber:
			return this->createPubSubNodes(&this->readDataNodeIds, getCommFB()->getNumRD(), false);
		/*case e_Server:

			int numData = getCommFB()->getNumSD();
			CIEC_ANY* dataArray = getCommFB()->getSDs();
			int numData = getCommFB()->getNumRD();
			CIEC_ANY* dataArray = getCommFB()->getRDs();

			for(int i = 0; i<2; i++){
				//COPC_UA_Handler::getInstance().updateNodeValue(m_apUANodeId[1], dataArray[1]);
			}
			break;*/
		default:
			DEVLOG_WARNING("Invalid Comm Service Type for Function Block\n");
	}
	return e_InitOk;
}

/**
 * This method is required to inline initialize the connection point.
 */
static const SConnectionPoint& getFirstListEntry(const CSinglyLinkedList<SConnectionPoint> &list) {
	CSinglyLinkedList<SConnectionPoint>::Iterator it = list.begin();
	return *it;
}

forte::com_infra::EComResponse COPC_UA_Layer::createPubSubNodes(struct FB_NodeIds **nodeIds, unsigned int numPorts, bool isPub) {
	if (*nodeIds != nullptr) {
		DEVLOG_ERROR("Publish Nodes already initialized.");
		return e_InitInvalidId;
	}

	if(numPorts == 0){
		// For pure event indication
		// TODO: add event handling stuff
		DEVLOG_ERROR("OPC UA Publisher/Subscriber without SD/RD Signal, pure event handling");
		return e_InitInvalidId;
	}

	*nodeIds = static_cast<struct FB_NodeIds*>(forte_malloc(sizeof(struct FB_NodeIds)*numPorts));

	for(unsigned int i = 0; i < numPorts; i++) {
		int portId = i + 2;

		/* Retrieve Publisher, Connection and Signals Source Function Block Information */
		const SFBInterfaceSpec* interfaceSpec = getCommFB()->getFBInterfaceSpec();
		const CStringDictionary::TStringId portNameId = isPub ? interfaceSpec->m_aunDINames[portId] : interfaceSpec->m_aunDONames[portId];

		const char* portName = CStringDictionary::getInstance().get(portNameId);
		DEVLOG_INFO("Processing %s signal %s at port %i.\n",isPub ? "publish" : "subscribe", portName, portId);

		const CDataConnection* portConnection = isPub ? getCommFB()->getDIConnection(portNameId) : getCommFB()->getDOConnection(portNameId);
		if (portConnection == nullptr) {
			DEVLOG_ERROR("Got invalid port connection at port %d", portId);
			for (unsigned int j = 0; j < i; j++) {
				if ((*nodeIds)[j].functionBlockId != nullptr)
					UA_NodeId_delete((*nodeIds)[j].functionBlockId);
				if ((*nodeIds)[j].functionBlockId != nullptr)
					UA_NodeId_delete((*nodeIds)[j].variableId);
			}
			forte_free((*nodeIds));
			(*nodeIds) = nullptr;
			return e_InitInvalidId;
		}


		//TODO for now we assume that the connection only has one destination. Needs fix!

		if (!isPub && portConnection->getDestinationList().isEmpty()) {
			DEVLOG_ERROR("Subscriber does not have any connection.");
			for (unsigned int j = 0; j < i; j++) {
				if ((*nodeIds)[j].functionBlockId != nullptr)
					UA_NodeId_delete((*nodeIds)[j].functionBlockId);
				if ((*nodeIds)[j].functionBlockId != nullptr)
					UA_NodeId_delete((*nodeIds)[j].variableId);
			}
			forte_free((*nodeIds));
			(*nodeIds) = nullptr;
			return e_InitInvalidId;
		}


		const SConnectionPoint& sourceConnectionPoint(isPub ? portConnection->getSourceId() : getFirstListEntry(portConnection->getDestinationList()));


		const CFunctionBlock* sourceFB = sourceConnectionPoint.mFB;    // pointer to Signals Source Function Block
		const SFBInterfaceSpec* sourceInterfaceSpec = sourceFB->getFBInterfaceSpec();
		const CStringDictionary::TStringId sourceNameId = isPub ? sourceInterfaceSpec->m_aunDONames[sourceConnectionPoint.mPortId] : sourceInterfaceSpec->m_aunDINames[sourceConnectionPoint.mPortId];
		const char* sourceName = CStringDictionary::getInstance().get(sourceNameId);

		UA_StatusCode retVal;

		{
			// create/get node for connected FB
			size_t len = strlen(sourceFB->getInstanceName()) + 2; // include slash and nullbyte
			char* fbBrowseName = static_cast<char*>(forte_malloc(len));
			snprintf(fbBrowseName, len, "/%s", sourceFB->getInstanceName());
			UA_NodeId newNodeType = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE);
			retVal = COPC_UA_Handler::getInstance().getNodeForPath(&(*nodeIds)[i].functionBlockId, fbBrowseName, true, fbNodeId, &newNodeType);
			forte_free(fbBrowseName);
		}

		if (retVal == UA_STATUSCODE_GOOD) {
			// create/get variable node for port on connected FB
			size_t len = strlen(sourceName) + 2; // include slash and nullbyte
			char* sourceVarBrowseName = static_cast<char*>(forte_malloc(len));
			snprintf(sourceVarBrowseName, len, "/%s", sourceName);
			retVal = COPC_UA_Handler::getInstance().getNodeForPath(&(*nodeIds)[i].variableId, sourceVarBrowseName, false,
																   (*nodeIds)[i].functionBlockId);
			if (retVal == UA_STATUSCODE_GOOD && (*nodeIds)[i].variableId == nullptr) {
				// we need to create the variable

				const CStringDictionary::TStringId varTypeId = isPub ? sourceInterfaceSpec->m_aunDODataTypeNames[sourceConnectionPoint.mPortId] : sourceInterfaceSpec->m_aunDIDataTypeNames[sourceConnectionPoint.mPortId];
				auto mapType = COPC_UA_Helper::mapForteStringIdToOpcUa.find(varTypeId);
				if (mapType == COPC_UA_Helper::mapForteStringIdToOpcUa.end()) {

					DEVLOG_ERROR("Mapping of type %s to OPC UA not defined.", CStringDictionary::getInstance().get(varTypeId));
					retVal = UA_STATUSCODE_BADNOTIMPLEMENTED;
				} else {

					const UA_TypeConvert *conv = (*nodeIds)[i].convert = mapType->second;
					void *varValue = UA_new(conv->type);
					UA_init(varValue, conv->type);
					(*nodeIds)[i].variableId = UA_NodeId_new();
					retVal = COPC_UA_Handler::getInstance().createUAVariableNode((*nodeIds)[i].functionBlockId, sourceName, conv->type,
																				 varValue, (*nodeIds)[i].variableId);
					UA_delete(varValue, conv->type);
					if (retVal == UA_STATUSCODE_GOOD && !isPub) {
						COPC_UA_Handler::getInstance().registerNodeCallBack((*nodeIds)[i].variableId, this, conv, i);
					}
				}
			} // else if retVal = UA_STATUSCODE_GOOD the node already exists
		}

		if (retVal != UA_STATUSCODE_GOOD) {
			for (unsigned int j = 0; j <= i; j++) {
				if ((*nodeIds)[j].functionBlockId != nullptr)
					UA_NodeId_delete((*nodeIds)[j].functionBlockId);
				if ((*nodeIds)[j].functionBlockId != nullptr)
					UA_NodeId_delete((*nodeIds)[j].variableId);
			}
			forte_free((*nodeIds));
			(*nodeIds) = nullptr;
			return e_InitInvalidId;
		}
	}
	return e_InitOk;
}

EComResponse COPC_UA_Layer::sendData(void *paData, unsigned int paSize){

	EComResponse retVal = e_ProcessDataOk;

	if(0 == paSize){
		//TODO change to an update now with out the need for a new allocation
		// sfp_item_update_data_allocated(*mSFPItem, sfp_variant_new_null(), sfp_time_in_millis ());
	}else {
		const CIEC_ANY *SDs(static_cast<CIEC_ANY*>(paData));
		for(unsigned int i = 0; i < paSize; i++){
			FB_NodeIds *ni = &this->sendDataNodeIds[i];
			if (COPC_UA_Handler::getInstance().updateNodeValue(ni->variableId, &SDs[i], ni->convert) != UA_STATUSCODE_GOOD) {
				DEVLOG_ERROR("Could not convert publisher value for port %d to OPC UA.", i);
				retVal = e_ProcessDataDataTypeError;
			}
		}
	}
	return retVal;
}



EComResponse COPC_UA_Layer::recvData(const void * pa_pvData, __attribute__((unused)) unsigned int pa_unSize){
	mInterruptResp = e_ProcessDataRecvFaild;

	struct recvData_handle {
		const struct UA_TypeConvert* convert;
		unsigned int portIndex;
		const UA_Variant *data;
	};
	const struct recvData_handle *handleRecv = static_cast<const recvData_handle*>(pa_pvData);


	if(getCommFB()->getNumRD() == 0){
		//TODO handle pure event subscription
		mInterruptResp = e_ProcessDataOk;
	}else{
		if(UA_Variant_isScalar(handleRecv->data) && handleRecv->data->type == handleRecv->convert->type && handleRecv->data->data) {
			if (handleRecv->convert->set(handleRecv->data->data, &getCommFB()->getRDs()[handleRecv->portIndex])) {
				mInterruptResp = e_ProcessDataOk;
				getCommFB()->interruptCommFB(this);
			}
		}
	}

	return mInterruptResp;
}


EComResponse COPC_UA_Layer::processInterrupt(){
	return mInterruptResp;
}







