/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
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

#include "opcua_objectstruct_helper.h"
#include "opcua_layer.h"
#include "struct_action_info.h"
#include "../../core/cominfra/basecommfb.h"
#include "opcua_local_handler.h"

using namespace forte::com_infra;

const std::string COPC_UA_ObjectStruct_Helper::structTypesBrowsePath = "/Types/0:ObjectTypes/0:BaseObjectType/2:";

const std::string COPC_UA_ObjectStruct_Helper::memberNamespaceIndex = "/2:";

COPC_UA_ObjectStruct_Helper::COPC_UA_ObjectStruct_Helper(COPC_UA_Layer &paLayer, COPC_UA_HandlerAbstract *paHandler):
  mLayer(paLayer), mHandler(paHandler) {
}

COPC_UA_ObjectStruct_Helper::~COPC_UA_ObjectStruct_Helper() {
  mHandler = nullptr;
}

void COPC_UA_ObjectStruct_Helper::uninitializeStruct() {
  for(std::shared_ptr<CActionInfo> actionInfo : mStructMemberActionInfos) {
      mHandler->uninitializeAction(*actionInfo);
    }
    if(mCreateNodeActionInfo) {
      mHandler->uninitializeAction(*mCreateNodeActionInfo);
  }
}

bool COPC_UA_ObjectStruct_Helper::checkStructTypeConnection(bool paIsPublisher) {
  std::string browsePath;
  COPC_UA_ObjectStruct_Helper::getStructBrowsePath(browsePath, COPC_UA_ObjectStruct_Helper::structTypesBrowsePath, paIsPublisher);
  if(COPC_UA_ObjectStruct_Helper::isOPCUAObjectPresent(browsePath)) {
    return true;
  }
  DEVLOG_ERROR("[OPC UA LAYER]: Invalid Struct type connected to FB %s\n", mLayer.getCommFB()->getInstanceName());
  return false;
}

forte::com_infra::EComResponse COPC_UA_ObjectStruct_Helper::createObjectNode(CActionInfo& paActionInfo, bool paIsPublisher) {
  EComResponse response = e_InitTerminated;
  std::string browsePath = (*paActionInfo.getNodePairInfo().begin())->mBrowsePath;
  if(!isOPCUAObjectPresent(browsePath)) {
    mCreateNodeActionInfo = getCreateObjectActionInfo(paActionInfo, browsePath, paIsPublisher);
    if( (UA_STATUSCODE_GOOD != mHandler->initializeAction(*mCreateNodeActionInfo)) || (UA_STATUSCODE_GOOD != mHandler->executeAction(*mCreateNodeActionInfo)) ) {
      return response;
    }
  }
  return initializeMemberAction(paActionInfo, browsePath, paIsPublisher);
}

CIEC_ANY const *COPC_UA_ObjectStruct_Helper::getStructMember(CActionInfo &paActionInfo, bool paIsSD) {
  CIEC_ANY** apoDataPorts = paIsSD ? mLayer.getCommFB()->getSDs() : mLayer.getCommFB()->getRDs();
  CIEC_STRUCT& structType = static_cast<CIEC_STRUCT&>(apoDataPorts[0]->unwrap());
  const std::string actionBrowsePath = (*paActionInfo.getNodePairInfo().begin())->mBrowsePath;

  for(size_t i = 0; i < mStructMemberActionInfos.size(); i++) {
    std::shared_ptr<CActionInfo> actionInfo = mStructMemberActionInfos[i];
    std::string browsePath = (*actionInfo->getNodePairInfo().begin())->mBrowsePath;
    if(browsePath == actionBrowsePath) {
      return structType.getMember(i);
    }
  }
  return nullptr;
}

forte::com_infra::EComResponse COPC_UA_ObjectStruct_Helper::executeStructAction() {
  for(std::shared_ptr<CActionInfo> actionInfo : mStructMemberActionInfos) {
    if(UA_STATUSCODE_GOOD != mHandler->executeAction(*actionInfo)) {
      return e_ProcessDataDataTypeError;
    }
  }
  return e_ProcessDataOk;
}

int COPC_UA_ObjectStruct_Helper::getRDBufferIndexFromNodeId(const UA_NodeId *paNodeId) {
  for(size_t i = 0; i < mStructMemberActionInfos.size(); i++) {
    std::shared_ptr<CActionInfo> actionInfo = mStructMemberActionInfos[i];
    UA_NodeId *nodeId = (*actionInfo->getNodePairInfo().begin())->mNodeId;
    if(UA_NodeId_equal(nodeId, paNodeId)) {
      return (int)i;
    }
  }
  return -1;
}

void COPC_UA_ObjectStruct_Helper::setMemberValues(CIEC_ANY ***paRDBuffer) {
  // TODO implement layer to handle more than 1 struct
  CIEC_ANY** apoRDs = mLayer.getCommFB()->getRDs();
  CIEC_STRUCT& structType = static_cast<CIEC_STRUCT&>(apoRDs[0]->unwrap());
  for(size_t i = 0; i < structType.getStructSize(); i++) {
    structType.getMember(i)->setValue(*(*paRDBuffer)[i]);
  }
}

void COPC_UA_ObjectStruct_Helper::initializeRDBuffer(CIEC_ANY ***paRDBuffer) {
  // TODO implement layer to handle more than 1 struct
  CIEC_ANY** apoRDs = mLayer.getCommFB()->getRDs();
  CIEC_STRUCT& structType = static_cast<CIEC_STRUCT&>(apoRDs[0]->unwrap());
  const size_t structSize = structType.getStructSize();
  *paRDBuffer = new CIEC_ANY*[structSize];
  for(size_t i = 0; i < structSize; i++) {
    (*paRDBuffer)[i] = structType.getMember(i)->clone(nullptr);
  }
}

void COPC_UA_ObjectStruct_Helper::deleteRDBufferEntries(CIEC_ANY ***paRDBuffer) {
  if(mLayer.getCommFB()->getComServiceType() == e_Subscriber) {
    CIEC_ANY** apoRDs = mLayer.getCommFB()->getRDs();
    CIEC_STRUCT& structType = static_cast<CIEC_STRUCT&>(apoRDs[0]->unwrap());
    for(size_t i = 0; i < structType.getStructSize(); i++) {
      delete (*paRDBuffer)[i];
    }
  }
}

bool COPC_UA_ObjectStruct_Helper::isStructType(const COPC_UA_Layer &paLayer, bool paIsPublisher) {
  // TODO implement layer to handle more than 1 struct
  // For the concept, only the first SD/RD port needs to be a struct 
  TPortId nrOfPorts = paIsPublisher ? paLayer.getCommFB()->getNumSD() : paLayer.getCommFB()->getNumRD();
  CIEC_ANY** apoDataPorts = paIsPublisher ? paLayer.getCommFB()->getSDs() : paLayer.getCommFB()->getRDs();
  return nrOfPorts > 0 && CIEC_ANY::e_STRUCT == apoDataPorts[0]->unwrap().getDataTypeID();
}

std::shared_ptr<CActionInfo> COPC_UA_ObjectStruct_Helper::getCreateObjectActionInfo(CActionInfo& paActionInfo, std::string &paBrowsePath, bool paIsPublisher) {
  // TODO implement layer to handle more than 1 struct
  std::shared_ptr<CActionInfo> actionInfo = std::make_shared<CActionInfo>(mLayer, CActionInfo::UA_ActionType::eCreateObject, paActionInfo.getEndpoint());
  std::string typeBrowsePath;
  getStructBrowsePath(typeBrowsePath, COPC_UA_ObjectStruct_Helper::structTypesBrowsePath, paIsPublisher);

  CSinglyLinkedList<CActionInfo::CNodePairInfo*>& nodePairs = actionInfo->getNodePairInfo();
  nodePairs.pushBack(new CActionInfo::CNodePairInfo(nullptr, typeBrowsePath));
  nodePairs.pushBack(new CActionInfo::CNodePairInfo(nullptr, paBrowsePath));
  return actionInfo;
}

forte::com_infra::EComResponse COPC_UA_ObjectStruct_Helper::initializeMemberAction(CActionInfo& paActionInfo, std::string &paBrowsePath, bool paIsPublisher) {
  // TODO implement layer to handle more than 1 struct
  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  if(!localHandler) {
    DEVLOG_ERROR("[OPC UA LAYER]: Failed to get LocalHandler because LocalHandler is null!\n");
    return e_InitTerminated;
  }
  CIEC_ANY** apoDataPorts = paIsPublisher ? mLayer.getCommFB()->getSDs() : mLayer.getCommFB()->getRDs();
  CIEC_STRUCT& structType = static_cast<CIEC_STRUCT&>(apoDataPorts[0]->unwrap());
  const CStringDictionary::TStringId* structMemberNames = structType.elementNames();
  
  for(size_t i = 0; i < structType.getStructSize(); i++) {
    std::string memberBrowsePath;
    getStructMemberBrowsePath(memberBrowsePath, paBrowsePath, structMemberNames[i]);

    std::shared_ptr<CActionInfo> actionInfo = std::make_shared<CStructMemberActionInfo>(*this, mLayer, paActionInfo.getAction(), paActionInfo.getEndpoint());
    CIEC_ANY* memberVariable = structType.getMember(i);
    actionInfo->getNodePairInfo().pushBack(new CActionInfo::CNodePairInfo(nullptr, memberBrowsePath));
    if(UA_STATUSCODE_GOOD != localHandler->initializeActionForObjectStruct(actionInfo, *memberVariable)) {
      DEVLOG_ERROR("[OPC UA LAYER]: Error occured in FB %s while initializing Struct member %s\n", mLayer.getCommFB()->getInstanceName(),
      CStringDictionary::getInstance().get(structMemberNames[i]));
      return e_InitTerminated;
    }
    mStructMemberActionInfos.push_back(std::move(actionInfo));
  }
  return e_InitOk;
}

bool COPC_UA_ObjectStruct_Helper::isOPCUAObjectPresent(std::string &paBrowsePath) {
  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  if(localHandler) {
    CActionInfo::CNodePairInfo nodePair(nullptr, paBrowsePath);
    return localHandler->isOPCUAObjectPresent(nodePair);
  } else {
    DEVLOG_ERROR("[OPC UA LAYER]: Failed to get LocalHandler because LocalHandler is null!\n");
  }
  return false;
}

void COPC_UA_ObjectStruct_Helper::getStructBrowsePath(std::string &paBrowsePath, const std::string &paPathPrefix, bool paIsPublisher) {
  std::string structTypeName;
  getStructTypeName(structTypeName, paIsPublisher);
  if(!structTypeName.empty()) {
    paBrowsePath.append(paPathPrefix);
    paBrowsePath.append(structTypeName);
  }
}

void COPC_UA_ObjectStruct_Helper::getStructMemberBrowsePath(std::string &paMemberBrowsePath, std::string &paBrowsePathPrefix, const CStringDictionary::TStringId structMemberNameId) {
  paMemberBrowsePath.append(paBrowsePathPrefix);
  paMemberBrowsePath.append(memberNamespaceIndex);
  paMemberBrowsePath.append(CStringDictionary::getInstance().get(structMemberNameId));
}

void COPC_UA_ObjectStruct_Helper::getStructTypeName(std::string &paStructTypeName, bool paIsPublisher) {
  CIEC_ANY* type = paIsPublisher ? mLayer.getCommFB()->getSDs()[0] : mLayer.getCommFB()->getRDs()[0];
  if(type) {
    paStructTypeName = CStringDictionary::getInstance().get(type->unwrap().getTypeNameID());
  }
}
