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


DEFINE_FIRMWARE_FB(FORTE_E_STOPWATCH, g_nStringIdE_STOPWATCH)


const CStringDictionary::TStringId FORTE_E_STOPWATCH::scm_anDataOutputNames[] = {g_nStringIdTD};

const CStringDictionary::TStringId FORTE_E_STOPWATCH::scm_anDataOutputTypeIds[] = {g_nStringIdTIME};

const TForteInt16 FORTE_E_STOPWATCH::scm_anEIWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_E_STOPWATCH::scm_anEventInputNames[] = {g_nStringIdSTART, g_nStringIdSTOP};

const TDataIOID FORTE_E_STOPWATCH::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_E_STOPWATCH::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_STOPWATCH::scm_anEventOutputNames[] = {g_nStringIdEO};


const SFBInterfaceSpec FORTE_E_STOPWATCH::scm_stFBInterfaceSpec = {
  2, scm_anEventInputNames, nullptr, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  0, nullptr, nullptr,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

const CStringDictionary::TStringId FORTE_E_STOPWATCH::scm_anInternalsNames[] = {g_nStringIdstartTime};
const CStringDictionary::TStringId FORTE_E_STOPWATCH::scm_anInternalsTypeIds[] = {g_nStringIdTIME};
const SInternalVarsInformation FORTE_E_STOPWATCH::scm_stInternalVars = {1, scm_anInternalsNames, scm_anInternalsTypeIds};

void FORTE_E_STOPWATCH::alg_captureStartTime(void) {
  st_startTime() = NOW_MONOTONIC();
}

void FORTE_E_STOPWATCH::alg_calcDiff(void) {
  st_TD() = SUB(NOW_MONOTONIC(), st_startTime());
}


void FORTE_E_STOPWATCH::enterStateSTART(void) {
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_STOPWATCH::enterStateMeasure(void) {
  m_nECCState = scm_nStateMeasure;
  alg_captureStartTime();
}

void FORTE_E_STOPWATCH::enterStateSTOP(void) {
  m_nECCState = scm_nStateSTOP;
  alg_calcDiff();
  sendOutputEvent(scm_nEventEOID);
}


void FORTE_E_STOPWATCH::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do {
    bTransitionCleared = true;
    switch(m_nECCState) {
      case scm_nStateSTART:
        if(scm_nEventSTARTID == pa_nEIID)
          enterStateMeasure();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateMeasure:
        if(scm_nEventSTOPID == pa_nEIID)
          enterStateSTOP();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateSTOP:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; // 0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(bTransitionCleared);
}


