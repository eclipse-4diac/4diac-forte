/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "E_T_FF.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_T_FF_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_T_FF, g_nStringIdE_T_FF)

const CStringDictionary::TStringId FORTE_E_T_FF::scm_anDataOutputNames[] = {g_nStringIdQ};
const CStringDictionary::TStringId FORTE_E_T_FF::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};
const TForteInt16 FORTE_E_T_FF::scm_anEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_T_FF::scm_anEventInputNames[] = {g_nStringIdCLK};
const TDataIOID FORTE_E_T_FF::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_E_T_FF::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_T_FF::scm_anEventOutputNames[] = {g_nStringIdEO};
const SFBInterfaceSpec FORTE_E_T_FF::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, nullptr, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  0, nullptr, nullptr,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

FORTE_E_T_FF::FORTE_E_T_FF(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_Q(CIEC_BOOL(0)),
    var_conn_Q(var_Q),
    conn_EO(this, 0),
    conn_Q(this, 0, &var_conn_Q) {
}

void FORTE_E_T_FF::alg_TOGGLE(void) {
  
  var_Q = func_NOT<CIEC_BOOL>(var_Q);
}


void FORTE_E_T_FF::executeEvent(int pa_nEIID){
  do {
    switch(m_nECCState) {
      case scm_nStateSTART:
        if(scm_nEventCLKID == pa_nEIID) enterStateSET();
        else return; //no transition cleared
        break;
      case scm_nStateSET:
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

void FORTE_E_T_FF::readInputData(size_t pa_nEIID) {
}

void FORTE_E_T_FF::writeOutputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventEOID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      writeData(0, &var_Q, &conn_Q);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_T_FF::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_E_T_FF::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
  }
  return nullptr;
}

CEventConnection *FORTE_E_T_FF::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_T_FF::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_E_T_FF::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_T_FF::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_E_T_FF::enterStateSTART(void) {
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_T_FF::enterStateSET(void) {
  m_nECCState = scm_nStateSET;
  alg_TOGGLE();
  sendOutputEvent(scm_nEventEOID);
}



