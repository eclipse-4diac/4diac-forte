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

#include "E_SWITCH.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SWITCH_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_SWITCH, g_nStringIdE_SWITCH)

const CStringDictionary::TStringId FORTE_E_SWITCH::scm_anDataInputNames[] = {g_nStringIdG};
const CStringDictionary::TStringId FORTE_E_SWITCH::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};
const TDataIOID FORTE_E_SWITCH::scm_anEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_SWITCH::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_SWITCH::scm_anEventInputNames[] = {g_nStringIdEI};
const TForteInt16 FORTE_E_SWITCH::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_E_SWITCH::scm_anEventOutputNames[] = {g_nStringIdEO0, g_nStringIdEO1};
const SFBInterfaceSpec FORTE_E_SWITCH::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  2, scm_anEventOutputNames, nullptr, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  0, nullptr, nullptr,
  0, nullptr
};

FORTE_E_SWITCH::FORTE_E_SWITCH(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_G(CIEC_BOOL(0)),
    conn_EO0(this, 0),
    conn_EO1(this, 1),
    conn_G(nullptr) {
}



void FORTE_E_SWITCH::executeEvent(TEventID pa_nEIID){
  do {
    switch(m_nECCState) {
      case scm_nStateSTART:
        if((scm_nEventEIID == pa_nEIID) && (func_NOT<CIEC_BOOL>(var_G))) enterStateG0();
        else
        if((scm_nEventEIID == pa_nEIID) && (var_G)) enterStateG1();
        else return; //no transition cleared
        break;
      case scm_nStateG0:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      case scm_nStateG1:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; // 0 is always the initial state
        return;
    }
    pa_nEIID = cg_nInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_SWITCH::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventEIID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_G, conn_G);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_SWITCH::writeOutputData(TEventID) {
}

CIEC_ANY *FORTE_E_SWITCH::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_G;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_SWITCH::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_SWITCH::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO0;
    case 1: return &conn_EO1;
  }
  return nullptr;
}

CDataConnection **FORTE_E_SWITCH::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_G;
  }
  return nullptr;
}

CDataConnection *FORTE_E_SWITCH::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_E_SWITCH::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_E_SWITCH::enterStateSTART(void) {
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_SWITCH::enterStateG0(void) {
  m_nECCState = scm_nStateG0;
  sendOutputEvent(scm_nEventEO0ID);
}

void FORTE_E_SWITCH::enterStateG1(void) {
  m_nECCState = scm_nStateG1;
  sendOutputEvent(scm_nEventEO1ID);
}



