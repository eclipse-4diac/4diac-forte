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

#include "forte/iec61499/events/E_SWITCH_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  namespace {
    const auto cDataInputNames = std::array{"G"_STRID};
    const auto cEventInputNames = std::array{"EI"_STRID};
    const auto cEventOutputNames = std::array{"EO0"_STRID, "EO1"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_SWITCH, "iec61499::events::E_SWITCH"_STRID)

  FORTE_E_SWITCH::FORTE_E_SWITCH(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      conn_EO0(*this, 0),
      conn_EO1(*this, 1),
      conn_G(nullptr) {
  }

  void FORTE_E_SWITCH::setInitialValues() {
    CBasicFB::setInitialValues();
    var_G = 0_BOOL;
  }

  void FORTE_E_SWITCH::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if ((scmEventEIID == paEIID) && (func_NOT<CIEC_BOOL>(var_G)))
            enterStateG0(paECET);
          else if ((scmEventEIID == paEIID) && (var_G))
            enterStateG1(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateG0:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateG1:
          if (1)
            enterStateSTART(paECET);
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

  void FORTE_E_SWITCH::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_E_SWITCH::enterStateG0(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateG0;
    sendOutputEvent(scmEventEO0ID, paECET);
  }

  void FORTE_E_SWITCH::enterStateG1(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateG1;
    sendOutputEvent(scmEventEO1ID, paECET);
  }

  void FORTE_E_SWITCH::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEIID: {
        readData(0, var_G, conn_G);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_SWITCH::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_SWITCH::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_G;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_SWITCH::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_E_SWITCH::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO0;
      case 1: return &conn_EO1;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_SWITCH::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_G;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_SWITCH::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_SWITCH::getVarInternal(size_t) {
    return nullptr;
  }

} // namespace forte::iec61499::events
