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


COPC_UA_Layer::COPC_UA_Layer(CComLayer * pa_poUpperLayer, CCommFB * pa_poComFB) : CComLayer(pa_poUpperLayer, pa_poComFB){
	// constructor list initialization
}


COPC_UA_Layer::~COPC_UA_Layer() {
	// TODO Auto-generated destructor stub
}


void COPC_UA_Layer::closeConnection(){
	//COPC_UA_Handler::getInstance().
}


EComResponse COPC_UA_Layer::openConnection(char * paLayerParameter){
	int numData;
	CIEC_ANY *dataArray;

	if(e_Subscriber == getCommFB()->getComServiceType()){
		numData = getCommFB()->getNumRD();
		dataArray = getCommFB()->getRDs();

	}else{
		numData = getCommFB()->getNumSD();
		dataArray = getCommFB()->getSDs();
	}

	EComResponse retVal = createItems(dataArray, numData, paLayerParameter);

	// Register notification Callback for Subscriber Function Blocks
	if(e_InitOk == retVal){
		if(e_Subscriber == getCommFB()->getComServiceType()){
			if(0 == numData){
				numData = 1;  //register for the item used for the event transmition
			}
			for(int i = 0; i < numData; i++){
				// COPC_UA_Handler::getInstance().registerWriteCallBack(mSFPItem[i], this);
			}
		}
	}

	// for OPC_UA also pass the parameters necessary to locate the variable at the correct
	//position in the address space.


	return retVal;


}


EComResponse COPC_UA_Layer::createItems(CIEC_ANY *paDataArray, int numSD, char* paLayerParameter){
	EComResponse retValEcom = e_InitOk;
	UA_StatusCode retVal = UA_STATUSCODE_GOOD;
	if(0 == numSD){
		//handle pure event message
		retValEcom = e_InitInvalidId;
		// implement handling for pure event connections
		DEVLOG_ERROR("OPC_UA Publisher no SD Ports, pure Event messages\n");
		retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;

	} else{
		// allocate memory for an array of pointers to pointers pointing to values of type NodeId
		st_ParentChildNodeId.ppNodeId_ParentFB = new UA_NodeId *[numSD];
		st_ParentChildNodeId.ppNodeId_SrcPoint = new UA_NodeId *[numSD];

		memset(st_ParentChildNodeId.ppNodeId_ParentFB, 0, sizeof(UA_NodeId *) * numSD);		//!< initialize pointer memory: multiply size of NodeID pointer with amount
		memset(st_ParentChildNodeId.ppNodeId_SrcPoint, 0, sizeof(UA_NodeId *) * numSD);		//!< initialize pointer memory: multiply size of NodeID pointer with amount

		/* check for FBParent existence
		 * create FBParent Node
		 * check for SourcePoint Node existing
		 * create SourcePoint Node
		 */

		for(int i = 0; i < (numSD); i++){
			int portCnt = i + 2;
			const SFBInterfaceSpec* pstInterfaceSpec = getCommFB()->getFBInterfaceSpec();
			const CStringDictionary::TStringId paDINameId = pstInterfaceSpec->m_aunDINames[portCnt];

			const char* myname = CStringDictionary::getInstance().get(paDINameId);
			DEVLOG_INFO("%s with port ID %i and name %s found\n",CStringDictionary::getInstance().get(getCommFB()->getFBTypeId()), portCnt, myname);

			const CDataConnection* pC_DIConn = getCommFB()->getDIConnection(paDINameId); 	//!< pointer to a Connection Object

			// retrieve connection source point (SP)
			//const SConnectionPoint& rst_sourceRD = const_cast<const CDataConnection*>(pC_DIConn)->getSourceId();
			const SConnectionPoint& rst_sourceRD(pC_DIConn->getSourceId());

			const CFunctionBlock *sourceFB = rst_sourceRD.mFB;	// pointer to Parent Function Block
			UA_NodeId* returnFBNodeId = UA_NodeId_new();

			// check if Function Block is present in the Address Space otherwise create it
			UA_StatusCode retValgetNode = COPC_UA_Handler::getInstance().getFBNodeId(sourceFB, returnFBNodeId);

			if(retValgetNode == UA_STATUSCODE_GOOD){
				// Parent object node (FB Node) was present in the UA Address Space.
				DEVLOG_INFO("Function block node already present in the Address space.\n");
				st_ParentChildNodeId.ppNodeId_ParentFB[i] = returnFBNodeId;
				retVal = UA_STATUSCODE_GOOD;

			}else{
				// Parent object node (FB Node) was not present in the UA Address space -> create it.
				DEVLOG_INFO("Parent object node was not present in AS, create it.\n");
				UA_NodeId * returnObjNodeId = UA_NodeId_new();
				UA_StatusCode retValcreateObjNode = COPC_UA_Handler::getInstance().createUAObjNode(sourceFB, returnObjNodeId);

				if(retValcreateObjNode == UA_STATUSCODE_GOOD){
					// Node creation successful
					//returnObjNodeId->identifier;
					//DEVLOG_INFO("Object node %s successfully created.\n",returnObjNodeId->identifier.string);		//FIXME add objectnode identifier here
					DEVLOG_INFO("Object node successfully created.\n"); //FIXME convert returnObjNode identifier to string ->then output
					//FIXME obacht bei union
					st_ParentChildNodeId.ppNodeId_ParentFB[i] = returnObjNodeId;
					retVal = UA_STATUSCODE_GOOD;

				}else{
					// Node creation not successful
					DEVLOG_ERROR("Error creating node %s\n", retValcreateObjNode);
					retVal = retValcreateObjNode;

				}
			}

			UA_NodeId* returnSPNodeId = UA_NodeId_new();
			// check if Variable Node is present in Address Space otherwise create it.
			retValgetNode = COPC_UA_Handler::getInstance().getSPNodeId(sourceFB, const_cast<SConnectionPoint&>(rst_sourceRD), returnSPNodeId);
			//retValgetNode = COPC_UA_Handler::getInstance().getSPNodeId(sourceFB, sourceRD, returnSPNodeId);

			if(retValgetNode == UA_STATUSCODE_GOOD){
				// SourcePoint node (SP Node) was present in the UA Address Space.
				DEVLOG_INFO("SP node already present in the Address space.\n");
				st_ParentChildNodeId.ppNodeId_SrcPoint[i] = returnSPNodeId;
				retVal = UA_STATUSCODE_GOOD;

			}else{
				// Create SourcePort Node (Publishers SD Port) in UA Server Address Space
				// pass Function Block pointer
				// pass SourcePort reference
				UA_NodeId * returnVarNodeId = UA_NodeId_new();
				UA_StatusCode retValcreateVarNode = COPC_UA_Handler::getInstance().createUAVarNode(sourceFB, const_cast<SConnectionPoint&>(rst_sourceRD), returnVarNodeId);

				if(retValcreateVarNode == UA_STATUSCODE_GOOD){
					// Node creation successful
					//DEVLOG_INFO("Object node %s successfully created.\n",returnVarNodeId->identifier.string);  //FIXME
					DEVLOG_INFO("Object node successfully created. \n");
					st_ParentChildNodeId.ppNodeId_SrcPoint[i] = returnVarNodeId;
					retVal = UA_STATUSCODE_GOOD;

				}else{
					// Node creation not successful
					DEVLOG_ERROR("Error creating node %s\n", retValcreateVarNode);
					retVal = retValcreateVarNode;

				}

				if(st_ParentChildNodeId.ppNodeId_SrcPoint[i]){
					// write the initial value to the OPC_UA Address Space so that the data type of the item gets set
					//COPC_UA_Handler::getInstance().updateNodeValue(st_ParentChildNodeId.ppNodeId_SrcPoint[i], paDataArray[i]);

				}else {
					retVal = e_InitInvalidId;
					break;
				}

			}
		}

	}

	//FIXME: mapping from UA_StatusCode to EComResponse type!
	// FIXME: set return value correct.
	retValEcom = e_InitOk;
	return retValEcom;
}

EComResponse COPC_UA_Layer::sendData(void *paData, unsigned int paSize){

	EComResponse retVal = e_ProcessDataOk;
	return retVal;
}


EComResponse COPC_UA_Layer::recvData(const void *paData, unsigned int ){
}

EComResponse COPC_UA_Layer::processInterrupt(){
}






