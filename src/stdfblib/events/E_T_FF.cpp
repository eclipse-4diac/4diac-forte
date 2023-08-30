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

const CStringDictionary::TStringId FORTE_E_T_FF::scmDataOutputNames[] = {g_nStringIdQ};
const CStringDictionary::TStringId FORTE_E_T_FF::scmDataOutputTypeIds[] = {g_nStringIdBOOL};
const TForteInt16 FORTE_E_T_FF::scmEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_T_FF::scmEventInputNames[] = {g_nStringIdCLK};
const TDataIOID FORTE_E_T_FF::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_T_FF::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_T_FF::scmEventOutputNames[] = {g_nStringIdEO};
const SFBInterfaceSpec FORTE_E_T_FF::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  0, nullptr, nullptr,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_E_T_FF::FORTE_E_T_FF(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CBasicFB(paSrcRes, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_Q(CIEC_BOOL(0)),
    var_conn_Q(var_Q),
    conn_EO(this, 0),
    conn_Q(this, 0, &var_conn_Q) {
}

void FORTE_E_T_FF::alg_TOGGLE(void) {
  
  var_Q = func_NOT<CIEC_BOOL>(var_Q);
}


void FORTE_E_T_FF::executeEvent(TEventID paEIID){
  do {
    switch(mECCState) {
      case scmStateSTART:
        if(scmEventCLKID == paEIID) enterStateSET();
        else return; //no transition cleared
        break;
      case scmStateSET:
        if(1) enterStateSTART();
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

void FORTE_E_T_FF::readInputData(TEventID) {
}

void FORTE_E_T_FF::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventEOID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_Q, conn_Q);
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
  mECCState = scmStateSTART;
}

void FORTE_E_T_FF::enterStateSET(void) {
  mECCState = scmStateSET;
  alg_TOGGLE();
  sendOutputEvent(scmEventEOID);
}



