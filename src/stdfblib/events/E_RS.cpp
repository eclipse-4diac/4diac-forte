/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Martin Melik Merkumians, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_RS.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_RS_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(E_RS, g_nStringIdE_RS)

const CStringDictionary::TStringId E_RS::scm_anDataOutputNames[] = {g_nStringIdQ};
const CStringDictionary::TStringId E_RS::scm_aunDODataTypeIds[] = {g_nStringIdBOOL};

const CStringDictionary::TStringId E_RS::scm_anEventInputNames[] = {g_nStringIdR, g_nStringIdS};

const TDataIOID E_RS::scm_anEOWith[] = {0, 255};
const TForteInt16 E_RS::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId E_RS::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_RS::scm_stFBInterfaceSpec = {
  2,
  scm_anEventInputNames,
  0,
  0,
  1,
  scm_anEventOutputNames,
  scm_anEOWith,
  scm_anEOWithIndexes,
  0,
  0,
  0,
  1,
  scm_anDataOutputNames, scm_aunDODataTypeIds,
  0,
  0
};

void E_RS::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventRID:
      if(true == Q()){
        Q() = false;
        sendOutputEvent( scm_nEventEOID);
      }
      break;
    case scm_nEventSID:
      if(false == Q()){
        Q() = true;
        sendOutputEvent( scm_nEventEOID);
      }
      break;
    default:
      break;
  }
}

#else

DEFINE_FIRMWARE_FB(E_RS, g_nStringIdE_RS)

const CStringDictionary::TStringId E_RS::scm_anDataOutputNames[] = {g_nStringIdQ};

const CStringDictionary::TStringId E_RS::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_RS::scm_anEIWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId E_RS::scm_anEventInputNames[] = {g_nStringIdR, g_nStringIdS};

const TDataIOID E_RS::scm_anEOWith[] = {0, 255};
const TForteInt16 E_RS::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId E_RS::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_RS::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  0,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  0,  0, 0,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void E_RS::alg_SET(void){
Q() = true;

}

void E_RS::alg_RESET(void){
Q() = false;

}


void E_RS::enterStateQ0(void){
  m_nECCState = scm_nStateQ0;
}

void E_RS::enterStateRESET(void){
  m_nECCState = scm_nStateRESET;
  alg_RESET();
  sendOutputEvent( scm_nEventEOID);
}

void E_RS::enterStateSET(void){
  m_nECCState = scm_nStateSET;
  alg_SET();
  sendOutputEvent( scm_nEventEOID);
}

void E_RS::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateQ0:
        if(scm_nEventSID == pa_nEIID)
          enterStateSET();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateRESET:
        if(scm_nEventSID == pa_nEIID)
          enterStateSET();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateSET:
        if(scm_nEventRID == pa_nEIID)
          enterStateRESET();
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


#endif

