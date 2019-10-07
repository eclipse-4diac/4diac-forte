/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: WagoBusAdapter
 *** Description: 
 *** Version: 
 *************************************************************************/

#include "WagoBusAdapter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "WagoBusAdapter_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_WagoBusAdapter, g_nStringIdWagoBusAdapter)

const CStringDictionary::TStringId FORTE_WagoBusAdapter::scm_anDataInputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_WagoBusAdapter::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_WagoBusAdapter::scm_anDataOutputNames[] = {g_nStringIdQI, g_nStringIdMasterId, g_nStringIdIndex};

const CStringDictionary::TStringId FORTE_WagoBusAdapter::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdUINT};

const TDataIOID FORTE_WagoBusAdapter::scm_anEIWith[] = {0, 255};
const TForteInt16 FORTE_WagoBusAdapter::scm_anEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scm_anEventInputNames[] = {g_nStringIdINITO};

const TDataIOID FORTE_WagoBusAdapter::scm_anEOWith[] = {2, 1, 0, 255};
const TForteInt16 FORTE_WagoBusAdapter::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_WagoBusAdapter::scm_anEventOutputNames[] = {g_nStringIdINIT};

const SFBInterfaceSpec FORTE_WagoBusAdapter::scm_stFBInterfaceSpecSocket = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_WagoBusAdapter::scm_stFBInterfaceSpecPlug = {
  1,  scm_anEventOutputNames,  scm_anEOWith,  scm_anEOWithIndexes,
  1,  scm_anEventInputNames,  scm_anEIWith, scm_anEIWithIndexes,  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0, 0
};

const TForteUInt8 FORTE_WagoBusAdapter::scmSlaveConfigurationIO[] = { };
const TForteUInt8 FORTE_WagoBusAdapter::scmSlaveConfigurationIONum = 0;



