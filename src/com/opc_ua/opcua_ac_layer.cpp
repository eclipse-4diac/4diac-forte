

#include "opcua_ac_layer.h"
#include "../../core/utils/parameterParser.h"
#include "../../core/cominfra/basecommfb.h"
#include "opcua_local_handler.h"
#include "opcua_objectstruct_helper.h"

using namespace forte::com_infra;

const std::string COPC_UA_AC_Layer::scmModeINITMSG = "INITMSG";
const std::string COPC_UA_AC_Layer::scmModeINITUSERTEXT = "INITUSERTEXT";
const std::string COPC_UA_AC_Layer::scmModeTRIGGER = "TRIGGER";
const std::string COPC_UA_AC_Layer::scmTypeALARM = "Alarm";
const std::string COPC_UA_AC_Layer::scmTypeEVENT = "Event";

const std::string COPC_UA_AC_Layer::scmAlarmTypeBrowsePath = "/Types/0:EventTypes/0:BaseEventType/0:ConditionType/0:AcknowledgeableConditionType/0:AlarmConditionType/%d:";
const std::string COPC_UA_AC_Layer::scmEventTypeBrowsePath = "/Types/0:EventTypes/0:BaseEventType/%d:";
const std::string COPC_UA_AC_Layer::scmUserTextAttribute = "TEXT0%d";


const std::string COPC_UA_AC_Layer::scmAlarmConditionName = "AlarmCondition";

char COPC_UA_AC_Layer::smEmptyString[] = "";

COPC_UA_AC_Layer::COPC_UA_AC_Layer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
  CComLayer(paUpperLayer, paComFB), mHandler(nullptr) {
  
}

COPC_UA_AC_Layer::~COPC_UA_AC_Layer() {
  UA_NodeId_clear(&mConditionInstanceId);
  UA_NodeId_clear(&mConditionSourceId);
  for(UA_NodeId nodeId : mTypePropertyNodes) {
    UA_NodeId_clear(&nodeId);
  }
  UA_NodeId_clear(&mTypeNodeId);
  for(char *typeName : mNames) {
    delete[] typeName;
  }
}

EComResponse COPC_UA_AC_Layer::openConnection(char *paLayerParameter) {
  EComResponse eRetVal = e_InitTerminated;
  CParameterParser parser(paLayerParameter, ';', scmNumberOfParameters);
  size_t nrOfParams = parser.parseParameters();
  if(nrOfParams == scmNumberOfParameters) {
    std::string mode = parser[Mode];
    mInitType = parser[InitType];
    mHandler = static_cast<COPC_UA_HandlerAbstract*>(&getExtEvHandler<COPC_UA_Local_Handler>());
    if(mode == scmModeINITMSG || mode == scmModeINITUSERTEXT) {
      eRetVal = initOPCUAType(mode, mInitType, parser[TypeName]);
    } else if (mode == scmModeTRIGGER) {
      eRetVal = createOPCUAObject(mInitType, parser[TypeName]);
    } else {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Wrong usage of layer parameters! Expected first param: %s | %s | %s, actual: %s\n", scmModeINITMSG.c_str(), scmModeINITUSERTEXT.c_str(), scmModeTRIGGER.c_str(), mode.c_str());
      return eRetVal;   
    }
  }
  return eRetVal;
}

void COPC_UA_AC_Layer::closeConnection() {
  // TODO
}

EComResponse COPC_UA_AC_Layer::recvData(const void*, unsigned int) {
  return e_ProcessDataOk;
}

EComResponse COPC_UA_AC_Layer::sendData(void*, unsigned int) {
  if(mInitType == scmTypeALARM) {
    if(triggerAlarm() != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Sending Alarm Data failed!\n");
      return e_ProcessDataSendFailed;
    }
  } else if(mInitType == scmTypeEVENT) {
    if(triggerEvent() != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Sending Event Data failed!\n");
      return e_ProcessDataSendFailed;
    }
  } else {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: On Send Data - %s is not a valid Init type!\n", mInitType); 
  }
  return e_ProcessDataOk;
}

EComResponse COPC_UA_AC_Layer::processInterrupt() {
  // TODO
  return e_ProcessDataOk;
}

UA_StatusCode COPC_UA_AC_Layer::triggerAlarm() {
  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  UA_Server *server = localHandler->getUAServer();
  char *activeStateProperty = getNameFromString("ActiveState");
  char *idProperty = getNameFromString("Id");
  char *timeProperty = getNameFromString("Time");
  char *retainProperty = getNameFromString("Retain");
  UA_QualifiedName activeStateField = UA_QUALIFIEDNAME(0,activeStateProperty);
  UA_QualifiedName activeStateIdField = UA_QUALIFIEDNAME(0,idProperty);
  UA_QualifiedName retainField = UA_QUALIFIEDNAME(0,retainProperty);

  UA_Variant value;
  UA_Boolean retainValue = true;
  UA_Variant_setScalar(&value, &retainValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
  UA_StatusCode status = UA_Server_setConditionField(server, mConditionInstanceId,
                                                      &value, retainField);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Writing Retain Property failed, StatusCode: %s\n", UA_StatusCode_name(status));
    return status;
  }
  UA_DateTime alarmTime = UA_DateTime_now();
  status = UA_Server_writeObjectProperty_scalar(server, mConditionInstanceId,
                                                  UA_QUALIFIEDNAME(0, timeProperty),
                                                  &alarmTime,
                                                  &UA_TYPES[UA_TYPES_DATETIME]);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Writing Alarm Property failed, StatusCode: %s\n", UA_StatusCode_name(status));
    return status;
  }
  UA_Boolean activeState = true;
  UA_Variant_setScalar(&value, &activeState, &UA_TYPES[UA_TYPES_BOOLEAN]);
  status = UA_Server_setConditionVariableFieldProperty(server, mConditionInstanceId,
                                              &value, activeStateField,
                                              activeStateIdField);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Activating Alarm failed, StatusCode: %s\n", UA_StatusCode_name(status));
  }                                       
  return status;
}

UA_StatusCode COPC_UA_AC_Layer::triggerEvent() {
  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  UA_Server *server = localHandler->getUAServer();
  UA_NodeId eventNode;
  UA_StatusCode status = UA_Server_createEvent(server, mTypeNodeId, &eventNode);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to create OPC UA Event for FB %s. Status: %s\n", getCommFB()->getInstanceName(), UA_StatusCode_name(status));
    return status;
  }

  char *timeProperty = getNameFromString("Time");
  UA_DateTime eventTime = UA_DateTime_now();
  status = UA_Server_writeObjectProperty_scalar(server, eventNode, UA_QUALIFIEDNAME(0, timeProperty),
                                              &eventTime, &UA_TYPES[UA_TYPES_DATETIME]);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to write TimeProperty for OPC UA Event for FB %s. Status: %s\n", getCommFB()->getInstanceName(), UA_StatusCode_name(status));
    return status;
  }
  char *sourceNameProperty = getNameFromString("SourceName");
  char *sourceName = getNameFromString("Server");
  UA_String uaEventSourceName = UA_STRING(sourceName);
  status = UA_Server_writeObjectProperty_scalar(server, eventNode, UA_QUALIFIEDNAME(0, sourceNameProperty),
                                        &uaEventSourceName, &UA_TYPES[UA_TYPES_STRING]);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to write SourceProperty for OPC UA Event for FB %s. Status: %s\n", getCommFB()->getInstanceName(), UA_StatusCode_name(status));
  }

  status = UA_Server_triggerEvent(server, eventNode, 
                                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER),
                                    nullptr, UA_TRUE);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to trigger OPC UA Event for FB %s. Status: %s\n", getCommFB()->getInstanceName(), UA_StatusCode_name(status));
  }
  return status;
}

EComResponse COPC_UA_AC_Layer::initOPCUAType(const std::string &paMode, const std::string &paType, const std::string &paTypeName) {
  EComResponse eRetVal = e_InitTerminated;
  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  localHandler->enableHandler();
  UA_Server *server = localHandler->getUAServer();
  if(paType == scmTypeALARM) {
    eRetVal = createAlarmType(server, paTypeName);
  } else if(paType == scmTypeEVENT) {
    eRetVal = createEventType(server, paTypeName);
  } else {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Wrong usage of layer parameters! Expected second param: %s/%s, actual: %s\n", scmTypeALARM.c_str(), scmTypeEVENT.c_str(), paType.c_str());
    return e_InitTerminated;
  }
    return eRetVal == e_InitOk ? addOPCUATypeProperties(server, paMode, paTypeName) : eRetVal;
}

EComResponse COPC_UA_AC_Layer::createOPCUAObject(const std::string &paType, const std::string &paTypeName) {
  std::string browsePathPrefix;
  if(paType == scmTypeALARM) {
    browsePathPrefix = scmAlarmTypeBrowsePath;
  } else if(paType == scmTypeEVENT) {
    browsePathPrefix = scmEventTypeBrowsePath;
  } else {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Wrong usage of layer parameters! Expected second param: %s/%s, actual: %s\n", scmTypeALARM.c_str(), scmTypeEVENT.c_str(), paType.c_str());
    return e_InitTerminated;
  }
  std::string browsePath(COPC_UA_ObjectStruct_Helper::getBrowsePath(browsePathPrefix, paTypeName, 1));  // TODO Change 1 to namespaceIndex
  if(!isOPCUAObjectPresent(browsePath)) {
     DEVLOG_ERROR("[OPC UA A&C LAYER]: Type %s does not exist in Address Space!\n", paTypeName.c_str());
    return e_InitTerminated;
  }
  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  localHandler->enableHandler();
  UA_Server *server = localHandler->getUAServer();
  if(paType == scmTypeALARM) {
    if(createOPCUAObjectNode(server) != UA_STATUSCODE_GOOD) {
      return e_InitTerminated;
    }
    if(addOPCUACondition(server) != UA_STATUSCODE_GOOD) {
      return e_InitTerminated;
    }
  }
  return e_InitOk;
}

UA_StatusCode COPC_UA_AC_Layer::createOPCUAObjectNode(UA_Server *paServer) {
  std::string instanceNameStr(getFBNameFromConnection());
  if(instanceNameStr.empty()) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Retrieving FB Instance Name failed!");
    return UA_STATUSCODE_BAD;
  }
  char *instanceName = getNameFromString(instanceNameStr);
  char *browsePath = getNameFromString(COPC_UA_ObjectStruct_Helper::getMemberBrowsePath("/Objects", instanceNameStr));    
  mConditionSourceId = UA_NODEID_STRING(1, browsePath);     // TODO Change 1 to namespaceIndex
  UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    attr.eventNotifier = UA_EVENTNOTIFIER_SUBSCRIBE_TO_EVENT;
    attr.displayName = UA_LOCALIZEDTEXT(smEmptyString, instanceName);
    UA_StatusCode status =  UA_Server_addObjectNode(paServer, mConditionSourceId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              UA_QUALIFIEDNAME(1, instanceName),            // TODO Change 1 to namespaceIndex
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                              attr, NULL, &mConditionSourceId);

    if(status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Creating Object Node failed. StatusCode %s\n", UA_StatusCode_name(status));
      return status;
    }
    status = UA_Server_addReference(paServer, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER),
              UA_NODEID_NUMERIC(0, UA_NS0ID_HASNOTIFIER),
              UA_EXPANDEDNODEID_STRING(mConditionSourceId.namespaceIndex, getNameFromString(browsePath)),
              UA_TRUE);
    if(status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Adding reference to Object Node failed. StatusCode %s\n", UA_StatusCode_name(status));
    }
    return status;
}

UA_StatusCode COPC_UA_AC_Layer::addOPCUACondition(UA_Server *paServer) {
  char *conditionName = getNameFromString(scmAlarmConditionName);
  char *conditionBrowsePath = getNameFromString(COPC_UA_ObjectStruct_Helper::getMemberBrowsePath("/Objects/OPCUAMessageHandlerTest", scmAlarmConditionName));
  mConditionInstanceId = UA_NODEID_STRING(1, conditionBrowsePath);      // TODO Change 1 to namespaceIndex   
  UA_StatusCode status = UA_Server_createCondition(paServer, mConditionInstanceId, mTypeNodeId,
                          UA_QUALIFIEDNAME(1, conditionName), mConditionSourceId,
                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                          &mConditionInstanceId);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Adding Condition failed for FB %s. StatusCode %s\n", getCommFB()->getInstanceName(), UA_StatusCode_name(status)); 
    return status;
  }
  char *enabledStateProperty = getNameFromString("EnabledState");
  char *idProperty = getNameFromString("Id");
  UA_QualifiedName enabledStateField = UA_QUALIFIEDNAME(0,enabledStateProperty);
  UA_QualifiedName enabledStateIdField = UA_QUALIFIEDNAME(0,idProperty);
  UA_Boolean enabledState = true;

  UA_Variant value;
  UA_Variant_setScalar(&value, &enabledState, &UA_TYPES[UA_TYPES_BOOLEAN]);
  status = UA_Server_setConditionVariableFieldProperty(paServer, mConditionInstanceId,
                                                        &value, enabledStateField,
                                                        enabledStateIdField);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Enabling Condition failed for FB %s, StatusCode: %s\n", getCommFB()->getInstanceName(), UA_StatusCode_name(status));
  }  
  return status;
}

EComResponse COPC_UA_AC_Layer::createAlarmType(UA_Server *paServer, const std::string &paTypeName) {
  std::string browsePath(COPC_UA_ObjectStruct_Helper::getBrowsePath(scmAlarmTypeBrowsePath, paTypeName, 1));  // TODO Change 1 to namespaceIndex
  if(isOPCUAObjectPresent(browsePath)) {
    return e_InitOk;
  }
  char* typeName = getNameFromString(paTypeName);
  mTypeNodeId = UA_NODEID_STRING(1, typeName);          // TODO Change 1 to namespaceIndex
  UA_ObjectTypeAttributes oAttr = UA_ObjectTypeAttributes_default;
  oAttr.displayName = UA_LOCALIZEDTEXT(smEmptyString, typeName);
  UA_StatusCode status = UA_Server_addObjectTypeNode(paServer, mTypeNodeId,
                          UA_NODEID_NUMERIC(0, UA_NS0ID_ALARMCONDITIONTYPE),
                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                          UA_QUALIFIEDNAME(1, typeName), oAttr,             // TODO Change 1 to namespaceIndex
                          nullptr, &mTypeNodeId);

  if (status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to create OPC UA Alarm Type Node for Type %s, Status Code: %s\n", paTypeName.c_str(), UA_StatusCode_name(status));
    return e_InitTerminated;
  }
  return e_InitOk;
}

EComResponse COPC_UA_AC_Layer::createEventType(UA_Server *paServer, const std::string &paTypeName) {
  std::string browsePath(COPC_UA_ObjectStruct_Helper::getBrowsePath(scmEventTypeBrowsePath, paTypeName, 1));  // TODO Change 1 to namespaceIndex
  if(isOPCUAObjectPresent(browsePath)) {
    return e_InitOk;
  }
  char* typeName = getNameFromString(paTypeName);
  mTypeNodeId = UA_NODEID_STRING(1, typeName);          // TODO Change 1 to namespaceIndex
  UA_ObjectTypeAttributes oAttr = UA_ObjectTypeAttributes_default;
  oAttr.displayName = UA_LOCALIZEDTEXT(smEmptyString, typeName);
  UA_StatusCode status = UA_Server_addObjectTypeNode(paServer, mTypeNodeId,
                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEEVENTTYPE),
                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                          UA_QUALIFIEDNAME(1, typeName), oAttr,             // TODO Change 1 to namespaceIndex
                          nullptr, &mTypeNodeId);

  if (status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to create OPC UA Event Type Node for Type %s, Status Code: %s\n", paTypeName.c_str(), UA_StatusCode_name(status));
    return e_InitTerminated;
  }
  return e_InitOk;
}

EComResponse COPC_UA_AC_Layer::addOPCUATypeProperties(UA_Server *paServer, const std::string &paMode, const std::string &paTypeName) {
  EComResponse eRetVal = e_InitTerminated;
  bool isPublisher;
  switch (mFb->getComServiceType()) {
  case e_Publisher:
    isPublisher = true;
    break;
  case e_Subscriber: 
    isPublisher = false;
    break;
  default:
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Wrong CommFB used for FB %s! Expected: Publish/Subscribe\n", getCommFB()->getInstanceName());
    return e_InitTerminated;
  }

  if(paMode == scmModeINITMSG) {
    eRetVal = addOPCUATypeMSGProperties(paServer, paTypeName, isPublisher);
  } else if (paMode == scmModeINITUSERTEXT) {
    eRetVal = addOPCUATypeUSERProperties(paServer, paTypeName, isPublisher);
  } else {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Wrong mode specified for FB %s, Mode: %s\n", getCommFB()->getInstanceName(), paMode.c_str());
  }
  return eRetVal;
}

forte::com_infra::EComResponse COPC_UA_AC_Layer::addOPCUATypeMSGProperties(UA_Server *paServer, const std::string &paParentTypeName, bool paIsPublisher) {
  CIEC_ANY **apoDataPorts = paIsPublisher ? getCommFB()->getSDs() : getCommFB()->getRDs();
  size_t numDataPorts = paIsPublisher ? getCommFB()->getNumSD() : getCommFB()->getNumRD();
  const SFBInterfaceSpec *interfaceSpec = getCommFB()->getFBInterfaceSpec();
  const CStringDictionary::TStringId *dataPortNameIds = paIsPublisher ? interfaceSpec->mDINames : interfaceSpec->mDONames;
  for(size_t i = 0; i < numDataPorts; i++) {
    std::string dataPortName = getPortNameFromConnection(dataPortNameIds[i+2], paIsPublisher);
    char* propertyName = getNameFromString(dataPortName);
    UA_StatusCode status = addVariableNode(paServer, paParentTypeName, propertyName, apoDataPorts[i]->unwrap());
    if(status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to add OPCUA MSG Property for FB %s, Port: %s, Status: %s\n", getCommFB()->getInstanceName(), dataPortName, UA_StatusCode_name(status));
      return e_InitTerminated;
    }
  }
  return e_InitOk;
}

forte::com_infra::EComResponse COPC_UA_AC_Layer::addOPCUATypeUSERProperties(UA_Server *paServer, const std::string &paParentTypeName, bool paIsPublisher) {
  CIEC_ANY **apoDataPorts = paIsPublisher ? getCommFB()->getSDs() : getCommFB()->getRDs();
  size_t numDataPorts = paIsPublisher ? getCommFB()->getNumSD() : getCommFB()->getNumRD();
  for(size_t i = 0; i < numDataPorts; i++) {
    char* propertyName = new char[scmUserTextAttribute.length() + 2];
    snprintf(propertyName, scmUserTextAttribute.length() + 2, scmUserTextAttribute.c_str(), i+1);
    propertyName[scmUserTextAttribute.length() + 1] = '\0';
    mNames.emplace_back(propertyName);

    UA_StatusCode status = addVariableNode(paServer, paParentTypeName, propertyName, apoDataPorts[i]->unwrap());
    if(status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to add OPCUA MSG Property for FB %s, Port: %s, Status: %s\n", getCommFB()->getInstanceName(), propertyName, UA_StatusCode_name(status));
      return e_InitTerminated;
    }
  }
  return e_InitOk;
}

UA_StatusCode COPC_UA_AC_Layer::addVariableNode(UA_Server *paServer, const std::string &paParentTypeName, char *paVariableName, CIEC_ANY &paVariableType) {
  std::string propertyBrowsePathStr = COPC_UA_ObjectStruct_Helper::getMemberBrowsePath(paParentTypeName, paVariableName);
  char* propertyBrowsePath = getNameFromString(propertyBrowsePathStr);

  UA_VariableAttributes vAttr = UA_VariableAttributes_default;
    vAttr.displayName = UA_LOCALIZEDTEXT(smEmptyString, paVariableName);
    vAttr.valueRank = UA_VALUERANK_SCALAR;
    vAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    vAttr.dataType = COPC_UA_Helper::getOPCUATypeFromAny(paVariableType)->typeId;

  UA_NodeId memberNodeId;
  memberNodeId = UA_NODEID_STRING(1, propertyBrowsePath);     // TODO Change 1 to namespaceIndex
  UA_StatusCode status = UA_Server_addVariableNode(paServer, memberNodeId, mTypeNodeId,
                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                          UA_QUALIFIEDNAME(1, propertyBrowsePath),    // TODO Change 1 to namespaceIndex
                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), vAttr, nullptr, &memberNodeId);
  mTypePropertyNodes.emplace_back(memberNodeId);
  if(status != UA_STATUSCODE_GOOD) {
    return status;
  }
  return status;
}

bool COPC_UA_AC_Layer::isOPCUAObjectPresent(std::string &paBrowsePath) {
  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  if(localHandler) {
    CActionInfo::CNodePairInfo nodePair(nullptr, paBrowsePath);
    if(localHandler->isOPCUAObjectPresent(nodePair)) {
      UA_NodeId_copy(nodePair.getNodeId(), &mTypeNodeId);  
      return true;
    }
  } else {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to get LocalHandler because LocalHandler is null!\n");
  }
  return false;
}

std::string COPC_UA_AC_Layer::getPortNameFromConnection(CStringDictionary::TStringId paPortNameId, bool paIsPublisher) {
  const CDataConnection *portConnection = paIsPublisher ? getCommFB()->getDIConnection(paPortNameId) : getCommFB()->getDOConnection(paPortNameId);
  const CConnectionPoint connectionPoint = portConnection->getSourceId();
  TPortId portId = connectionPoint.mPortId;
  return std::string(CStringDictionary::getInstance().get(connectionPoint.mFB->getFBInterfaceSpec()->mDINames[portId]));
}

std::string COPC_UA_AC_Layer::getFBNameFromConnection() {
  bool isPublisher;
  switch (getCommFB()->getComServiceType()) {
  case e_Publisher:
    isPublisher = true;
    break;
  case e_Subscriber: 
    isPublisher = false;
    break;
  default:
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Wrong CommFB used for FB %s! Expected: Publish/Subscribe\n", getCommFB()->getInstanceName());
    return std::string();
  }
  const CStringDictionary::TStringId *dataPortNameIds = isPublisher ? getCommFB()->getFBInterfaceSpec()->mDINames : getCommFB()->getFBInterfaceSpec()->mDONames;
  const CDataConnection *portConnection = isPublisher ? getCommFB()->getDIConnection(dataPortNameIds[2]) : getCommFB()->getDOConnection(dataPortNameIds[2]);
  if(!portConnection) {
     DEVLOG_ERROR("[OPC UA A&C LAYER]: Error at connection of FB %s!\n", getCommFB()->getInstanceName());
     return std::string();
  }
  const CConnectionPoint connectionPoint = portConnection->getSourceId();
  return std::string(connectionPoint.mFB->getInstanceName());
}

char *COPC_UA_AC_Layer::getNameFromString(const std::string &paName) {
  size_t length = paName.length();
  char* name = new char[length + 1];
  strncpy(name, paName.c_str(), length);
  name[length] = '\0';
  mNames.emplace_back(name);
  return name;
}