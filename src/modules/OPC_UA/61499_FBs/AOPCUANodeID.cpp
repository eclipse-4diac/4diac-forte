/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: AOPCUANodeID
 *** Description: null
 *** Version: 
 *************************************************************************/

#include "AOPCUANodeID.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "AOPCUANodeID_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_AOPCUANodeID, g_nStringIdAOPCUANodeID)

const CStringDictionary::TStringId FORTE_AOPCUANodeID::scm_anDataOutputNames[] = {g_nStringIdnsIndex, g_nStringIdidentifierType, g_nStringIdidentifier};

const CStringDictionary::TStringId FORTE_AOPCUANodeID::scm_anDataOutputTypeIds[] = {g_nStringIdUINT, g_nStringIdUSINT, g_nStringIdANY};

const TForteInt16 FORTE_AOPCUANodeID::scm_anEIWithIndexes[] = {-1};
const TDataIOID FORTE_AOPCUANodeID::scm_anEOWith[] = {0, 1, 2, 255};
const TForteInt16 FORTE_AOPCUANodeID::scm_anEOWithIndexes[] = {0};
const SFBInterfaceSpec FORTE_AOPCUANodeID::scm_stFBInterfaceSpecSocket = {
  0,  0,  0,  0,
  0,  0,   scm_anEOWith, scm_anEOWithIndexes,  0,  0, 0, 
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_AOPCUANodeID::scm_stFBInterfaceSpecPlug = {
  0,  0,  0,  0,
  0,  0,   0, 0,  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0,  0, 0,
  0, 0
};



