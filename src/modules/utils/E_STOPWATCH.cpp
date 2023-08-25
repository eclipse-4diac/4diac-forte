/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 *               2020 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Ben Schneider - initial API and implementation and/or initial documentation
 *   Alois Zoitl   - Changed to a full basic FB implementation utilizing the new
 *                   NOW_MONOTONIC function
 *******************************************************************************/

#include "E_STOPWATCH.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_STOPWATCH_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_time.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_STOPWATCH, g_nStringIdE_STOPWATCH)


const CStringDictionary::TStringId FORTE_E_STOPWATCH::scmDataOutputNames[] = {g_nStringIdTD};

const CStringDictionary::TStringId FORTE_E_STOPWATCH::scmDataOutputTypeIds[] = {g_nStringIdTIME};

const TForteInt16 FORTE_E_STOPWATCH::scmEIWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_E_STOPWATCH::scmEventInputNames[] = {g_nStringIdSTART, g_nStringIdSTOP};

const TDataIOID FORTE_E_STOPWATCH::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_STOPWATCH::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_STOPWATCH::scmEventOutputNames[] = {g_nStringIdEO};


const SFBInterfaceSpec FORTE_E_STOPWATCH::scmFBInterfaceSpec = {
  2, scmEventInputNames, nullptr, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  0, nullptr, nullptr,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

const CStringDictionary::TStringId FORTE_E_STOPWATCH::scmInternalsNames[] = {g_nStringIdstartTime};
const CStringDictionary::TStringId FORTE_E_STOPWATCH::scmInternalsTypeIds[] = {g_nStringIdTIME};
const SInternalVarsInformation FORTE_E_STOPWATCH::scmInternalVars = {1, scmInternalsNames, scmInternalsTypeIds};


FORTE_E_STOPWATCH::FORTE_E_STOPWATCH(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CBasicFB(paSrcRes, &scmFBInterfaceSpec, paInstanceNameId, &scmInternalVars),
    var_startTime(CIEC_TIME(0)),
    var_TD(CIEC_TIME(0)),
    var_conn_TD(var_TD),
    conn_EO(this, 0),
    conn_TD(this, 0, &var_conn_TD) {
}

void FORTE_E_STOPWATCH::alg_captureStartTime(void) {
  
  var_startTime = func_NOW_MONOTONIC();
}
void FORTE_E_STOPWATCH::alg_calcDiff(void) {
  
  var_TD = func_SUB<CIEC_TIME>(func_NOW_MONOTONIC(), var_startTime);
}


void FORTE_E_STOPWATCH::executeEvent(TEventID paEIID){
  do {
    switch(mECCState) {
      case scmStateSTART:
        if(scmEventSTARTID == paEIID) enterStateMeasure();
        else return; //no transition cleared
        break;
      case scmStateMeasure:
        if(scmEventSTOPID == paEIID) enterStateSTOP();
        else return; //no transition cleared
        break;
      case scmStateSTOP:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.", mECCState.operator TForteUInt16 ());
        mECCState = 0; // 0 is always the initial state
        return;
    }
    paEIID = cg_nInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_STOPWATCH::readInputData(TEventID) {
}

void FORTE_E_STOPWATCH::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventEOID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_TD, conn_TD);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_STOPWATCH::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_E_STOPWATCH::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_TD;
  }
  return nullptr;
}

CEventConnection *FORTE_E_STOPWATCH::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_STOPWATCH::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_E_STOPWATCH::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_TD;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_STOPWATCH::getVarInternal(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_startTime;
  }
  return nullptr;
}


void FORTE_E_STOPWATCH::enterStateSTART(void) {
  mECCState = scmStateSTART;
}

void FORTE_E_STOPWATCH::enterStateMeasure(void) {
  mECCState = scmStateMeasure;
  alg_captureStartTime();
}

void FORTE_E_STOPWATCH::enterStateSTOP(void) {
  mECCState = scmStateSTOP;
  alg_calcDiff();
  sendOutputEvent(scmEventEOID);
}



