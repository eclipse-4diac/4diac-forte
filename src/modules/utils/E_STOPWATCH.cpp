/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Ben Schneider
 *   - initial API and implementation and/or initial documentation
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
const TForteInt16 FORTE_E_STOPWATCH::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_STOPWATCH::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec FORTE_E_STOPWATCH::scm_stFBInterfaceSpec = {
    2,  scm_anEventInputNames,  0,  scm_anEIWithIndexes,
    1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  0,  0, 0,
    1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
    0, 0
};

void FORTE_E_STOPWATCH::writeElapsedTimeToTD(){

  mCurrentTime = static_cast<int>(chrono::duration_cast<chrono::microseconds>(m_end - m_begin).count());
  TD().setFromMicroSeconds(mCurrentTime);
}

void FORTE_E_STOPWATCH::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_STOPWATCH::enterStateMeasure(void){
  m_nECCState = scm_nStateMeasure;
  m_begin = chrono::high_resolution_clock::now();
}

void FORTE_E_STOPWATCH::enterStateStopped(void){
  m_nECCState = scm_nStateStopped;
  m_end = chrono::high_resolution_clock::now();
  writeElapsedTimeToTD();
  sendOutputEvent( scm_nEventEOID);
}

void FORTE_E_STOPWATCH::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if(scm_nEventSTARTID == pa_nEIID)
          enterStateMeasure();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateMeasure:
        if(scm_nEventSTOPID == pa_nEIID)
          enterStateStopped();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateStopped:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 2.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; //0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}
