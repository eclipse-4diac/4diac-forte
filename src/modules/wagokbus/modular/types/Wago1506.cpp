/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: Wago1506
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     0.0: 2016-11-30/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#include "Wago1506.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "Wago1506_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_Wago1506, g_nStringIdWago1506)

const CStringDictionary::TStringId FORTE_Wago1506::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdDigitalInput_1, g_nStringIdDigitalInput_2, g_nStringIdDigitalInput_3, g_nStringIdDigitalInput_4, g_nStringIdDigitalInput_5, g_nStringIdDigitalInput_6, g_nStringIdDigitalInput_7, g_nStringIdDigitalInput_8, g_nStringIdDigitalOutput_1, g_nStringIdDigitalOutput_2, g_nStringIdDigitalOutput_3, g_nStringIdDigitalOutput_4, g_nStringIdDigitalOutput_5, g_nStringIdDigitalOutput_6, g_nStringIdDigitalOutput_7, g_nStringIdDigitalOutput_8};

const CStringDictionary::TStringId FORTE_Wago1506::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_Wago1506::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_Wago1506::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_Wago1506::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_Wago1506::scmEIWith[] = {1, 2, 5, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_Wago1506::scmEventInputNames[] = {g_nStringIdMAP};

const TDataIOID FORTE_Wago1506::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_Wago1506::scmEOWithIndexes[] = {0, 2, -1};
const CStringDictionary::TStringId FORTE_Wago1506::scmEventOutputNames[] = {g_nStringIdMAPO, g_nStringIdIND};

const SAdapterInstanceDef FORTE_Wago1506::scmAdapterInstances[] = {
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterOut, true },
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterIn, false }};

const SFBInterfaceSpec FORTE_Wago1506::scmFBInterfaceSpec = {
  1,  scmEventInputNames,  scmEIWith,  scmEIWithIndexes,
  2,  scmEventOutputNames,  scmEOWith, scmEOWithIndexes,  17,  scmDataInputNames, scmDataInputTypeIds,
  2,  scmDataOutputNames, scmDataOutputTypeIds,
  2,scmAdapterInstances};
