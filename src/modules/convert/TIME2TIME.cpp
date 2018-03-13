/*************************************************************************
 *** FORTE Library Element
 ***
 *** Name: TIME2TIME
 *** Description: Basic Function Block Type
 *** Version:
 ***     1.0: 2006-11-14/TS - PROFACTOR GmbH -
 *************************************************************************/

#include "TIME2TIME.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "TIME2TIME_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(TIME2TIME, g_nStringIdTIME2TIME)

const CStringDictionary::TStringId TIME2TIME::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId TIME2TIME::scm_anDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId TIME2TIME::scm_aunDIDataTypeIds[] = {g_nStringIdTIME};
const CStringDictionary::TStringId TIME2TIME::scm_aunDODataTypeIds[] = {g_nStringIdTIME};

const TForteInt16 TIME2TIME::scm_anEIWithIndexes[] = {0};
const TDataIOID TIME2TIME::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId TIME2TIME::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID TIME2TIME::scm_anEOWith[] = {0, 255};
const TForteInt16 TIME2TIME::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId TIME2TIME::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec TIME2TIME::scm_stFBInterfaceSpec = {
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

void TIME2TIME::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    OUT() = IN();
    sendOutputEvent(scm_nEventCNFID);
  }
}

TIME2TIME::~TIME2TIME(){
}

#else

DEFINE_FIRMWARE_FB(TIME2TIME, g_nStringIdTIME2TIME)

const CStringDictionary::TStringId TIME2TIME::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId TIME2TIME::scm_anDataInputTypeIds[] = {g_nStringIdTIME};

const CStringDictionary::TStringId TIME2TIME::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId TIME2TIME::scm_anDataOutputTypeIds[] = {g_nStringIdTIME};

const TForteInt16 TIME2TIME::scm_anEIWithIndexes[] = {0};
const TDataIOID TIME2TIME::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId TIME2TIME::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID TIME2TIME::scm_anEOWith[] = {0, 255};
const TForteInt16 TIME2TIME::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId TIME2TIME::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec TIME2TIME::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void TIME2TIME::alg_REQ(void){
OUT() = IN();

}


void TIME2TIME::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void TIME2TIME::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void TIME2TIME::executeEvent(int pa_nEIID){
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

