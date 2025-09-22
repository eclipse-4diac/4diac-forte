/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Martin Melik Merkumians, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/iec61499/events/E_D_FF_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  namespace {
    const auto cDataInputNames = std::array{"D"_STRID};
    const auto cDataOutputNames = std::array{"Q"_STRID};
    const auto cEventInputNames = std::array{"CLK"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};
    const auto cEventOutputNames = std::array{"EO"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_D_FF, "iec61499::events::E_D_FF"_STRID)

  FORTE_E_D_FF::FORTE_E_D_FF(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      conn_EO(*this, 0),
      conn_D(nullptr),
      conn_Q(*this, 0, var_Q) {
  }

  void FORTE_E_D_FF::setInitialValues() {
    CBasicFB::setInitialValues();
    var_D = 0_BOOL;
    var_Q = 0_BOOL;
  }

  void FORTE_E_D_FF::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if ((scmEventCLKID == paEIID) && (var_D))
            enterStateSET(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateSET:
          if ((scmEventCLKID == paEIID) && (func_NOT<CIEC_BOOL>(var_D)))
            enterStateRESET(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateRESET:
          if ((scmEventCLKID == paEIID) && (var_D))
            enterStateSET(paECET);
          else
            return; // no transition cleared
          break;
        default:
          DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.",
                       mECCState.operator TForteUInt16());
          mECCState = 0; // 0 is always the initial state
          return;
      }
      paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
    } while (true);
  }

  void FORTE_E_D_FF::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_E_D_FF::enterStateSET(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateSET;
    alg_LATCH();
    sendOutputEvent(scmEventEOID, paECET);
  }

  void FORTE_E_D_FF::enterStateRESET(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateRESET;
    alg_LATCH();
    sendOutputEvent(scmEventEOID, paECET);
  }

  void FORTE_E_D_FF::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCLKID: {
        readData(0, var_D, conn_D);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_D_FF::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_Q, conn_Q);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_D_FF::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_D;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_D_FF::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Q;
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_D_FF::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_D_FF::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_D;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_D_FF::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_D_FF::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_E_D_FF::alg_LATCH(void) {

#line 2 "E_D_FF.fbt"
    var_Q = var_D;
  }

} // namespace forte::iec61499::events
