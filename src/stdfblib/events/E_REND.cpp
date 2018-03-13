/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_REND.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_REND_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(E_REND, g_nStringIdE_REND)

const CStringDictionary::TStringId E_REND::scm_anEventInputNames[] = {g_nStringIdEI1, g_nStringIdEI2, g_nStringIdR};

const CStringDictionary::TStringId E_REND::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_REND::scm_stFBInterfaceSpec = {
  3,
  scm_anEventInputNames,
  0,
  0,
  1,
  scm_anEventOutputNames,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

void E_REND::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventEI1ID:
      if(m_bE2Occ){
        m_bE2Occ = false;
        sendOutputEvent( scm_nEventEOID);
      }
      else{
        m_bE1Occ = true;
      }
      break;
    case scm_nEventEI2ID:
      if(m_bE1Occ){
        m_bE1Occ = false;
        sendOutputEvent( scm_nEventEOID);
      }
      else{
        m_bE2Occ = true;
      }
      break;
    case scm_nEventRID:
      m_bE1Occ = m_bE2Occ = false;
      break;
    default:
      break;
  }
}


#else

DEFINE_FIRMWARE_FB(E_REND, g_nStringIdE_REND)

const TForteInt16 E_REND::scm_anEIWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId E_REND::scm_anEventInputNames[] = {g_nStringIdEI1, g_nStringIdEI2, g_nStringIdR};

const TForteInt16 E_REND::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId E_REND::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_REND::scm_stFBInterfaceSpec = {
  3,  scm_anEventInputNames,  0,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  0, 0,  0,  0, 0,
  0,  0, 0,
  0, 0
};


void E_REND::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void E_REND::enterStateR(void){
  m_nECCState = scm_nStateR;
}

void E_REND::enterStateR1(void){
  m_nECCState = scm_nStateR1;
}

void E_REND::enterStateEI1(void){
  m_nECCState = scm_nStateEI1;
}

void E_REND::enterStateEO(void){
  m_nECCState = scm_nStateEO;
  sendOutputEvent( scm_nEventEOID);
}

void E_REND::enterStateEI2(void){
  m_nECCState = scm_nStateEI2;
}

void E_REND::enterStateR2(void){
  m_nECCState = scm_nStateR2;
}

void E_REND::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if(scm_nEventRID == pa_nEIID)
          enterStateR();
        else
        if(scm_nEventEI1ID == pa_nEIID)
          enterStateEI1();
        else
        if(scm_nEventEI2ID == pa_nEIID)
          enterStateEI2();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateR:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateR1:
        if(1)
          enterStateEI1();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateEI1:
        if(scm_nEventRID == pa_nEIID)
          enterStateSTART();
        else
        if(scm_nEventEI1ID == pa_nEIID)
          enterStateR1();
        else
        if(scm_nEventEI2ID == pa_nEIID)
          enterStateEO();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateEO:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateEI2:
        if(scm_nEventRID == pa_nEIID)
          enterStateSTART();
        else
        if(scm_nEventEI2ID == pa_nEIID)
          enterStateR2();
        else
        if(scm_nEventEI1ID == pa_nEIID)
          enterStateEO();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateR2:
        if(1)
          enterStateEI2();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      default:
      DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 6.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; //0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}




#endif
