/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: Wago459
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     0.0: 2016-11-30/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#include "Wago459.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "Wago459_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_Wago459, g_nStringIdWago459)

const CStringDictionary::TStringId FORTE_Wago459::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdAnalogInput_1, g_nStringIdAnalogInput_2, g_nStringIdAnalogInput_3, g_nStringIdAnalogInput_4};

const CStringDictionary::TStringId FORTE_Wago459::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_Wago459::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_Wago459::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_Wago459::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_Wago459::scm_anEIWith[] = {1, 2, 3, 4, 0, 255};
const CStringDictionary::TStringId FORTE_Wago459::scm_anEventInputNames[] = {g_nStringIdMAP};

const TDataIOID FORTE_Wago459::scm_anEOWith[] = {0, 255, 0, 1, 255};
const TForteInt16 FORTE_Wago459::scm_anEOWithIndexes[] = {0, 2, -1};
const CStringDictionary::TStringId FORTE_Wago459::scm_anEventOutputNames[] = {g_nStringIdMAPO, g_nStringIdIND};

const SAdapterInstanceDef FORTE_Wago459::scm_astAdapterInstances[] = {
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterOut, true },
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterIn, false }};

const SFBInterfaceSpec FORTE_Wago459::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  5,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  2,scm_astAdapterInstances};
