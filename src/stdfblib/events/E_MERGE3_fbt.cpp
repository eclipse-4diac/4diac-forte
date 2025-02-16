/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_MERGE3
 *** Description: Merge (OR) of three input events
 *** Version:
 ***     1.0: 2017/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 ***     1.0: 2025-02-12/Franz Höpfinger - HR Agrartechnik GmbH - copy from E_MERGE and make a E_MERGE3 in the same way
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *************************************************************************/

#include "E_MERGE3_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_MERGE3_fbt_gen.cpp"
#endif

#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_MERGE3, g_nStringIdE_MERGE3)

const TForteInt16 FORTE_E_MERGE3::scmEIWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_E_MERGE3::scmEventInputNames[] = {g_nStringIdEI1, g_nStringIdEI2, g_nStringIdEI3};
const TForteInt16 FORTE_E_MERGE3::scmEOWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_MERGE3::scmEventOutputNames[] = {g_nStringIdEO};
const SFBInterfaceSpec FORTE_E_MERGE3::scmFBInterfaceSpec = {
  3, scmEventInputNames, nullptr, nullptr, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, nullptr, scmEOWithIndexes,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

FORTE_E_MERGE3::FORTE_E_MERGE3(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CBasicFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    conn_EO(this, 0) {
}

void FORTE_E_MERGE3::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  do {
    switch(mECCState) {
      case scmStateSTART:
        if(scmEventEI1ID == paEIID) enterStateEO(paECET);
        else
        if(scmEventEI2ID == paEIID) enterStateEO(paECET);
        else
        if(scmEventEI3ID == paEIID) enterStateEO(paECET);
        else return; //no transition cleared
        break;
      case scmStateEO:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 2.", mECCState.operator TForteUInt16 ());
        mECCState = 0; // 0 is always the initial state
        return;
    }
    paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_MERGE3::enterStateSTART(CEventChainExecutionThread *const) {
  mECCState = scmStateSTART;
}

void FORTE_E_MERGE3::enterStateEO(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateEO;
  sendOutputEvent(scmEventEOID, paECET);
}

void FORTE_E_MERGE3::readInputData(TEventID) {
  // nothing to do
}

void FORTE_E_MERGE3::writeOutputData(TEventID) {
  // nothing to do
}

CIEC_ANY *FORTE_E_MERGE3::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_E_MERGE3::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_MERGE3::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_MERGE3::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_E_MERGE3::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_E_MERGE3::getVarInternal(size_t) {
  return nullptr;
}
