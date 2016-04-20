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


#ifndef SRC_MODULES_OPC_UA_OPCUA_LAYER_H_
#define SRC_MODULES_OPC_UA_OPCUA_LAYER_H_

#include "comlayer.h"
#include <forte_any.h>
#include <open62541/build/open62541.h>
#include "commfb.h"
#include "../../arch/devlog.h"



class COPC_UA_Layer : public forte::com_infra::CComLayer{
public:
	COPC_UA_Layer(forte::com_infra::CComLayer* pa_poUpperLayer, forte::com_infra::CCommFB* pa_poComFB);
	virtual ~COPC_UA_Layer();

	void closeConnection();
	forte::com_infra::EComResponse sendData(void* pa_pvData, unsigned int pa_unSize);
	forte::com_infra::EComResponse recvData(const void * pa_pvData, unsigned int pa_unSize);
	forte::com_infra::EComResponse processInterrupt();

private:
	forte::com_infra::EComResponse openConnection(char * paLayerParameter);

	forte::com_infra::EComResponse createItems(CIEC_ANY *paDataArray, int numDI, char *paLayerParameter);

	forte::com_infra::EComResponse mInterruptResp;

	struct {UA_NodeId **ppNodeId_ParentFB;	//!< pointer to the array of Parent FunctionBlock NodeIds managed by the OCP_UA Server Stack
				UA_NodeId **ppNodeId_SrcPoint;} st_ParentChildNodeId;	//!< pointer to the array of SourcePoint (SDs) NodeIds managed by the OCP_UA Server Stack

};

#endif /* SRC_MODULES_OPC_UA_OPCUA_LAYER_H_ */

