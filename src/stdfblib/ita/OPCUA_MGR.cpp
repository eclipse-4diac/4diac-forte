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
#include <string>
#include <vector>
#include "OPCUA_DEV.h"
#include "stringdict.h"

char OPCUA_MGR::smEmptyLocale[] = "";
char OPCUA_MGR::smMgmtType[] = "IEC61499MgmtType";
char OPCUA_MGR::smMgmtName[] = "IEC61499Mgmt";
char OPCUA_MGR::smResType[] = "ResourceType";

/* Initialize UA Status Codes */
const std::map<EMGMResponse, UA_StatusCode> scResponseMap = {
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

OPCUA_MGR::OPCUA_MGR(OPCUA_DEV& paUaDevice) :
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
  return addMgmtObjectInstance();
}

EMGMResponse OPCUA_MGR::defineIEC61499MgmtObjectType(UA_Server* paServer) {
  EMGMResponse eRetVal = EMGMResponse::Ready;
  mMgmtTypeId = UA_NODEID_STRING(1, smMgmtType);
  UA_ObjectTypeAttributes rtAttr = UA_ObjectTypeAttributes_default;
  rtAttr.displayName = UA_LOCALIZEDTEXT(smEmptyLocale, smMgmtType);
  UA_StatusCode status = UA_Server_addObjectTypeNode(paServer, mMgmtTypeId,
    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
    UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
    UA_QUALIFIEDNAME(1, smMgmtType), rtAttr,
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
  UA_StatusCode status = UA_Server_addObjectNode(mUaHandler.getUAServer(), UA_NODEID_STRING(1, smMgmtName),
    UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
    UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
    UA_QUALIFIEDNAME(1, smMgmtName),
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
  return EMGMResponse::Ready;
}

EMGMResponse OPCUA_MGR::defineIEC61499ResourceObjectType(UA_Server* paServer) {
  EMGMResponse eRetVal = EMGMResponse::Ready;
  mResourceTypeId = UA_NODEID_STRING(1, smResType);
  UA_ObjectTypeAttributes rtAttr = UA_ObjectTypeAttributes_default;
  rtAttr.displayName = UA_LOCALIZEDTEXT(smEmptyLocale, smResType);
  UA_StatusCode status = UA_Server_addObjectTypeNode(paServer, mResourceTypeId,
    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
    UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
    UA_QUALIFIEDNAME(1, smResType), rtAttr,
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
    UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
    UA_QUALIFIEDNAME(1, paResourceName),
    mResourceTypeId, oAttr, paResourceName, 
    &paNodeId);
  if (status != UA_STATUSCODE_GOOD) {
    return EMGMResponse::InvalidState;
  }
  return eRetVal;
}