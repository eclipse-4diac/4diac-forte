/*******************************************************************************
 * Copyright (c) 2015, 2025 Florian Froschermeier <florian.froschermeier@tum.de>,
 *               fortiss GmbH, Primetals Technologies Austria GmbH
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
 *    Markus Meingast:
 *      - add support for Object Structs
 *******************************************************************************/

#include "opcua_layer.h"
#include "opcua_helper.h"
#include "opcua_local_handler.h"
#include "forte/cominfra/basecommfb.h"
#include "forte/util/devlog.h"
#include <cstddef>
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_struct.h"
#include "forte/util/criticalregion.h"
#include "opcua_action_info.h"
#include "opcua_remote_handler.h"
#include "opcua_objectstruct_helper.h"
#include "forte/cominfra/comlayersmanager.h"
#include "forte/arch/forte_architecture_time.h"
#include "forte/util/mainparam_utils.h"
#include "opcua_defaults.h"

using namespace forte::literals;

namespace forte::com_infra::opc_ua {
  namespace {
    constexpr char smSeparator = '.'; // separator for the full FB path in log messages
    [[maybe_unused]] const ComLayerManager::EntryImpl<COPC_UA_Layer> entry("opc_ua"_STRID);

    class OpcuaClientInitTimeoutOption final
        : public util::CommandLineParser::IntOptionImpl<
              TForteUInt32,
              "oi",
              "opc_ua-client-init-timeout",
              "<ms>",
              "Set the time a remote OPC UA client FB waits for its first initialization, 0 waits forever"> {
      public:
        bool setOption(const TForteUInt32 paArgument) override {
          mArgument = paArgument;
          return true;
        }

        TForteUInt32 mArgument = FORTE_COM_OPC_UA_CLIENT_INIT_TIMEOUT;
    };

    OpcuaClientInitTimeoutOption gOpcuaClientInitTimeout;
  } // namespace

  COPC_UA_Layer::COPC_UA_Layer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
      CComLayer(paUpperLayer, paComFB),
      mInterruptResp(e_Nothing),
      mHandler(nullptr),
      mDataAlreadyPresent(false),
      mIsObjectNodeStruct(false) {
  }

  COPC_UA_Layer::~COPC_UA_Layer() = default;

  EComResponse COPC_UA_Layer::openConnection(char *paLayerParameter) {
    EComResponse response = e_InitTerminated;
    mActionInfo = CActionInfo::getActionInfoFromParams(paLayerParameter, *this);
    if (mActionInfo) {
      mHandler = (mActionInfo->isRemote())
                     ? static_cast<COPC_UA_HandlerAbstract *>(&getExtEvHandler<COPC_UA_Remote_Handler>())
                     : static_cast<COPC_UA_HandlerAbstract *>(&getExtEvHandler<COPC_UA_Local_Handler>());
      if (checkTypesFromInterface()) {
        if (mActionInfo->isRemote()) {
          // set before the action is handed to the handler, which may initialize it at once in another thread
          util::CCriticalRegion dataReadRegion(mDataAlreadyPresentMutex);
          mRemoteInitState = ERemoteInitState::ePending;
          mRemoteInitDeadline = (0 != gOpcuaClientInitTimeout.mArgument)
                                    ? arch::getNanoSecondsMonotonic() +
                                          static_cast<uint_fast64_t>(gOpcuaClientInitTimeout.mArgument) * 1000000
                                    : 0;
        }
        if (UA_STATUSCODE_GOOD == mHandler->initializeAction(*mActionInfo)) {
          util::CCriticalRegion criticalRegion(mRDBufferMutex);
          // a remote action is only queued here, INITO is sent when it is initialized
          response = mActionInfo->isRemote() ? e_Nothing : e_InitOk;
          for (size_t i = 0; i < getCommFB()->getNumRD(); ++i) {
            mRDBuffer.emplace_back(getCommFB()->getRDs()[i]->clone(nullptr));
          }
        } else {
          util::CCriticalRegion dataReadRegion(mDataAlreadyPresentMutex);
          mRemoteInitState = ERemoteInitState::eNone;
        }
      } else {
        bool isPublisher;
        if (getCommFB()->getComServiceType() == e_Publisher) {
          isPublisher = true;
        } else if (getCommFB()->getComServiceType() == e_Subscriber) {
          isPublisher = false;
        } else {
          return response;
        }
        CActionInfo::UA_ActionType action = mActionInfo->getAction();
        mStructObjectHelper = std::make_unique<COPC_UA_ObjectStruct_Helper>(*this, mHandler);
        if (COPC_UA_ObjectStruct_Helper::isStructType(*this, isPublisher) &&
            (CActionInfo::eWrite == action || CActionInfo::eRead == action)) {
          mIsObjectNodeStruct = true;
          CIEC_ANY **apoDataPorts = isPublisher ? getCommFB()->getSDs() : getCommFB()->getRDs();
          CIEC_STRUCT &structType = static_cast<CIEC_STRUCT &>(apoDataPorts[0]->unwrap());
          if (!mStructObjectHelper->checkStructTypeConnection(*mActionInfo, structType)) {
            return response;
          }
          response = mStructObjectHelper->createObjectNode(*mActionInfo, structType);
          if (!isPublisher && (response == e_InitOk)) {
            util::CCriticalRegion criticalRegion(mRDBufferMutex);
            mRDBuffer = mStructObjectHelper->initializeRDBuffer(structType);
          }
        }
      }
    }
    return response;
  }

  void COPC_UA_Layer::closeConnection() {
    if (mHandler) {
      util::CCriticalRegion criticalRegion(mRDBufferMutex);
      mHandler->uninitializeAction(*mActionInfo);
      mActionInfo.reset();

      if (mIsObjectNodeStruct) {
        mStructObjectHelper->uninitializeStruct();
      }
      mHandler = nullptr;
      if (mIsObjectNodeStruct) {
        COPC_UA_ObjectStruct_Helper::deleteRDBufferEntries(*getCommFB(), mRDBuffer);
      } else {
        mRDBuffer.clear();
      }
    }
  }

  EComResponse COPC_UA_Layer::recvData(const void *paData, unsigned int) {
    mInterruptResp = e_ProcessDataOk;
    auto handleRecv = static_cast<const COPC_UA_Helper::UA_RecvVariable_handle *>(paData);

    if (!handleRecv->mFailed) {
      if (handleRecv->mData.size() != 0) {
        if (handleRecv->mData.size() + handleRecv->mOffset <= getCommFB()->getNumRD()) {
          util::CCriticalRegion criticalRegion(mRDBufferMutex);
          for (size_t i = 0; i < handleRecv->mData.size(); i++) {
            long long bufferIndex = mIsObjectNodeStruct
                                        ? mStructObjectHelper->getRDBufferIndexFromNodeId(handleRecv->mNodeId)
                                        : handleRecv->mOffset + i;
            if (bufferIndex == -1) {
              DEVLOG_ERROR("[OPC UA LAYER]: Received Node ID %d does not match with any registered Node ID for FB %s\n",
                           handleRecv->mNodeId,
                           getCommFB()->getFullQualifiedApplicationInstanceName(smSeparator).c_str());
              mInterruptResp = e_ProcessDataRecvFaild;
              break;
            }
            if (UA_Variant_isScalar(handleRecv->mData[i]) && handleRecv->mData[i]->data &&
                handleRecv->mData[i]->type == COPC_UA_Helper::getOPCUATypeFromAny(*mRDBuffer[bufferIndex])) {
              COPC_UA_Helper::convertFromOPCUAType(handleRecv->mData[i]->data, *mRDBuffer[bufferIndex]);
            } else {
              DEVLOG_ERROR("[OPC UA LAYER]: RD_%d of FB %s has no data, is not a scalar or there is a type mismatch\n",
                           bufferIndex, getCommFB()->getFullQualifiedApplicationInstanceName(smSeparator).c_str());
              mInterruptResp = e_ProcessDataRecvFaild;
              break;
            }
          }
        } else {
          DEVLOG_ERROR("[OPC UA LAYER]: Receiving data for FB %s failed because the response size is %zu with an "
                       "offset of %zu but the FB has %zu RDs\n",
                       getCommFB()->getFullQualifiedApplicationInstanceName(smSeparator).c_str(),
                       handleRecv->mData.size(), handleRecv->mOffset, getCommFB()->getNumRD());
          mInterruptResp = e_ProcessDataRecvFaild;
        }
      } else { // no data received. When remote writing this will happen
        mInterruptResp = e_ProcessDataOk;
      }
    } else { // this fail is logged in the handler
      mInterruptResp = e_ProcessDataRecvFaild;
    }

    // won't trigger another external event on subscription if data is already present or INITO is not sent yet
    return markDataPresent(mInterruptResp) ? mInterruptResp : e_Nothing;
  }

  EComResponse COPC_UA_Layer::sendData(void *, unsigned int) {
    if (mIsObjectNodeStruct) {
      return mStructObjectHelper->executeStructAction();
    }
    return (UA_STATUSCODE_GOOD == mHandler->executeAction(*mActionInfo) ? e_ProcessDataOk : e_ProcessDataDataTypeError);
  }

  EComResponse COPC_UA_Layer::processInterrupt() {
    EComResponse initResp;
    bool dataPresent;
    EComResponse dataResp = mInterruptResp;
    {
      util::CCriticalRegion dataReadRegion(mDataAlreadyPresentMutex);
      initResp = mInitResp;
      mInitResp = e_Nothing;
      dataPresent = mDataAlreadyPresent;
      if (e_Nothing == initResp && e_Nothing != mFailedRespBeforeInit) {
        // a receive that failed before INITO is reported, even if later data was fine
        dataResp = mFailedRespBeforeInit;
        mFailedRespBeforeInit = e_Nothing;
      }
    }
    if (e_Nothing != initResp) {
      if (e_InitOk == initResp && dataPresent) {
        // data received before INITO is reported with the next external event
        getCommFB()->interruptCommFB(this);
        triggerNewEvent();
      }
      return initResp;
    }

    util::CCriticalRegion criticalRegion(mRDBufferMutex);
    if (mIsObjectNodeStruct) {
      CIEC_STRUCT &structType = static_cast<CIEC_STRUCT &>(getCommFB()->getRDs()[0]->unwrap());
      COPC_UA_ObjectStruct_Helper::setMemberValues(structType, mRDBuffer);
    } else {
      for (size_t i = 0; i < getCommFB()->getNumRD(); ++i) {
        getCommFB()->getRDs()[i]->setValue(*mRDBuffer[i]);
      }
    }
    setDataAlreadyPresentRead(false);
    return dataResp;
  }

  void COPC_UA_Layer::triggerNewEvent() {
    mHandler->triggerNewEvent(*this->getCommFB());
  }

  void COPC_UA_Layer::remoteInitializationFinished(bool paSuccess) {
    {
      util::CCriticalRegion dataReadRegion(mDataAlreadyPresentMutex);
      if (ERemoteInitState::ePending != mRemoteInitState) {
        return;
      }
      mRemoteInitState = paSuccess ? ERemoteInitState::eNone : ERemoteInitState::eFailed;
      mInitResp = paSuccess ? e_InitOk : e_InitTerminated;
    }
    if (!paSuccess) {
      DEVLOG_ERROR("[OPC UA LAYER]: The remote action of FB %s was not initialized within %u ms\n",
                   getCommFB()->getFullQualifiedApplicationInstanceName(smSeparator).c_str(),
                   static_cast<unsigned int>(gOpcuaClientInitTimeout.mArgument));
    }
    getCommFB()->interruptCommFB(this);
    triggerNewEvent();
  }

  bool COPC_UA_Layer::isRemoteInitializationTimedOut(uint_fast64_t paNow) {
    util::CCriticalRegion dataReadRegion(mDataAlreadyPresentMutex);
    return ERemoteInitState::ePending == mRemoteInitState && 0 != mRemoteInitDeadline && paNow >= mRemoteInitDeadline;
  }

  bool COPC_UA_Layer::hasRemoteInitializationFailed() {
    util::CCriticalRegion dataReadRegion(mDataAlreadyPresentMutex);
    return ERemoteInitState::eFailed == mRemoteInitState;
  }

  bool COPC_UA_Layer::checkTypesFromInterface() const {
    for (size_t i = 0; i < getCommFB()->getNumSD(); i++) {
      if (!checkPortType(i + 2, true)) {
        return false;
      }
    }
    for (size_t i = 0; i < getCommFB()->getNumRD(); i++) {
      if (!checkPortType(i + 2, false)) {
        return false;
      }
    }
    return true;
  }

  bool COPC_UA_Layer::checkPortType(size_t paPortIndex, bool paIsSD) const {
    CIEC_ANY &pin = (paIsSD) ? getCommFB()->getDI(paPortIndex)->unwrap() : getCommFB()->getDO(paPortIndex)->unwrap();

    if (!COPC_UA_Helper::getOPCUATypeFromAny(pin)) {
      if (!COPC_UA_ObjectStruct_Helper::isStructType(*this, paIsSD)) {
        DEVLOG_ERROR("[OPC UA LAYER]: Invalid  type %s in FB %s at pin %s\n", pin.getTypeNameID().data(),
                     getCommFB()->getFullQualifiedApplicationInstanceName(smSeparator).c_str(),
                     getLocalPortNameId(paPortIndex, paIsSD).data());
      }
      return false;
    }
    return true;
  }

  bool COPC_UA_Layer::markDataPresent(EComResponse paResp) {
    util::CCriticalRegion dataReadRegion(mDataAlreadyPresentMutex);
    if (ERemoteInitState::ePending == mRemoteInitState || e_Nothing != mInitResp) {
      // INITO is not sent yet: the data, and a failed receive, are reported with the first external event after it
      if (e_ProcessDataOk != paResp) {
        mFailedRespBeforeInit = paResp;
      }
      mDataAlreadyPresent = true;
      return false;
    }
    if (e_ProcessDataOk != paResp) {
      return true; // a failed receive is always reported
    }
    if (mDataAlreadyPresent) {
      return false;
    }
    mDataAlreadyPresent = true;
    return true;
  }

  void COPC_UA_Layer::setDataAlreadyPresentRead(bool paDataRead) {
    util::CCriticalRegion dataReadRegion(mDataAlreadyPresentMutex);
    mDataAlreadyPresent = paDataRead;
  }

  StringId COPC_UA_Layer::getLocalPortNameId(size_t paPortIndex, bool paIsSD) const {
    const SFBInterfaceSpec &localInterfaceSpec(getCommFB()->getFBInterfaceSpec());
    return paIsSD ? localInterfaceSpec.mDINames[paPortIndex] : localInterfaceSpec.mDONames[paPortIndex];
  }
} // namespace forte::com_infra::opc_ua
