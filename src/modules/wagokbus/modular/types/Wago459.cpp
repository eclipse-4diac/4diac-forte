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

const CStringDictionary::TStringId FORTE_Wago459::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdAnalogInput_1, g_nStringIdAnalogInput_2, g_nStringIdAnalogInput_3, g_nStringIdAnalogInput_4};

const CStringDictionary::TStringId FORTE_Wago459::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_Wago459::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_Wago459::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_Wago459::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_Wago459::scmEIWith[] = {1, 2, 3, 4, 0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_Wago459::scmEventInputNames[] = {g_nStringIdMAP};

const TDataIOID FORTE_Wago459::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_Wago459::scmEOWithIndexes[] = {0, 2, -1};
const CStringDictionary::TStringId FORTE_Wago459::scmEventOutputNames[] = {g_nStringIdMAPO, g_nStringIdIND};

const SAdapterInstanceDef FORTE_Wago459::scmAdapterInstances[] = {
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterOut, true },
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterIn, false }};

const SFBInterfaceSpec FORTE_Wago459::scmFBInterfaceSpec = {
  1,  scmEventInputNames,  scmEIWith,  scmEIWithIndexes,
  2,  scmEventOutputNames,  scmEOWith, scmEOWithIndexes,  5,  scmDataInputNames, scmDataInputTypeIds,
  2,  scmDataOutputNames, scmDataOutputTypeIds,
  2,scmAdapterInstances};
