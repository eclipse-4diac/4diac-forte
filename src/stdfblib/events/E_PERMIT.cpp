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
#include "E_PERMIT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_PERMIT_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(E_PERMIT, g_nStringIdE_PERMIT)

const CStringDictionary::TStringId E_PERMIT::scm_anDataInputNames[] = {g_nStringIdPERMIT};
const CStringDictionary::TStringId E_PERMIT::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_PERMIT::scm_anEIWithIndexes[] = { 0 };
const TDataIOID E_PERMIT::scm_anEIWith[] = { 0, 255 };
const CStringDictionary::TStringId E_PERMIT::scm_anEventInputNames[] = { g_nStringIdEI };

const CStringDictionary::TStringId E_PERMIT::scm_anEventOutputNames[] = { g_nStringIdEO };

const SFBInterfaceSpec
    E_PERMIT::scm_stFBInterfaceSpec = { 1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 1,
      scm_anEventOutputNames, 0, 0, 1, scm_anDataInputNames, scm_aunDIDataTypeIds, 0, 0, 0,
      0,
      0
};

void E_PERMIT::executeEvent(int pa_nEIID){
  if((scm_nEventEIID == pa_nEIID) && (PERMIT())){
    sendOutputEvent(scm_nEventEOID);
  }
}

#else

DEFINE_FIRMWARE_FB(E_PERMIT, g_nStringIdE_PERMIT)

const CStringDictionary::TStringId E_PERMIT::scm_anDataInputNames[] = {g_nStringIdPERMIT};

const CStringDictionary::TStringId E_PERMIT::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_PERMIT::scm_anEIWithIndexes[] = {0};
const TDataIOID E_PERMIT::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId E_PERMIT::scm_anEventInputNames[] = {g_nStringIdEI};

const TForteInt16 E_PERMIT::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId E_PERMIT::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_PERMIT::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  0, 0,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  0, 0
};


void E_PERMIT::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void E_PERMIT::enterStateEO(void){
  m_nECCState = scm_nStateEO;
  sendOutputEvent( scm_nEventEOID);
}

void E_PERMIT::enterStateNULL(void){
  m_nECCState = scm_nStateNULL;
}

void E_PERMIT::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if((scm_nEventEIID == pa_nEIID) && ((PERMIT())))
          enterStateEO();
        else
        if((scm_nEventEIID == pa_nEIID) && ((((!PERMIT())))))
          enterStateNULL();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateEO:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateNULL:
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
