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

#include "forte/iec61499/events/E_SPLIT_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
namespace forte::iec61499::events {
  namespace {
    const auto cEventInputNames = std::array{"EI"_STRID};
    const auto cEventOutputNames = std::array{"EO1"_STRID, "EO2"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = {},
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_SPLIT, "iec61499::events::E_SPLIT"_STRID)

  FORTE_E_SPLIT::FORTE_E_SPLIT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      conn_EO1(*this, 0),
      conn_EO2(*this, 1) {
  }

  void FORTE_E_SPLIT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if (scmEventEIID == paEIID)
            enterStateState(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateState:
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

  void FORTE_E_SPLIT::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_E_SPLIT::enterStateState(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateState;
    sendOutputEvent(scmEventEO1ID, paECET);
    sendOutputEvent(scmEventEO2ID, paECET);
  }

  void FORTE_E_SPLIT::readInputData(TEventID) {
    // nothing to do
  }

  void FORTE_E_SPLIT::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_SPLIT::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_SPLIT::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_E_SPLIT::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO1;
      case 1: return &conn_EO2;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_SPLIT::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_SPLIT::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_SPLIT::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_E_SPLIT::setInitialValues() {
    CBasicFB::setInitialValues();
    // no variables so nothing to do
  }
} // namespace forte::iec61499::events
