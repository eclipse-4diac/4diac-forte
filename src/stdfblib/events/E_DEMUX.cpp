/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Martin Melik Merkumians, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "E_DEMUX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_DEMUX_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "forte_uint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_DEMUX, g_nStringIdE_DEMUX)

const CStringDictionary::TStringId FORTE_E_DEMUX::scm_anDataInputNames[] = {g_nStringIdK};
const CStringDictionary::TStringId FORTE_E_DEMUX::scm_anDataInputTypeIds[] = {g_nStringIdUINT};
const TDataIOID FORTE_E_DEMUX::scm_anEIWith[] = {0, 255};
const TForteInt16 FORTE_E_DEMUX::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_DEMUX::scm_anEventInputNames[] = {g_nStringIdEI};
const TForteInt16 FORTE_E_DEMUX::scm_anEOWithIndexes[] = {-1, -1, -1, -1};
const CStringDictionary::TStringId FORTE_E_DEMUX::scm_anEventOutputNames[] = {g_nStringIdEO0, g_nStringIdEO1, g_nStringIdEO2, g_nStringIdEO3};
const SFBInterfaceSpec FORTE_E_DEMUX::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  4, scm_anEventOutputNames, nullptr, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  0, nullptr, nullptr,
  0, nullptr
};

FORTE_E_DEMUX::FORTE_E_DEMUX(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_K(CIEC_UINT(0)),
    conn_EO0(this, 0),
    conn_EO1(this, 1),
    conn_EO2(this, 2),
    conn_EO3(this, 3),
    conn_K(nullptr) {
}



void FORTE_E_DEMUX::executeEvent(int pa_nEIID){
  do {
    switch(m_nECCState) {
      case scm_nStateSTART:
        if(scm_nEventEIID == pa_nEIID) enterStateState();
        else return; //no transition cleared
        break;
      case scm_nStateState:
        if(func_EQ(var_K, CIEC_UINT(0))) enterStateState_1();
        else
        if(func_EQ(var_K, CIEC_UINT(1))) enterStateState_2();
        else
        if(func_EQ(var_K, CIEC_UINT(2))) enterStateState_3();
        else
        if(func_EQ(var_K, CIEC_UINT(3))) enterStateState_4();
        else
        if(func_GT(var_K, CIEC_UINT(3))) enterStateSTART();
        else return; //no transition cleared
        break;
      case scm_nStateState_1:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      case scm_nStateState_2:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      case scm_nStateState_3:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      case scm_nStateState_4:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 6.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; // 0 is always the initial state
        return;
    }
    pa_nEIID = cg_nInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_DEMUX::readInputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventEIID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      readData(0, &var_K, conn_K);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_DEMUX::writeOutputData(size_t pa_nEIID) {
}

CIEC_ANY *FORTE_E_DEMUX::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_K;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_DEMUX::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_DEMUX::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO0;
    case 1: return &conn_EO1;
    case 2: return &conn_EO2;
    case 3: return &conn_EO3;
  }
  return nullptr;
}

CDataConnection **FORTE_E_DEMUX::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_K;
  }
  return nullptr;
}

CDataConnection *FORTE_E_DEMUX::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_E_DEMUX::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_E_DEMUX::enterStateSTART(void) {
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_DEMUX::enterStateState(void) {
  m_nECCState = scm_nStateState;
}

void FORTE_E_DEMUX::enterStateState_1(void) {
  m_nECCState = scm_nStateState_1;
  sendOutputEvent(scm_nEventEO0ID);
}

void FORTE_E_DEMUX::enterStateState_2(void) {
  m_nECCState = scm_nStateState_2;
  sendOutputEvent(scm_nEventEO1ID);
}

void FORTE_E_DEMUX::enterStateState_3(void) {
  m_nECCState = scm_nStateState_3;
  sendOutputEvent(scm_nEventEO2ID);
}

void FORTE_E_DEMUX::enterStateState_4(void) {
  m_nECCState = scm_nStateState_4;
  sendOutputEvent(scm_nEventEO3ID);
}



