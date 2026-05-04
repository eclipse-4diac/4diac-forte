/*******************************************************************************
 * Copyright (c) 2026 Thomas Mayr
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Mayr - Initial implementation.
 *******************************************************************************/

#include "forte/iec61499/events/SoftPWM_fbt.h"

using namespace forte::literals;

namespace forte::iec61499::events {

  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};

    const auto cDataInputNames = std::array{"QI"_STRID, "PeriodeTime"_STRID, "DutyCycle"_STRID};

    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_SoftPWM, "iec61499::events::SoftPWM"_STRID)

  FORTE_SoftPWM::FORTE_SoftPWM(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CEventSourceFB(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QI(false),
      var_PeriodeTime(0),
      var_DutyCycle(0),
      var_QO(false),
      var_STATUS(),
      conn_QI(nullptr),
      conn_PeriodeTime(nullptr),
      conn_DutyCycle(nullptr),
      conn_CNF(*this, scmEventCNFID),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      mRunning(false),
      mHighPhase(false),
      mOnTime(0),
      mOffTime(0),
      mPendingUpdate(false),
      mPendingDutyCycle(0),
      mPendingOnTime(0),
      mPendingOffTime(0) {
  }

  void FORTE_SoftPWM::setInitialValues() {
    var_QI = CIEC_BOOL(false);
    var_PeriodeTime = CIEC_TIME(0);
    var_DutyCycle = CIEC_UINT(0);
    var_QO = CIEC_BOOL(false);
    setStatus("SoftPWM inactive");

    mRunning = false;
    mHighPhase = false;
    mOnTime = CIEC_TIME(0);
    mOffTime = CIEC_TIME(0);

    mPendingUpdate = false;
    mPendingDutyCycle = CIEC_UINT(0);
    mPendingOnTime = CIEC_TIME(0);
    mPendingOffTime = CIEC_TIME(0);
  }

  void FORTE_SoftPWM::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case cgExternalEventID: handleTimerTick(paECET); break;

      case scmEventREQID: {
        readInputData(paEIID);

        if (!var_QI) {
          stopSequence();
          var_QO = CIEC_BOOL(false);
          setStatus("SoftPWM stopped");
          sendOutputEvent(scmEventCNFID, paECET);
          break;
        }

        setEventChainExecutor(paECET);

        // Start PWM immediately if not running
        if (!mRunning) {
          if (!computeTiming(var_PeriodeTime, var_DutyCycle, mOnTime, mOffTime)) {
            stopSequence();
            var_QO = CIEC_BOOL(false);
            sendOutputEvent(scmEventCNFID, paECET);
            break;
          }

          startSequence(paECET);
          break;
        }

        // If already running: store new values as pending update
        if (!computeTiming(var_PeriodeTime, var_DutyCycle, mPendingOnTime, mPendingOffTime)) {
          stopSequence();
          var_QO = CIEC_BOOL(false);
          sendOutputEvent(scmEventCNFID, paECET);
          break;
        }

        mPendingDutyCycle = var_DutyCycle;
        mPendingUpdate = true;

        // Update will be applied at the next period boundary
        sendOutputEvent(scmEventCNFID, paECET);
        break;
      }

      default: break;
    }
  }

  void FORTE_SoftPWM::startSequence(CEventChainExecutionThread *const paECET) {
    getTimer().unregisterTimedFB(this);
    mPendingUpdate = false;

    const auto duty = static_cast<TForteUInt16>(var_DutyCycle);

    if (0 == duty) {
      mRunning = false;
      mHighPhase = false;
      var_QO = CIEC_BOOL(false);
      setStatus("SoftPWM active 0%");
      sendOutputEvent(scmEventCNFID, paECET);
      return;
    }

    if (100 == duty) {
      mRunning = false;
      mHighPhase = true;
      var_QO = CIEC_BOOL(true);
      setStatus("SoftPWM active 100%");
      sendOutputEvent(scmEventCNFID, paECET);
      return;
    }

    mRunning = true;
    mHighPhase = true;
    var_QO = CIEC_BOOL(true);
    setStatus("SoftPWM active");
    sendOutputEvent(scmEventCNFID, paECET);
    scheduleNextInterval();
  }

  void FORTE_SoftPWM::handleTimerTick(CEventChainExecutionThread *const paECET) {
    if (!mRunning) {
      return;
    }

    if (!mHighPhase && mPendingUpdate) {
      applyPendingUpdate();

      if (!mRunning) {
        sendOutputEvent(scmEventCNFID, paECET);
        return;
      }
    }

    mHighPhase = !mHighPhase;
    var_QO = CIEC_BOOL(mHighPhase);
    sendOutputEvent(scmEventCNFID, paECET);
    scheduleNextInterval();
  }

  void FORTE_SoftPWM::scheduleNextInterval() {
    if (!mRunning) {
      return;
    }

    const CIEC_TIME &nextInterval = mHighPhase ? mOnTime : mOffTime;
    getTimer().registerOneShotTimedFB(this, nextInterval);
  }

  bool FORTE_SoftPWM::computeTiming(const CIEC_TIME &paPeriod,
                                    const CIEC_UINT &paDuty,
                                    CIEC_TIME &paOnTime,
                                    CIEC_TIME &paOffTime) {
    const auto duty = static_cast<TForteUInt16>(paDuty);
    if (duty > 100) {
      setStatus("DutyCycle out of range");
      return false;
    }

    if (paPeriod <= CIEC_TIME(0)) {
      setStatus("PeriodeTime must be > T#0ms");
      return false;
    }

    const TForteInt64 periodNs = static_cast<TForteInt64>(paPeriod);
    TForteInt64 onNs = (periodNs * duty) / 100;
    TForteInt64 offNs = periodNs - onNs;

    if (duty > 0 && duty < 100) {
      if (onNs <= 0) {
        onNs = 1;
        offNs = periodNs - 1;
      }

      if (offNs <= 0) {
        offNs = 1;
        onNs = periodNs - 1;
      }
    }

    if (onNs < 0 || offNs < 0) {
      setStatus("Calculated timing invalid");
      return false;
    }

    paOnTime = CIEC_TIME(onNs);
    paOffTime = CIEC_TIME(offNs);
    return true;
  }

  void FORTE_SoftPWM::applyPendingUpdate() {
    if (!mPendingUpdate) {
      return;
    }

    mPendingUpdate = false;

    const auto duty = static_cast<TForteUInt16>(mPendingDutyCycle);

    mOnTime = mPendingOnTime;
    mOffTime = mPendingOffTime;

    if (0 == duty) {
      getTimer().unregisterTimedFB(this);
      mRunning = false;
      mHighPhase = false;
      var_QO = CIEC_BOOL(false);
      setStatus("SoftPWM active 0%");
      return;
    }

    if (100 == duty) {
      getTimer().unregisterTimedFB(this);
      mRunning = false;
      mHighPhase = true;
      var_QO = CIEC_BOOL(true);
      setStatus("SoftPWM active 100%");
      return;
    }

    mRunning = true;
    setStatus("SoftPWM active");
  }

  void FORTE_SoftPWM::stopSequence() {
    getTimer().unregisterTimedFB(this);
    mRunning = false;
    mHighPhase = false;
    mOnTime = CIEC_TIME(0);
    mOffTime = CIEC_TIME(0);

    mPendingUpdate = false;
    mPendingDutyCycle = CIEC_UINT(0);
    mPendingOnTime = CIEC_TIME(0);
    mPendingOffTime = CIEC_TIME(0);
  }

  void FORTE_SoftPWM::readInputData(TEventID paEIID) {
    if (scmEventREQID == paEIID) {
      readData(0, var_QI, conn_QI);
      readData(1, var_PeriodeTime, conn_PeriodeTime);
      readData(2, var_DutyCycle, conn_DutyCycle);
    }
  }

  void FORTE_SoftPWM::writeOutputData(TEventID paEOID) {
    switch (paEOID) {
      case scmEventCNFID: {
        writeData(getFBInterfaceSpec().getNumDIs() + 0, var_QO, conn_QO);
        writeData(getFBInterfaceSpec().getNumDIs() + 1, var_STATUS, conn_STATUS);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_SoftPWM::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_PeriodeTime;
      case 2: return &var_DutyCycle;
      default: return nullptr;
    }
  }

  CIEC_ANY *FORTE_SoftPWM::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
      default: return nullptr;
    }
  }

  CEventConnection *FORTE_SoftPWM::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
      default: return nullptr;
    }
  }

  CDataConnection **FORTE_SoftPWM::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_PeriodeTime;
      case 2: return &conn_DutyCycle;
      default: return nullptr;
    }
  }

  CDataConnection *FORTE_SoftPWM::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
      default: return nullptr;
    }
  }

  CIEC_ANY *FORTE_SoftPWM::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_SoftPWM::setStatus(const char *paStatus) {
    var_STATUS.fromString(paStatus);
  }

} // namespace forte::iec61499::events
