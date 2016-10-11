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


#ifndef SRC_MODULES_OPC_UA_OPCUAHANDLER_H_
#define SRC_MODULES_OPC_UA_OPCUAHANDLER_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <open62541.h>
#pragma GCC diagnostic pop
#include <singlet.h>
#include <thread.h>
#include <extevhan.h>
#include <conn.h>
#include "funcbloc.h"
#include <stdio.h>
#include "../../arch/devlog.h"
#include "comlayer.h"



class COPC_UA_Handler : public CExternalEventHandler, public CThread{
	DECLARE_SINGLETON(COPC_UA_Handler);

public:


	/* functions needed for the external event handler interface */
	void enableHandler(void);
	void disableHandler(void);
	void setPriority(int pa_nPriority);
	int getPriority(void) const;

	/* OPC_UA Server interaction */
	UA_Server * getServer(void);
	UA_Client * getClient(void);

	UA_StatusCode getFBNodeId(const CFunctionBlock *pCFB, UA_NodeId* returnFBNodeId);		// get function block (FB) NodeId
	UA_StatusCode getSPNodeId(const CFunctionBlock *pCFB, SConnectionPoint& sourceRD, UA_NodeId* returnSPNodeId);		// get source point (SP) NodeId

	/**
	 * For a given connection SourcePoint between two 61499 FBs add a variable Node to the OPC_UA address space.
	 * Node is described by the name of the port and the name of the parent function block.
	 * If creation successful the NodeId is returned otherwise
	 * UA_StatusCode from node creation with error message.
	 * @param parentNode Parent node for the variable
	 * @param varName browse name of the variable
	 * @param varType Datatype of the varialbe
	 * @param varValue initial value of the variable
	 * @param returnVarNodeId the node ID of the created variable node
	 * @return UA_STATUSCODE_GOOD on succes, the error code otherwise
	 */
	UA_StatusCode createUAVariableNode(const UA_NodeId *parentNode, const char* varName, const UA_DataType* varType, void* varValue, UA_NodeId * returnVarNodeId);	// create variable node from SourcePoint Node Id

	UA_StatusCode createUAObjNode(const CFunctionBlock* pCFB, UA_NodeId * returnObjNodeId);	// create object node from Parent Function Block Node Id
	//UA_StatusCode createUAMethodNode(const CFunctionBlock* pCFB, UA_NodeId * returnMethodNodeId);
	//UA_StatusCode assembleUANodeId(char* NodeIdString, UA_NodeId *returnNodeId);

	/* OPC_UA Handler interaction */
	UA_StatusCode updateNodeValue(UA_NodeId * pNodeId, CIEC_ANY &paDataPoint);
	bool readBackDataPoint(const UA_Variant *pstValue, CIEC_ANY &paDataPoint);
	UA_StatusCode registerNodeCallBack(UA_NodeId *paNodeId, forte::com_infra::CComLayer *paLayer);
	static void onWrite(void *h, const UA_NodeId nodeid, const UA_Variant *data,
			const UA_NumericRange *range);
	// void handleWriteNodeCallback();		// Value Callback on write UA_Variable Node

	static const int scmUADataTypeMapping[];


	/**
	 * Get the node id of the node which is represented by the given path.
	 * If createIfNotFound is set to true, all nodes which do not exist yet will be created.
	 *
	 * @param foundNodeId the node ID which corresponds to the nodePath or null if it is not found and createIfNotFound is false.
	 * @param nodePath Full path to the node, e.g. '/Objects/FlipFlop'
	 * @param createIfNotFound if true all the missing nodes will be created as FolderType objects.
	 * @param startingNode the nodeId from where the given browse path starts. If NULL, the starting Node is Root and the path has to
	 * 			start with '/Objects'
	 * @param newNodeType type of the new node, if it has to be created. Default is FolderType
	 * @return UA_STATUSCODE_GOOD on success or the corresponding error code.
	 */
	UA_StatusCode getNodeForPath(UA_NodeId **foundNodeId, char* nodePath, bool createIfNotFound, const UA_NodeId *startingNode = NULL, const UA_NodeId *newNodeType = NULL);

protected:

private:
	// OPC_UA Server and configuration
	UA_Server *mOPCUAServer;
	UA_ServerConfig m_server_config;

	// OPC_UA Client and configuration
	volatile UA_Boolean* mbServerRunning = new UA_Boolean(UA_TRUE);
	UA_ServerNetworkLayer m_server_networklayer;


	void setServerRunning();
	void stopServerRunning();
	//static forte::com_infra::EComResponse m_eComResponse;
	void configureUAServer(TForteUInt16 UAServerPort);

	// implementation of thread.h virtual method start
	virtual void run();

	void registerNode();

	CSyncObject getNodeForPathMutex;


};

#endif /* SRC_MODULES_OPC_UA_OPCUAHANDLER_H_ */
