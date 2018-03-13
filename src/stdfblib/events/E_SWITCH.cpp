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
#include "E_SWITCH.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SWITCH_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(E_SWITCH, g_nStringIdE_SWITCH)

const CStringDictionary::TStringId E_SWITCH::scm_anDataInputNames[] = {g_nStringIdG};
const CStringDictionary::TStringId E_SWITCH::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_SWITCH::scm_anEIWithIndexes[] = {0};
const TDataIOID E_SWITCH::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId E_SWITCH::scm_anEventInputNames[] = {g_nStringIdEI};

const CStringDictionary::TStringId E_SWITCH::scm_anEventOutputNames[] = {g_nStringIdEO0, g_nStringIdEO1};

const SFBInterfaceSpec E_SWITCH::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  2,
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

void E_SWITCH::executeEvent(int pa_nEIID){
  if(scm_nEventEIID == pa_nEIID){
    //sendOutputEvent( (true == G) ? scm_nEventEO1ID : scm_nEventEO0ID);
    if(true == G()){
      sendOutputEvent(scm_nEventEO1ID);
    }
    else{
      sendOutputEvent(scm_nEventEO0ID);
    }
  }
}


#else

DEFINE_FIRMWARE_FB(E_SWITCH, g_nStringIdE_SWITCH)

const CStringDictionary::TStringId E_SWITCH::scm_anDataInputNames[] = {g_nStringIdG};

const CStringDictionary::TStringId E_SWITCH::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_SWITCH::scm_anEIWithIndexes[] = {0};
const TDataIOID E_SWITCH::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId E_SWITCH::scm_anEventInputNames[] = {g_nStringIdEI};

const TForteInt16 E_SWITCH::scm_anEOWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId E_SWITCH::scm_anEventOutputNames[] = {g_nStringIdEO0, g_nStringIdEO1};

const SFBInterfaceSpec E_SWITCH::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  0, 0,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  0, 0
};


void E_SWITCH::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void E_SWITCH::enterStateG0(void){
  m_nECCState = scm_nStateG0;
  sendOutputEvent( scm_nEventEO0ID);
}

void E_SWITCH::enterStateG1(void){
  m_nECCState = scm_nStateG1;
  sendOutputEvent( scm_nEventEO1ID);
}

void E_SWITCH::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if((scm_nEventEIID == pa_nEIID) && ((((!G())))))
          enterStateG0();
        else
        if((scm_nEventEIID == pa_nEIID) && ((G())))
          enterStateG1();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateG0:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateG1:
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

#endif
