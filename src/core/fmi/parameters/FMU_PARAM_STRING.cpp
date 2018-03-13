/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: FMU_PARAM_STRING
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     0.0: 2017-08-11/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#include "FMU_PARAM_STRING.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FMU_PARAM_STRING_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_FMU_PARAM_STRING, g_nStringIdFMU_PARAM_STRING)

const CStringDictionary::TStringId FORTE_FMU_PARAM_STRING::scm_anDataOutputNames[] = {g_nStringIdPARAM};

const CStringDictionary::TStringId FORTE_FMU_PARAM_STRING::scm_anDataOutputTypeIds[] = {g_nStringIdSTRING};

const TForteInt16 FORTE_FMU_PARAM_STRING::scm_anEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_FMU_PARAM_STRING::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FMU_PARAM_STRING::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_FMU_PARAM_STRING::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FMU_PARAM_STRING::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_FMU_PARAM_STRING::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  0,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  0,  0, 0, 
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_FMU_PARAM_STRING::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventREQID:
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}



