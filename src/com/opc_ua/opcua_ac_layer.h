

#pragma once

#include "../../core/cominfra/comlayer.h"
#include "open62541.h"

// class COPC_UA_Local_Handler;
class COPC_UA_HandlerAbstract;

class COPC_UA_AC_Layer : public forte::com_infra::CComLayer {
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
      Mode,
      InitType,
      TypeName
    };

    static const std::string scmModeINITMSG;
    static const std::string scmModeINITUSERTEXT;
    static const std::string scmModeTRIGGER;
    static const std::string scmTypeALARM;
    static const std::string scmTypeEVENT;

    static const std::string scmAlarmTypeBrowsePath;
    static const std::string scmEventTypeBrowsePath;
    static const std::string scmUserTextAttribute;

    static char smEmptyString[];

    static const size_t scmNumberOfParameters = 3;

    COPC_UA_HandlerAbstract *mHandler;

    UA_NodeId mTypeNodeId;
    UA_NodeId mConditionSourceId;
    UA_NodeId mConditionInstanceId;
    std::vector<char*> mTypeNames;
    std::vector<UA_NodeId> mTypePropertyNodes;

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

    forte::com_infra::EComResponse initOPCUAType(const std::string &paMode, const std::string &paType, const std::string &paTypeName);

    forte::com_infra::EComResponse createOPCUAObject(const std::string &paType, const std::string &paTypeName);

    UA_StatusCode createOPCUAObjectNode(UA_Server *paServer);

    UA_StatusCode addOPCUACondition(UA_Server *paServer);

    forte::com_infra::EComResponse createAlarmType(UA_Server *paServer, const std::string &paTypeName);

    forte::com_infra::EComResponse createEventType(UA_Server *paServer, const std::string &paTypeName);
    
    forte::com_infra::EComResponse addOPCUATypeProperties(UA_Server *paServer, const std::string &paMode, const std::string &paTypeName);

    forte::com_infra::EComResponse addOPCUATypeMSGProperties(UA_Server *paServer, const std::string &paParentTypeName, bool paIsPublisher);

    forte::com_infra::EComResponse addOPCUATypeUSERProperties(UA_Server *paServer, const std::string &paParentTypeName, bool paIsPublisher);

    UA_StatusCode addVariableNode(UA_Server *paServer, const std::string &paParentTypeName, char *paVariableName, CIEC_ANY &paVariableType);

    bool isOPCUAObjectPresent(std::string &paBrowsePath);

    std::string getPortNameFromConnection(CStringDictionary::TStringId paPortNameId, bool paIsPublisher);

    std::string getFBNameFromConnection();

    char *getNameFromString(const std::string &paName);
};