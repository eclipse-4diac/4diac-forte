/*******************************************************************************
 * Copyright (c) 2024, 2025 Primetals Technologies Austria GmbH
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

#include "opcua_layer.h"
#include "open62541.h"

namespace forte::com_infra::opc_ua {
  class COPC_UA_HandlerAbstract;
  class CAlarmActionInfo;

  class COPC_UA_AC_Layer : public COPC_UA_Layer {
    public:
      COPC_UA_AC_Layer(CComLayer *paUpperLayer, CBaseCommFB *paComFB);

      ~COPC_UA_AC_Layer() override;

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
      enum Parameters { TypeName, PathToInstance };

      COPC_UA_HandlerAbstract *mHandler;

      UA_NodeId mTypeNodeId;
      UA_NodeId mConditionSourceId;
      UA_NodeId mConditionInstanceId;

      std::vector<char *> mNames;
      std::vector<UA_NodeId> mTypePropertyNodes;
      std::unique_ptr<CAlarmActionInfo> mMemberActionInfo;

      bool mIsStateActive;
      bool mIsStateAcked;
      bool mHasSeverityProperty = false;
      int mMessageTextPortIndex = -1;
      std::unordered_map<std::string, UA_NodeId> mUAPropertyMap = {
          {"ClientUserId", UA_NODEID_NULL},
          {"ConditionName", UA_NODEID_NULL},
          {"SourceName", UA_NODEID_NULL},
          {"Severity", UA_NODEID_NULL},
      };

      std::unordered_map<std::string, TPortId> mFBOutputMap = {
          {"Active", -1},
          {"Acknowledged", -1},
      };

      std::vector<std::unique_ptr<CIEC_ANY>> mRDBuffer;

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

      void initializeRDBuffer();

      UA_StatusCode triggerAlarm(bool paActivate);

      UA_StatusCode resetAckedState(UA_Server *paServer);

      UA_StatusCode setConditionField(UA_Server *paServer,
                                      UA_QualifiedName paQualifiedName,
                                      void *dataValue,
                                      UA_DataType *paDataType);
      UA_StatusCode setConditionVariableFieldProperty(UA_Server *paServer,
                                                      UA_QualifiedName paQualifiedName,
                                                      void *dataValue,
                                                      UA_DataType *paDataType);

      EComResponse initOPCUAType(UA_Server *paServer, const std::string &paTypeName, bool paIsPublisher);

      EComResponse createOPCUAObject(UA_Server *paServer, const std::string &paPathToInstance, bool paIsPublisher);

      UA_StatusCode
      createOPCUAObjectNode(UA_Server *paServer, const std::string &paPathToInstance, std::string &paBrowsePath);

      UA_StatusCode addOPCUACondition(UA_Server *paServer, std::string &paBrowsePath);

      UA_StatusCode initializeMapping();

      UA_BrowseResult browseNode(UA_NodeId &paNodeId);

      bool isFullyInitialized(const std::string &paTypeName);

      bool checkFBOutputNames();

      bool checkFirstDataPinType();

      bool getTriggerValue();

      EComResponse setConditionCallbacks(UA_Server *paServer);

      EComResponse createAlarmType(UA_Server *paServer, const std::string &paTypeName);

      EComResponse addOPCUATypeProperties(UA_Server *paServer, const std::string &paTypeName);

      EComResponse addOPCUATypeEnableStateProperty(UA_Server *paServer);

      UA_StatusCode addVariableNode(UA_Server *paServer,
                                    const std::string &paParentTypeName,
                                    char *paVariableName,
                                    CIEC_ANY &paVariableType);

      EComResponse initializeMemberActions(const std::string &paParentBrowsePath);

      void addNewNodeId(UA_NodeId *paNodeIdToAdd);

      bool isOPCUAObjectPresent(std::string &paBrowsePath, UA_NodeId *paNewNodeId);

      const SFBInterfaceSpec &getParentInterfaceSpec();

      char *getNameFromString(const std::string &paName);

      static UA_StatusCode onEnabled(UA_Server *server, const UA_NodeId *condition);

      static UA_StatusCode onActive(UA_Server *server, const UA_NodeId *condition);

      static UA_StatusCode onAcknowledged(UA_Server *server, const UA_NodeId *condition);
  };
} // namespace forte::com_infra::opc_ua
