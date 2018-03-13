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
#include "E_SELECT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SELECT_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(E_SELECT, g_nStringIdE_SELECT)

const CStringDictionary::TStringId E_SELECT::scm_anDataInputNames[] = {g_nStringIdG};
const CStringDictionary::TStringId E_SELECT::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_SELECT::scm_anEIWithIndexes[] = {0, 2};
const TDataIOID E_SELECT::scm_anEIWith[] = {0, 255, 0, 255};
const CStringDictionary::TStringId E_SELECT::scm_anEventInputNames[] = {g_nStringIdEI0, g_nStringIdEI1};

const CStringDictionary::TStringId E_SELECT::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_SELECT::scm_stFBInterfaceSpec = {
  2,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  1,
  scm_anEventOutputNames,
  0,
  0,
  1,
  scm_anDataInputNames, scm_aunDIDataTypeIds,
  0,
  0,
  0,
  0,
  0
};

void E_SELECT::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventEI0ID:
      if(false == G()){
        sendOutputEvent( scm_nEventEOID);
      }
      break;
    case scm_nEventEI1ID:
      if(true == G()){
        sendOutputEvent( scm_nEventEOID);
      }
      break;
    default:
      break;
  }
}

#else

DEFINE_FIRMWARE_FB(E_SELECT, g_nStringIdE_SELECT)

const CStringDictionary::TStringId E_SELECT::scm_anDataInputNames[] = {g_nStringIdG};

const CStringDictionary::TStringId E_SELECT::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_SELECT::scm_anEIWithIndexes[] = {0, 2};
const TDataIOID E_SELECT::scm_anEIWith[] = {0, 255, 0, 255};
const CStringDictionary::TStringId E_SELECT::scm_anEventInputNames[] = {g_nStringIdEI0, g_nStringIdEI1};

const TForteInt16 E_SELECT::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId E_SELECT::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_SELECT::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  0, 0,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  0, 0
};


void E_SELECT::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void E_SELECT::enterStateEO(void){
  m_nECCState = scm_nStateEO;
  sendOutputEvent( scm_nEventEOID);
}

void E_SELECT::enterStateEI0(void){
  m_nECCState = scm_nStateEI0;
}

void E_SELECT::enterStateEI1(void){
  m_nECCState = scm_nStateEI1;
}

void E_SELECT::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if(scm_nEventEI0ID == pa_nEIID)
          enterStateEI0();
        else
        if(scm_nEventEI1ID == pa_nEIID)
          enterStateEI1();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateEO:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateEI0:
        if(!G())
          enterStateEO();
        else
        if(G())
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateEI1:
        if(G())
          enterStateEO();
        else
        if(!G())
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      default:
      DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; //0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}

#endif


