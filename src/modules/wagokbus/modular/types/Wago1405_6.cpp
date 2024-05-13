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
const TDataIOID FORTE_Wago1405_6::scmEIWith[] = {1, 2, 5, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0, scmWithListDelimiter};
const TForteInt16 FORTE_Wago1405_6::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_Wago1405_6::scmEventInputNames[] = {g_nStringIdMAP};
const TDataIOID FORTE_Wago1405_6::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_Wago1405_6::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_Wago1405_6::scmEventOutputNames[] = {g_nStringIdMAPO, g_nStringIdIND};
const SAdapterInstanceDef FORTE_Wago1405_6::scmAdapterInstances[] = {
  {g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterOut, true},
  {g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterIn, false}
};
const SFBInterfaceSpec FORTE_Wago1405_6::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  17, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  2, scmAdapterInstances
};

FORTE_Wago1405_6::FORTE_Wago1405_6(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    WagoSlaveBase(36865, paContainer, &scmFBInterfaceSpec, paInstanceNameId),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_MAPO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_DigitalInput_1(nullptr),
    conn_DigitalInput_2(nullptr),
    conn_DigitalInput_3(nullptr),
    conn_DigitalInput_4(nullptr),
    conn_DigitalInput_5(nullptr),
    conn_DigitalInput_6(nullptr),
    conn_DigitalInput_7(nullptr),
    conn_DigitalInput_8(nullptr),
    conn_DigitalInput_9(nullptr),
    conn_DigitalInput_10(nullptr),
    conn_DigitalInput_11(nullptr),
    conn_DigitalInput_12(nullptr),
    conn_DigitalInput_13(nullptr),
    conn_DigitalInput_14(nullptr),
    conn_DigitalInput_15(nullptr),
    conn_DigitalInput_16(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

void FORTE_Wago1405_6::setInitialValues() {
  var_QI = 0_BOOL;
  var_DigitalInput_1 = u""_WSTRING;
  var_DigitalInput_2 = u""_WSTRING;
  var_DigitalInput_3 = u""_WSTRING;
  var_DigitalInput_4 = u""_WSTRING;
  var_DigitalInput_5 = u""_WSTRING;
  var_DigitalInput_6 = u""_WSTRING;
  var_DigitalInput_7 = u""_WSTRING;
  var_DigitalInput_8 = u""_WSTRING;
  var_DigitalInput_9 = u""_WSTRING;
  var_DigitalInput_10 = u""_WSTRING;
  var_DigitalInput_11 = u""_WSTRING;
  var_DigitalInput_12 = u""_WSTRING;
  var_DigitalInput_13 = u""_WSTRING;
  var_DigitalInput_14 = u""_WSTRING;
  var_DigitalInput_15 = u""_WSTRING;
  var_DigitalInput_16 = u""_WSTRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_Wago1405_6::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventMAPID: {
      readData(1, var_DigitalInput_1, conn_DigitalInput_1);
      readData(2, var_DigitalInput_2, conn_DigitalInput_2);
      readData(5, var_DigitalInput_5, conn_DigitalInput_5);
      readData(3, var_DigitalInput_3, conn_DigitalInput_3);
      readData(4, var_DigitalInput_4, conn_DigitalInput_4);
      readData(6, var_DigitalInput_6, conn_DigitalInput_6);
      readData(7, var_DigitalInput_7, conn_DigitalInput_7);
      readData(8, var_DigitalInput_8, conn_DigitalInput_8);
      readData(9, var_DigitalInput_9, conn_DigitalInput_9);
      readData(10, var_DigitalInput_10, conn_DigitalInput_10);
      readData(11, var_DigitalInput_11, conn_DigitalInput_11);
      readData(12, var_DigitalInput_12, conn_DigitalInput_12);
      readData(13, var_DigitalInput_13, conn_DigitalInput_13);
      readData(14, var_DigitalInput_14, conn_DigitalInput_14);
      readData(15, var_DigitalInput_15, conn_DigitalInput_15);
      readData(16, var_DigitalInput_16, conn_DigitalInput_16);
      readData(0, var_QI, conn_QI);
      break;
    }
    default:
      break;
  }
}

void FORTE_Wago1405_6::writeOutputData(const TEventID paEIID) {
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

CIEC_ANY *FORTE_Wago1405_6::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_DigitalInput_1;
    case 2: return &var_DigitalInput_2;
    case 3: return &var_DigitalInput_3;
    case 4: return &var_DigitalInput_4;
    case 5: return &var_DigitalInput_5;
    case 6: return &var_DigitalInput_6;
    case 7: return &var_DigitalInput_7;
    case 8: return &var_DigitalInput_8;
    case 9: return &var_DigitalInput_9;
    case 10: return &var_DigitalInput_10;
    case 11: return &var_DigitalInput_11;
    case 12: return &var_DigitalInput_12;
    case 13: return &var_DigitalInput_13;
    case 14: return &var_DigitalInput_14;
    case 15: return &var_DigitalInput_15;
    case 16: return &var_DigitalInput_16;
  }
  return nullptr;
}

CIEC_ANY *FORTE_Wago1405_6::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_Wago1405_6::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_MAPO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_Wago1405_6::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_DigitalInput_1;
    case 2: return &conn_DigitalInput_2;
    case 3: return &conn_DigitalInput_3;
    case 4: return &conn_DigitalInput_4;
    case 5: return &conn_DigitalInput_5;
    case 6: return &conn_DigitalInput_6;
    case 7: return &conn_DigitalInput_7;
    case 8: return &conn_DigitalInput_8;
    case 9: return &conn_DigitalInput_9;
    case 10: return &conn_DigitalInput_10;
    case 11: return &conn_DigitalInput_11;
    case 12: return &conn_DigitalInput_12;
    case 13: return &conn_DigitalInput_13;
    case 14: return &conn_DigitalInput_14;
    case 15: return &conn_DigitalInput_15;
    case 16: return &conn_DigitalInput_16;
  }
  return nullptr;
}

CDataConnection *FORTE_Wago1405_6::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
