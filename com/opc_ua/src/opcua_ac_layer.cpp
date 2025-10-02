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

#include "opcua_ac_layer.h"
#include "forte/util/parameterParser.h"
#include "forte/cominfra/basecommfb.h"
#include "opcua_local_handler.h"
#include "opcua_objectstruct_helper.h"
#include "alarm_action_info.h"
#include "forte/cominfra/comlayersmanager.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

#include <memory>

using namespace forte::literals;

namespace forte::com_infra::opc_ua {
  namespace {
    const std::string scmAlarmTypeBrowsePath =
        "/Types/0:EventTypes/0:BaseEventType/0:ConditionType/0:AcknowledgeableConditionType/0:AlarmConditionType/%d:";
    const std::string scmAlarmConditionName = "AlarmCondition";

    char smEmptyString[] = "";
    char smEnabledState[] = "EnabledState";
    char smEnableStateProperty[] =
        "EnableState"; // This is needed to avoid potential delete-subscription error with HMI tools
    char smActiveState[] = "ActiveState";
    char smAckedState[] = "AckedState";
    char smActive[] = "Active";
    char smAcked[] = "Acknowledged";
    char smId[] = "Id";
    char smTime[] = "Time";
    char smRetain[] = "Retain";
    char smMessage[] = "Message";
    char smSeverity[] = "Severity";
    char smMessageText[] = "MsgText";

    const size_t smFirstDataIndex = 1;
    const size_t scmNumberOfAlarmParameters = 2;
    UA_UInt16 smSeverityValue = 500;

    const std::unordered_map<std::string, std::string> sm1499ToUAMap = {
        {"Area", "ClientUserId"},
        {"Device", "ConditionName"},
        {"Source", "SourceName"},
    };

    [[maybe_unused]] const ComLayerManager::EntryImpl<COPC_UA_AC_Layer> entry("ua_ac"_STRID);
  } // namespace

  COPC_UA_AC_Layer::COPC_UA_AC_Layer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
      COPC_UA_Layer(paUpperLayer, paComFB),
      mHandler(nullptr),
      mMemberActionInfo(nullptr),
      mIsStateActive(false),
      mIsStateAcked(false) {
  }

  COPC_UA_AC_Layer::~COPC_UA_AC_Layer() {
    UA_NodeId_clear(&mConditionInstanceId);
    UA_NodeId_clear(&mConditionSourceId);
    for (UA_NodeId nodeId : mTypePropertyNodes) {
      UA_NodeId_clear(&nodeId);
    }
    UA_NodeId_clear(&mTypeNodeId);
    for (char *typeName : mNames) {
      delete[] typeName;
    }
  }

  EComResponse COPC_UA_AC_Layer::openConnection(char *paLayerParameter) {
    EComResponse eRetVal = e_InitTerminated;
    util::CParameterParser parser(paLayerParameter, ';');
    size_t nrOfParams = parser.parseParameters();
    if (nrOfParams != scmNumberOfAlarmParameters) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Wrong number of parameters for FB %s. Expected: %d, Actual: %d\n",
                   getCommFB()->getInstanceName(), scmNumberOfAlarmParameters, nrOfParams);
      return eRetVal;
    }
    bool isPublisher;
    switch (mFb->getComServiceType()) {
      case e_Publisher: isPublisher = true; break;
      case e_Subscriber: isPublisher = false; break;
      default:
        DEVLOG_ERROR("[OPC UA A&C LAYER]: Wrong CommFB used for FB %s! Expected: Publish/Subscribe\n",
                     getCommFB()->getInstanceName());
        return eRetVal;
    }
    if (!checkFirstDataPinType()) {
      DEVLOG_ERROR("[OPC UA A&C Layer]: First Input of FB %s must be of type BOOL!\n",
                   getCommFB()->getParent().getInstanceName());
      return eRetVal;
    }
    mHandler = static_cast<COPC_UA_HandlerAbstract *>(&getExtEvHandler<COPC_UA_Local_Handler>());
    COPC_UA_Local_Handler *localHandler = static_cast<COPC_UA_Local_Handler *>(mHandler);
    localHandler->enableHandler();
    UA_Server *server = localHandler->getUAServer();
    if (initOPCUAType(server, parser[TypeName], isPublisher) != e_InitOk) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Initializing Alarm Type for FB %s failed!\n", getCommFB()->getInstanceName());
      return eRetVal;
    }
    std::string instancePath(parser[PathToInstance] ? parser[PathToInstance] : smEmptyString);
    eRetVal = createOPCUAObject(server, instancePath, isPublisher);
    if (eRetVal == e_InitOk) {
      initializeRDBuffer();
    }
    return eRetVal;
  }

  void COPC_UA_AC_Layer::closeConnection() {
    if (mMemberActionInfo) {
      mHandler->uninitializeAction(*mMemberActionInfo);
    }
  }

  EComResponse COPC_UA_AC_Layer::recvData(const void *paData, unsigned int) {
    auto data = static_cast<const std::pair<TPortId, const CIEC_BOOL> *>(paData);
    TPortId portId = data->first;
    if (portId >= mRDBuffer.size()) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Wrong PortId for receiving data. Maximum allowed Id: %d, Actual: %d\n",
                   mRDBuffer.size(), portId);
      return e_ProcessDataRecvFaild;
    }
    (*mRDBuffer[portId]).setValue(data->second);
    return e_ProcessDataOk;
  }

  EComResponse COPC_UA_AC_Layer::sendData(void *, unsigned int) {
    bool activate = getTriggerValue();
    if ((mIsStateActive && activate) || (!mIsStateActive && !activate)) {
      return e_ProcessDataOk;
    }
    if (mMemberActionInfo) {
      if (mHandler->executeAction(*mMemberActionInfo) != UA_STATUSCODE_GOOD) {
        DEVLOG_ERROR("[OPC UA A&C LAYER]: Sending FB Data failed for FB %s!\n", getCommFB()->getInstanceName());
        return e_ProcessDataSendFailed;
      }
    }
    if (triggerAlarm(activate) != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Sending Alarm Data failed for FB %s!\n", getCommFB()->getInstanceName());
      return e_ProcessDataSendFailed;
    }
    return e_ProcessDataOk;
  }

  EComResponse COPC_UA_AC_Layer::processInterrupt() {
    for (size_t i = 0; i < getCommFB()->getNumRD(); i++) {
      getCommFB()->getRDs()[i]->setValue(*mRDBuffer[i]);
    }
    return e_ProcessDataOk;
  }

  void COPC_UA_AC_Layer::initializeRDBuffer() {
    for (size_t i = 0; i < getCommFB()->getNumRD(); ++i) {
      mRDBuffer.emplace_back(getCommFB()->getRDs()[i]->clone(nullptr));
      mRDBuffer[i]->setValue(false_BOOL);
    }
  }

  UA_StatusCode COPC_UA_AC_Layer::triggerAlarm(bool paActivate) {
    COPC_UA_Local_Handler *localHandler = static_cast<COPC_UA_Local_Handler *>(mHandler);
    UA_Server *server = localHandler->getUAServer();
    UA_StatusCode status = UA_STATUSCODE_GOOD;
    UA_Boolean activeState = paActivate;
    if (paActivate) {
      status |= resetAckedState(server);
      if (!mHasSeverityProperty) {
        UA_UInt16 *severityValue = &smSeverityValue;
        status |= setConditionField(server, UA_QUALIFIEDNAME(0, smSeverity), severityValue, &UA_TYPES[UA_TYPES_UINT16]);
      }
      if (mMessageTextPortIndex >= 0) {
        CIEC_STRING &messagePort = static_cast<CIEC_STRING &>(getCommFB()->getDI(mMessageTextPortIndex)->unwrap());
        UA_LocalizedText messageValue = UA_LOCALIZEDTEXT(smEmptyString, getNameFromString(messagePort.c_str()));
        status |=
            setConditionField(server, UA_QUALIFIEDNAME(0, smMessage), &messageValue, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
      }
      UA_Boolean retainValue = true;
      status |= setConditionField(server, UA_QUALIFIEDNAME(0, smRetain), &retainValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
      status |= setConditionVariableFieldProperty(server, UA_QUALIFIEDNAME(0, smActiveState), &activeState,
                                                  &UA_TYPES[UA_TYPES_BOOLEAN]);
      UA_DateTime alarmTime = UA_DateTime_now();
      status |= UA_Server_writeObjectProperty_scalar(server, mConditionInstanceId, UA_QUALIFIEDNAME(0, smTime),
                                                     &alarmTime, &UA_TYPES[UA_TYPES_DATETIME]);

      if (status != UA_STATUSCODE_GOOD) {
        DEVLOG_ERROR("[OPC UA A&C LAYER]: Triggering Alarm failed for FB %s, StatusCode: %s\n",
                     getCommFB()->getInstanceName(), UA_StatusCode_name(status));
        return status;
      }
      mIsStateActive = true;
    } else {
      status |= setConditionVariableFieldProperty(server, UA_QUALIFIEDNAME(0, smActiveState), &activeState,
                                                  &UA_TYPES[UA_TYPES_BOOLEAN]);
      /* Alarm time needs to be set prior to triggering condition. Otherwise time is not displayed correctly */
      UA_DateTime alarmTime = UA_DateTime_now();
      status |= UA_Server_writeObjectProperty_scalar(server, mConditionInstanceId, UA_QUALIFIEDNAME(0, smTime),
                                                     &alarmTime, &UA_TYPES[UA_TYPES_DATETIME]);
      status |= UA_Server_triggerConditionEvent(server, mConditionInstanceId, mConditionSourceId, nullptr);
      if (status != UA_STATUSCODE_GOOD) {
        DEVLOG_ERROR("[OPC UA A&C LAYER]: Resetting Alarm failed for FB %s, StatusCode: %s\n",
                     getCommFB()->getInstanceName(), UA_StatusCode_name(status));
        return e_ProcessDataSendFailed;
      }
      mIsStateActive = false;
    }
    return status;
  }

  UA_StatusCode COPC_UA_AC_Layer::resetAckedState(UA_Server *paServer) {
    UA_Boolean ackedState = false;
    return setConditionVariableFieldProperty(paServer, UA_QUALIFIEDNAME(0, smAckedState), &ackedState,
                                             &UA_TYPES[UA_TYPES_BOOLEAN]);
  }

  UA_StatusCode COPC_UA_AC_Layer::setConditionField(UA_Server *paServer,
                                                    UA_QualifiedName paQualifiedName,
                                                    void *dataValue,
                                                    UA_DataType *paDataType) {
    UA_Variant value;
    UA_Variant_setScalar(&value, dataValue, paDataType);
    return UA_Server_setConditionField(paServer, mConditionInstanceId, &value, paQualifiedName);
  }

  UA_StatusCode COPC_UA_AC_Layer::setConditionVariableFieldProperty(UA_Server *paServer,
                                                                    UA_QualifiedName paQualifiedName,
                                                                    void *dataValue,
                                                                    UA_DataType *paDataType) {
    UA_Variant value;
    UA_Variant_setScalar(&value, dataValue, paDataType);
    return UA_Server_setConditionVariableFieldProperty(paServer, mConditionInstanceId, &value, paQualifiedName,
                                                       UA_QUALIFIEDNAME(0, smId));
  }

  EComResponse COPC_UA_AC_Layer::initOPCUAType(UA_Server *paServer, const std::string &paTypeName, bool paIsPublisher) {
    if (!paIsPublisher && !checkFBOutputNames()) {
      return e_InitTerminated;
    }
    std::string browsePath(COPC_UA_ObjectStruct_Helper::getBrowsePath(scmAlarmTypeBrowsePath, paTypeName,
                                                                      1)); // TODO Change 1 to namespaceIndex
    if (isOPCUAObjectPresent(browsePath, &mTypeNodeId)) {
      if (paIsPublisher && !isFullyInitialized(paTypeName)) {
        return addOPCUATypeProperties(paServer, paTypeName);
      }
      return e_InitOk;
    }
    EComResponse eRetVal = createAlarmType(paServer, paTypeName);
    if (eRetVal == e_InitOk && paIsPublisher) {
      eRetVal = addOPCUATypeProperties(paServer, paTypeName);
    }
    return eRetVal;
  }

  EComResponse
  COPC_UA_AC_Layer::createOPCUAObject(UA_Server *paServer, const std::string &paPathToInstance, bool paIsPublisher) {
    std::string objectBrowsePath;
    if (createOPCUAObjectNode(paServer, paPathToInstance, objectBrowsePath) != UA_STATUSCODE_GOOD) {
      return e_InitTerminated;
    }
    std::string conditionBrowsePath =
        COPC_UA_ObjectStruct_Helper::getMemberBrowsePath(objectBrowsePath, scmAlarmConditionName);
    if (isOPCUAObjectPresent(conditionBrowsePath, &mConditionInstanceId)) {
      if (paIsPublisher) {
        if (initializeMapping() != UA_STATUSCODE_GOOD) {
          return e_InitTerminated;
        }
        if (initializeMemberActions(conditionBrowsePath) != e_InitOk) {
          return e_InitTerminated;
        }
      }
      return e_InitOk;
    }

    if (addOPCUACondition(paServer, conditionBrowsePath) != UA_STATUSCODE_GOOD) {
      return e_InitTerminated;
    }
    if (paIsPublisher) {
      if (initializeMapping() != UA_STATUSCODE_GOOD) {
        return e_InitTerminated;
      }
      if (initializeMemberActions(conditionBrowsePath) != e_InitOk) {
        return e_InitTerminated;
      }
    }
    return setConditionCallbacks(paServer);
  }

  UA_StatusCode COPC_UA_AC_Layer::createOPCUAObjectNode(UA_Server *paServer,
                                                        const std::string &paPathToInstance,
                                                        std::string &paBrowsePath) {
    if (!COPC_UA_Helper::isBrowsePathValid(paPathToInstance)) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Browsepath is invalid!");
      return UA_STATUSCODE_BAD;
    }
    std::string instanceNameStr{getCommFB()->getParent().getInstanceName()};
    if (instanceNameStr.empty()) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Retrieving FB Instance Name failed!");
      return UA_STATUSCODE_BAD;
    }
    paBrowsePath = COPC_UA_ObjectStruct_Helper::getMemberBrowsePath(paPathToInstance, instanceNameStr);
    if (isOPCUAObjectPresent(paBrowsePath, &mConditionSourceId)) {
      return UA_STATUSCODE_GOOD;
    }

    COPC_UA_Local_Handler *localHandler = static_cast<COPC_UA_Local_Handler *>(mHandler);
    std::vector<UA_NodeId *> referencedNodes;
    UA_StatusCode status = localHandler->splitAndCreateFolders(
        paBrowsePath, instanceNameStr,
        referencedNodes); // Overwrites instanceNameStr to the same value as before, but we get the OPC UA folders
    if (status != UA_STATUSCODE_GOOD || referencedNodes.empty()) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Creating OPC UA Folders failed, Browsepath: %s, StatusCode: %s\n",
                   paBrowsePath.c_str(), UA_StatusCode_name(status));
      return UA_STATUSCODE_BAD;
    }

    char *instanceName = getNameFromString(instanceNameStr);
    char *browsePath = getNameFromString(paBrowsePath);
    const UA_NodeId *parentNodeId = referencedNodes.back();
    mConditionSourceId = UA_NODEID_STRING(1, browsePath); // TODO Change 1 to namespaceIndex
    UA_ObjectAttributes attr = UA_ObjectAttributes_default;
    attr.eventNotifier = UA_EVENTNOTIFIER_SUBSCRIBE_TO_EVENT;
    attr.displayName = UA_LOCALIZEDTEXT(smEmptyString, instanceName);
    status =
        UA_Server_addObjectNode(paServer, mConditionSourceId, *parentNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                UA_QUALIFIEDNAME(1, instanceName), // TODO Change 1 to namespaceIndex
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), attr, NULL, &mConditionSourceId);

    if (status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Creating Object Node failed. StatusCode %s\n", UA_StatusCode_name(status));
      return status;
    }
    status = UA_Server_addReference(
        paServer, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASNOTIFIER),
        UA_EXPANDEDNODEID_STRING(mConditionSourceId.namespaceIndex, getNameFromString(browsePath)), UA_TRUE);
    if (status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Adding reference to Object Node failed. StatusCode %s\n",
                   UA_StatusCode_name(status));
    }
    for (auto referenceNode : referencedNodes) {
      UA_NodeId_delete(referenceNode);
    }
    return status;
  }

  UA_StatusCode COPC_UA_AC_Layer::addOPCUACondition(UA_Server *paServer, std::string &paBrowsePath) {
    char *conditionName = getNameFromString(scmAlarmConditionName);
    char *conditionBrowsePath = getNameFromString(paBrowsePath);
    mConditionInstanceId = UA_NODEID_STRING(1, conditionBrowsePath); // TODO Change 1 to namespaceIndex
    UA_StatusCode status = UA_Server_createConditionWithContext(
        paServer, mConditionInstanceId, mTypeNodeId, UA_QUALIFIEDNAME(1, conditionName), mConditionSourceId,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), this, &mConditionInstanceId);
    if (status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Adding Condition failed for FB %s. StatusCode %s\n",
                   getCommFB()->getInstanceName(), UA_StatusCode_name(status));
      return status;
    }
    UA_QualifiedName enabledStateField = UA_QUALIFIEDNAME(0, smEnabledState);
    UA_QualifiedName enabledStateIdField = UA_QUALIFIEDNAME(0, smId);
    UA_Boolean enabledState = true;

    UA_Variant value;
    UA_Variant_setScalar(&value, &enabledState, &UA_TYPES[UA_TYPES_BOOLEAN]);
    status = UA_Server_setConditionVariableFieldProperty(paServer, mConditionInstanceId, &value, enabledStateField,
                                                         enabledStateIdField);
    if (status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Enabling Condition failed for FB %s, StatusCode: %s\n",
                   getCommFB()->getInstanceName(), UA_StatusCode_name(status));
    }
    return status;
  }

  UA_StatusCode COPC_UA_AC_Layer::initializeMapping() {
    UA_BrowseResult result = browseNode(mConditionInstanceId);
    size_t foundProperties = 0;
    for (size_t i = 0; i < result.referencesSize; i++) {
      UA_ReferenceDescription *ref = &result.references[i];
      std::string browseName((const char *) ref->browseName.name.data, ref->browseName.name.length);
      if (mUAPropertyMap.find(browseName) != mUAPropertyMap.end()) {
        mUAPropertyMap[browseName] = ref->nodeId.nodeId;
        foundProperties++;
      }
    }
    UA_BrowseResult_clear(&result);
    if (foundProperties != mUAPropertyMap.size()) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Number of found Input properties does not match number of properties to be "
                   "mapped. Expected: %d, Actual: %d\n",
                   mUAPropertyMap.size(), foundProperties);
      return UA_STATUSCODE_BADNODEIDUNKNOWN;
    }
    return UA_STATUSCODE_GOOD;
  }

  UA_BrowseResult COPC_UA_AC_Layer::browseNode(UA_NodeId &paNodeId) {
    COPC_UA_Local_Handler *localHandler = static_cast<COPC_UA_Local_Handler *>(mHandler);
    UA_BrowseDescription nodesToBrowse;
    UA_BrowseDescription_init(&nodesToBrowse);
    nodesToBrowse.nodeId = paNodeId;
    nodesToBrowse.browseDirection = UA_BROWSEDIRECTION_FORWARD;
    nodesToBrowse.nodeClassMask = UA_NODECLASS_VARIABLE;
    nodesToBrowse.resultMask = UA_BROWSERESULTMASK_BROWSENAME;
    return localHandler->browseServer(nodesToBrowse);
  }

  bool COPC_UA_AC_Layer::checkFBOutputNames() {
    const SFBInterfaceSpec &interfaceSpec = getParentInterfaceSpec();
    std::span<const StringId> portNameIds = interfaceSpec.mDONames;
    size_t foundProperties = 0;
    for (TPortId portId = 0; portId < interfaceSpec.getNumDOs(); portId++) {
      const char *portName = portNameIds[portId].data();
      if (mFBOutputMap.find(portName) != mFBOutputMap.end()) {
        mFBOutputMap[portName] = portId;
        foundProperties++;
      }
    }
    if (foundProperties != mFBOutputMap.size()) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Missing FB Output Ports %s / %s! Expected: %d, Actual: %d\n", smActive, smAcked,
                   mFBOutputMap.size(), foundProperties);
      return false;
    }
    return true;
  }

  bool COPC_UA_AC_Layer::isFullyInitialized(const std::string &paTypeName) {
    bool retVal = false;
    UA_BrowseResult result = browseNode(mTypeNodeId);
    std::string variableName{getParentInterfaceSpec().mDINames[smFirstDataIndex].data()};
    std::string memberBrowsePath{COPC_UA_ObjectStruct_Helper::getMemberBrowsePath(paTypeName, variableName)};
    for (size_t i = 0; i < result.referencesSize; i++) {
      UA_ReferenceDescription *ref = &result.references[i];
      std::string browseName((const char *) ref->browseName.name.data, ref->browseName.name.length);
      if (browseName == memberBrowsePath) {
        retVal = true;
        break;
      }
    }
    UA_BrowseResult_clear(&result);
    return retVal;
  }

  bool COPC_UA_AC_Layer::checkFirstDataPinType() {
    CFunctionBlock &parent = static_cast<CFunctionBlock &>(getCommFB()->getParent());
    return parent.getDI(0)->getDataTypeID() == CIEC_ANY::e_BOOL;
  }

  bool COPC_UA_AC_Layer::getTriggerValue() {
    CIEC_BOOL &dataPort = static_cast<CIEC_BOOL &>(getCommFB()->getSDs()[0]->unwrap());
    return static_cast<bool>(dataPort);
  }

  EComResponse COPC_UA_AC_Layer::setConditionCallbacks(UA_Server *paServer) {
    UA_TwoStateVariableChangeCallback callback = onEnabled;
    UA_StatusCode status = UA_Server_setConditionTwoStateVariableCallback(
        paServer, mConditionInstanceId, mConditionSourceId, false, callback, UA_ENTERING_ENABLEDSTATE);
    callback = onActive;
    status |= UA_Server_setConditionTwoStateVariableCallback(paServer, mConditionInstanceId, mConditionSourceId, false,
                                                             callback, UA_ENTERING_ACTIVESTATE);
    callback = onAcknowledged;
    status |= UA_Server_setConditionTwoStateVariableCallback(paServer, mConditionInstanceId, mConditionSourceId, false,
                                                             callback, UA_ENTERING_ACKEDSTATE);
    if (status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Setting Condition callback methods failed for FB %s, Status Code: %s\n",
                   getCommFB()->getInstanceName(), UA_StatusCode_name(status));
      return e_InitTerminated;
    }
    return e_InitOk;
  }

  EComResponse COPC_UA_AC_Layer::initializeMemberActions(const std::string &paParentBrowsePath) {
    mMemberActionInfo.reset(
        new CAlarmActionInfo(*this, CActionInfo::UA_ActionType::eWrite, std::string(), smFirstDataIndex));
    size_t numPorts = getCommFB()->getNumSD();
    const std::span<const StringId> dataPortNameIds = getParentInterfaceSpec().mDINames;
    for (size_t i = smFirstDataIndex; i < numPorts; i++) {
      std::string dataPortName{dataPortNameIds[i].data()};
      auto propertyKeyIt = sm1499ToUAMap.find(dataPortName);
      if (propertyKeyIt != sm1499ToUAMap.end()) {
        addNewNodeId(&mUAPropertyMap[propertyKeyIt->second]);
      } else if (dataPortName == smSeverity) {
        addNewNodeId(&mUAPropertyMap[smSeverity]);
        mHasSeverityProperty = true;
      } else {
        std::string memberBrowsePath(
            COPC_UA_ObjectStruct_Helper::getMemberBrowsePath(paParentBrowsePath, dataPortName));
        UA_NodeId *nodeId = COPC_UA_ObjectStruct_Helper::createStringNodeIdFromBrowsepath(memberBrowsePath);
        mMemberActionInfo->getNodePairInfo().emplace_back(nodeId, memberBrowsePath);
        if (dataPortName == smMessageText) {
          mMessageTextPortIndex = static_cast<int>(i + 2);
        }
      }
    }
    if (!mHasSeverityProperty) {
      DEVLOG_INFO("[OPC UA A&C LAYER]: No Data Port \"%s\" defined for FB %s. Using default value instead.", smSeverity,
                  getCommFB()->getInstanceName());
    }
    if (mMessageTextPortIndex == -1) {
      DEVLOG_INFO("[OPC UA A&C LAYER]: No Data Port \"%s\" defined for FB %s. Using default value instead.",
                  smMessageText, getCommFB()->getInstanceName());
    }
    if (mHandler->initializeAction(*mMemberActionInfo) != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Error occured in FB %s while initializing members\n",
                   getCommFB()->getInstanceName());
      return e_InitTerminated;
    }
    return e_InitOk;
  }

  void COPC_UA_AC_Layer::addNewNodeId(UA_NodeId *paNodeIdToAdd) {
    UA_NodeId *nodeId = UA_NodeId_new();
    UA_NodeId_copy(paNodeIdToAdd, nodeId);
    mMemberActionInfo->getNodePairInfo().emplace_back(nodeId, std::string());
  }

  EComResponse COPC_UA_AC_Layer::createAlarmType(UA_Server *paServer, const std::string &paTypeName) {
    char *typeName = getNameFromString(paTypeName);
    mTypeNodeId = UA_NODEID_STRING(1, typeName); // TODO Change 1 to namespaceIndex
    UA_ObjectTypeAttributes oAttr = UA_ObjectTypeAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT(smEmptyString, typeName);
    UA_StatusCode status =
        UA_Server_addObjectTypeNode(paServer, mTypeNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_ALARMCONDITIONTYPE),
                                    UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, typeName),
                                    oAttr, // TODO Change 1 to namespaceIndex
                                    nullptr, &mTypeNodeId);

    if (status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to create OPC UA Alarm Type Node for Type %s, Status Code: %s\n",
                   paTypeName.c_str(), UA_StatusCode_name(status));
      return e_InitTerminated;
    }
    return e_InitOk;
  }

  EComResponse COPC_UA_AC_Layer::addOPCUATypeProperties(UA_Server *paServer, const std::string &paTypeName) {
    CIEC_ANY **apoDataPorts = getCommFB()->getSDs();
    size_t numDataPorts = getCommFB()->getNumSD();
    const std::span<const StringId> dataPortNameIds = getParentInterfaceSpec().mDINames;
    for (size_t i = smFirstDataIndex; i < numDataPorts; i++) {
      std::string dataPortName{dataPortNameIds[i].data()};
      char *propertyName = getNameFromString(dataPortName);
      UA_StatusCode status = addVariableNode(paServer, paTypeName, propertyName, apoDataPorts[i]->unwrap());
      if (status != UA_STATUSCODE_GOOD) {
        DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to add OPCUA AlarmType Property for FB %s, Port: %s, Status: %s\n",
                     getCommFB()->getInstanceName(), dataPortName.c_str(), UA_StatusCode_name(status));
        return e_InitTerminated;
      }
    }
    return addOPCUATypeEnableStateProperty(paServer);
  }

  EComResponse COPC_UA_AC_Layer::addOPCUATypeEnableStateProperty(UA_Server *paServer) {
    UA_VariableAttributes vAttr = UA_VariableAttributes_default;
    vAttr.displayName = UA_LOCALIZEDTEXT(smEmptyString, smEnableStateProperty);
    vAttr.valueRank = UA_VALUERANK_ANY;
    vAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    vAttr.dataType = UA_TYPES[UA_TYPES_LOCALIZEDTEXT].typeId;

    UA_NodeId memberNodeId;
    UA_StatusCode status =
        UA_Server_addVariableNode(paServer, UA_NODEID_NULL, mTypeNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                                  UA_QUALIFIEDNAME(0, smEnableStateProperty), // TODO Change 1 to namespaceIndex
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_TWOSTATEVARIABLETYPE), vAttr, nullptr, &memberNodeId);
    mTypePropertyNodes.emplace_back(memberNodeId);
    if (status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to add OPCUA EnableState Property for FB %s, Status: %s\n",
                   getCommFB()->getInstanceName(), UA_StatusCode_name(status));
      return e_InitTerminated;
    }
    return e_InitOk;
  }

  UA_StatusCode COPC_UA_AC_Layer::addVariableNode(UA_Server *paServer,
                                                  const std::string &paParentTypeName,
                                                  char *paVariableName,
                                                  CIEC_ANY &paVariableType) {
    std::string propertyBrowsePathStr =
        COPC_UA_ObjectStruct_Helper::getMemberBrowsePath(paParentTypeName, paVariableName);
    char *propertyBrowsePath = getNameFromString(propertyBrowsePathStr);

    UA_VariableAttributes vAttr = UA_VariableAttributes_default;
    vAttr.displayName = UA_LOCALIZEDTEXT(smEmptyString, paVariableName);
    vAttr.valueRank = UA_VALUERANK_SCALAR;
    vAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    vAttr.dataType = COPC_UA_Helper::getOPCUATypeFromAny(paVariableType)->typeId;

    UA_NodeId memberNodeId;
    memberNodeId = UA_NODEID_STRING(1, propertyBrowsePath); // TODO Change 1 to namespaceIndex
    UA_StatusCode status =
        UA_Server_addVariableNode(paServer, memberNodeId, mTypeNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                                  UA_QUALIFIEDNAME(1, propertyBrowsePath), // TODO Change 1 to namespaceIndex
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), vAttr, nullptr, &memberNodeId);
    mTypePropertyNodes.emplace_back(memberNodeId);
    if (status != UA_STATUSCODE_GOOD) {
      return status;
    }
    return status;
  }

  bool COPC_UA_AC_Layer::isOPCUAObjectPresent(std::string &paBrowsePath, UA_NodeId *paNewNodeId) {
    COPC_UA_Local_Handler *localHandler = static_cast<COPC_UA_Local_Handler *>(mHandler);
    if (localHandler) {
      CActionInfo::CNodePairInfo nodePair(nullptr, paBrowsePath);
      if (localHandler->isOPCUAObjectPresent(nodePair)) {
        UA_NodeId_copy(nodePair.getNodeId(), paNewNodeId);
        return true;
      }
    } else {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Failed to get LocalHandler because LocalHandler is null!\n");
    }
    return false;
  }

  const SFBInterfaceSpec &COPC_UA_AC_Layer::getParentInterfaceSpec() {
    CFunctionBlock &parent = static_cast<CFunctionBlock &>(getCommFB()->getParent());
    return parent.getFBInterfaceSpec();
  }

  char *COPC_UA_AC_Layer::getNameFromString(const std::string &paName) {
    size_t length = paName.length();
    char *name = new char[length + 1];
    strncpy(name, paName.c_str(), length);
    name[length] = '\0';
    mNames.emplace_back(name);
    return name;
  }

  UA_StatusCode COPC_UA_AC_Layer::onEnabled(UA_Server *server, const UA_NodeId *condition) {
    UA_DateTime dateTime = UA_DateTime_now();
    UA_StatusCode status = UA_Server_writeObjectProperty_scalar(server, *condition, UA_QUALIFIEDNAME(0, smTime),
                                                                &dateTime, &UA_TYPES[UA_TYPES_DATETIME]);
    if (status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Enable State callback failed, Status Code: %s\n", UA_StatusCode_name(status));
    }
    return status;
  }

  UA_StatusCode COPC_UA_AC_Layer::onActive(UA_Server *server, const UA_NodeId *condition) {
    COPC_UA_AC_Layer *layer = nullptr;
    UA_Server_getNodeContext(server, *condition, (void **) &layer);
    COPC_UA_Local_Handler *localHandler = static_cast<COPC_UA_Local_Handler *>(layer->mHandler);
    if (layer->getCommFB()->getComServiceType() == e_Subscriber) {
      const CIEC_BOOL &value = layer->mIsStateActive ? false_BOOL : true_BOOL;
      std::pair<TPortId, const CIEC_BOOL> data = std::make_pair(layer->mFBOutputMap[smActive], value);
      localHandler->onAlarmStateChanged(static_cast<const void *>(&data), 0, layer);
    }
    layer->mIsStateActive = !layer->mIsStateActive;

    UA_DateTime dateTime = UA_DateTime_now();
    UA_StatusCode status = UA_Server_writeObjectProperty_scalar(server, *condition, UA_QUALIFIEDNAME(0, smTime),
                                                                &dateTime, &UA_TYPES[UA_TYPES_DATETIME]);
    if (status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Active State callback failed, Status Code: %s\n", UA_StatusCode_name(status));
    }
    return status;
  }

  UA_StatusCode COPC_UA_AC_Layer::onAcknowledged(UA_Server *server, const UA_NodeId *condition) {
    COPC_UA_AC_Layer *layer = nullptr;
    UA_Server_getNodeContext(server, *condition, (void **) &layer);
    COPC_UA_Local_Handler *localHandler = static_cast<COPC_UA_Local_Handler *>(layer->mHandler);
    if (layer->getCommFB()->getComServiceType() == e_Subscriber) {
      const CIEC_BOOL &value = layer->mIsStateAcked ? false_BOOL : true_BOOL;
      std::pair<TPortId, const CIEC_BOOL> data = std::make_pair(layer->mFBOutputMap[smAcked], value);
      localHandler->onAlarmStateChanged(static_cast<const void *>(&data), 0, layer);
    }
    layer->mIsStateAcked = !layer->mIsStateAcked;
    UA_DateTime dateTime = UA_DateTime_now();
    UA_StatusCode status = UA_Server_writeObjectProperty_scalar(server, *condition, UA_QUALIFIEDNAME(0, smTime),
                                                                &dateTime, &UA_TYPES[UA_TYPES_DATETIME]);
    if (status != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("[OPC UA A&C LAYER]: Acknowledged State callback failed, Status Code: %s\n",
                   UA_StatusCode_name(status));
    }
    return status;
  }
} // namespace forte::com_infra::opc_ua
