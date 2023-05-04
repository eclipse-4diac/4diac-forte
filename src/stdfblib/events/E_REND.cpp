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

#include "E_REND.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_REND_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_REND, g_nStringIdE_REND)

const TForteInt16 FORTE_E_REND::scm_anEIWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_E_REND::scm_anEventInputNames[] = {g_nStringIdEI1, g_nStringIdEI2, g_nStringIdR};
const TForteInt16 FORTE_E_REND::scm_anEOWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_REND::scm_anEventOutputNames[] = {g_nStringIdEO};
const SFBInterfaceSpec FORTE_E_REND::scm_stFBInterfaceSpec = {
  3, scm_anEventInputNames, nullptr, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, nullptr, scm_anEOWithIndexes,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr
};

FORTE_E_REND::FORTE_E_REND(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    conn_EO(this, 0) {
}



void FORTE_E_REND::executeEvent(int pa_nEIID){
  do {
    switch(m_nECCState) {
      case scm_nStateSTART:
        if(scm_nEventEI2ID == pa_nEIID) enterStateEI2();
        else
        if(scm_nEventEI1ID == pa_nEIID) enterStateEI1();
        else return; //no transition cleared
        break;
      case scm_nStateEI1:
        if(scm_nEventRID == pa_nEIID) enterStateSTART();
        else
        if(scm_nEventEI2ID == pa_nEIID) enterStateEO();
        else return; //no transition cleared
        break;
      case scm_nStateEO:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      case scm_nStateEI2:
        if(scm_nEventRID == pa_nEIID) enterStateSTART();
        else
        if(scm_nEventEI1ID == pa_nEIID) enterStateEO();
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 4.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; // 0 is always the initial state
        return;
    }
    pa_nEIID = cg_nInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_REND::readInputData(size_t pa_nEIID) {
}

void FORTE_E_REND::writeOutputData(size_t pa_nEIID) {
}

CIEC_ANY *FORTE_E_REND::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_E_REND::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_REND::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_REND::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_E_REND::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_E_REND::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_E_REND::enterStateSTART(void) {
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_REND::enterStateEI1(void) {
  m_nECCState = scm_nStateEI1;
}

void FORTE_E_REND::enterStateEO(void) {
  m_nECCState = scm_nStateEO;
  sendOutputEvent(scm_nEventEOID);
}

void FORTE_E_REND::enterStateEI2(void) {
  m_nECCState = scm_nStateEI2;
}



