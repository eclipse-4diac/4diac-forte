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
#include "WORD2WORD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "WORD2WORD_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(WORD2WORD, g_nStringIdWORD2WORD)

const CStringDictionary::TStringId WORD2WORD::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId WORD2WORD::scm_anDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId WORD2WORD::scm_aunDIDataTypeIds[] = {g_nStringIdWORD};
const CStringDictionary::TStringId WORD2WORD::scm_aunDODataTypeIds[] = {g_nStringIdWORD};

const TForteInt16 WORD2WORD::scm_anEIWithIndexes[] = {0};
const TDataIOID WORD2WORD::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId WORD2WORD::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID WORD2WORD::scm_anEOWith[] = {0, 255};
const TForteInt16 WORD2WORD::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId WORD2WORD::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec WORD2WORD::scm_stFBInterfaceSpec = {
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

void WORD2WORD::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    OUT() = IN();
    sendOutputEvent(scm_nEventCNFID);
  }
}

WORD2WORD::~WORD2WORD(){
}

#else

DEFINE_FIRMWARE_FB(WORD2WORD, g_nStringIdWORD2WORD)

const CStringDictionary::TStringId WORD2WORD::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId WORD2WORD::scm_anDataInputTypeIds[] = {g_nStringIdWORD};

const CStringDictionary::TStringId WORD2WORD::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId WORD2WORD::scm_anDataOutputTypeIds[] = {g_nStringIdWORD};

const TForteInt16 WORD2WORD::scm_anEIWithIndexes[] = {0};
const TDataIOID WORD2WORD::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId WORD2WORD::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID WORD2WORD::scm_anEOWith[] = {0, 255};
const TForteInt16 WORD2WORD::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId WORD2WORD::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec WORD2WORD::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void WORD2WORD::alg_REQ(void){
OUT() = IN();

}


void WORD2WORD::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void WORD2WORD::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void WORD2WORD::executeEvent(int pa_nEIID){
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
