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

#include "E_D_FF.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_D_FF_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_D_FF, g_nStringIdE_D_FF)

const CStringDictionary::TStringId FORTE_E_D_FF::scmDataInputNames[] = {g_nStringIdD};
const CStringDictionary::TStringId FORTE_E_D_FF::scmDataInputTypeIds[] = {g_nStringIdBOOL};
const CStringDictionary::TStringId FORTE_E_D_FF::scmDataOutputNames[] = {g_nStringIdQ};
const CStringDictionary::TStringId FORTE_E_D_FF::scmDataOutputTypeIds[] = {g_nStringIdBOOL};
const TDataIOID FORTE_E_D_FF::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_D_FF::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_D_FF::scmEventInputNames[] = {g_nStringIdCLK};
const TDataIOID FORTE_E_D_FF::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_D_FF::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_D_FF::scmEventOutputNames[] = {g_nStringIdEO};
const SFBInterfaceSpec FORTE_E_D_FF::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_E_D_FF::FORTE_E_D_FF(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CBasicFB(paSrcRes, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_D(CIEC_BOOL(0)),
    var_Q(CIEC_BOOL(0)),
    var_conn_Q(var_Q),
    conn_EO(this, 0),
    conn_D(nullptr),
    conn_Q(this, 0, &var_conn_Q) {
}

void FORTE_E_D_FF::alg_LATCH(void) {
  
  var_Q = var_D;
}


void FORTE_E_D_FF::executeEvent(TEventID paEIID){
  do {
    switch(mECCState) {
      case scmStateSTART:
        if((scmEventCLKID == paEIID) && (var_D)) enterStateSET();
        else return; //no transition cleared
        break;
      case scmStateSET:
        if((scmEventCLKID == paEIID) && (func_NOT<CIEC_BOOL>(var_D))) enterStateRESET();
        else return; //no transition cleared
        break;
      case scmStateRESET:
        if((scmEventCLKID == paEIID) && (var_D)) enterStateSET();
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.", mECCState.operator TForteUInt16 ());
        mECCState = 0; // 0 is always the initial state
        return;
    }
    paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_D_FF::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCLKID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_D, conn_D);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_D_FF::writeOutputData(TEventID paEIID) {
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

CIEC_ANY *FORTE_E_D_FF::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_D;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_D_FF::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
  }
  return nullptr;
}

CEventConnection *FORTE_E_D_FF::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_D_FF::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_D;
  }
  return nullptr;
}

CDataConnection *FORTE_E_D_FF::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_D_FF::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_E_D_FF::enterStateSTART(void) {
  mECCState = scmStateSTART;
}

void FORTE_E_D_FF::enterStateSET(void) {
  mECCState = scmStateSET;
  alg_LATCH();
  sendOutputEvent(scmEventEOID);
}

void FORTE_E_D_FF::enterStateRESET(void) {
  mECCState = scmStateRESET;
  alg_LATCH();
  sendOutputEvent(scmEventEOID);
}



