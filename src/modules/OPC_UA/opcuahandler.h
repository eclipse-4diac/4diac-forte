/*******************************************************************************
 * Copyright (c) 2015-2016 Florian Froschermeier <florian.froschermeier@tum.de>
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Florian Froschermeier
 *      - initial integration of the OPC-UA protocol
 *******************************************************************************/


#ifndef SRC_MODULES_OPC_UA_OPCUAHANDLER_H_
#define SRC_MODULES_OPC_UA_OPCUAHANDLER_H_

#include <open62541/build/open62541.h>
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
	UA_StatusCode createUAVarNode(const CFunctionBlock* pCFB, SConnectionPoint& sourceRD, UA_NodeId * returnVarNodeId);	// create variable node from SourcePoint Node Id
	UA_StatusCode createUAObjNode(const CFunctionBlock* pCFB, UA_NodeId * returnObjNodeId);	// create object node from Parent Function Block Node Id
	//UA_StatusCode createUAMethodNode(const CFunctionBlock* pCFB, UA_NodeId * returnMethodNodeId);
	UA_StatusCode assembleUANodeId(char* NodeIdString, UA_NodeId *returnNodeId);

	/* OPC_UA Handler interaction */
	void updateNodeValue(UA_NodeId * pNodeId, CIEC_ANY &paDataPoint);
	UA_StatusCode registerNodeCallBack(UA_NodeId *paNodeId, forte::com_infra::CComLayer *paLayer);
	static void onWrite(void *h, const UA_NodeId nodeid, const UA_Variant *data,
			const UA_NumericRange *range);
	// void handleWriteNodeCallback();		// Value Callback on write UA_Variable Node

	static const int scmUADataTypeMapping[];


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
	void configureUAServer(int UAServerPort);

	// implementation of thread.h virtual method start
	virtual void run();

	void registerNode();


};

#endif /* SRC_MODULES_OPC_UA_OPCUAHANDLER_H_ */
