/*******************************************************************************
 * Copyright (c) 2024 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Markus Meingast - initial implementation
 *******************************************************************************/

#pragma once

#include "../../core/cominfra/comlayer.h"
#include "open62541.h"

// class COPC_UA_Local_Handler;
class COPC_UA_HandlerAbstract;

class COPC_UA_Event_Layer : public forte::com_infra::CComLayer {
  public:

    COPC_UA_Event_Layer(forte::com_infra::CComLayer *paUpperLayer, forte::com_infra::CBaseCommFB *paComFB);

    ~COPC_UA_Event_Layer() override;

    /**
     * The handler uses this functions to let the layer know that some data has arrived
     * @param paData Data of type COPC_UA_Helper::UA_RecvVariable_handle passed from the handler
     * @param paSize not used
     * @return
     */
    forte::com_infra::EComResponse recvData(const void *paData, unsigned int paSize) override;

    /**
     * Executes the action in the handler
     * @param paData not used
     * @param paSize not used
     * @return
     */
    forte::com_infra::EComResponse sendData(void *paData, unsigned int paSize) override;

    /**
     * Function called when the external event (triggered when data is received) is executed in the FB
     * @return
     */
    forte::com_infra::EComResponse processInterrupt() override;

  private:

    static const size_t scmNumberOfParameters = 1;

    static const UA_UInt16 scmServerNSIndex = 0;

    static char smEmptyString[];
    
    static char smEventTimeProperty[];

    static char smEventSeverityProperty[];
    
    static char smEventMessageProperty[];
    
    static char smEventSourceProperty[];

    COPC_UA_HandlerAbstract *mHandler;

    std::string mEventTypeName;

    std::string mEventInstanceName;

    UA_NodeId mEventTypeNode;

    UA_NodeId mEventInstanceNode;

    /**
     * Called when INIT is triggered in the FB and QI is set to true
     * @param paLayerParameter String conatained between the square brackets in the ID data input (opc_ua[...])
     * @return e_InitOk is initialization was ok, e_InitTerminated otherwise
     */
    forte::com_infra::EComResponse openConnection(char *paLayerParameter) override;

    /**
     * Called when INIT is triggered in the FB and QI is set to false
     */
    void closeConnection() override;

    forte::com_infra::EComResponse createOPCUAEvent(UA_Server *paServer);

    static UA_StatusCode addNewEventType(UA_Server *paServer, UA_NodeId &paEventType, char* eventTypeName);

    static UA_StatusCode addNewEventInstance(UA_Server *paServer, UA_NodeId &paEventType, UA_NodeId &paNodeId);
};