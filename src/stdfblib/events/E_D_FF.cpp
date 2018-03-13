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
#include "E_D_FF.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_D_FF_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(E_D_FF, g_nStringIdE_D_FF)

const CStringDictionary::TStringId E_D_FF::scm_anDataInputNames[] = {g_nStringIdD};

const CStringDictionary::TStringId E_D_FF::scm_anDataOutputNames[] = {g_nStringIdQ};
const CStringDictionary::TStringId E_D_FF::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL};
const CStringDictionary::TStringId E_D_FF::scm_aunDODataTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_D_FF::scm_anEIWithIndexes[] = {0};
const TDataIOID E_D_FF::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId E_D_FF::scm_anEventInputNames[] = {g_nStringIdCLK};

const TDataIOID E_D_FF::scm_anEOWith[] = {0, 255};
const TForteInt16 E_D_FF::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId E_D_FF::scm_anEventOutputNames[] = {g_nStringIdEO};

void E_D_FF::executeEvent(int pa_nEIID){
  if(scm_nEventCLKID == pa_nEIID){
    if(D() != Q()){
      Q() = D();
      sendOutputEvent( scm_nEventEOID);
    }
  }
}

const SFBInterfaceSpec E_D_FF::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  1,
  scm_anEventOutputNames,
  scm_anEOWith,
  scm_anEOWithIndexes,
  1,
  scm_anDataInputNames, scm_aunDIDataTypeIds,
  1,
  scm_anDataOutputNames, scm_aunDODataTypeIds,
  0,
  0
};

#else

DEFINE_FIRMWARE_FB(E_D_FF, g_nStringIdE_D_FF)

const CStringDictionary::TStringId E_D_FF::scm_anDataInputNames[] = {g_nStringIdD};

const CStringDictionary::TStringId E_D_FF::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};

const CStringDictionary::TStringId E_D_FF::scm_anDataOutputNames[] = {g_nStringIdQ};

const CStringDictionary::TStringId E_D_FF::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_D_FF::scm_anEIWithIndexes[] = {0};
const TDataIOID E_D_FF::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId E_D_FF::scm_anEventInputNames[] = {g_nStringIdCLK};

const TDataIOID E_D_FF::scm_anEOWith[] = {0, 255};
const TForteInt16 E_D_FF::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId E_D_FF::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_D_FF::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void E_D_FF::alg_LATCH(void){
Q() = D();

}


void E_D_FF::enterStateQ0(void){
  m_nECCState = scm_nStateQ0;
}

void E_D_FF::enterStateRESET(void){
  m_nECCState = scm_nStateRESET;
  alg_LATCH();
  sendOutputEvent( scm_nEventEOID);
}

void E_D_FF::enterStateSET(void){
  m_nECCState = scm_nStateSET;
  alg_LATCH();
  sendOutputEvent( scm_nEventEOID);
}

void E_D_FF::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateQ0:
        if((scm_nEventCLKID == pa_nEIID) && ((D())))
          enterStateSET();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateRESET:
        if((scm_nEventCLKID == pa_nEIID) && ((D())))
          enterStateSET();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateSET:
        if((scm_nEventCLKID == pa_nEIID) && ((((!D())))))
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



