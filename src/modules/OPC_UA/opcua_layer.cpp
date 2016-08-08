/*******************************************************************************
 * Copyright (c) 2015-2016 Florian Froschermeier <florian.froschermeier@tum.de>
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Florian Froschermeier
 *      - initial integration of the OPC-UA protocol.
 *******************************************************************************/

#include "opcua_layer.h"
#include "opcuahandler.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace forte::com_infra;





COPC_UA_Layer::COPC_UA_Layer(CComLayer * pa_poUpperLayer, CCommFB * pa_poComFB) : CComLayer(pa_poUpperLayer, pa_poComFB), m_apUANodeId(0), mInterruptResp(e_Nothing){
	// constructor list initialization

}


COPC_UA_Layer::~COPC_UA_Layer() {
	// TODO Auto-generated destructor stub
}


void COPC_UA_Layer::closeConnection(){
	//COPC_UA_Handler::getInstance().
}


EComResponse COPC_UA_Layer::openConnection(char * paLayerParameter){
	EComResponse retValEcom = e_InitOk;
	UA_StatusCode retValUA;


	char *pch;
	int i = 0;
	pch = strtok (paLayerParameter,";");
	while (pch != NULL) {
		i++;
		if(i==1){
			/* First string is omitted for now */
			// TODO: pass port ID to ua handler constructor
			// COPC_UA_Handler::getInstance(portId)  -> Singleton pattern not supporting parameters?
		} else{
			/* Process NodeIds */
			COPC_UA_Handler::getInstance().assembleUANodeId(pch, m_apUANodeId[i-1]);
		}
		pch = strtok (NULL, ";");
	}


	/* PUBLISHER */
	if(e_Publisher == getCommFB()->getComServiceType()){

		CIEC_ANY* dataArray = getCommFB()->getSDs();


		/* check if node exist in external namespace or need to be created */
#ifdef FORTE_COM_OPC_UA_ENABLE_INIT_NAMESPACE
		/* Necessary if custom OPC UA FunctionBlocks are used
		 * Solution was deprecated because of inability to create generic version of these OPC UA Blocks.
		 * m_apUANodeId = new UA_NodeId *[1];  //TODO for now Publisher only publishes a single value. Extend to handling multiple NodeId plus corresponding SDs and RDs.
		 * memset(m_apUANodeId, 0, sizeof(UA_NodeId *) * 1);
		 */


		// Use nodes from external namespace
		//COPC_UA_Handler::getInstance().assembleUANodeId(dataArray, m_apUANodeId[1]);	// Assemble node id from ANodeId Adapter information

		// write the initial value to the OPC_UA Address Space so that the data type of the item gets set
		COPC_UA_Handler::getInstance().updateNodeValue(m_apUANodeId[1], dataArray[1]);



#else
		// Automatic creation of UA Server AddressSpace from IEC61499 Application through model transition.
		// TODO: remove. This option is only kept as an idea source. Obsolete because solution is applicable to Publishers only.
		int numData = getCommFB()->getNumSD();
		m_apUANodeId = new UA_NodeId *[numData];  //TODO for now Publisher only publishes a single value. Extend to handling multiple NodeId Adapters plus corresponding SDs and RDs.
		memset(m_apUANodeId, 0, sizeof(UA_NodeId *) * numData);
		retValUA = createItems(dataArray, numData, paLayerParameter);

#endif


	} else if (e_Subscriber == getCommFB()->getComServiceType()){

		/* SUBSCRIBER */
		// Subscribe has initial value and then new one after each update.
		// for now only one subscription per subscriber block
		//int numData = getCommFB()->getNumRD(); //numData not used
		CIEC_ANY* dataArray = getCommFB()->getRDs();

		m_apUANodeId = new UA_NodeId *[1];  //TODO for now Publisher only publishes a single value. Extend to handling multiple NodeId Adapters plus corresponding SDs and RDs.
		memset(m_apUANodeId, 0, sizeof(UA_NodeId *) * 1);

		//COPC_UA_Handler::getInstance().assembleUANodeId(dataArray, m_apUANodeId[1]);	// Assemble node id from ANodeId Adapter information

		// write the initial value to the OPC_UA Address Space so that the data type of the item gets set
		COPC_UA_Handler::getInstance().updateNodeValue(m_apUANodeId[1], dataArray[1]);

		// Register notification Callback for Subscriber Function Blocks
		if(e_InitOk == retValEcom){
			COPC_UA_Handler::getInstance().registerNodeCallBack(m_apUANodeId[1], this);
		}


	} else if(e_Server == getCommFB()->getComServiceType()) {
		/* SERVER */
/*
		int numData = getCommFB()->getNumSD();
		CIEC_ANY* dataArray = getCommFB()->getSDs();
		int numData = getCommFB()->getNumRD();
		CIEC_ANY* dataArray = getCommFB()->getRDs();
*/
		for(int i = 0; i<2; i++){
			//COPC_UA_Handler::getInstance().updateNodeValue(m_apUANodeId[1], dataArray[1]);
		}

	} else{
		// Other Function Block Type
	}
	return retValEcom;
}



EComResponse COPC_UA_Layer::createItems(CIEC_ANY *paDataArray, int numSD, char* paLayerParameter){
	EComResponse retValEcom = e_InitOk;
	UA_StatusCode retVal = UA_STATUSCODE_GOOD;

	if(0 == numSD){		// For pure event indication

		retValEcom = e_InitInvalidId;
		DEVLOG_ERROR("OPCUA Publisher without SD Signal, pure event handling\n");
		retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;	//FIXME add event handling

	} else{		// For handling publish requests


		for(int i = 0; i < (numSD); i++)	// iteration over number of SendData (Publish) Signals
		{
			int portId = i + 2;

			/* Retrieve Publisher, Connection and Signals Source Function Block Information */
			const SFBInterfaceSpec* pstInterfaceSpec = getCommFB()->getFBInterfaceSpec();
			const CStringDictionary::TStringId paSDNameId = pstInterfaceSpec->m_aunDINames[portId];

			const char* SDName = CStringDictionary::getInstance().get(paSDNameId);
			DEVLOG_INFO("Processing publish signal %s at port %i.\n", SDName, portId);

			const CDataConnection* pC_DIConn = getCommFB()->getDIConnection(paSDNameId); 	//!< pointer to a Connection Object

			const SConnectionPoint& rst_sourceRD(pC_DIConn->getSourceId());

			const CFunctionBlock *sourceFB = rst_sourceRD.mFB;	// pointer to Signals Source Function Block
			UA_NodeId* returnFBNodeId = UA_NodeId_new();


			/*
			 * Check if Signals ParentNode with NodeId already exists in the server address space
			 */
			retVal = COPC_UA_Handler::getInstance().getFBNodeId(sourceFB, returnFBNodeId);
			UA_NodeId_delete(returnFBNodeId);

			/* Handle Parent Function Block Node */
			if(retVal == UA_STATUSCODE_GOOD){
				// Parent Function Block node already exists
				DEVLOG_INFO("Function block node already present in the server address space.\n");

			}else{	// Create Function Block Node

				DEVLOG_INFO("Creating parent Function Block Object Node.\n");
				UA_NodeId * returnObjNodeId = UA_NodeId_new();
				retVal = COPC_UA_Handler::getInstance().createUAObjNode(sourceFB, returnObjNodeId);

				if(retVal == UA_STATUSCODE_GOOD){  // Node creation successful

					DEVLOG_INFO("Object node creation successful: %s\n", retVal);

				}else{  // Node creation not successful

					DEVLOG_ERROR("Error creating node: %s\n", retVal);
				}
			}


			/*
			 * Check for existing Signal Node in Address Space or create it.
			 */
			UA_NodeId* returnSPNodeId = UA_NodeId_new();
			// check if Signal Variable Node is present in Address Space
			retVal = COPC_UA_Handler::getInstance().getSPNodeId(sourceFB, const_cast<SConnectionPoint&>(rst_sourceRD), returnSPNodeId);

			if(retVal == UA_STATUSCODE_GOOD){
				// SourcePoint node (SP Node) was present in the UA Address Space.
				DEVLOG_INFO("SP node already present in the Address space.\n");
				m_apUANodeId[i] = returnSPNodeId;

			}else{
				// Create SourcePort Node (Publishers SD Port) in UA Server Address Space
				UA_StatusCode retVal = COPC_UA_Handler::getInstance().createUAVarNode(sourceFB, const_cast<SConnectionPoint&>(rst_sourceRD), returnSPNodeId);

				if(retVal == UA_STATUSCODE_GOOD){ 	//Node creation successful
					DEVLOG_INFO("Variable node successfully created. \n");
					m_apUANodeId[i] = returnSPNodeId;

				}else{ // Node creation not successful
					DEVLOG_ERROR("Error creating node %s\n", retVal);
				}
			}
			// Initialize address space node with value
			COPC_UA_Handler::getInstance().updateNodeValue(m_apUANodeId[i],paDataArray[i]);
		}
	}
	if(retVal == UA_STATUSCODE_GOOD){
		retValEcom = e_InitOk;
	}else{
		retValEcom = e_InitInvalidId;
	}
	return retValEcom;
}

EComResponse COPC_UA_Layer::sendData(void *paData, unsigned int paSize){

	EComResponse retVal = e_ProcessDataOk;

	if(0 == paSize){
		//TODO change to an update now with out the need for a new allocation
		// sfp_item_update_data_allocated(*mSFPItem, sfp_variant_new_null(), sfp_time_in_millis ());
	}else {
		//CIEC_ANY const *SDs(static_cast<TConstIEC_ANYPtr>(paData));
		CIEC_ANY *SDs(static_cast<CIEC_ANY*>(paData));	// direct initialization of pointer to CIEC_ANY class

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



EComResponse COPC_UA_Layer::recvData(const void * pa_pvData, unsigned int pa_unSize){
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







