/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/iec61499/events/E_T_FF_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
namespace forte::iec61499::events {
  namespace {
    const auto cDataOutputNames = std::array{"Q"_STRID};
    const auto cEventInputNames = std::array{"CLK"_STRID};
    const auto cEventOutputNames = std::array{"EO"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = {},
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_T_FF, "iec61499::events::E_T_FF"_STRID)

  FORTE_E_T_FF::FORTE_E_T_FF(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      conn_EO(*this, 0),
      conn_Q(*this, 0, var_Q) {
  }

  void FORTE_E_T_FF::setInitialValues() {
    CBasicFB::setInitialValues();
    var_Q = 0_BOOL;
  }

  void FORTE_E_T_FF::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if (scmEventCLKID == paEIID)
            enterStateSET(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateSET:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        default:
          DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 2.",
                       mECCState.operator TForteUInt16());
          mECCState = 0; // 0 is always the initial state
          return;
      }
      paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
    } while (true);
  }

  void FORTE_E_T_FF::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_E_T_FF::enterStateSET(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateSET;
    alg_TOGGLE();
    sendOutputEvent(scmEventEOID, paECET);
  }

  void FORTE_E_T_FF::readInputData(TEventID) {
    // nothing to do
  }

  void FORTE_E_T_FF::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_Q, conn_Q);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_T_FF::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_T_FF::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Q;
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_T_FF::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_T_FF::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_T_FF::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_T_FF::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_E_T_FF::alg_TOGGLE(void) {

#line 2 "E_T_FF.fbt"
    var_Q = func_NOT<CIEC_BOOL>(var_Q);
  }
} // namespace forte::iec61499::events
