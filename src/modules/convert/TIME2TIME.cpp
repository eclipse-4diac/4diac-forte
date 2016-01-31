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



