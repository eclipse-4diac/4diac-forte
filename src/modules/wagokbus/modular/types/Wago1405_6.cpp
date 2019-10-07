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

const CStringDictionary::TStringId FORTE_Wago1405_6::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdDigitalInput_1, g_nStringIdDigitalInput_2, g_nStringIdDigitalInput_3, g_nStringIdDigitalInput_4, g_nStringIdDigitalInput_5, g_nStringIdDigitalInput_6, g_nStringIdDigitalInput_7, g_nStringIdDigitalInput_8, g_nStringIdDigitalInput_9, g_nStringIdDigitalInput_10, g_nStringIdDigitalInput_11, g_nStringIdDigitalInput_12, g_nStringIdDigitalInput_13, g_nStringIdDigitalInput_14, g_nStringIdDigitalInput_15, g_nStringIdDigitalInput_16};

const CStringDictionary::TStringId FORTE_Wago1405_6::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_Wago1405_6::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_Wago1405_6::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_Wago1405_6::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_Wago1405_6::scm_anEIWith[] = {1, 2, 5, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0, 255};
const CStringDictionary::TStringId FORTE_Wago1405_6::scm_anEventInputNames[] = {g_nStringIdMAP};

const TDataIOID FORTE_Wago1405_6::scm_anEOWith[] = {0, 255, 0, 1, 255};
const TForteInt16 FORTE_Wago1405_6::scm_anEOWithIndexes[] = {0, 2, -1};
const CStringDictionary::TStringId FORTE_Wago1405_6::scm_anEventOutputNames[] = {g_nStringIdMAPO, g_nStringIdIND};

const SAdapterInstanceDef FORTE_Wago1405_6::scm_astAdapterInstances[] = {
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterOut, true },
{g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterIn, false }};

const SFBInterfaceSpec FORTE_Wago1405_6::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  17,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  2,scm_astAdapterInstances};
