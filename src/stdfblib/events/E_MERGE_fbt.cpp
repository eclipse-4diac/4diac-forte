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

#include "E_MERGE_fbt.h"

USE_STRING_ID(EI1);
USE_STRING_ID(EI2);
USE_STRING_ID(E_MERGE);
USE_STRING_ID(EO);
USE_STRING_ID(Event);

#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_MERGE, STRID(E_MERGE))

namespace {
  const auto cEventInputNames = std::array{STRID(EI1), STRID(EI2)};
  const auto cEventInputTypeIds = std::array{STRID(Event), STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(EO)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = {},
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace
FORTE_E_MERGE::FORTE_E_MERGE(const CStringDictionary::TStringId paInstanceNameId,
                             forte::core::CFBContainer &paContainer) :
    CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, nullptr),
    conn_EO(*this, 0) {
}

void FORTE_E_MERGE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  do {
    switch (mECCState) {
      case scmStateSTART:
        if (scmEventEI1ID == paEIID)
          enterStateEO(paECET);
        else if (scmEventEI2ID == paEIID)
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

void FORTE_E_MERGE::enterStateSTART(CEventChainExecutionThread *const) {
  mECCState = scmStateSTART;
}

void FORTE_E_MERGE::enterStateEO(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateEO;
  sendOutputEvent(scmEventEOID, paECET);
}

void FORTE_E_MERGE::readInputData(TEventID) {
  // nothing to do
}

void FORTE_E_MERGE::writeOutputData(TEventID) {
  // nothing to do
}

CIEC_ANY *FORTE_E_MERGE::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_E_MERGE::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_MERGE::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_MERGE::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_E_MERGE::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_E_MERGE::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_E_MERGE::setInitialValues() {
  CBasicFB::setInitialValues();
  // no variables so nothing to do
}
