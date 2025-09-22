/*******************************************************************************
 * Copyright (c) 2018, 2024 fortiss GmbH, Johannes Kepler University Linz, HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0

 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_STOPWATCH
 *** Description: FB for stopping time between events
 *** Version:
 ***     1.0: 2018-08-29/Ben Schneider - fortiss GmbH - initial API and implementation and/or initial documentation
 ***     1.1: 2020-05-24/alois Zoitl - Johannes Kepler University Linz - Changed to a full basic FB implementation
 utilizing the new NOW_MONOTONIC function
 ***     1.2: 2024-09-19/Franz Höpfinger - HR Agrartechnik GmbH - Added additional Events
 *************************************************************************/

#include "forte/eclipse4diac/utils/timing/E_STOPWATCH_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_time.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::timing {
  namespace {
    const auto cDataOutputNames = std::array{"TD"_STRID};
    const auto cEventInputNames = std::array{"START"_STRID, "ET"_STRID, "STOP"_STRID, "RESET"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID, "Event"_STRID, "Event"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"EO"_STRID, "ETO"_STRID, "RESETO"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID, "Event"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = {},
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };

    const auto cInternalsNames = std::array{"startTime"_STRID};
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_STOPWATCH, "eclipse4diac::utils::timing::E_STOPWATCH"_STRID)

  FORTE_E_STOPWATCH::FORTE_E_STOPWATCH(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cInternalsNames),
      conn_EO(*this, 0),
      conn_ETO(*this, 1),
      conn_RESETO(*this, 2),
      conn_TD(*this, 0, var_TD) {
  }

  void FORTE_E_STOPWATCH::setInitialValues() {
    CBasicFB::setInitialValues();
    var_startTime = 0_TIME;
    var_TD = 0_TIME;
  }

  void FORTE_E_STOPWATCH::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if (scmEventSTARTID == paEIID)
            enterStateMeasure(paECET);
          else if (scmEventRESETID == paEIID)
            enterStateRESET(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateMeasure:
          if (scmEventSTOPID == paEIID)
            enterStateSTOP(paECET);
          else if (scmEventETID == paEIID)
            enterStateTrig(paECET);
          else if (scmEventRESETID == paEIID)
            enterStateRESET(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateSTOP:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateTrig:
          if (scmEventSTOPID == paEIID)
            enterStateSTOP(paECET);
          else if (scmEventETID == paEIID)
            enterStateTrig(paECET);
          else if (scmEventRESETID == paEIID)
            enterStateRESET(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateRESET:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        default:
          DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 5.",
                       mECCState.operator TForteUInt16());
          mECCState = 0; // 0 is always the initial state
          return;
      }
      paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
    } while (true);
  }

  void FORTE_E_STOPWATCH::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_E_STOPWATCH::enterStateMeasure(CEventChainExecutionThread *const) {
    mECCState = scmStateMeasure;
    alg_captureStartTime();
  }

  void FORTE_E_STOPWATCH::enterStateSTOP(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateSTOP;
    alg_calcDiff();
    sendOutputEvent(scmEventEOID, paECET);
  }

  void FORTE_E_STOPWATCH::enterStateTrig(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateTrig;
    alg_calcDiff();
    sendOutputEvent(scmEventETOID, paECET);
  }

  void FORTE_E_STOPWATCH::enterStateRESET(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateRESET;
    alg_reset();
    sendOutputEvent(scmEventRESETOID, paECET);
  }

  void FORTE_E_STOPWATCH::readInputData(TEventID) {
    // nothing to do
  }

  void FORTE_E_STOPWATCH::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_TD, conn_TD);
        break;
      }
      case scmEventETOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_TD, conn_TD);
        break;
      }
      case scmEventRESETOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_TD, conn_TD);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_STOPWATCH::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_STOPWATCH::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_TD;
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_STOPWATCH::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
      case 1: return &conn_ETO;
      case 2: return &conn_RESETO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_STOPWATCH::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_STOPWATCH::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_TD;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_STOPWATCH::getVarInternal(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_startTime;
    }
    return nullptr;
  }

  void FORTE_E_STOPWATCH::alg_captureStartTime(void) {

#line 2 "E_STOPWATCH.fbt"
    var_startTime = func_NOW_MONOTONIC();
  }

  void FORTE_E_STOPWATCH::alg_calcDiff(void) {

#line 6 "E_STOPWATCH.fbt"
    var_TD = func_SUB<CIEC_TIME>(func_NOW_MONOTONIC(), var_startTime);
  }

  void FORTE_E_STOPWATCH::alg_reset(void) {

#line 10 "E_STOPWATCH.fbt"
    var_TD = 0_TIME;
  }
} // namespace forte::eclipse4diac::utils::timing
