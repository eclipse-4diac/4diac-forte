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
#include "BYTE2BYTE.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BYTE2BYTE_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(BYTE2BYTE, g_nStringIdBYTE2BYTE)

const CStringDictionary::TStringId BYTE2BYTE::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId BYTE2BYTE::scm_anDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId BYTE2BYTE::scm_aunDIDataTypeIds[] = {g_nStringIdBYTE};
const CStringDictionary::TStringId BYTE2BYTE::scm_aunDODataTypeIds[] = {g_nStringIdBYTE};

const TForteInt16 BYTE2BYTE::scm_anEIWithIndexes[] = {0};
const TDataIOID BYTE2BYTE::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId BYTE2BYTE::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID BYTE2BYTE::scm_anEOWith[] = {0, 255};
const TForteInt16 BYTE2BYTE::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId BYTE2BYTE::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec BYTE2BYTE::scm_stFBInterfaceSpec = {
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

void BYTE2BYTE::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    OUT() = IN();
    sendOutputEvent(scm_nEventCNFID);
  }
}

BYTE2BYTE::~BYTE2BYTE(){
}

#else

DEFINE_FIRMWARE_FB(BYTE2BYTE, g_nStringIdBYTE2BYTE)

const CStringDictionary::TStringId BYTE2BYTE::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId BYTE2BYTE::scm_anDataInputTypeIds[] = {g_nStringIdBYTE};

const CStringDictionary::TStringId BYTE2BYTE::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId BYTE2BYTE::scm_anDataOutputTypeIds[] = {g_nStringIdBYTE};

const TForteInt16 BYTE2BYTE::scm_anEIWithIndexes[] = {0};
const TDataIOID BYTE2BYTE::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId BYTE2BYTE::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID BYTE2BYTE::scm_anEOWith[] = {0, 255};
const TForteInt16 BYTE2BYTE::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId BYTE2BYTE::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec BYTE2BYTE::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void BYTE2BYTE::alg_REQ(void){
OUT() = IN();

}


void BYTE2BYTE::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void BYTE2BYTE::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void BYTE2BYTE::executeEvent(int pa_nEIID){
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



