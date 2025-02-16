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
 *** Name: E_SPLIT3
 *** Description: Split an event
 *** Version:
 ***     1.0: 2017/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 ***     1.0: 2025-02-12/Franz Höpfinger - HR Agrartechnik GmbH - copy from E_SPLIT and make a E_SPLIT3 in the same way
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *************************************************************************/

#include "E_SPLIT3_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SPLIT3_fbt_gen.cpp"
#endif

#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_SPLIT3, g_nStringIdE_SPLIT3)

const TForteInt16 FORTE_E_SPLIT3::scmEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_SPLIT3::scmEventInputNames[] = {g_nStringIdEI};
const TForteInt16 FORTE_E_SPLIT3::scmEOWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_E_SPLIT3::scmEventOutputNames[] = {g_nStringIdEO1, g_nStringIdEO2, g_nStringIdEO3};
const SFBInterfaceSpec FORTE_E_SPLIT3::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, nullptr, scmEIWithIndexes,
  3, scmEventOutputNames, nullptr, nullptr, scmEOWithIndexes,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

FORTE_E_SPLIT3::FORTE_E_SPLIT3(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CBasicFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    conn_EO1(this, 0),
    conn_EO2(this, 1),
    conn_EO3(this, 2) {
}

void FORTE_E_SPLIT3::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  do {
    switch(mECCState) {
      case scmStateSTART:
        if(scmEventEIID == paEIID) enterStateState(paECET);
        else return; //no transition cleared
        break;
      case scmStateState:
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

void FORTE_E_SPLIT3::enterStateSTART(CEventChainExecutionThread *const) {
  mECCState = scmStateSTART;
}

void FORTE_E_SPLIT3::enterStateState(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateState;
  sendOutputEvent(scmEventEO1ID, paECET);
  sendOutputEvent(scmEventEO2ID, paECET);
  sendOutputEvent(scmEventEO3ID, paECET);
}

void FORTE_E_SPLIT3::readInputData(TEventID) {
  // nothing to do
}

void FORTE_E_SPLIT3::writeOutputData(TEventID) {
  // nothing to do
}

CIEC_ANY *FORTE_E_SPLIT3::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_E_SPLIT3::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_SPLIT3::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO1;
    case 1: return &conn_EO2;
    case 2: return &conn_EO3;
  }
  return nullptr;
}

CDataConnection **FORTE_E_SPLIT3::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_E_SPLIT3::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_E_SPLIT3::getVarInternal(size_t) {
  return nullptr;
}
