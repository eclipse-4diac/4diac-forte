/*******************************************************************************
 * Copyright (c) 2022, 2024 Markus Meingast, Johannes Kepler University Linz
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

#include "OPCUA_MGR.h"

#include "device.h"
#include "stringdict.h"

/* Management and Resource Type */
char OPCUA_MGR::smEmptyLocale[] = "";
char OPCUA_MGR::smMgmtType[] = "IEC61499MgmtType";
char OPCUA_MGR::smMgmtName[] = "IEC61499Mgmt";
char OPCUA_MGR::smResType[] = "ResourceType";

/* Write Device */
char OPCUA_MGR::smWriteDevMethodName[] = "writeDevice";
char OPCUA_MGR::smWriteDevArgName[] = "Value";
char OPCUA_MGR::smWriteDevArgDescription[] = "Value to be written";
char OPCUA_MGR::smWriteDevAttrDisplayName[] = "Write Device Parameter";
char OPCUA_MGR::smWriteDevAttrDescription[] = "Write Device Parameter";

/* Start Device */
char OPCUA_MGR::smStartDevMethodName[] = "startDevice";
char OPCUA_MGR::smStartDevAttrDisplayName[] = "Start Device";
char OPCUA_MGR::smStartDevAttrDescription[] = "Start Device";

/* Stop Device */
char OPCUA_MGR::smStopDevMethodName[] = "stopDevice";
char OPCUA_MGR::smStopDevAttrDisplayName[] = "Stop Device";
char OPCUA_MGR::smStopDevAttrDescription[] = "Stop Device";

/* Reset Device */
char OPCUA_MGR::smResetDevMethodName[] = "resetDevice";
char OPCUA_MGR::smResetDevAttrDisplayName[] = "Reset Device";
char OPCUA_MGR::smResetDevAttrDescription[] = "Reset Device";

/* Kill Device */
char OPCUA_MGR::smKillDevMethodName[] = "killDevice";
char OPCUA_MGR::smKillDevAttrDisplayName[] = "Kill Device";
char OPCUA_MGR::smKillDevAttrDescription[] = "Kill Device";

/* Create Resource */
char OPCUA_MGR::smCreateResMethodName[] = "createResource";
char OPCUA_MGR::smCreateResArg1Name[] = "Resource_Name";
char OPCUA_MGR::smCreateResArg1Description[] = "Name of Resource";
char OPCUA_MGR::smCreateResArg2Name[] = "Type_Name";
char OPCUA_MGR::smCreateResArg2Description[] = "Type of Resource";
char OPCUA_MGR::smCreateResOutArgName[] = "Resource NodeId";
char OPCUA_MGR::smCreateResOutArgDescription[] = "Resource NodeId";
char OPCUA_MGR::smCreateResAttrDisplayName[] = "Create Resource";
char OPCUA_MGR::smCreateResAttrDescription[] = "Create a Resource";

/* Write Resource */
char OPCUA_MGR::smWriteResMethodName[] = "writeResource";
char OPCUA_MGR::smWriteResArg1Name[] = "Resource Name";
char OPCUA_MGR::smWriteResArg1Description[] = "Destination Resource Name";
char OPCUA_MGR::smWriteResArg2Name[] = "Value";
char OPCUA_MGR::smWriteResArg2Description[] = "Value to be written";
char OPCUA_MGR::smWriteResAttrDisplayName[] = "Write Resource Parameter";
char OPCUA_MGR::smWriteResAttrDescription[] = "Write Resource Parameter";

/* Start Resource */
char OPCUA_MGR::smStartResMethodName[] = "startResource";
char OPCUA_MGR::smStartResArgName[] = "Resource Name";
char OPCUA_MGR::smStartResArgDescription[] = "Resource Name";
char OPCUA_MGR::smStartResAttrDisplayName[] = "Start Resource";
char OPCUA_MGR::smStartResAttrDescription[] = "Start Resource";

/* Stop Resource */
char OPCUA_MGR::smStopResMethodName[] = "stopResource";
char OPCUA_MGR::smStopResArgName[] = "Resource Name";
char OPCUA_MGR::smStopResArgDescription[] = "Resource Name";
char OPCUA_MGR::smStopResAttrDisplayName[] = "Stop Resource";
char OPCUA_MGR::smStopResAttrDescription[] = "Stop Resource";

/* Reset Resource */
char OPCUA_MGR::smResetResMethodName[] = "resetResource";
char OPCUA_MGR::smResetResArgName[] = "Resource Name";
char OPCUA_MGR::smResetResArgDescription[] = "Resource Name";
char OPCUA_MGR::smResetResAttrDisplayName[] = "Reset Resource";
char OPCUA_MGR::smResetResAttrDescription[] = "Reset Resource";

/* Kill Resource */
char OPCUA_MGR::smKillResMethodName[] = "killResource";
char OPCUA_MGR::smKillResArgName[] = "Resource Name";
char OPCUA_MGR::smKillResArgDescription[] = "Resource Name";
char OPCUA_MGR::smKillResAttrDisplayName[] = "Kill Resource";
char OPCUA_MGR::smKillResAttrDescription[] = "Kill Resource";

/* Delete Resource */
char OPCUA_MGR::smDeleteResMethodName[] = "deleteResource";
char OPCUA_MGR::smDeleteResArgName[] = "Resource Name";
char OPCUA_MGR::smDeleteResArgDescription[] = "Resource Name";
char OPCUA_MGR::smDeleteResAttrDisplayName[] = "Delete Resource";
char OPCUA_MGR::smDeleteResAttrDescription[] = "Delete Resource";

/* Create FB */
char OPCUA_MGR::smCreateFBMethodName[] = "createFB";
char OPCUA_MGR::smCreateFBArg1Name[] = "FB_Name";
char OPCUA_MGR::smCreateFBArg1Description[] = "Fully qualified name of FB";
char OPCUA_MGR::smCreateFBArg2Name[] = "FB_Type";
char OPCUA_MGR::smCreateFBArg2Description[] = "Type of FB";
char OPCUA_MGR::smCreateFBAttrDisplayName[] = "Create FB";
char OPCUA_MGR::smCreateFBAttrDescription[] = "Create FB";

/* Write FB */
char OPCUA_MGR::smWriteFBMethodName[] = "writeFB";
char OPCUA_MGR::smWriteFBArg1Name[] = "FB_Name";
char OPCUA_MGR::smWriteFBArg1Description[] = "Fully qualified name of FB";
char OPCUA_MGR::smWriteFBArg2Name[] = "Value";
char OPCUA_MGR::smWriteFBArg2Description[] = "Value to be written";
char OPCUA_MGR::smWriteFBAttrDisplayName[] = "Write FB Parameter";
char OPCUA_MGR::smWriteFBAttrDescription[] = "Write FB Parameter";

/* Start FB */
char OPCUA_MGR::smStartFBMethodName[] = "startFB";
char OPCUA_MGR::smStartFBArg1Name[] = "FB_Name";
char OPCUA_MGR::smStartFBArg1Description[] = "Fully qualified name of FB";
char OPCUA_MGR::smStartFBAttrDisplayName[] = "Start FB";
char OPCUA_MGR::smStartFBAttrDescription[] = "Start FB";

/* Stop FB */
char OPCUA_MGR::smStopFBMethodName[] = "stopFB";
char OPCUA_MGR::smStopFBArg1Name[] = "FB_Name";
char OPCUA_MGR::smStopFBArg1Description[] = "Fully qualified name of FB";
char OPCUA_MGR::smStopFBAttrDisplayName[] = "Stop FB";
char OPCUA_MGR::smStopFBAttrDescription[] = "Stop FB";

/* Reset FB */
char OPCUA_MGR::smResetFBMethodName[] = "resetFB";
char OPCUA_MGR::smResetFBArg1Name[] = "FB_Name";
char OPCUA_MGR::smResetFBArg1Description[] = "Fully qualified name of FB";
char OPCUA_MGR::smResetFBAttrDisplayName[] = "Reset FB";
char OPCUA_MGR::smResetFBAttrDescription[] = "Reset FB";

/* Kill FB */
char OPCUA_MGR::smKillFBMethodName[] = "killFB";
char OPCUA_MGR::smKillFBArg1Name[] = "FB_Name";
char OPCUA_MGR::smKillFBArg1Description[] = "Fully qualified name of FB";
char OPCUA_MGR::smKillFBAttrDisplayName[] = "Kill FB";
char OPCUA_MGR::smKillFBAttrDescription[] = "Kill FB";

/* Delete FB */
char OPCUA_MGR::smDeleteFBMethodName[] = "deleteFB";
char OPCUA_MGR::smDeleteFBArg1Name[] = "FB_Name";
char OPCUA_MGR::smDeleteFBArg1Description[] = "Fully qualified name of FB";
char OPCUA_MGR::smDeleteFBAttrDisplayName[] = "Delete FB";
char OPCUA_MGR::smDeleteFBAttrDescription[] = "Delete FB";


/* Create Connection */
char OPCUA_MGR::smCreateConnMethodName[] = "createConnection";
char OPCUA_MGR::smCreateConnArg1Name[] = "Connection_Source";
char OPCUA_MGR::smCreateConnArg1Description[] = "{SubApp.FbName.PortName}";
char OPCUA_MGR::smCreateConnArg2Name[] = "Connection_Dst";
char OPCUA_MGR::smCreateConnArg2Description[] = "{SubApp.FbName.PortName}";
char OPCUA_MGR::smCreateConnAttrDisplayName[] = "Create Connection";
char OPCUA_MGR::smCreateConnAttrDescription[] = "Create Connection";

/* Delete Connection */
char OPCUA_MGR::smDeleteConnMethodName[] = "deleteConnection";
char OPCUA_MGR::smDeleteConnArg1Name[] = "Connection_Source";
char OPCUA_MGR::smDeleteConnArg1Description[] = "{SubApp.FbName.PortName}";
char OPCUA_MGR::smDeleteConnArg2Name[] = "Connection_Dst";
char OPCUA_MGR::smDeleteConnArg2Description[] = "{SubApp.FbName.PortName}";
char OPCUA_MGR::smDeleteConnAttrDisplayName[] = "Delete Connection";
char OPCUA_MGR::smDeleteConnAttrDescription[] = "Delete Connection";

/* Query Resources */
char OPCUA_MGR::smQueryResourcesMethodName[] = "queryResources";
char OPCUA_MGR::smQueryResourcesOutArgName[] = "Response";
char OPCUA_MGR::smQueryResourcesOutArgDescription[] = "Read Response";
char OPCUA_MGR::smQueryResourcesDisplayName[] = "Query Resources";
char OPCUA_MGR::smQueryResourcesDescription[] = "Query Resources";

#ifdef FORTE_SUPPORT_MONITORING

/* Add Watch */
char OPCUA_MGR::smAddWatchMethodName[] = "addWatch";
char OPCUA_MGR::smAddWatchArgName[] = "FB Port";
char OPCUA_MGR::smAddWatchArgDescription[] = "Fully qualified name of FB Port";
char OPCUA_MGR::smAddWatchAttrDisplayName[] = "Add Watch";
char OPCUA_MGR::smAddWatchAttrDescription[] = "Add Watch";

/* Read Watches */
char OPCUA_MGR::smReadWatchesMethodName[] = "readWatches";
char OPCUA_MGR::smReadWatchesOutArgName[] = "Response";
char OPCUA_MGR::smReadWatchesOutArgDescription[] = "Read Response";
char OPCUA_MGR::smReadWatchesDisplayName[] = "Read Watches";
char OPCUA_MGR::smReadWatchesDescription[] = "Read Watches";

/* Remove Watch */
char OPCUA_MGR::smRemoveWatchMethodName[] = "removeWatch";
char OPCUA_MGR::smRemoveWatchArgName[] = "FB Port";
char OPCUA_MGR::smRemoveWatchArgDescription[] = "Fully qualified name of FB Port";
char OPCUA_MGR::smRemoveWatchAttrDisplayName[] = "Remove Watch";
char OPCUA_MGR::smRemoveWatchAttrDescription[] = "Remove Watch";

/* Trigger Event */
char OPCUA_MGR::smTriggerEventMethodName[] = "triggerEvent";
char OPCUA_MGR::smTriggerEventArgName[] = "FB Port";
char OPCUA_MGR::smTriggerEventArgDescription[] = "Fully qualified name of FB Port";
char OPCUA_MGR::smTriggerEventAttrDisplayName[] = "Trigger Event";
char OPCUA_MGR::smTriggerEventAttrDescription[] = "Trigger Event";
const char* OPCUA_MGR::scmTriggerEventParam = "$e";

/* Force Value */
char OPCUA_MGR::smForceValueMethodName[] = "forceValue";
char OPCUA_MGR::smForceValueArg1Name[] = "FB Port";
char OPCUA_MGR::smForceValueArg1Description[] = "Fully qualified name of FB Port";
char OPCUA_MGR::smForceValueArg2Name[] = "Value";
char OPCUA_MGR::smForceValueArg2Description[] = "Value to be written";
char OPCUA_MGR::smForceValueAttrDisplayName[] = "Force Value";
char OPCUA_MGR::smForceValueAttrDescription[] = "Force Value";

/* Clear Force */
char OPCUA_MGR::smClearForceMethodName[] = "clearForce";
char OPCUA_MGR::smClearForceArgName[] = "FB Port";
char OPCUA_MGR::smClearForceArgDescription[] = "Fully qualified name of FB Port";
char OPCUA_MGR::smClearForceAttrDisplayName[] = "Clear Force";
char OPCUA_MGR::smClearForceAttrDescription[] = "Clear Force";

#endif // FORTE_SUPPORT_MONITORING

/* Initialize UA Status Codes */
const std::map<EMGMResponse, UA_StatusCode> OPCUA_MGR::scResponseMap = {
  {EMGMResponse::Ready, UA_STATUSCODE_GOOD},
  {EMGMResponse::BadParams, UA_STATUSCODE_BADINVALIDARGUMENT},
  {EMGMResponse::LocalTermination, UA_STATUSCODE_BADSHUTDOWN}, 
  {EMGMResponse::SystemTermination, UA_STATUSCODE_BADRESOURCEUNAVAILABLE},
  {EMGMResponse::NotReady, UA_STATUSCODE_BADSTATENOTACTIVE}, 
  {EMGMResponse::UnsupportedCmd, UA_STATUSCODE_BADNOTIMPLEMENTED},
  {EMGMResponse::UnsupportedType, UA_STATUSCODE_BADDATATYPEIDUNKNOWN}, 
  {EMGMResponse::NoSuchObject, UA_STATUSCODE_BADNOTFOUND}, 
  {EMGMResponse::InvalidObject, UA_STATUSCODE_BADNOTSUPPORTED},
  {EMGMResponse::InvalidOperation, UA_STATUSCODE_BADREQUESTNOTALLOWED},
  {EMGMResponse::InvalidState, UA_STATUSCODE_BADINVALIDSTATE},
  {EMGMResponse::Overflow, UA_STATUSCODE_BADTCPNOTENOUGHRESOURCES},
};

const UA_UInt16 OPCUA_MGR::smNamespaces[] = {0, 1};

OPCUA_MGR::OPCUA_MGR(CDevice& paUaDevice) :
  mUaDevice(paUaDevice),
    mUaHandler(paUaDevice.getDeviceExecution().getExtEvHandler<COPC_UA_Local_Handler>()){
}

OPCUA_MGR::~OPCUA_MGR() {
}

EMGMResponse OPCUA_MGR::initialize(){
  EMGMResponse eRetVal = EMGMResponse::Ready;
  mUaHandler.enableHandler();
  UA_Server* uaServer = mUaHandler.getUAServer();
  if (uaServer != nullptr) {
    if(createIEC61499MgmtObject(uaServer) != EMGMResponse::Ready) return EMGMResponse::InvalidState;
    eRetVal = createIEC61499ResourceObjectType(uaServer);
  }
  return eRetVal;
}

bool OPCUA_MGR::isInitialized(){
  return mUaHandler.isAlive();
}

/**** OPCUA Methods ****/

EMGMResponse OPCUA_MGR::createIEC61499MgmtObject(UA_Server* paServer) {
  EMGMResponse eRetVal = EMGMResponse::InvalidState;
  if (defineIEC61499MgmtObjectType(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addCreateResourceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addWriteDeviceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addWriteResourceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addStartDeviceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addStartResourceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addStopDeviceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addStopResourceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addResetDeviceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addResetResourceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addKillDeviceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addKillResourceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addDeleteResourceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addQueryResourcesMethod(paServer) != EMGMResponse::Ready) return eRetVal;
#ifdef FORTE_SUPPORT_MONITORING
  if (addReadWatchesMethod(paServer) != EMGMResponse::Ready) return eRetVal;
#endif // FORTE_SUPPORT_MONITORING
  if(addExtraMethods(paServer, mMgmtTypeId, mExtraMgmMethods) != EMGMResponse::Ready) return eRetVal;
  return addMgmtObjectInstance();
}

EMGMResponse OPCUA_MGR::defineIEC61499MgmtObjectType(UA_Server* paServer) {
  EMGMResponse eRetVal = EMGMResponse::Ready;
  mMgmtTypeId = UA_NODEID_STRING(smNamespaces[1], smMgmtType);
  UA_ObjectTypeAttributes rtAttr = UA_ObjectTypeAttributes_default;
  rtAttr.displayName = UA_LOCALIZEDTEXT(smEmptyLocale, smMgmtType);
  UA_StatusCode status = UA_Server_addObjectTypeNode(paServer, mMgmtTypeId,
    UA_NODEID_NUMERIC(smNamespaces[0], UA_NS0ID_BASEOBJECTTYPE),
    UA_NODEID_NUMERIC(smNamespaces[0], UA_NS0ID_HASSUBTYPE),
    UA_QUALIFIEDNAME(smNamespaces[1], smMgmtType), rtAttr,
    nullptr, &mMgmtTypeId);

  if (status != UA_STATUSCODE_GOOD) {
    return EMGMResponse::InvalidState;
  }
  return eRetVal;
}

EMGMResponse OPCUA_MGR::addMgmtObjectInstance() {
  EMGMResponse eRetVal = EMGMResponse::Ready;
  UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
  oAttr.displayName = UA_LOCALIZEDTEXT(smEmptyLocale, smMgmtName);
  UA_StatusCode status = UA_Server_addObjectNode(mUaHandler.getUAServer(), UA_NODEID_STRING(smNamespaces[1], smMgmtName),
    UA_NODEID_NUMERIC(smNamespaces[0], UA_NS0ID_OBJECTSFOLDER),
    UA_NODEID_NUMERIC(smNamespaces[0], UA_NS0ID_ORGANIZES),
    UA_QUALIFIEDNAME(smNamespaces[1], smMgmtName),
    mMgmtTypeId,
    oAttr, nullptr, &mMgmtNodeId);

  if (status != UA_STATUSCODE_GOOD) {
    return EMGMResponse::InvalidState;
  }
  return eRetVal;
}

EMGMResponse OPCUA_MGR::createIEC61499ResourceObjectType(UA_Server* paServer) {
  EMGMResponse eRetVal = EMGMResponse::InvalidState;
  if (defineIEC61499ResourceObjectType(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addCreateFBMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addCreateConnectionMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addWriteFBMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addStartFBMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addStopFBMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addResetFBMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addKillFBMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addDeleteFBMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addDeleteConnectionMethod(paServer) != EMGMResponse::Ready) return eRetVal;
#ifdef FORTE_SUPPORT_MONITORING
  if (addAddWatchMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addRemoveWatchMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addTriggerEventMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addForceValueMethod(paServer) != EMGMResponse::Ready) return eRetVal;
  if (addClearForceMethod(paServer) != EMGMResponse::Ready) return eRetVal;
#endif // FORTE_SUPPORT_MONITORING
  if(addExtraMethods(paServer, mResourceTypeId, mExtraResourceMethods) != EMGMResponse::Ready) return eRetVal;
  return EMGMResponse::Ready;
}

EMGMResponse OPCUA_MGR::defineIEC61499ResourceObjectType(UA_Server* paServer) {
  EMGMResponse eRetVal = EMGMResponse::Ready;
  mResourceTypeId = UA_NODEID_STRING(smNamespaces[1], smResType);
  UA_ObjectTypeAttributes rtAttr = UA_ObjectTypeAttributes_default;
  rtAttr.displayName = UA_LOCALIZEDTEXT(smEmptyLocale, smResType);
  UA_StatusCode status = UA_Server_addObjectTypeNode(paServer, mResourceTypeId,
    UA_NODEID_NUMERIC(smNamespaces[0], UA_NS0ID_BASEOBJECTTYPE),
    UA_NODEID_NUMERIC(smNamespaces[0], UA_NS0ID_HASSUBTYPE),
    UA_QUALIFIEDNAME(smNamespaces[1], smResType), rtAttr,
    nullptr, &mResourceTypeId);
  if (status != UA_STATUSCODE_GOOD) {
    return EMGMResponse::InvalidState;
  }

  return eRetVal;
}

EMGMResponse OPCUA_MGR::addResourceObjectInstance(char* paResourceName, UA_NodeId& paNodeId, UA_NodeId paParentNodeId) {
  EMGMResponse eRetVal = EMGMResponse::Ready;
  UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
  oAttr.displayName = UA_LOCALIZEDTEXT(smEmptyLocale, paResourceName);

  UA_StatusCode status = UA_Server_addObjectNode(mUaHandler.getUAServer(), UA_NODEID_NULL,
    paParentNodeId,
    UA_NODEID_NUMERIC(smNamespaces[0], UA_NS0ID_ORGANIZES),
    UA_QUALIFIEDNAME(smNamespaces[1], paResourceName),
    mResourceTypeId, oAttr, paResourceName, 
    &paNodeId);
  if (status != UA_STATUSCODE_GOOD) {
    return EMGMResponse::InvalidState;
  }
  return eRetVal;
}

EMGMResponse OPCUA_MGR::removeResourceObjectInstance(std::string resourceName) {
  std::map<std::string, UA_NodeId>::iterator it = resourceNodeMap.find(resourceName);
  if (it == resourceNodeMap.end()) {
    return EMGMResponse::InvalidState;
  }
  if (UA_Server_deleteNode(mUaHandler.getUAServer(), it->second, true) != UA_STATUSCODE_GOOD) {
    return EMGMResponse::InvalidState;
  }
  resourceNodeMap.erase(it);
  return EMGMResponse::Ready;
}

EMGMResponse OPCUA_MGR::addCreateResourceMethod(UA_Server* paServer) {
  UA_Argument inputArguments[2];
  initArgument(inputArguments[0], UA_TYPES_STRING, smCreateResArg1Name, smCreateResArg1Description);
  initArgument(inputArguments[1], UA_TYPES_STRING, smCreateResArg2Name, smCreateResArg2Description);

  UA_Argument outputArgument;
  initArgument(outputArgument, UA_TYPES_NODEID, smCreateResOutArgName, smCreateResOutArgDescription);

  UA_MethodAttributes createResAttr = createAttribute(smCreateResAttrDisplayName, smCreateResAttrDescription);
  return addMethodNode(paServer, smCreateResMethodName, mMgmtTypeId, createResAttr, inputArguments, 2, &outputArgument, 1, &onCreateResource);
}

UA_StatusCode OPCUA_MGR::onCreateResource(UA_Server*,
    const UA_NodeId*, void*,
    const UA_NodeId*, void* methodContext,
    const UA_NodeId*, void*,
    size_t, const UA_Variant* input,
    size_t, UA_Variant* output) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  const std::string resourceName(getInputValue(*static_cast<UA_String*>(input[0].data)));
  const std::string resType(getInputValue(*static_cast<UA_String*>(input[1].data)));

  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::CreateFBInstance, CStringDictionary::scmInvalidStringId, nullptr, resourceName.c_str(), resType.c_str());
  EMGMResponse eMGMRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);

  int status = scResponseMap.find(eMGMRetVal)->second;
  if (status != UA_STATUSCODE_GOOD) {
    return status;
  }

  UA_NodeId nodeId;
  char* resNameChar = new char[resourceName.length() + 1];
  strcpy(resNameChar, resourceName.c_str());
  eMGMRetVal = uaMGR->addResourceObjectInstance(resNameChar, nodeId, uaMGR->mMgmtNodeId);
  status = scResponseMap.find(eMGMRetVal)->second;
  if (status != UA_STATUSCODE_GOOD){
    return status;
  }
  uaMGR->resourceNodeMap.emplace(resourceName, nodeId);
  /* Return Output */
  status = UA_Variant_setScalarCopy(output, &nodeId, &UA_TYPES[UA_TYPES_NODEID]);
  UA_NodeId_clear(&nodeId);
  return status;
}

EMGMResponse OPCUA_MGR::addCreateFBMethod(UA_Server* paServer) {
  UA_Argument inputArguments[2];
  initArgument(inputArguments[0], UA_TYPES_STRING, smCreateFBArg1Name, smCreateFBArg1Description);
  initArgument(inputArguments[1], UA_TYPES_STRING, smCreateFBArg2Name, smCreateFBArg2Description);

  UA_MethodAttributes createFBAttr = createAttribute(smCreateFBAttrDisplayName, smCreateFBAttrDescription);
  return addMethodNode(paServer, smCreateFBMethodName, mResourceTypeId, createFBAttr, inputArguments, 2, nullptr, 0, &onCreateFB);
}

UA_StatusCode OPCUA_MGR::onCreateFB(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  const std::string fbTypeStr(getInputValue(*static_cast<UA_String*>(input[1].data)));

  std::vector<std::string> fullFbName;
  parseDestinationName(destination, fullFbName);
  std::vector<std::string> fbType{ fbTypeStr };

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::CreateFBInstance, CStringDictionary::insert(resourceName), nullptr, fullFbName, fbType);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addCreateConnectionMethod(UA_Server* paServer) {
  UA_Argument inputArguments[2];
  initArgument(inputArguments[0], UA_TYPES_STRING, smCreateConnArg1Name, smCreateConnArg1Description);
  initArgument(inputArguments[1], UA_TYPES_STRING, smCreateConnArg2Name, smCreateConnArg2Description);

  UA_MethodAttributes createConnAttr = createAttribute(smCreateConnAttrDisplayName, smCreateConnAttrDescription);
  return addMethodNode(paServer, smCreateConnMethodName, mResourceTypeId, createConnAttr, inputArguments, 2, nullptr, 0, &onCreateConnection);
}

UA_StatusCode OPCUA_MGR::onCreateConnection(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string source(getInputValue(*static_cast<UA_String*>(input[0].data)));
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[1].data)));

  std::vector<std::string> sourceFullName;
  std::vector<std::string> destinationFullName;
  parseDestinationName(source, sourceFullName);
  parseDestinationName(destination, destinationFullName);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::CreateConnection, CStringDictionary::insert(resourceName), nullptr, sourceFullName, destinationFullName);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addWriteDeviceMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smWriteDevArgName, smWriteDevArgDescription);

  UA_MethodAttributes writeDevAttr = createAttribute(smWriteDevAttrDisplayName, smWriteDevAttrDescription);
  return addMethodNode(paServer, smWriteDevMethodName, mMgmtTypeId, writeDevAttr, &inputArgument, 1, nullptr, 0, &onWriteDevice);
}

UA_StatusCode OPCUA_MGR::onWriteDevice(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string writeValue(getInputValue(*static_cast<UA_String*>(input[0].data)));

  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Write, CStringDictionary::scmInvalidStringId, writeValue.c_str(), nullptr, nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addWriteResourceMethod(UA_Server* paServer) {
  UA_Argument inputArguments[2];
  initArgument(inputArguments[0], UA_TYPES_STRING, smWriteResArg1Name, smWriteResArg1Description);
  initArgument(inputArguments[1], UA_TYPES_STRING, smWriteResArg2Name, smWriteResArg2Description);

  UA_MethodAttributes writeResAttr = createAttribute(smWriteResAttrDisplayName, smWriteResAttrDescription);
  return addMethodNode(paServer, smWriteResMethodName, mMgmtTypeId, writeResAttr, inputArguments, 2, nullptr, 0, &onWriteResource);
}

UA_StatusCode OPCUA_MGR::onWriteResource(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string resourceName(getInputValue(*static_cast<UA_String*>(input[0].data)));
  const std::string writeValue(getInputValue(*static_cast<UA_String*>(input[1].data)));

  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Write, CStringDictionary::insert(resourceName.c_str()), writeValue.c_str(), nullptr, nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addWriteFBMethod(UA_Server* paServer) {
  UA_Argument inputArguments[2];
  initArgument(inputArguments[0], UA_TYPES_STRING, smWriteFBArg1Name, smWriteFBArg1Description);
  initArgument(inputArguments[1], UA_TYPES_STRING, smWriteFBArg2Name, smWriteFBArg2Description);

  UA_MethodAttributes writeFBAttr = createAttribute(smWriteFBAttrDisplayName, smWriteFBAttrDescription);
  return addMethodNode(paServer, smWriteFBMethodName, mResourceTypeId, writeFBAttr, inputArguments, 2, nullptr, 0, &onWriteFB);
}

UA_StatusCode OPCUA_MGR::onWriteFB(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  const std::string writeValue(getInputValue(*static_cast<UA_String*>(input[1].data)));
  std::vector<std::string> writeDestination;
  parseDestinationName(destination, writeDestination);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Write, CStringDictionary::insert(resourceName), writeValue.c_str(), writeDestination);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addStartDeviceMethod(UA_Server* paServer) {
  UA_MethodAttributes startDevAttr = createAttribute(smStartDevAttrDisplayName, smStartDevAttrDescription);
  return addMethodNode(paServer, smStartDevMethodName, mMgmtTypeId, startDevAttr, nullptr, 0, nullptr, 0, &onStartDevice);
}

UA_StatusCode OPCUA_MGR::onStartDevice(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant*,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Start, CStringDictionary::scmInvalidStringId, nullptr, nullptr, nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addStartResourceMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smStartResArgName, smStartResArgDescription);

  UA_MethodAttributes startResAttr = createAttribute(smStartResAttrDisplayName, smStartResAttrDescription);
  return addMethodNode(paServer, smStartResMethodName, mMgmtTypeId, startResAttr, &inputArgument, 1, nullptr, 0, &onStartResource);
}

UA_StatusCode OPCUA_MGR::onStartResource(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string resourceName(getInputValue(*static_cast<UA_String*>(input[0].data)));
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Start, CStringDictionary::insert(resourceName.c_str()), nullptr, nullptr, nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addStartFBMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smStartFBArg1Name, smStartFBArg1Description);

  UA_MethodAttributes startFBAttr = createAttribute(smStartFBAttrDisplayName, smStartFBAttrDescription);
  return addMethodNode(paServer, smStartFBMethodName, mResourceTypeId, startFBAttr, &inputArgument, 1, nullptr, 0, &onStartFB);
}

UA_StatusCode OPCUA_MGR::onStartFB(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  std::vector<std::string> fullFbName;
  parseDestinationName(destination, fullFbName);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Start, CStringDictionary::insert(resourceName), nullptr, fullFbName);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addStopDeviceMethod(UA_Server* paServer) {
  UA_MethodAttributes stopDevAttr = createAttribute(smStopDevAttrDisplayName, smStopDevAttrDescription);
  return addMethodNode(paServer, smStopDevMethodName, mMgmtTypeId, stopDevAttr, nullptr, 0, nullptr, 0, &onStopDevice);
}

UA_StatusCode OPCUA_MGR::onStopDevice(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant*,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Stop, CStringDictionary::scmInvalidStringId, nullptr, nullptr, nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addStopResourceMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smStopResArgName, smStopResArgDescription);

  UA_MethodAttributes stopResAttr = createAttribute(smStopResAttrDisplayName, smStopResAttrDescription);
  return addMethodNode(paServer, smStopResMethodName, mMgmtTypeId, stopResAttr, &inputArgument, 1, nullptr, 0, &onStopResource);
}

UA_StatusCode OPCUA_MGR::onStopResource(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string resourceName(getInputValue(*static_cast<UA_String*>(input[0].data)));
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Stop, CStringDictionary::scmInvalidStringId, nullptr, resourceName.c_str(), nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addStopFBMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smStopFBArg1Name, smStopFBArg1Description);

  UA_MethodAttributes stopFBAttr = createAttribute(smStopFBAttrDisplayName, smStopFBAttrDescription);
  return addMethodNode(paServer, smStopFBMethodName, mResourceTypeId, stopFBAttr, &inputArgument, 1, nullptr, 0, &onStopFB);
}

UA_StatusCode OPCUA_MGR::onStopFB(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  std::vector<std::string> fullFbName;
  parseDestinationName(destination, fullFbName);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Stop, CStringDictionary::insert(resourceName), nullptr, fullFbName);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;

}

EMGMResponse OPCUA_MGR::addResetDeviceMethod(UA_Server* paServer) {
  UA_MethodAttributes resetDevAttr = createAttribute(smResetDevAttrDisplayName, smResetDevAttrDescription);
  return addMethodNode(paServer, smResetDevMethodName, mMgmtTypeId, resetDevAttr, nullptr, 0, nullptr, 0, &onResetDevice);
}

UA_StatusCode OPCUA_MGR::onResetDevice(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant*,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Reset, CStringDictionary::scmInvalidStringId, nullptr, nullptr, nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;

}

EMGMResponse OPCUA_MGR::addResetResourceMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smResetResArgName, smResetResArgDescription);

  UA_MethodAttributes resetResAttr = createAttribute(smResetResAttrDisplayName, smResetResAttrDescription);
  return addMethodNode(paServer, smResetResMethodName, mMgmtTypeId, resetResAttr, &inputArgument, 1, nullptr, 0, &onResetResource);
}

UA_StatusCode OPCUA_MGR::onResetResource(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string resourceName(getInputValue(*static_cast<UA_String*>(input[0].data)));
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Reset, CStringDictionary::scmInvalidStringId, nullptr, resourceName.c_str(), nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addResetFBMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smResetFBArg1Name, smResetFBArg1Description);

  UA_MethodAttributes resetFBAttr = createAttribute(smResetFBAttrDisplayName, smResetFBAttrDescription);
  return addMethodNode(paServer, smResetFBMethodName, mResourceTypeId, resetFBAttr, &inputArgument, 1, nullptr, 0, &onResetFB);
}

UA_StatusCode OPCUA_MGR::onResetFB(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  std::vector<std::string> fullFbName;
  parseDestinationName(destination, fullFbName);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Reset, CStringDictionary::insert(resourceName), nullptr, fullFbName);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addKillDeviceMethod(UA_Server* paServer) {
  UA_MethodAttributes killDevAttr = createAttribute(smKillDevAttrDisplayName, smKillDevAttrDescription);
  return addMethodNode(paServer, smKillDevMethodName, mMgmtTypeId, killDevAttr, nullptr, 0, nullptr, 0, &onKillDevice);
}

UA_StatusCode OPCUA_MGR::onKillDevice(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant*,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Kill, CStringDictionary::scmInvalidStringId, nullptr, nullptr, nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addKillResourceMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smKillResArgName, smKillResArgDescription);

  UA_MethodAttributes killResAttr = createAttribute(smKillResAttrDisplayName, smKillResAttrDescription);
  return addMethodNode(paServer, smKillResMethodName, mMgmtTypeId, killResAttr, &inputArgument, 1, nullptr, 0, &onKillResource);
}

UA_StatusCode OPCUA_MGR::onKillResource(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string resourceName(getInputValue(*static_cast<UA_String*>(input[0].data)));
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Kill, CStringDictionary::scmInvalidStringId, nullptr, resourceName.c_str(), nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addKillFBMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smKillFBArg1Name, smKillFBArg1Description);

  UA_MethodAttributes killFBAttr = createAttribute(smKillFBAttrDisplayName, smKillFBAttrDescription);
  return addMethodNode(paServer, smKillFBMethodName, mResourceTypeId, killFBAttr, &inputArgument, 1, nullptr, 0, &onKillFB);
}

UA_StatusCode OPCUA_MGR::onKillFB(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  std::vector<std::string> fullFbName;
  parseDestinationName(destination, fullFbName);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::Kill, CStringDictionary::insert(resourceName), nullptr, fullFbName);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addDeleteResourceMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smDeleteResArgName, smDeleteResArgDescription);

  UA_MethodAttributes deleteResAttr = createAttribute(smDeleteResAttrDisplayName, smDeleteResAttrDescription);
  return addMethodNode(paServer, smDeleteResMethodName, mMgmtTypeId, deleteResAttr, &inputArgument, 1, nullptr, 0, &onDeleteResource);
}

UA_StatusCode OPCUA_MGR::onDeleteResource(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string resourceName(getInputValue(*static_cast<UA_String*>(input[0].data)));

  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::DeleteFBInstance, CStringDictionary::scmInvalidStringId, nullptr, resourceName.c_str(), nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  UA_StatusCode status = scResponseMap.find(eRetVal)->second;

  if (status != UA_STATUSCODE_GOOD) {
    return status;
  }
  if (uaMGR->removeResourceObjectInstance(resourceName) != EMGMResponse::Ready) {
    return UA_STATUSCODE_BADINVALIDSTATE;
  }
  return status;
}

EMGMResponse OPCUA_MGR::addDeleteFBMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smDeleteFBArg1Name, smDeleteFBArg1Description);

  UA_MethodAttributes deleteFBAttr = createAttribute(smDeleteFBAttrDisplayName, smDeleteFBAttrDescription);
  return addMethodNode(paServer, smDeleteFBMethodName, mResourceTypeId, deleteFBAttr, &inputArgument, 1, nullptr, 0, &onDeleteFB);
}

UA_StatusCode OPCUA_MGR::onDeleteFB(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  std::vector<std::string> fullFbName;
  parseDestinationName(destination, fullFbName);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);

  uaMGR->setMGMCommand(EMGMCommandType::DeleteFBInstance, CStringDictionary::insert(resourceName), nullptr, fullFbName);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addDeleteConnectionMethod(UA_Server* paServer) {
  UA_Argument inputArguments[2];
  initArgument(inputArguments[0], UA_TYPES_STRING, smDeleteConnArg1Name, smDeleteConnArg1Description);
  initArgument(inputArguments[1], UA_TYPES_STRING, smDeleteConnArg2Name, smDeleteConnArg2Description);

  UA_MethodAttributes deleteConnAttr = createAttribute(smDeleteConnAttrDisplayName, smDeleteConnAttrDescription);
  return addMethodNode(paServer, smDeleteConnMethodName, mResourceTypeId, deleteConnAttr, inputArguments, 2, nullptr, 0, &onDeleteConnection);
}

UA_StatusCode OPCUA_MGR::onDeleteConnection(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string source(getInputValue(*static_cast<UA_String*>(input[0].data)));
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[1].data)));
  std::vector<std::string> sourceFullName;
  std::vector<std::string> destinationFullName;
  parseDestinationName(source, sourceFullName);
  parseDestinationName(destination, destinationFullName);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::DeleteConnection, CStringDictionary::insert(resourceName), nullptr, sourceFullName, destinationFullName);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addQueryResourcesMethod(UA_Server* paServer) {
  UA_Argument outputArgument;
  initArgument(outputArgument, UA_TYPES_STRING, smQueryResourcesOutArgName, smQueryResourcesOutArgDescription);
  UA_MethodAttributes queryResourcesAttr = createAttribute(smQueryResourcesDisplayName, smQueryResourcesDescription);
  return addMethodNode(paServer, smQueryResourcesMethodName, mMgmtTypeId, queryResourcesAttr, nullptr, 0, &outputArgument, 1, &onQueryResources);
}

UA_StatusCode OPCUA_MGR::onQueryResources(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant*,
  size_t, UA_Variant* output) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::QueryFB, CStringDictionary::scmInvalidStringId, nullptr, nullptr, nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  int status = scResponseMap.find(eRetVal)->second;
  if (status != UA_STATUSCODE_GOOD) {
    return status;
  }
  UA_String uaResp = UA_String_fromChars(uaMGR->mCommand.mAdditionalParams.c_str());
  status = UA_Variant_setScalarCopy(output, &uaResp, &UA_TYPES[UA_TYPES_STRING]);
  UA_String_clear(&uaResp);
  return status;
}

/* FORTE Monitoring */
#ifdef FORTE_SUPPORT_MONITORING

EMGMResponse OPCUA_MGR::addAddWatchMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smAddWatchArgName, smAddWatchArgDescription);

  UA_MethodAttributes addWatchAttr = createAttribute(smAddWatchAttrDisplayName, smAddWatchAttrDescription);
  return addMethodNode(paServer, smAddWatchMethodName, mResourceTypeId, addWatchAttr, &inputArgument, 1, nullptr, 0, &onAddWatch);
}

UA_StatusCode OPCUA_MGR::onAddWatch(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  std::vector<std::string> fullFbName;
  parseDestinationName(destination, fullFbName);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::MonitoringAddWatch, CStringDictionary::insert(resourceName), nullptr, fullFbName);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addReadWatchesMethod(UA_Server* paServer) {
  UA_Argument outputArgument;
  initArgument(outputArgument, UA_TYPES_STRING, smReadWatchesOutArgName, smReadWatchesOutArgDescription);
  UA_MethodAttributes readWatchesAttr = createAttribute(smReadWatchesDisplayName, smReadWatchesDescription);
  return addMethodNode(paServer, smReadWatchesMethodName, mMgmtTypeId, readWatchesAttr, nullptr, 0, &outputArgument, 1, &onReadWatches);
}

UA_StatusCode OPCUA_MGR::onReadWatches(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void*,
  size_t, const UA_Variant*,
  size_t, UA_Variant* output) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::MonitoringReadWatches, CStringDictionary::scmInvalidStringId, nullptr, nullptr, nullptr);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  int status = scResponseMap.find(eRetVal)->second;
  if (status != UA_STATUSCODE_GOOD) {
    return status;
  }
  UA_String uaResp = UA_String_fromChars(uaMGR->mCommand.mMonitorResponse.c_str());
  status = UA_Variant_setScalarCopy(output, &uaResp, &UA_TYPES[UA_TYPES_STRING]);
  UA_String_clear(&uaResp);
  return status;
}

EMGMResponse OPCUA_MGR::addRemoveWatchMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smRemoveWatchArgName, smRemoveWatchArgDescription);

  UA_MethodAttributes removeWatchAttr = createAttribute(smRemoveWatchAttrDisplayName, smRemoveWatchAttrDescription);
  return addMethodNode(paServer, smRemoveWatchMethodName, mResourceTypeId, removeWatchAttr, &inputArgument, 1, nullptr, 0, &onRemoveWatch);
}

UA_StatusCode OPCUA_MGR::onRemoveWatch(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  std::vector<std::string> fullFbName;
  parseDestinationName(destination, fullFbName);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::MonitoringRemoveWatch, CStringDictionary::insert(resourceName), nullptr, fullFbName);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addTriggerEventMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smTriggerEventArgName, smTriggerEventArgDescription);

  UA_MethodAttributes triggerEventAttr = createAttribute(smTriggerEventAttrDisplayName, smTriggerEventAttrDescription);
  return addMethodNode(paServer, smTriggerEventMethodName, mResourceTypeId, triggerEventAttr, &inputArgument, 1, nullptr, 0, &onTriggerEvent);
}

UA_StatusCode OPCUA_MGR::onTriggerEvent(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  std::vector<std::string> fullFbName;
  parseDestinationName(destination, fullFbName);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::MonitoringTriggerEvent, CStringDictionary::insert(resourceName), OPCUA_MGR::scmTriggerEventParam, fullFbName);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addForceValueMethod(UA_Server* paServer) {
  UA_Argument inputArguments[2];
  initArgument(inputArguments[0], UA_TYPES_STRING, smForceValueArg1Name, smForceValueArg1Description);
  initArgument(inputArguments[1], UA_TYPES_STRING, smForceValueArg2Name, smForceValueArg2Description);

  UA_MethodAttributes forceValueAttr = createAttribute(smForceValueAttrDisplayName, smForceValueAttrDescription);
  return addMethodNode(paServer, smForceValueMethodName, mResourceTypeId, forceValueAttr, inputArguments, 2, nullptr, 0, &onForceValue);
}

UA_StatusCode OPCUA_MGR::onForceValue(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  const std::string writeValue(getInputValue(*static_cast<UA_String*>(input[1].data)));
  std::vector<std::string> writeDestination;
  parseDestinationName(destination, writeDestination);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::MonitoringForce, CStringDictionary::insert(resourceName), writeValue.c_str(), writeDestination);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

EMGMResponse OPCUA_MGR::addClearForceMethod(UA_Server* paServer) {
  UA_Argument inputArgument;
  initArgument(inputArgument, UA_TYPES_STRING, smClearForceArgName, smClearForceArgDescription);

  UA_MethodAttributes clearForceAttr = createAttribute(smClearForceAttrDisplayName, smClearForceAttrDescription);
  return addMethodNode(paServer, smClearForceMethodName, mResourceTypeId, clearForceAttr, &inputArgument, 1, nullptr, 0, &onClearForce);
}

UA_StatusCode OPCUA_MGR::onClearForce(UA_Server*,
  const UA_NodeId*, void*,
  const UA_NodeId*, void* methodContext,
  const UA_NodeId*, void* objectContext,
  size_t, const UA_Variant* input,
  size_t, UA_Variant*) {
  if (!methodContext) {
    return UA_STATUSCODE_BADUNKNOWNRESPONSE;
  }
  EMGMResponse eRetVal = EMGMResponse::UnsupportedType;
  const std::string destination(getInputValue(*static_cast<UA_String*>(input[0].data)));
  std::vector<std::string> fullFbName;
  parseDestinationName(destination, fullFbName);

  const char* resourceName = static_cast<const char*>(objectContext);
  OPCUA_MGR* uaMGR = static_cast<OPCUA_MGR*>(methodContext);
  uaMGR->setMGMCommand(EMGMCommandType::MonitoringClearForce, CStringDictionary::insert(resourceName), nullptr, fullFbName);
  eRetVal = uaMGR->mUaDevice.executeMGMCommand(uaMGR->mCommand);
  return scResponseMap.find(eRetVal)->second;
}

#endif // FORTE_SUPPORT_MONITORING

/* Helpers */

EMGMResponse OPCUA_MGR::addMethodNode(UA_Server* paServer, char* paMethodNodeName, UA_NodeId paParentNodeId, UA_MethodAttributes paAttr, 
    const UA_Argument *paInArgs, size_t paInArgSize, const UA_Argument *paOutArgs, 
    size_t paOutArgSize, UA_MethodCallback paCallback, void* nodeContext) {
  EMGMResponse eRetVal = EMGMResponse::Ready;
  UA_NodeId methodId;
  UA_StatusCode status = UA_Server_addMethodNode(paServer, UA_NODEID_STRING(smNamespaces[1], paMethodNodeName),
    paParentNodeId,
    UA_NODEID_NUMERIC(smNamespaces[0], UA_NS0ID_HASCOMPONENT),
    UA_QUALIFIEDNAME(smNamespaces[1], paMethodNodeName),
    paAttr, paCallback,
    paInArgSize, paInArgs, paOutArgSize, paOutArgs,
    nodeContext == nullptr ? this : nodeContext, &methodId);

  if (status != UA_STATUSCODE_GOOD) {
    return EMGMResponse::InvalidState;
  }
  status = addReference(paServer, methodId);
  if (status != UA_STATUSCODE_GOOD) {
    return EMGMResponse::InvalidState;
  }
  return eRetVal;
}

void OPCUA_MGR::initArgument(UA_Argument& paArgument, int paTypeId, char* paName, char* paDescription) {
  UA_Argument_init(&paArgument);
  paArgument.description = UA_LOCALIZEDTEXT(smEmptyLocale, paDescription);
  paArgument.name = UA_STRING(paName);
  paArgument.dataType = UA_TYPES[paTypeId].typeId;
  paArgument.valueRank = UA_VALUERANK_SCALAR;
}

UA_MethodAttributes OPCUA_MGR::createAttribute(char* paDisplayName, char* paDescription) {
  UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
  methodAttr.description = UA_LOCALIZEDTEXT(smEmptyLocale, paDescription);
  methodAttr.displayName = UA_LOCALIZEDTEXT(smEmptyLocale, paDisplayName);
  methodAttr.executable = true;
  methodAttr.userExecutable = true;
  return methodAttr;
}

UA_StatusCode OPCUA_MGR::addReference(UA_Server* paServer, UA_NodeId& paMethodId) {
  return UA_Server_addReference(paServer, paMethodId,
    UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
    UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);
}

void OPCUA_MGR::clearMGMCommand() {
  mCommand.mFirstParam.clear();
  mCommand.mSecondParam.clear();
  mCommand.mAdditionalParams.clear();
#ifdef FORTE_SUPPORT_MONITORING
  mCommand.mMonitorResponse.clear();
#endif // FORTE_SUPPORT_MONITORING
}

void OPCUA_MGR::setMGMCommand(EMGMCommandType paCMD, CStringDictionary::TStringId paDestination,
  const char* paAdditionalParams, const char* paFirstParam, const char* paSecondParam) {
  clearMGMCommand();
  mCommand.mCMD = paCMD;
  mCommand.mDestination = paDestination;
  if (paFirstParam != nullptr) {
    mCommand.mFirstParam.push_back(CStringDictionary::insert(paFirstParam));
  }
  if (paSecondParam != nullptr) {
    mCommand.mSecondParam.push_back(CStringDictionary::insert(paSecondParam));
  }
  if (paAdditionalParams != nullptr) {
    mCommand.mAdditionalParams = paAdditionalParams;
  }
}

void OPCUA_MGR::setMGMCommand(EMGMCommandType paCMD, CStringDictionary::TStringId paDestination,
    const char* paAdditionalParams, std::vector<std::string>& paFirstParam, 
    std::vector<std::string> paSecondParam) {
  clearMGMCommand();
  mCommand.mCMD = paCMD;
  mCommand.mDestination = paDestination;
  if (!paFirstParam.empty()) {
    for (std::string param : paFirstParam) {
      mCommand.mFirstParam.push_back(CStringDictionary::insert(param.c_str()));
    }
  }
  if (!paSecondParam.empty()) {
    for (std::string param : paSecondParam) {
      mCommand.mSecondParam.push_back(CStringDictionary::insert(param.c_str()));
    }
  }
  if (paAdditionalParams != nullptr) {
    mCommand.mAdditionalParams = paAdditionalParams;
  }
}

std::string OPCUA_MGR::getInputValue(UA_String paInput) {
  if (paInput.length == 0) {
    return std::string(" ");
  }
  std::string inputNameStr((const char*)paInput.data, paInput.length);
  return inputNameStr.substr(0, paInput.length);
}

void OPCUA_MGR::parseDestinationName(const std::string& paDestination, std::vector<std::string>& paFbName) {
  std::string dst(paDestination);
  size_t index = dst.find_first_of(".");
  while (index != std::string::npos) {
    paFbName.push_back(dst.substr(0, index));
    dst = dst.substr(paFbName.back().length() + 1);
    index = dst.find_first_of(".");
  }
  paFbName.push_back(dst.substr(0, index));
}

void OPCUA_MGR::addExtraMgmMethod(const MethodInformation& paMethod){
  mExtraMgmMethods.emplace_back(paMethod);
}

void OPCUA_MGR::addExtraResourceMethod(const MethodInformation& paMethod){
  mExtraResourceMethods.emplace_back(paMethod);
}

EMGMResponse OPCUA_MGR::addExtraMethods(UA_Server* paServer, UA_NodeId paParentNodeId, std::vector<MethodInformation>& paMethods) {
  for(auto& method : paMethods){
    auto attributes = createAttribute(method.mDisplayName.data(), method.mDescription.data());
    if (auto result = addMethodNode(paServer, method.mMethodName.data(), paParentNodeId, attributes, 
          method.mInArguments.data(), method.mInArguments.size(), 
          method.mOutArguments.data(), method.mOutArguments.size(), method.mCallback, method.mNodeContext); result != EMGMResponse::Ready){
        
        return result;
    }
  }
  return EMGMResponse::Ready;
}