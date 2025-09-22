/*******************************************************************************
 * Copyright (c) 2005 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Gerhard Ebenhofer, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/iec61499/events/E_SR_fbt.h"

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
    const auto cEventInputNames = std::array{"S"_STRID, "R"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"EO"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};
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
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_SR, "iec61499::events::E_SR"_STRID)

  FORTE_E_SR::FORTE_E_SR(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      conn_EO(*this, 0),
      conn_Q(*this, 0, var_Q) {
  }

  void FORTE_E_SR::setInitialValues() {
    CBasicFB::setInitialValues();
    var_Q = 0_BOOL;
  }

  void FORTE_E_SR::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if (scmEventSID == paEIID)
            enterStateSET(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateSET:
          if (scmEventRID == paEIID)
            enterStateRESET(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateRESET:
          if (scmEventSID == paEIID)
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

  void FORTE_E_SR::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_E_SR::enterStateSET(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateSET;
    alg_SET();
    sendOutputEvent(scmEventEOID, paECET);
  }

  void FORTE_E_SR::enterStateRESET(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateRESET;
    alg_RESET();
    sendOutputEvent(scmEventEOID, paECET);
  }

  void FORTE_E_SR::readInputData(TEventID) {
    // nothing to do
  }

  void FORTE_E_SR::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_Q, conn_Q);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_SR::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_SR::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Q;
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_SR::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_SR::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_SR::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_SR::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_E_SR::alg_SET(void) {

#line 2 "E_SR.fbt"
    var_Q = true_BOOL;
  }

  void FORTE_E_SR::alg_RESET(void) {

#line 6 "E_SR.fbt"
    var_Q = false_BOOL;
  }

} // namespace forte::iec61499::events
