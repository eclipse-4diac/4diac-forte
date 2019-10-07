/*******************************************************************************
 * Copyright (c) 2012, 20113, 2015 Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Gerhard Ebenhofer, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "FB_RANDOM.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_RANDOM_gen.cpp"
#endif
#include <time.h>
#include <stdlib.h>

DEFINE_FIRMWARE_FB(FORTE_FB_RANDOM, g_nStringIdFB_RANDOM)

const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anDataInputNames[] = {g_nStringIdSEED};

const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anDataInputTypeIds[] = {g_nStringIdUINT};

const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anDataOutputNames[] = {g_nStringIdVAL};

const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anDataOutputTypeIds[] = {g_nStringIdREAL};

const TForteInt16 FORTE_FB_RANDOM::scm_anEIWithIndexes[] = {0, -1};
const TDataIOID FORTE_FB_RANDOM::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID FORTE_FB_RANDOM::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_FB_RANDOM::scm_anEOWithIndexes[] = {-1, 0, -1};
const CStringDictionary::TStringId FORTE_FB_RANDOM::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};

const SFBInterfaceSpec FORTE_FB_RANDOM::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_FB_RANDOM::setInitialValues(){
  SEED().fromString("0");
}

void FORTE_FB_RANDOM::alg_INIT(void){
// WARNING - Don't forget to add #include <time.h>
  if (SEED() == 0) {
    srand((unsigned int) time(NULL) );
  } else {
    srand( SEED() );
  }
}

void FORTE_FB_RANDOM::alg_REQ(void){
  VAL() = static_cast<TForteFloat>(rand())/static_cast<TForteFloat>(RAND_MAX);
}


void FORTE_FB_RANDOM::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_FB_RANDOM::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void FORTE_FB_RANDOM::enterStateState(void){
  m_nECCState = scm_nStateState;
  alg_INIT();
  sendOutputEvent( scm_nEventINITOID);
}

void FORTE_FB_RANDOM::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if(scm_nEventREQID == pa_nEIID)
          enterStateREQ();
        else
        if(scm_nEventINITID == pa_nEIID)
          enterStateState();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateREQ:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateState:
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


