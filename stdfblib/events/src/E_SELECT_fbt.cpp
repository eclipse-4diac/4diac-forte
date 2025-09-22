/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/iec61499/events/E_SELECT_fbt.h"

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
    const auto cEventInputNames = std::array{"EI0"_STRID, "EI1"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"EO"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_SELECT, "iec61499::events::E_SELECT"_STRID)

  FORTE_E_SELECT::FORTE_E_SELECT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      conn_EO(*this, 0),
      conn_G(nullptr) {
  }

  void FORTE_E_SELECT::setInitialValues() {
    CBasicFB::setInitialValues();
    var_G = 0_BOOL;
  }

  void FORTE_E_SELECT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if ((scmEventEI0ID == paEIID) && (func_NOT<CIEC_BOOL>(var_G)))
            enterStateEO(paECET);
          else if ((scmEventEI1ID == paEIID) && (var_G))
            enterStateEO(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateEO:
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

  void FORTE_E_SELECT::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_E_SELECT::enterStateEO(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateEO;
    sendOutputEvent(scmEventEOID, paECET);
  }

  void FORTE_E_SELECT::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEI0ID: {
        readData(0, var_G, conn_G);
        break;
      }
      case scmEventEI1ID: {
        readData(0, var_G, conn_G);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_SELECT::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_SELECT::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_G;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_SELECT::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_E_SELECT::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_SELECT::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_G;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_SELECT::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_SELECT::getVarInternal(size_t) {
    return nullptr;
  }

} // namespace forte::iec61499::events
