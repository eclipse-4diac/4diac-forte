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

#include "forte/cominfra/comlayer.h"
#include "open62541.h"

namespace forte::com_infra::opc_ua {
  class COPC_UA_HandlerAbstract;

  class COPC_UA_Event_Layer : public CComLayer {
    public:
      COPC_UA_Event_Layer(CComLayer *paUpperLayer, CBaseCommFB *paComFB);

      ~COPC_UA_Event_Layer() override;

      /**
       * The handler uses this functions to let the layer know that some data has arrived
       * @param paData Data of type COPC_UA_Helper::UA_RecvVariable_handle passed from the handler
       * @param paSize not used
       * @return
       */
      EComResponse recvData(const void *paData, unsigned int paSize) override;

      /**
       * Executes the action in the handler
       * @param paData not used
       * @param paSize not used
       * @return
       */
      EComResponse sendData(void *paData, unsigned int paSize) override;

      /**
       * Function called when the external event (triggered when data is received) is executed in the FB
       * @return
       */
      EComResponse processInterrupt() override;

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

      UA_NodeId mEventTypeNode;

      /**
       * Called when INIT is triggered in the FB and QI is set to true
       * @param paLayerParameter String conatained between the square brackets in the ID data input (opc_ua[...])
       * @return e_InitOk is initialization was ok, e_InitTerminated otherwise
       */
      EComResponse openConnection(char *paLayerParameter) override;

      /**
       * Called when INIT is triggered in the FB and QI is set to false
       */
      void closeConnection() override;

      EComResponse createOPCUAEvent(UA_Server *paServer);

      static UA_StatusCode addNewEventType(UA_Server *paServer, UA_NodeId &paEventType, std::string paEventTypeName);

      static UA_StatusCode
      addNewEventInstance(UA_Server *paServer, UA_NodeId &paEventType, UA_NodeId &paNodeId, CBaseCommFB *paFb);

      static UA_StatusCode writeTimeAndSourceProperty(UA_Server *paServer, UA_NodeId &paNodeId);

      static UA_StatusCode writeDefaultProperties(UA_Server *paServer, UA_NodeId &paNodeId);
  };
} // namespace forte::com_infra::opc_ua
