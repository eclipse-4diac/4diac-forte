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

#include "forte/iec61499/events/E_PERMIT_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  namespace {
    const auto cDataInputNames = std::array{"PERMIT"_STRID};
    const auto cEventInputNames = std::array{"EI"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_E_PERMIT, "iec61499::events::E_PERMIT"_STRID)

  FORTE_E_PERMIT::FORTE_E_PERMIT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      conn_EO(*this, 0),
      conn_PERMIT(nullptr) {
  }

  void FORTE_E_PERMIT::setInitialValues() {
    CBasicFB::setInitialValues();
    var_PERMIT = 0_BOOL;
  }

  void FORTE_E_PERMIT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if ((scmEventEIID == paEIID) && (var_PERMIT))
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

  void FORTE_E_PERMIT::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_E_PERMIT::enterStateEO(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateEO;
    sendOutputEvent(scmEventEOID, paECET);
  }

  void FORTE_E_PERMIT::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEIID: {
        readData(0, var_PERMIT, conn_PERMIT);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_PERMIT::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_PERMIT::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_PERMIT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_PERMIT::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_E_PERMIT::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_PERMIT::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_PERMIT;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_PERMIT::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_PERMIT::getVarInternal(size_t) {
    return nullptr;
  }

} // namespace forte::iec61499::events
