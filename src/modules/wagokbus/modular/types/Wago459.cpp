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
const TDataIOID FORTE_Wago459::scmEIWith[] = {1, 2, 3, 4, 0, scmWithListDelimiter};
const TForteInt16 FORTE_Wago459::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_Wago459::scmEventInputNames[] = {g_nStringIdMAP};
const TDataIOID FORTE_Wago459::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_Wago459::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_Wago459::scmEventOutputNames[] = {g_nStringIdMAPO, g_nStringIdIND};
const SAdapterInstanceDef FORTE_Wago459::scmAdapterInstances[] = {
  {g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterOut, true},
  {g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterIn, false}
};
const SFBInterfaceSpec FORTE_Wago459::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  5, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  2, scmAdapterInstances
};

FORTE_Wago459::FORTE_Wago459(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    WagoSlaveBase(459, paContainer, &scmFBInterfaceSpec, paInstanceNameId),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_MAPO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_AnalogInput_1(nullptr),
    conn_AnalogInput_2(nullptr),
    conn_AnalogInput_3(nullptr),
    conn_AnalogInput_4(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

void FORTE_Wago459::setInitialValues() {
  var_QI = 0_BOOL;
  var_AnalogInput_1 = u""_WSTRING;
  var_AnalogInput_2 = u""_WSTRING;
  var_AnalogInput_3 = u""_WSTRING;
  var_AnalogInput_4 = u""_WSTRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_Wago459::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventMAPID: {
      readData(1, var_AnalogInput_1, conn_AnalogInput_1);
      readData(2, var_AnalogInput_2, conn_AnalogInput_2);
      readData(3, var_AnalogInput_3, conn_AnalogInput_3);
      readData(4, var_AnalogInput_4, conn_AnalogInput_4);
      readData(0, var_QI, conn_QI);
      break;
    }
    default:
      break;
  }
}

void FORTE_Wago459::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventMAPOID: {
      writeData(0, var_QO, conn_QO);
      break;
    }
    case scmEventINDID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_Wago459::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_AnalogInput_1;
    case 2: return &var_AnalogInput_2;
    case 3: return &var_AnalogInput_3;
    case 4: return &var_AnalogInput_4;
  }
  return nullptr;
}

CIEC_ANY *FORTE_Wago459::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_Wago459::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_MAPO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_Wago459::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_AnalogInput_1;
    case 2: return &conn_AnalogInput_2;
    case 3: return &conn_AnalogInput_3;
    case 4: return &conn_AnalogInput_4;
  }
  return nullptr;
}

CDataConnection *FORTE_Wago459::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
