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

DEFINE_FIRMWARE_FB(FORTE_E_T_FF, g_nStringIdE_T_FF)

const CStringDictionary::TStringId FORTE_E_T_FF::scm_anDataOutputNames[] = {g_nStringIdQ};

const CStringDictionary::TStringId FORTE_E_T_FF::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_E_T_FF::scm_anEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_T_FF::scm_anEventInputNames[] = {g_nStringIdCLK};

const TDataIOID FORTE_E_T_FF::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_E_T_FF::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_T_FF::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec FORTE_E_T_FF::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  0,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  0,  0, 0, 
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void FORTE_E_T_FF::alg_TOGGLE(void){
Q() = !Q();
}


void FORTE_E_T_FF::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_T_FF::enterStateSET(void){
  m_nECCState = scm_nStateSET;
  alg_TOGGLE();
  sendOutputEvent( scm_nEventEOID);
}

void FORTE_E_T_FF::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if(scm_nEventCLKID == pa_nEIID)
          enterStateSET();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateSET:
        if((1))
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      default:
      DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 1.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; //0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}


