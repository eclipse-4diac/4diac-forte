/*******************************************************************************
 * Copyright (c) 2006 - 2013 Profactor GmbH, ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Thomas Strasser, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny,
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "UDINT2UDINT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "UDINT2UDINT_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(UDINT2UDINT, g_nStringIdUDINT2UDINT)

const CStringDictionary::TStringId UDINT2UDINT::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId UDINT2UDINT::scm_anDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId UDINT2UDINT::scm_aunDIDataTypeIds[] = {g_nStringIdUDINT};
const CStringDictionary::TStringId UDINT2UDINT::scm_aunDODataTypeIds[] = {g_nStringIdUDINT};

const TForteInt16 UDINT2UDINT::scm_anEIWithIndexes[] = {0};
const TDataIOID UDINT2UDINT::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId UDINT2UDINT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID UDINT2UDINT::scm_anEOWith[] = {0, 255};
const TForteInt16 UDINT2UDINT::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId UDINT2UDINT::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec UDINT2UDINT::scm_stFBInterfaceSpec = {
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

void UDINT2UDINT::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    OUT() = IN();
    sendOutputEvent(scm_nEventCNFID);
  }
}

UDINT2UDINT::~UDINT2UDINT(){
}

#else

DEFINE_FIRMWARE_FB(UDINT2UDINT, g_nStringIdUDINT2UDINT)

const CStringDictionary::TStringId UDINT2UDINT::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId UDINT2UDINT::scm_anDataInputTypeIds[] = {g_nStringIdUDINT};

const CStringDictionary::TStringId UDINT2UDINT::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId UDINT2UDINT::scm_anDataOutputTypeIds[] = {g_nStringIdUDINT};

const TForteInt16 UDINT2UDINT::scm_anEIWithIndexes[] = {0};
const TDataIOID UDINT2UDINT::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId UDINT2UDINT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID UDINT2UDINT::scm_anEOWith[] = {0, 255};
const TForteInt16 UDINT2UDINT::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId UDINT2UDINT::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec UDINT2UDINT::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void UDINT2UDINT::alg_REQ(void){
OUT() = IN();

}


void UDINT2UDINT::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void UDINT2UDINT::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void UDINT2UDINT::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if(scm_nEventREQID == pa_nEIID)
          enterStateREQ();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateREQ:
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


