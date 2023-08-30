/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: Wago1405_6
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     0.0: 2016-11-30/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#include "Wago1405_6.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "Wago1405_6_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_Wago1405_6, g_nStringIdWago1405_6)

const CStringDictionary::TStringId FORTE_Wago1405_6::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdDigitalInput_1, g_nStringIdDigitalInput_2, g_nStringIdDigitalInput_3, g_nStringIdDigitalInput_4, g_nStringIdDigitalInput_5, g_nStringIdDigitalInput_6, g_nStringIdDigitalInput_7, g_nStringIdDigitalInput_8, g_nStringIdDigitalInput_9, g_nStringIdDigitalInput_10, g_nStringIdDigitalInput_11, g_nStringIdDigitalInput_12, g_nStringIdDigitalInput_13, g_nStringIdDigitalInput_14, g_nStringIdDigitalInput_15, g_nStringIdDigitalInput_16};

const CStringDictionary::TStringId FORTE_Wago1405_6::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_Wago1405_6::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_Wago1405_6::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_Wago1405_6::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_Wago1405_6::scmEIWith[] = {1, 2, 5, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_Wago1405_6::scmEventInputNames[] = {g_nStringIdMAP};

const TDataIOID FORTE_Wago1405_6::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_Wago1405_6::scmEOWithIndexes[] = {0, 2, -1};
const CStringDictionary::TStringId FORTE_Wago1405_6::scmEventOutputNames[] = {g_nStringIdMAPO, g_nStringIdIND};

const SAdapterInstanceDef FORTE_Wago1405_6::scmAdapterInstances[] = {
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterOut, true },
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterIn, false }};

const SFBInterfaceSpec FORTE_Wago1405_6::scmFBInterfaceSpec = {
  1,  scmEventInputNames,  scmEIWith,  scmEIWithIndexes,
  2,  scmEventOutputNames,  scmEOWith, scmEOWithIndexes,  17,  scmDataInputNames, scmDataInputTypeIds,
  2,  scmDataOutputNames, scmDataOutputTypeIds,
  2,scmAdapterInstances};
