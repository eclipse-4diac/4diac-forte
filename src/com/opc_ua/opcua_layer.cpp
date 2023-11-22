/*******************************************************************************
 * Copyright (c) 2015-2019 Florian Froschermeier <florian.froschermeier@tum.de>,
 *               fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Florian Froschermeier
 *      - initial integration of the OPC-UA protocol
 *    Stefan Profanter
 *      - refactoring and adaption to new concept
 *    Jose Cabral:
 *      - refactoring to cleaner architecture
 *******************************************************************************/

#include "opcua_layer.h"
#include "opcua_helper.h"
#include "opcua_local_handler.h"
#include "../../core/cominfra/basecommfb.h"
#include "../../arch/devlog.h"
#include <forte_string.h>
#include <criticalregion.h>
#include "opcua_action_info.h"
#include "opcua_remote_handler.h"
#include "struct_action_info.h"

using namespace forte::com_infra;

const std::string COPC_UA_Layer::structTypesBrowsePath = "/Types/0:ObjectTypes/0:BaseObjectType/2:";

COPC_UA_Layer::COPC_UA_Layer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
    CComLayer(paUpperLayer, paComFB), mInterruptResp(e_Nothing), mHandler(nullptr), mActionInfo(nullptr), mDataAlreadyPresent(false), mIsObjectNodeStruct(false), mRDBuffer(nullptr) {
}

COPC_UA_Layer::~COPC_UA_Layer() = default;

EComResponse COPC_UA_Layer::openConnection(char *paLayerParameter) {
  EComResponse response = e_InitTerminated;
  mActionInfo = CActionInfo::getActionInfoFromParams(paLayerParameter, *this);
  if(mActionInfo) {
      mHandler =
          (mActionInfo->isRemote()) ? static_cast<COPC_UA_HandlerAbstract*>(&getExtEvHandler<COPC_UA_Remote_Handler>()) :
            static_cast<COPC_UA_HandlerAbstract*>(&getExtEvHandler<COPC_UA_Local_Handler>());
    if(checkTypesFromInterface()) {
      if(UA_STATUSCODE_GOOD == mHandler->initializeAction(*mActionInfo)) {
        CCriticalRegion criticalRegion(mRDBufferMutex);
        response = e_InitOk;
        mRDBuffer = new CIEC_ANY*[getCommFB()->getNumRD()];
        for(size_t i = 0; i < getCommFB()->getNumRD(); ++i) {
          mRDBuffer[i] = getCommFB()->getRDs()[i]->clone(nullptr);
        }
      }
    } else {
      bool isPublisher;
      if(getCommFB()->getComServiceType() == EComServiceType::e_Publisher) {
        isPublisher = true;
      } else if(getCommFB()->getComServiceType() == EComServiceType::e_Subscriber) {
        isPublisher = false;
      } else {
        return response;
      }
      
      CActionInfo::UA_ActionType action = mActionInfo->getAction();
      if(isStructType(isPublisher) && checkObjectNodeStructTypeConnection(isPublisher) && (CActionInfo::eWrite == action || CActionInfo::eRead == action) ) {
        mIsObjectNodeStruct = true;
        response = createStructObjectNode(isPublisher);
        
        CCriticalRegion criticalRegion(mRDBufferMutex);
        response = e_InitOk;
        if(!isPublisher) {
          initialiseStructObjectRDBuffer();
        }
      }
    }   
  }
  return response;
}

void COPC_UA_Layer::closeConnection() {
  if(mHandler) {
    CCriticalRegion criticalRegion(mRDBufferMutex);
    mHandler->uninitializeAction(*mActionInfo);
    delete mActionInfo;

    for(std::shared_ptr<CActionInfo> actionInfo : mObjectNodeStructActionInfos) {
      mHandler->uninitializeAction(*actionInfo);
    }
    if(mCreateObjectStructNode) {
      mHandler->uninitializeAction(*mCreateObjectStructNode);
    }
    mHandler = nullptr;

    if(mRDBuffer) {
      if(mIsObjectNodeStruct) {
        deleteStructObjectRDBuffer();
      } else {
        for (size_t i = 0; i < getCommFB()->getNumRD(); ++i) {
          delete mRDBuffer[i];
        }
      }
      delete[] mRDBuffer;
      mRDBuffer = nullptr;
    }
  }
}

EComResponse COPC_UA_Layer::recvData(const void *paData, unsigned int) {
  mInterruptResp = e_ProcessDataOk;
  const COPC_UA_Helper::UA_RecvVariable_handle *handleRecv = static_cast<const COPC_UA_Helper::UA_RecvVariable_handle *>(paData);

  if(!handleRecv->mFailed) {
    if(handleRecv->mSize) {
      if(handleRecv->mSize + handleRecv->mOffset <= getCommFB()->getNumRD()) {
        CCriticalRegion criticalRegion(mRDBufferMutex);
        for(size_t i = 0; i < handleRecv->mSize; i++) {
          long long bufferIndex = mIsObjectNodeStruct ? getRDBufferIndexFromNodeId(handleRecv->mNodeId) : handleRecv->mOffset + i;
          if(bufferIndex == -1) {
            DEVLOG_ERROR("[OPC UA LAYER]: Received Node ID %d does not match with any registered Node ID for FB %s\n", handleRecv->mNodeId, getCommFB()->getInstanceName());
            mInterruptResp = e_ProcessDataRecvFaild;
            break;
          }
          if(UA_Variant_isScalar(handleRecv->mData[i]) && handleRecv->mData[i]->data
            && handleRecv->mData[i]->type == COPC_UA_Helper::getOPCUATypeFromAny(*mRDBuffer[bufferIndex])) {
            COPC_UA_Helper::convertFromOPCUAType(handleRecv->mData[i]->data, *mRDBuffer[bufferIndex]);
          } else {
            DEVLOG_ERROR("[OPC UA LAYER]: RD_%d of FB %s has no data, is not a scalar or there is a type mismatch\n", bufferIndex,
              getCommFB()->getInstanceName());
            mInterruptResp = e_ProcessDataRecvFaild;
            break;
          }
        }
      } else {
        DEVLOG_ERROR("[OPC UA LAYER]: Receiving data for FB %s failed because the response size is %u with an offset of %u but the FB has %u RDs\n",
          getCommFB()->getInstanceName(), handleRecv->mSize, handleRecv->mOffset, getCommFB()->getNumRD());
        mInterruptResp = e_ProcessDataRecvFaild;
      }
    } else { //no data received. When remote writing this will happen
      mInterruptResp = e_ProcessDataOk;
    }
  } else { //this fail is logged in the handler
    mInterruptResp = e_ProcessDataRecvFaild;
  }

  if(e_ProcessDataOk == mInterruptResp) {
    if(getDataAlreadyPresentRead()) {
      return e_Nothing; //won't trigger another external event on subscription
    } else {
      setDataAlreadyPresentRead(true);
      return e_ProcessDataOk; //will trigger an external event on subscription
    }
  }

  return mInterruptResp;
}

EComResponse COPC_UA_Layer::sendData(void *, unsigned int) {
  if(mIsObjectNodeStruct) {
    return executeActionForObjectNodeStruct();
  }
  return (UA_STATUSCODE_GOOD == mHandler->executeAction(*mActionInfo) ? e_ProcessDataOk : e_ProcessDataDataTypeError);
}


EComResponse COPC_UA_Layer::processInterrupt() {
  CCriticalRegion criticalRegion(mRDBufferMutex);
  if(mIsObjectNodeStruct) { 
    setObjectStructData();
  } else {
    for(size_t i = 0; i < getCommFB()->getNumRD(); ++i) {
      getCommFB()->getRDs()[i]->setValue(*mRDBuffer[i]);
    }
  }
  setDataAlreadyPresentRead(false);
  return mInterruptResp;
}

void COPC_UA_Layer::triggerNewEvent() {
  mHandler->triggerNewEvent(*this->getCommFB());
}

bool COPC_UA_Layer::checkTypesFromInterface() const {
  bool somethingFailed = false;
  for(unsigned int i = 0; i < getCommFB()->getNumSD(); i++) {
    if(!checkPortConnectionInfo(i + 2, true)) {
      somethingFailed = true;
      break;
    }
  }
  if(!somethingFailed) {
    for(unsigned int i = 0; i < getCommFB()->getNumRD(); i++) {
      if(!checkPortConnectionInfo(i + 2, false)) {
        somethingFailed = true;
        break;
      }
    }
  }

  return !somethingFailed;
}

bool COPC_UA_Layer::checkPortConnectionInfo(unsigned int paPortIndex, bool paIsSD) const {
  const CStringDictionary::TStringId localPortNameId = getLocalPortNameId(paPortIndex, paIsSD);
  const CDataConnection *localPortConnection = getLocalPortConnection(paPortIndex, paIsSD);
  if(!localPortConnection) {
    DEVLOG_ERROR("[OPC UA LAYER]: Got invalid port connection on FB %s at port %s. It must be connected to another FB.\n", getCommFB()->getInstanceName(),
      CStringDictionary::getInstance().get(localPortNameId));
    return false;
  }

  if(!localPortConnection->isConnected()) {
    DEVLOG_ERROR("[OPC UA LAYER]: Connection %s of FB %s is not connected to anything.\n", CStringDictionary::getInstance().get(localPortNameId),
      getCommFB()->getInstanceName());
    return false;
  }

  CIEC_ANY *remoteType = nullptr;
  if(paIsSD) {
    const CConnectionPoint &remoteConnectionPoint = localPortConnection->getSourceId();
    if(!getRemoteAny(&remoteType, remoteConnectionPoint, paIsSD)) {
      return false;
    }
  } else {
    if(!checkFanOutTypes(*localPortConnection, &remoteType)) {
      return false;
    }
  }

  if(!COPC_UA_Helper::getOPCUATypeFromAny(*remoteType)) {
    if(!isStructType(paIsSD)) {
      DEVLOG_ERROR("[OPC UA LAYER]: Invalid  type %d in FB %s at connection %s\n", remoteType, getCommFB()->getInstanceName(),
      CStringDictionary::getInstance().get(localPortNameId));
    }
    return false;
  }
  return true;
}

bool COPC_UA_Layer::getRemoteAny(CIEC_ANY **paResult, const CConnectionPoint &paRemoteConnectionPoint, bool paIsSD) const {
  if(!paRemoteConnectionPoint.mFB) {
    DEVLOG_ERROR(
      "[OPC UA LAYER]: FB %s has a problem. The connected FB in the current data input is a null pointer. Check last debug logging for more information\n",
      getCommFB()->getInstanceName());
    return false;
  }

  *paResult =
      paIsSD ? paRemoteConnectionPoint.mFB->getDOFromPortId(paRemoteConnectionPoint.mPortId) :
        paRemoteConnectionPoint.mFB->getDIFromPortId(paRemoteConnectionPoint.mPortId);

  return true;
}

bool COPC_UA_Layer::checkFanOutTypes(const CDataConnection &paPortConnection, CIEC_ANY **paResult) const {

  for(const auto& it : paPortConnection.getDestinationList()){
    if(paPortConnection.getDestinationList().front() == it) { //first one
      if(!getRemoteAny(paResult, it, false)) {
        return false;
      }
    } else {
      CIEC_ANY* newRemoteType;
      if(!getRemoteAny(&newRemoteType, it, false)) {
        return false;
      } else {
        if(newRemoteType->unwrap().getDataTypeID() != (*paResult)->unwrap().getDataTypeID()) {
          DEVLOG_ERROR("[OPC UA LAYER]: FB %s has one RD which is connected to many data inputs and the types are not the same.\n",
            getCommFB()->getInstanceName());
          return false;
        }
      }
    }
  }
  return true;
}

bool COPC_UA_Layer::getDataAlreadyPresentRead() {
  CCriticalRegion dataReadRegion(mDataAlreadyPresentMutex);
  return mDataAlreadyPresent;
}
void COPC_UA_Layer::setDataAlreadyPresentRead(bool paDataRead) {
  CCriticalRegion dataReadRegion(mDataAlreadyPresentMutex);
  mDataAlreadyPresent = paDataRead;
}

forte::com_infra::EComResponse COPC_UA_Layer::executeActionForObjectNodeStruct() {
  for(std::shared_ptr<CActionInfo> actionInfo : mObjectNodeStructActionInfos) {
    if(UA_STATUSCODE_GOOD != mHandler->executeAction(*actionInfo)) {
      return e_ProcessDataDataTypeError;
    }
  }
  return e_ProcessDataOk;
}

const CDataConnection* COPC_UA_Layer::getLocalPortConnection(int paPortIndex, bool paIsSD) const {
  const CStringDictionary::TStringId localPortNameId = getLocalPortNameId(paPortIndex, paIsSD);
  const CDataConnection *localPortConnection = paIsSD ? getCommFB()->getDIConnection(localPortNameId) : getCommFB()->getDOConnection(localPortNameId);
  return localPortConnection;
}

CStringDictionary::TStringId COPC_UA_Layer::getLocalPortNameId(int paPortIndex, bool paIsSD) const {
  const SFBInterfaceSpec *localInterfaceSpec = getCommFB()->getFBInterfaceSpec();
  return paIsSD ? localInterfaceSpec->mDINames[paPortIndex] : localInterfaceSpec->mDONames[paPortIndex];
}

bool COPC_UA_Layer::checkObjectNodeStructTypeConnection(bool paIsPublisher) {
  int portIndex = 2;
  if(mActionInfo && mHandler) {
    const CDataConnection* localPortConnection = getLocalPortConnection(portIndex, paIsPublisher);
    std::string browsePath;
    getObjectNodeStructBrowsePath(browsePath, localPortConnection, COPC_UA_Layer::structTypesBrowsePath, paIsPublisher);
    if(isOPCUAStructObjectPresent(browsePath)) {
      return true;
    }
    DEVLOG_ERROR("[OPC UA LAYER]: Invalid Struct type in FB %s at connection %s\n", getCommFB()->getInstanceName(),
    CStringDictionary::getInstance().get(getLocalPortNameId(portIndex, paIsPublisher)));
  } else {
    DEVLOG_ERROR("[OPC UA LAYER]: Failed to check Object Struct Type Connection because Action or Handler is null!\n");
  }
  return false;
}

void COPC_UA_Layer::initialiseStructObjectRDBuffer() {
  // TODO implement layer to handle more than 1 struct
  CIEC_ANY** apoRDs = getCommFB()->getRDs();
  CIEC_STRUCT& structType = static_cast<CIEC_STRUCT&>(apoRDs[0]->unwrap());
  const size_t structSize = structType.getStructSize();
  mRDBuffer = new CIEC_ANY*[structSize];
  for(size_t i = 0; i < structSize; i++) {
    mRDBuffer[i] = structType.getMember(i)->clone(nullptr);
  }
}

void COPC_UA_Layer::deleteStructObjectRDBuffer() {
  if(getCommFB()->getComServiceType() == e_Subscriber) {
    CIEC_ANY** apoRDs = getCommFB()->getRDs();
    CIEC_STRUCT& structType = static_cast<CIEC_STRUCT&>(apoRDs[0]->unwrap());
    for(size_t i = 0; i < structType.getStructSize(); i++) {
      delete mRDBuffer[i];
    }
  }
}

void COPC_UA_Layer::setObjectStructData() {
  // TODO implement layer to handle more than 1 struct
  CIEC_ANY** apoRDs = getCommFB()->getRDs();
  CIEC_STRUCT& structType = static_cast<CIEC_STRUCT&>(apoRDs[0]->unwrap());
  for(size_t i = 0; i < structType.getStructSize(); i++) {
    structType.getMember(i)->setValue(*mRDBuffer[i]);
  }
}

int COPC_UA_Layer::getRDBufferIndexFromNodeId(const UA_NodeId *paNodeId) {
  for(size_t i = 0; i < mObjectNodeStructActionInfos.size(); i++) {
    std::shared_ptr<CActionInfo> actionInfo = mObjectNodeStructActionInfos[i];
    UA_NodeId *nodeId = (*actionInfo->getNodePairInfo().begin())->mNodeId;
    if(UA_NodeId_equal(nodeId, paNodeId)) {
      return (int)i;
    }
  }
  return -1;
}

void COPC_UA_Layer::getObjectNodeStructBrowsePath(std::string &paBrowsePath, const CDataConnection *paLocalPortConnection, const std::string &paPathPrefix, bool paIsPublisher) {
  std::string structTypeName;
  getStructTypeName(structTypeName, paLocalPortConnection, paIsPublisher);
  if(!structTypeName.empty()) {
    paBrowsePath.append(paPathPrefix);
    paBrowsePath.append(structTypeName);
  }
}

void COPC_UA_Layer::getObjectNodeStructMemberBrowsePath(std::string &paMemberBrowsePath, std::string &paBrowsePathPrefix, const CStringDictionary::TStringId structMemberNameId) {
  paMemberBrowsePath.append(paBrowsePathPrefix);
  paMemberBrowsePath.append("/2:");
  paMemberBrowsePath.append(CStringDictionary::getInstance().get(structMemberNameId));
}

void COPC_UA_Layer::getStructTypeName(std::string &paStructTypeName, const CDataConnection *paLocalPortConnection, bool paIsPublisher) {
  CConnectionPoint connectionPoint = paIsPublisher ? paLocalPortConnection->getSourceId() : paLocalPortConnection->getDestinationList().front();
  CIEC_ANY* type = paIsPublisher ? connectionPoint.mFB->getDOFromPortId(connectionPoint.mPortId) : connectionPoint.mFB->getDIFromPortId(connectionPoint.mPortId);
  if(type) {
    paStructTypeName = CStringDictionary::getInstance().get(type->getTypeNameID());
  }
}

bool COPC_UA_Layer::isStructType(bool paIsPublisher) const {
  // TODO implement layer to handle more than 1 struct
  // For the concept, only the first SD/RD port needs to be a struct 
  TPortId nrOfPorts = paIsPublisher ? getCommFB()->getNumSD() : getCommFB()->getNumRD();
  CIEC_ANY** apoDataPorts = paIsPublisher ? getCommFB()->getSDs() : getCommFB()->getRDs();
  return nrOfPorts > 0 && CIEC_ANY::e_STRUCT == apoDataPorts[0]->unwrap().getDataTypeID();
}

bool COPC_UA_Layer::isOPCUAStructObjectPresent(std::string &paBrowsePath) {
  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  if(localHandler) {
    CActionInfo::CNodePairInfo nodePair(nullptr, paBrowsePath);
    return localHandler->isOPCUAObjectPresent(nodePair);
  } else {
    DEVLOG_ERROR("[OPC UA LAYER]: Failed to get LocalHandler because LocalHandler is null!\n");
  }
  return false;
}

CIEC_ANY const *COPC_UA_Layer::getObjectStructMember(CActionInfo &paActionInfo, bool paIsSD) {
  CIEC_ANY** apoDataPorts = paIsSD ? getCommFB()->getSDs() : getCommFB()->getRDs();
  CIEC_STRUCT& structType = static_cast<CIEC_STRUCT&>(apoDataPorts[0]->unwrap());
  const std::string actionBrowsePath = (*paActionInfo.getNodePairInfo().begin())->mBrowsePath;

  for(size_t i = 0; i < mObjectNodeStructActionInfos.size(); i++) {
    std::shared_ptr<CActionInfo> actionInfo = mObjectNodeStructActionInfos[i];
    std::string browsePath = (*actionInfo->getNodePairInfo().begin())->mBrowsePath;
    if(browsePath == actionBrowsePath) {
      return structType.getMember(i);
    }
  }
  return nullptr;
}

EComResponse COPC_UA_Layer::createStructObjectNode(bool paIsPublisher) {
  EComResponse response = e_InitTerminated;
  std::string browsePath = (*mActionInfo->getNodePairInfo().begin())->mBrowsePath;
  if(isOPCUAStructObjectPresent(browsePath)) {
    return initializeActionForStructMembers(browsePath, paIsPublisher);
  }
  mCreateObjectStructNode = getCreateObjectActionForObjectNodeStruct(browsePath, paIsPublisher);
  if( (UA_STATUSCODE_GOOD == mHandler->initializeAction(*mCreateObjectStructNode)) && (UA_STATUSCODE_GOOD == mHandler->executeAction(*mCreateObjectStructNode)) ) {
    response = initializeActionForStructMembers(browsePath, paIsPublisher);
  }
  return response;
}

std::shared_ptr<CActionInfo> COPC_UA_Layer::getCreateObjectActionForObjectNodeStruct(std::string &paBrowsePath, bool paIsPublisher) {
  // TODO implement layer to handle more than 1 struct
  int portIndex = 2;
  std::shared_ptr<CActionInfo> actionInfo = std::make_shared<CActionInfo>(*this, CActionInfo::UA_ActionType::eCreateObject, mActionInfo->getEndpoint());
  std::string typeBrowsePath;
  getObjectNodeStructBrowsePath(typeBrowsePath, getLocalPortConnection(portIndex, paIsPublisher), COPC_UA_Layer::structTypesBrowsePath, paIsPublisher);

  CSinglyLinkedList<CActionInfo::CNodePairInfo*>& nodePairs = actionInfo->getNodePairInfo();
  nodePairs.pushBack(new CActionInfo::CNodePairInfo(nullptr, typeBrowsePath));
  nodePairs.pushBack(new CActionInfo::CNodePairInfo(nullptr, paBrowsePath));
  return actionInfo;
}

forte::com_infra::EComResponse COPC_UA_Layer::initializeActionForStructMembers(std::string &paBrowsePath, bool paIsPublisher) {
  // TODO implement layer to handle more than 1 struct
  COPC_UA_Local_Handler* localHandler = static_cast<COPC_UA_Local_Handler*>(mHandler);
  if(!localHandler) {
    DEVLOG_ERROR("[OPC UA LAYER]: Failed to get LocalHandler because LocalHandler is null!\n");
    return e_InitTerminated;
  }
  CIEC_ANY** apoDataPorts = paIsPublisher ? getCommFB()->getSDs() : getCommFB()->getRDs();
  CIEC_STRUCT& structType = static_cast<CIEC_STRUCT&>(apoDataPorts[0]->unwrap());
  const CStringDictionary::TStringId* structMemberNames = structType.elementNames();
  
  for(size_t i = 0; i < structType.getStructSize(); i++) {
    std::string memberBrowsePath;
    getObjectNodeStructMemberBrowsePath(memberBrowsePath, paBrowsePath, structMemberNames[i]);

    std::shared_ptr<CActionInfo> actionInfo = std::make_shared<CStructMemberActionInfo>(*this, mActionInfo->getAction(), mActionInfo->getEndpoint());
    CIEC_ANY* memberVariable = structType.getMember(i);
    actionInfo->getNodePairInfo().pushBack(new CActionInfo::CNodePairInfo(nullptr, memberBrowsePath));
    if(UA_STATUSCODE_GOOD != localHandler->initializeActionForObjectStruct(actionInfo, *memberVariable)) {
      DEVLOG_ERROR("[OPC UA LAYER]: Error occured in FB %s while initializing Struct member %s\n", getCommFB()->getInstanceName(),
      CStringDictionary::getInstance().get(structMemberNames[i]));
      return e_InitTerminated;
    }
    mObjectNodeStructActionInfos.push_back(std::move(actionInfo));
  }
  return e_InitOk;
}
