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



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#undef NULL
#include <src_generated/ua_namespaceinit_generated.h>
#undef NULL
#pragma GCC diagnostic pop
#include "opcua_layer.h"
#include "opcua_handler.h"
#include "opcua_helper.h"

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
	if (fbNodeId != NULL) {
		UA_NodeId_delete(fbNodeId);
		fbNodeId = NULL;

		if(sendDataNodeIds != NULL && getCommFB()->getComServiceType() == e_Publisher){
			for (unsigned int i=0; i<getCommFB()->getNumSD(); i++) {
				UA_NodeId_delete(sendDataNodeIds[i].functionBlockId);
				UA_NodeId_delete(sendDataNodeIds[i].variableId);
			}
			forte_free(sendDataNodeIds);
			sendDataNodeIds = NULL;
		} else if(readDataNodeIds != NULL && getCommFB()->getComServiceType() == e_Subscriber){
			for (unsigned int i=0; i<getCommFB()->getNumRD(); i++) {
				UA_NodeId_delete(readDataNodeIds[i].functionBlockId);
				UA_NodeId_delete(readDataNodeIds[i].variableId);
			}
			forte_free(readDataNodeIds);
			readDataNodeIds = NULL;
		}
	}
}


EComResponse COPC_UA_Layer::openConnection(char * paLayerParameter){

	if (fbNodeId != NULL) {
		DEVLOG_WARNING("Already connected. Nothing to do.\n");
		return e_InitTerminated;
	}

	if (COPC_UA_Handler::getInstance().getNodeForPath(&fbNodeId, paLayerParameter, true)!= UA_STATUSCODE_GOOD)
		return e_InitTerminated;


	switch(getCommFB()->getComServiceType()) {
		case e_Publisher:
			// Make sure all the nodes exist and have the corresponding variable
			return this->createPublishNodes(getCommFB()->getSDs(), getCommFB()->getNumSD());
		case e_Subscriber:
			// Subscribe has initial value and then new one after each update.
			// for now only one subscription per subscriber block
			//int numData = getCommFB()->getNumRD(); //numData not used
			/*CIEC_ANY* dataArray = getCommFB()->getRDs();

			m_apUANodeId = new UA_NodeId *[1];  //TODO for now Publisher only publishes a single value. Extend to handling multiple NodeId Adapters plus corresponding SDs and RDs.
			memset(m_apUANodeId, 0, sizeof(UA_NodeId *) * 1);

			//COPC_UA_Handler::getInstance().assembleUANodeId(dataArray, m_apUANodeId[1]);	// Assemble node id from ANodeId Adapter information

			// write the initial value to the OPC_UA Address Space so that the data type of the item gets set
			COPC_UA_Handler::getInstance().updateNodeValue(m_apUANodeId[1], dataArray[1]);

			// Register notification Callback for Subscriber Function Blocks
			if(e_InitOk == retValEcom){
				COPC_UA_Handler::getInstance().registerNodeCallBack(m_apUANodeId[1], this);
			}*/
			break;
		case e_Server:
			/*
			int numData = getCommFB()->getNumSD();
			CIEC_ANY* dataArray = getCommFB()->getSDs();
			int numData = getCommFB()->getNumRD();
			CIEC_ANY* dataArray = getCommFB()->getRDs();
			*/
			for(int i = 0; i<2; i++){
				//COPC_UA_Handler::getInstance().updateNodeValue(m_apUANodeId[1], dataArray[1]);
			}
			break;
		default:
			DEVLOG_WARNING("Invalid Comm Service Type for Function Block\n");
	}
	return e_InitOk;
}



EComResponse COPC_UA_Layer::createPublishNodes(const CIEC_ANY *sdArray, unsigned int numSd) {
	if (this->sendDataNodeIds != NULL) {
		DEVLOG_ERROR("Publish Nodes already initialized.");
		return e_InitInvalidId;
	}

	if(numSd == 0){
		// For pure event indication
		// TODO: add event handling stuff
		DEVLOG_ERROR("OPC UA Publisher without SD Signal, pure event handling");
		return e_InitInvalidId;
	}

	// For handling publish requests

	this->sendDataNodeIds = static_cast<struct FB_NodeIds*>(forte_malloc(sizeof(struct FB_NodeIds)*numSd));

	for(unsigned int i = 0; i < numSd; i++) {
		int portId = i + 2;

		/* Retrieve Publisher, Connection and Signals Source Function Block Information */
		const SFBInterfaceSpec* interfaceSpec = getCommFB()->getFBInterfaceSpec();
		const CStringDictionary::TStringId sdNameId = interfaceSpec->m_aunDINames[portId];

		const char* SDName = CStringDictionary::getInstance().get(sdNameId);
		DEVLOG_INFO("Processing publish signal %s at port %i.\n", SDName, portId);

		const CDataConnection* diConnection = getCommFB()->getDIConnection(sdNameId);

		const SConnectionPoint& sourceConnectionPoint(diConnection->getSourceId());


		const CFunctionBlock* sourceFB = sourceConnectionPoint.mFB;    // pointer to Signals Source Function Block
		const SFBInterfaceSpec* sourceInterfaceSpec = sourceFB->getFBInterfaceSpec();
		const CStringDictionary::TStringId sourceNameId = sourceInterfaceSpec->m_aunDONames[sourceConnectionPoint.mPortId];
		const char* sourceName = CStringDictionary::getInstance().get(sourceNameId);

		UA_StatusCode retVal;

		{
			size_t len = strlen(sourceFB->getInstanceName()) + 2; // include slash and nullbyte
			char* fbBrowseName = static_cast<char*>(forte_malloc(len));
			snprintf(fbBrowseName, len, "/%s", sourceFB->getInstanceName());
			UA_NodeId newNodeType = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE);
			retVal = COPC_UA_Handler::getInstance().getNodeForPath(&this->sendDataNodeIds[i].functionBlockId, fbBrowseName, true, fbNodeId, &newNodeType);
			forte_free(fbBrowseName);
		}

		if (retVal == UA_STATUSCODE_GOOD) {
			size_t len = strlen(sourceName) + 2; // include slash and nullbyte
			char* sourceVarBrowseName = static_cast<char*>(forte_malloc(len));
			snprintf(sourceVarBrowseName, len, "/%s", sourceName);
			retVal = COPC_UA_Handler::getInstance().getNodeForPath(&this->sendDataNodeIds[i].variableId, sourceVarBrowseName, false,
																   this->sendDataNodeIds[i].functionBlockId);
			if (retVal == UA_STATUSCODE_GOOD && this->sendDataNodeIds[i].variableId == nullptr) {
				// we need to create the variable

				const CStringDictionary::TStringId varTypeId = sourceInterfaceSpec->m_aunDODataTypeNames[sourceConnectionPoint.mPortId];
				auto mapType = COPC_UA_Helper::mapForteStringIdToOpcUa.find(varTypeId);
				if (mapType == COPC_UA_Helper::mapForteStringIdToOpcUa.end()) {

					DEVLOG_ERROR("Mapping of type %s to OPC UA not defined.", CStringDictionary::getInstance().get(varTypeId));
					retVal = UA_STATUSCODE_BADNOTIMPLEMENTED;
				} else {

					const UA_TypeConvert *conv = mapType->second;
					void *varValue = nullptr;
					varValue = UA_new(conv->type);
					UA_init(varValue, conv->type);
					this->sendDataNodeIds[i].variableId = UA_NodeId_new();
					retVal = COPC_UA_Handler::getInstance().createUAVariableNode(this->sendDataNodeIds[i].functionBlockId, sourceName, conv->type,
																				 varValue, this->sendDataNodeIds[i].variableId);
					UA_delete(varValue, conv->type);
				}

			}
		}

		if (retVal != UA_STATUSCODE_GOOD) {
			for (unsigned int j = 0; j <= i; j++) {
				if (sendDataNodeIds[j].functionBlockId != nullptr)
					UA_NodeId_delete(sendDataNodeIds[j].functionBlockId);
				if (sendDataNodeIds[j].functionBlockId != nullptr)
					UA_NodeId_delete(sendDataNodeIds[j].variableId);
			}
			forte_free(sendDataNodeIds);
			sendDataNodeIds = NULL;
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
		//CIEC_ANY const *SDs(static_cast<TConstIEC_ANYPtr>(paData));
		CIEC_ANY *SDs(static_cast<CIEC_ANY*>(paData));	// direct initialization of pointer to CIEC_ANY class


	/*	if (conv->get(&sdArray[portId], varValue)) {
			retVal = COPC_UA_Handler::getInstance().createUAVariableNode(this->sendDataNodeIds[i].functionBlockId, sourceName, conv->type,
																		 varValue, this->sendDataNodeIds[i].variableId);
		} else {
			DEVLOG_ERROR("Could not convert type: %s", CStringDictionary::getInstance().get(varTypeId));
		}*/

#ifdef FORTE_COM_OPC_UA_ENABLE_INIT_NAMESPACE

		COPC_UA_Handler::getInstance().updateNodeValue(m_apUANodeId[1], SDs[1]);

#else
		for(unsigned int i = 0; i < paSize; i++){
			COPC_UA_Handler::getInstance().updateNodeValue(m_apUANodeId[i], SDs[i]);
			break;
		}
#endif


	}
	return retVal;
}



EComResponse COPC_UA_Layer::recvData(const void * pa_pvData, __attribute__((unused)) unsigned int pa_unSize){
	mInterruptResp = e_ProcessDataOk;

	const UA_Variant *value = static_cast<const UA_Variant*>(pa_pvData);

	if(0 == getCommFB()->getNumRD()){
		// handle pure event subscription

	}else{
		CIEC_ANY &RD(*getCommFB()->getRDs());
		if(!COPC_UA_Handler::getInstance().readBackDataPoint(value, RD))
			mInterruptResp = e_ProcessDataRecvFaild;
	}

	return mInterruptResp;
}


EComResponse COPC_UA_Layer::processInterrupt(){
	return mInterruptResp;
}







