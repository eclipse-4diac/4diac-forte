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

#include "E_SPLIT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SPLIT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_SPLIT, g_nStringIdE_SPLIT)

const TForteInt16 FORTE_E_SPLIT::scm_anEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_SPLIT::scm_anEventInputNames[] = {g_nStringIdEI};
const TForteInt16 FORTE_E_SPLIT::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_E_SPLIT::scm_anEventOutputNames[] = {g_nStringIdEO1, g_nStringIdEO2};
const SFBInterfaceSpec FORTE_E_SPLIT::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, nullptr, scm_anEIWithIndexes,
  2, scm_anEventOutputNames, nullptr, scm_anEOWithIndexes,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr
};

FORTE_E_SPLIT::FORTE_E_SPLIT(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    conn_EO1(this, 0),
    conn_EO2(this, 1) {
}



void FORTE_E_SPLIT::executeEvent(int pa_nEIID){
  do {
    switch(m_nECCState) {
      case scm_nStateSTART:
        if(scm_nEventEIID == pa_nEIID) enterStateState();
        else return; //no transition cleared
        break;
      case scm_nStateState:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 2.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; // 0 is always the initial state
        return;
    }
    pa_nEIID = cg_nInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_SPLIT::readInputData(size_t pa_nEIID) {
}

void FORTE_E_SPLIT::writeOutputData(size_t pa_nEIID) {
}

CIEC_ANY *FORTE_E_SPLIT::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_E_SPLIT::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_SPLIT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
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


void FORTE_E_SPLIT::enterStateSTART(void) {
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_SPLIT::enterStateState(void) {
  m_nECCState = scm_nStateState;
  sendOutputEvent(scm_nEventEO1ID);
  sendOutputEvent(scm_nEventEO2ID);
}



