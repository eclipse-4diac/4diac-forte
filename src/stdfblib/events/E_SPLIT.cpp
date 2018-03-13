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
#include "E_SPLIT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SPLIT_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(E_SPLIT, g_nStringIdE_SPLIT)

const CStringDictionary::TStringId E_SPLIT::scm_anEventInputNames[] = {g_nStringIdEI};

const CStringDictionary::TStringId E_SPLIT::scm_anEventOutputNames[] = {g_nStringIdEO1, g_nStringIdEO2};

const SFBInterfaceSpec E_SPLIT::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  0,
  0,
  2,
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

#else

DEFINE_FIRMWARE_FB(E_SPLIT, g_nStringIdE_SPLIT)

const TForteInt16 E_SPLIT::scm_anEIWithIndexes[] = {-1};
const CStringDictionary::TStringId E_SPLIT::scm_anEventInputNames[] = {g_nStringIdEI};

const TForteInt16 E_SPLIT::scm_anEOWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId E_SPLIT::scm_anEventOutputNames[] = {g_nStringIdEO1, g_nStringIdEO2};

const SFBInterfaceSpec E_SPLIT::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  0,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  0, 0,  0,  0, 0,
  0,  0, 0,
  0, 0
};


void E_SPLIT::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void E_SPLIT::enterStateEO(void){
  m_nECCState = scm_nStateEO;
  sendOutputEvent( scm_nEventEO1ID);
  sendOutputEvent( scm_nEventEO2ID);
}

void E_SPLIT::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if(scm_nEventEIID == pa_nEIID)
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
      default:
      DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 1.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; //0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}

#endif
