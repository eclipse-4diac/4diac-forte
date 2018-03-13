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
#include "WSTRING2WSTRING.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "WSTRING2WSTRING_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(WSTRING2WSTRING, g_nStringIdWSTRING2WSTRING)

const CStringDictionary::TStringId WSTRING2WSTRING::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId WSTRING2WSTRING::scm_anDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId WSTRING2WSTRING::scm_aunDIDataTypeIds[] = {g_nStringIdWSTRING};
const CStringDictionary::TStringId WSTRING2WSTRING::scm_aunDODataTypeIds[] = {g_nStringIdWSTRING};

const TForteInt16 WSTRING2WSTRING::scm_anEIWithIndexes[] = {0};
const TDataIOID WSTRING2WSTRING::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId WSTRING2WSTRING::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID WSTRING2WSTRING::scm_anEOWith[] = {0, 255};
const TForteInt16 WSTRING2WSTRING::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId WSTRING2WSTRING::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec WSTRING2WSTRING::scm_stFBInterfaceSpec = {
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

void WSTRING2WSTRING::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    OUT() = IN();
    sendOutputEvent(scm_nEventCNFID);
  }
}

WSTRING2WSTRING::~WSTRING2WSTRING(){
}

#else

DEFINE_FIRMWARE_FB(WSTRING2WSTRING, g_nStringIdWSTRING2WSTRING)

const CStringDictionary::TStringId WSTRING2WSTRING::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId WSTRING2WSTRING::scm_anDataInputTypeIds[] = {g_nStringIdWSTRING};

const CStringDictionary::TStringId WSTRING2WSTRING::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId WSTRING2WSTRING::scm_anDataOutputTypeIds[] = {g_nStringIdWSTRING};

const TForteInt16 WSTRING2WSTRING::scm_anEIWithIndexes[] = {0};
const TDataIOID WSTRING2WSTRING::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId WSTRING2WSTRING::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID WSTRING2WSTRING::scm_anEOWith[] = {0, 255};
const TForteInt16 WSTRING2WSTRING::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId WSTRING2WSTRING::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec WSTRING2WSTRING::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void WSTRING2WSTRING::alg_REQ(void){
OUT() = IN();

}


void WSTRING2WSTRING::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void WSTRING2WSTRING::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void WSTRING2WSTRING::executeEvent(int pa_nEIID){
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



