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

#include "E_SELECT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SELECT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_SELECT, g_nStringIdE_SELECT)

const CStringDictionary::TStringId FORTE_E_SELECT::scm_anDataInputNames[] = {g_nStringIdG};
const CStringDictionary::TStringId FORTE_E_SELECT::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};
const TDataIOID FORTE_E_SELECT::scm_anEIWith[] = {0, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_E_SELECT::scm_anEIWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_E_SELECT::scm_anEventInputNames[] = {g_nStringIdEI0, g_nStringIdEI1};
const TForteInt16 FORTE_E_SELECT::scm_anEOWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_SELECT::scm_anEventOutputNames[] = {g_nStringIdEO};
const SFBInterfaceSpec FORTE_E_SELECT::scm_stFBInterfaceSpec = {
  2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, nullptr, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  0, nullptr, nullptr,
  0, nullptr
};

FORTE_E_SELECT::FORTE_E_SELECT(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_G(CIEC_BOOL(0)),
    conn_EO(this, 0),
    conn_G(nullptr) {
}



void FORTE_E_SELECT::executeEvent(TEventID pa_nEIID){
  do {
    switch(m_nECCState) {
      case scm_nStateSTART:
        if((scm_nEventEI0ID == pa_nEIID) && (func_NOT<CIEC_BOOL>(var_G))) enterStateEO();
        else
        if((scm_nEventEI1ID == pa_nEIID) && (var_G)) enterStateEO();
        else return; //no transition cleared
        break;
      case scm_nStateEO:
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

void FORTE_E_SELECT::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventEI0ID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_G, conn_G);
      break;
    }
    case scm_nEventEI1ID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_G, conn_G);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_SELECT::writeOutputData(TEventID pa_nEIID) {
}

CIEC_ANY *FORTE_E_SELECT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_G;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_SELECT::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_SELECT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_SELECT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_G;
  }
  return nullptr;
}

CDataConnection *FORTE_E_SELECT::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_E_SELECT::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_E_SELECT::enterStateSTART(void) {
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_SELECT::enterStateEO(void) {
  m_nECCState = scm_nStateEO;
  sendOutputEvent(scm_nEventEOID);
}



