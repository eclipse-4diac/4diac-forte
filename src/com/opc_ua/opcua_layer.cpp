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

using namespace forte::com_infra;

COPC_UA_Layer::COPC_UA_Layer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
    CComLayer(paUpperLayer, paComFB), mInterruptResp(e_Nothing), mHandler(0), mActionInfo(0), mDataAlreadyPresent(false), mRDBuffer(0) {
}

COPC_UA_Layer::~COPC_UA_Layer() {
}

EComResponse COPC_UA_Layer::openConnection(char *paLayerParameter) {
  EComResponse response = e_InitTerminated;

  if(checkTypesFromInterface()) {
    mActionInfo = CActionInfo::getActionInfoFromParams(paLayerParameter, *this);
    if(mActionInfo) {
      mHandler =
          (mActionInfo->isRemote()) ? static_cast<COPC_UA_HandlerAbstract*>(&getExtEvHandler<COPC_UA_Remote_Handler>()) :
            static_cast<COPC_UA_HandlerAbstract*>(&getExtEvHandler<COPC_UA_Local_Handler>());

      if(UA_STATUSCODE_GOOD == mHandler->initializeAction(*mActionInfo)) {
        CCriticalRegion criticalRegion(mRDBufferMutex);
        response = e_InitOk;
        mRDBuffer = new CIEC_ANY[getCommFB()->getNumRD()];
        for(size_t i = 0; i < getCommFB()->getNumRD(); ++i) {
          getCommFB()->getRDs()[i].clone(reinterpret_cast<TForteByte*>(&(mRDBuffer[i])));
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
    mHandler = 0;
    delete mActionInfo;
    delete[] mRDBuffer;
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
          if(UA_Variant_isScalar(handleRecv->mData[i]) && handleRecv->mData[i]->data
            && handleRecv->mData[i]->type == COPC_UA_Helper::getOPCUATypeFromAny((mRDBuffer + handleRecv->mOffset)[i])) {
            COPC_UA_Helper::convertFromOPCUAType(handleRecv->mData[i]->data, (mRDBuffer + handleRecv->mOffset)[i]);
          } else {
            DEVLOG_ERROR("[OPC UA LAYER]: RD_%d of FB %s has no data, is not a scalar or there is a type mismatch\n", handleRecv->mOffset + i,
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
  return (UA_STATUSCODE_GOOD == mHandler->executeAction(*mActionInfo) ? e_ProcessDataOk : e_ProcessDataDataTypeError);
}


EComResponse COPC_UA_Layer::processInterrupt() {
  CCriticalRegion criticalRegion(mRDBufferMutex);
  for(size_t i = 0; i < getCommFB()->getNumRD(); ++i) {
    getCommFB()->getRDs()[i].setValue(mRDBuffer[i]);
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
  const SFBInterfaceSpec *localInterfaceSpec = getCommFB()->getFBInterfaceSpec();
  const CStringDictionary::TStringId localPortNameId = paIsSD ? localInterfaceSpec->m_aunDINames[paPortIndex] : localInterfaceSpec->m_aunDONames[paPortIndex];

  const CDataConnection *localPortConnection = paIsSD ? getCommFB()->getDIConnection(localPortNameId) : getCommFB()->getDOConnection(localPortNameId);
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

  CIEC_ANY *remoteType = 0;
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
    DEVLOG_ERROR("[OPC UA LAYER]: Invalid  type %d in FB %s at connection %s\n", remoteType, getCommFB()->getInstanceName(),
      CStringDictionary::getInstance().get(localPortNameId));
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

  for(CSinglyLinkedList<CConnectionPoint>::Iterator it = paPortConnection.getDestinationList().begin(); it != paPortConnection.getDestinationList().end();
      ++it) {
    if(paPortConnection.getDestinationList().begin() == it) { //first one
      if(!getRemoteAny(paResult, *it, false)) {
        return false;
      }
    } else {
      CIEC_ANY* newRemoteType;
      if(!getRemoteAny(&newRemoteType, *it, false)) {
        return false;
      } else {
        if(newRemoteType->getDataTypeID() != (*paResult)->getDataTypeID()) {
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
