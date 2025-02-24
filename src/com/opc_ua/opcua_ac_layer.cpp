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

#include "opcua_ac_layer.h"
#include "core/util/parameterParser.h"
#include "core/cominfra/basecommfb.h"
#include "opcua_local_handler.h"
#include "opcua_objectstruct_helper.h"
#include "opcua_action_info.h"
#include <memory>

using namespace forte::com_infra;

const std::string COPC_UA_AC_Layer::scmAlarmTypeBrowsePath = "/Types/0:EventTypes/0:BaseEventType/0:ConditionType/0:AcknowledgeableConditionType/0:AlarmConditionType/%d:";
const std::string COPC_UA_AC_Layer::scmAlarmConditionName = "AlarmCondition";

char COPC_UA_AC_Layer::smEmptyString[] = "";
char COPC_UA_AC_Layer::smEnabledState[] = "EnabledState";
char COPC_UA_AC_Layer::smActiveState[] = "ActiveState";
char COPC_UA_AC_Layer::smId[] = "Id";
char COPC_UA_AC_Layer::smTime[] = "Time";
char COPC_UA_AC_Layer::smRetain[] = "Retain";
char COPC_UA_AC_Layer::smSeverity[] = "Severity";

UA_UInt16 COPC_UA_AC_Layer::smSeverityValue = 500;

COPC_UA_AC_Layer::COPC_UA_AC_Layer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
  COPC_UA_Layer(paUpperLayer, paComFB), mHandler(nullptr), mMemberActionInfo(nullptr) {
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
  CParameterParser parser(paLayerParameter, ';');
  size_t nrOfParams = parser.parseParameters();
  if(nrOfParams != scmNumberOfAlarmParameters) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Wrong number of parameters for FB %s. Expected: %d, Actual: %d\n", getCommFB()->getInstanceName(), scmNumberOfAlarmParameters, nrOfParams);
    return eRetVal;
  }
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
    return eRetVal;
  }
  mHandler = static_cast<COPC_UA_HandlerAbstract*>(&getExtEvHandler<COPC_UA_Local_Handler>());
  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  localHandler->enableHandler();
  UA_Server *server = localHandler->getUAServer();
  if(initOPCUAType(server, parser[TypeName], isPublisher) != e_InitOk) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Initializing Alarm Type for FB %s failed!\n", getCommFB()->getInstanceName());
    return eRetVal;
  }
  std::string instancePath(parser[PathToInstance] ? parser[PathToInstance] : smEmptyString);
  eRetVal = createOPCUAObject(server, instancePath, isPublisher);
  return eRetVal;
}

void COPC_UA_AC_Layer::closeConnection() {
  if(mMemberActionInfo) {
    mHandler->uninitializeAction(*mMemberActionInfo);
  }
}

EComResponse COPC_UA_AC_Layer::recvData(const void*, unsigned int) {
  return e_ProcessDataOk;
}

EComResponse COPC_UA_AC_Layer::sendData(void*, unsigned int) {
  if(mMemberActionInfo) {
    if(mHandler->executeAction(*mMemberActionInfo) != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Sending Alarm Data failed for FB %s!\n", getCommFB()->getInstanceName());
      return e_ProcessDataSendFailed;
    }
  }
  if(triggerAlarm() != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Sending Alarm Data failed for FB %s!\n", getCommFB()->getInstanceName());
    return e_ProcessDataSendFailed;
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
  UA_QualifiedName activeStateField = UA_QUALIFIEDNAME(0,smActiveState);
  UA_QualifiedName activeStateIdField = UA_QUALIFIEDNAME(0,smId);
  UA_QualifiedName retainField = UA_QUALIFIEDNAME(0,smRetain);
  UA_QualifiedName severityField = UA_QUALIFIEDNAME(0,smSeverity);
  UA_QualifiedName timeField = UA_QUALIFIEDNAME(0,smTime);

  UA_Variant value;
  UA_UInt16 *severityValue = &smSeverityValue;
  UA_Variant_setScalar(&value, severityValue, &UA_TYPES[UA_TYPES_UINT16]);
  UA_StatusCode  status = UA_Server_setConditionField(server, mConditionInstanceId,
                                      &value, severityField);

  UA_Boolean retainValue = true;
  UA_Variant_setScalar(&value, &retainValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
  status |= UA_Server_setConditionField(server, mConditionInstanceId,
                                                      &value, retainField);
  
  UA_Boolean activeState = true;
  UA_Variant_setScalar(&value, &activeState, &UA_TYPES[UA_TYPES_BOOLEAN]);
  status |= UA_Server_setConditionVariableFieldProperty(server, mConditionInstanceId,
                                              &value, activeStateField,
                                              activeStateIdField);             

  UA_DateTime alarmTime = UA_DateTime_now();
  status |= UA_Server_writeObjectProperty_scalar(server, mConditionInstanceId,
                                                  timeField, &alarmTime,
                                                  &UA_TYPES[UA_TYPES_DATETIME]);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Triggering Alarm failed for FB %s, StatusCode: %s\n", getCommFB()->getInstanceName(), UA_StatusCode_name(status));
  }                      
  return status;
}

EComResponse COPC_UA_AC_Layer::initOPCUAType(UA_Server *paServer, const std::string &paTypeName, bool paIsPublisher) {
  EComResponse eRetVal = createAlarmType(paServer, paTypeName);
  return eRetVal == e_InitOk ? addOPCUATypeProperties(paServer, paTypeName, paIsPublisher) : eRetVal;
}

EComResponse COPC_UA_AC_Layer::createOPCUAObject(UA_Server *paServer, const std::string &paPathToInstance, bool paIsPublisher) {
  std::string objectBrowsePath;
  if(createOPCUAObjectNode(paServer, paPathToInstance, objectBrowsePath, paIsPublisher) != UA_STATUSCODE_GOOD) {
    return e_InitTerminated;
  }
  std::string conditionBrowsePath;
  if(addOPCUACondition(paServer, objectBrowsePath, conditionBrowsePath) != UA_STATUSCODE_GOOD) {
    return e_InitTerminated;
  }
  if(initializeMemberActions(conditionBrowsePath) != e_InitOk) {
    return e_InitTerminated;
  }
  return setConditionCallbacks(paServer);
}

UA_StatusCode COPC_UA_AC_Layer::createOPCUAObjectNode(UA_Server *paServer, const std::string &paPathToInstance, std::string &paBrowsePath, bool paIsPublisher) {
  if(!COPC_UA_Helper::isBrowsePathValid(paPathToInstance)) {
    return UA_STATUSCODE_BAD;
  }
  std::string instanceNameStr(getFBNameFromConnection(paIsPublisher));
  if(instanceNameStr.empty()) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Retrieving FB Instance Name failed!");
    return UA_STATUSCODE_BAD;
  }

  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  CSinglyLinkedList<UA_NodeId*> referencedNodes;
  paBrowsePath = COPC_UA_ObjectStruct_Helper::getMemberBrowsePath(paPathToInstance, instanceNameStr);
  UA_StatusCode status = localHandler->splitAndCreateFolders(paBrowsePath, instanceNameStr, referencedNodes);   // Overwrites instanceNameStr to the same value as before, but we get the OPC UA folders
  if(status != UA_STATUSCODE_GOOD || referencedNodes.isEmpty()) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Creating OPC UA Folders failed, Browsepath: %s, StatusCode: %s\n", paBrowsePath.c_str(), UA_StatusCode_name(status));
    return UA_STATUSCODE_BAD;
  }

  char *instanceName = getNameFromString(instanceNameStr);
  char *browsePath = getNameFromString(paBrowsePath);    
  const UA_NodeId *parentNodeId = *referencedNodes.back();
  mConditionSourceId = UA_NODEID_STRING(1, browsePath);     // TODO Change 1 to namespaceIndex
  UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    attr.eventNotifier = UA_EVENTNOTIFIER_SUBSCRIBE_TO_EVENT;
    attr.displayName = UA_LOCALIZEDTEXT(smEmptyString, instanceName);
    status =  UA_Server_addObjectNode(paServer, mConditionSourceId,
                              *parentNodeId,
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
    for(CSinglyLinkedList<UA_NodeId*>::Iterator itReferencedNodes = referencedNodes.begin(); itReferencedNodes != referencedNodes.end(); ++itReferencedNodes) {
      UA_NodeId_delete(*itReferencedNodes);
    }
    return status;
}

UA_StatusCode COPC_UA_AC_Layer::addOPCUACondition(UA_Server *paServer, const std::string &paPathToInstance, std::string &paBrowsePath) {
  char *conditionName = getNameFromString(scmAlarmConditionName);
  paBrowsePath = COPC_UA_ObjectStruct_Helper::getMemberBrowsePath(paPathToInstance, scmAlarmConditionName);
  char *conditionBrowsePath = getNameFromString(paBrowsePath);
  mConditionInstanceId = UA_NODEID_STRING(1, conditionBrowsePath);      // TODO Change 1 to namespaceIndex   
  UA_StatusCode status = UA_Server_createCondition(paServer, mConditionInstanceId, mTypeNodeId,
                          UA_QUALIFIEDNAME(1, conditionName), mConditionSourceId,
                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                          &mConditionInstanceId);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Adding Condition failed for FB %s. StatusCode %s\n", getCommFB()->getInstanceName(), UA_StatusCode_name(status)); 
    return status;
  }
  UA_QualifiedName enabledStateField = UA_QUALIFIEDNAME(0, smEnabledState);
  UA_QualifiedName enabledStateIdField = UA_QUALIFIEDNAME(0, smId);
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

EComResponse COPC_UA_AC_Layer::setConditionCallbacks(UA_Server *paServer) {
  EComResponse eRetVal = e_InitOk;
  UA_TwoStateVariableChangeCallback callback = enabledStateCallback;
  UA_StatusCode status = UA_Server_setConditionTwoStateVariableCallback(paServer, mConditionInstanceId,
                                                                        mConditionSourceId, false,
                                                                        callback, UA_ENTERING_ENABLEDSTATE);
  callback = activeStateCallback;
  status |= UA_Server_setConditionTwoStateVariableCallback(paServer, mConditionInstanceId,
                                                            mConditionSourceId, false,
                                                            callback, UA_ENTERING_ACTIVESTATE);
  callback = ackedStateCallback;
  status |= UA_Server_setConditionTwoStateVariableCallback(paServer, mConditionInstanceId,
                                                           mConditionSourceId, false,
                                                           callback, UA_ENTERING_ACKEDSTATE);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Setting Condition callback methods failed for FB %s, Status Code: %s\n", getCommFB()->getInstanceName(), UA_StatusCode_name(status));
    eRetVal = e_InitTerminated;
  }
  return eRetVal;
}

EComResponse COPC_UA_AC_Layer::initializeMemberActions(const std::string &paParentBrowsePath) {
  mMemberActionInfo.reset(new CActionInfo(*this, CActionInfo::UA_ActionType::eWrite, std::string()));
  size_t numPorts = getCommFB()->getNumSD();
  const SFBInterfaceSpec &interfaceSpec = getCommFB()->getFBInterfaceSpec();
  const CStringDictionary::TStringId *dataPortNameIds = interfaceSpec.mDINames;
  for(size_t i = 0; i < numPorts; i++) {
    std::string dataPortName = getPortNameFromConnection(dataPortNameIds[i+2], true);
    std::string memberBrowsePath(COPC_UA_ObjectStruct_Helper::getMemberBrowsePath(paParentBrowsePath, dataPortName));
    UA_NodeId *nodeId = COPC_UA_ObjectStruct_Helper::createStringNodeIdFromBrowsepath(memberBrowsePath);
    mMemberActionInfo->getNodePairInfo().emplace_back(nodeId, memberBrowsePath);
  }
  if(mHandler->initializeAction(*mMemberActionInfo) != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Error occured in FB %s while initializing members\n", getCommFB()->getInstanceName());
      return e_InitTerminated;
    }
  return e_InitOk;
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

EComResponse COPC_UA_AC_Layer::addOPCUATypeProperties(UA_Server *paServer, const std::string &paTypeName, bool paIsPublisher) {
  CIEC_ANY **apoDataPorts = paIsPublisher ? getCommFB()->getSDs() : getCommFB()->getRDs();
  size_t numDataPorts = paIsPublisher ? getCommFB()->getNumSD() : getCommFB()->getNumRD();
  const SFBInterfaceSpec &interfaceSpec = getCommFB()->getFBInterfaceSpec();
  const CStringDictionary::TStringId *dataPortNameIds = paIsPublisher ? interfaceSpec.mDINames : interfaceSpec.mDONames;
  for(size_t i = 0; i < numDataPorts; i++) {
    std::string dataPortName = getPortNameFromConnection(dataPortNameIds[i+2], paIsPublisher);
    char* propertyName = getNameFromString(dataPortName);
    UA_StatusCode status = addVariableNode(paServer, paTypeName, propertyName, apoDataPorts[i]->unwrap());
    if(status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to add OPCUA AlarmType Property for FB %s, Port: %s, Status: %s\n", getCommFB()->getInstanceName(), dataPortName, UA_StatusCode_name(status));
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
  return std::string(CStringDictionary::getInstance().get(connectionPoint.mFB->getFBInterfaceSpec().mDINames[portId]));
}

std::string COPC_UA_AC_Layer::getFBNameFromConnection(bool paIsPublisher) {
  const CStringDictionary::TStringId *dataPortNameIds = paIsPublisher ? getCommFB()->getFBInterfaceSpec().mDINames : getCommFB()->getFBInterfaceSpec().mDONames;
  const CDataConnection *portConnection = paIsPublisher ? getCommFB()->getDIConnection(dataPortNameIds[2]) : getCommFB()->getDOConnection(dataPortNameIds[2]);
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

UA_StatusCode COPC_UA_AC_Layer::enabledStateCallback(UA_Server *server, const UA_NodeId *condition) {
  UA_DateTime dateTime = UA_DateTime_now();
  UA_StatusCode status = UA_Server_writeObjectProperty_scalar(server, *condition,
                                            UA_QUALIFIEDNAME(0, smTime),
                                            &dateTime,
                                            &UA_TYPES[UA_TYPES_DATETIME]);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Enable State callback failed, Status Code: %s\n", UA_StatusCode_name(status));
  }
  return status;
}

UA_StatusCode COPC_UA_AC_Layer::activeStateCallback(UA_Server *server, const UA_NodeId *condition) {
  UA_DateTime dateTime = UA_DateTime_now();
  UA_StatusCode status = UA_Server_writeObjectProperty_scalar(server, *condition,
                                            UA_QUALIFIEDNAME(0, smTime),
                                            &dateTime,
                                            &UA_TYPES[UA_TYPES_DATETIME]);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Active State callback failed, Status Code: %s\n", UA_StatusCode_name(status));
  }
  return status;
}

UA_StatusCode COPC_UA_AC_Layer::ackedStateCallback(UA_Server *server, const UA_NodeId *condition) {
  UA_Boolean activeStateId = false;
  UA_Variant value;
  UA_QualifiedName activeStateField = UA_QUALIFIEDNAME(0,smActiveState);
  UA_QualifiedName activeStateIdField = UA_QUALIFIEDNAME(0,smId);

  UA_Variant_setScalar(&value, &activeStateId, &UA_TYPES[UA_TYPES_BOOLEAN]);
  UA_StatusCode status = UA_Server_setConditionVariableFieldProperty(server, *condition,
                                                                    &value, activeStateField,
                                                                    activeStateIdField);

  UA_DateTime dateTime = UA_DateTime_now();
  status |= UA_Server_writeObjectProperty_scalar(server, *condition,
                                                UA_QUALIFIEDNAME(0, smTime),
                                                &dateTime, &UA_TYPES[UA_TYPES_DATETIME]);
  if(status != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("[OPC UA A&C LAYER]: Acknowledged State callback failed, Status Code: %s\n", UA_StatusCode_name(status));
  }
  return status;
}