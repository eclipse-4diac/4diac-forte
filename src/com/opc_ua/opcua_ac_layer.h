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

#include "opcua_layer.h"
#include "open62541.h"

class COPC_UA_HandlerAbstract;
class CActionInfo;

class COPC_UA_AC_Layer : public COPC_UA_Layer {
  public:

    COPC_UA_AC_Layer(forte::com_infra::CComLayer *paUpperLayer, forte::com_infra::CBaseCommFB *paComFB);

    ~COPC_UA_AC_Layer() override;

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

    enum Parameters {
      TypeName,
      PathToInstance
    };

    static const std::string scmAlarmTypeBrowsePath;
    static const std::string scmAlarmConditionName;

    static char smEmptyString[];
    static char smEnabledState[];
    static char smActiveState[];
    static char smId[];
    static char smTime[];
    static char smRetain[];
    static char smSeverity[];

    static UA_UInt16 smSeverityValue;

    static const size_t scmNumberOfAlarmParameters = 2;

    COPC_UA_HandlerAbstract *mHandler;

    UA_NodeId mTypeNodeId;
    UA_NodeId mConditionSourceId;
    UA_NodeId mConditionInstanceId;
   
    std::vector<char*> mNames;
    std::vector<UA_NodeId> mTypePropertyNodes;
    std::unique_ptr<CActionInfo> mMemberActionInfo;

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

    UA_StatusCode triggerAlarm();

    forte::com_infra::EComResponse initOPCUAType(UA_Server *paServer, const std::string &paTypeName, bool paIsPublisher);

    forte::com_infra::EComResponse createOPCUAObject(UA_Server *paServer, const std::string &paPathToInstance, bool paIsPublisher);

    UA_StatusCode createOPCUAObjectNode(UA_Server *paServer, const std::string &paPathToInstance, std::string &paBrowsePath, bool paIsPublisher);

    UA_StatusCode addOPCUACondition(UA_Server *paServer, const std::string &paPathToInstance, std::string &paBrowsePath);

    forte::com_infra::EComResponse setConditionCallbacks(UA_Server *paServer);

    forte::com_infra::EComResponse createAlarmType(UA_Server *paServer, const std::string &paTypeName);
    
    forte::com_infra::EComResponse addOPCUATypeProperties(UA_Server *paServer, const std::string &paTypeName, bool paIsPublisher);

    UA_StatusCode addVariableNode(UA_Server *paServer, const std::string &paParentTypeName, char *paVariableName, CIEC_ANY &paVariableType);

    forte::com_infra::EComResponse initializeMemberActions(const std::string &paParentBrowsePath);

    bool isOPCUAObjectPresent(std::string &paBrowsePath);

    std::string getPortNameFromConnection(CStringDictionary::TStringId paPortNameId, bool paIsPublisher);

    std::string getFBNameFromConnection(bool paIsPublisher);

    char *getNameFromString(const std::string &paName);

    static UA_StatusCode enabledStateCallback(UA_Server *server, const UA_NodeId *condition);

    static UA_StatusCode activeStateCallback(UA_Server *server, const UA_NodeId *condition);

    static UA_StatusCode ackedStateCallback(UA_Server *server, const UA_NodeId *condition);
};