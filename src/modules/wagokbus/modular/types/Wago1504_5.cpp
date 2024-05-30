/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: Wago1504_5
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     0.0: 2016-11-30/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#include "Wago1504_5.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "Wago1504_5_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_Wago1504_5, g_nStringIdWago1504_5)

const CStringDictionary::TStringId FORTE_Wago1504_5::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdDigitalOutput_1, g_nStringIdDigitalOutput_2, g_nStringIdDigitalOutput_3, g_nStringIdDigitalOutput_4, g_nStringIdDigitalOutput_5, g_nStringIdDigitalOutput_6, g_nStringIdDigitalOutput_7, g_nStringIdDigitalOutput_8, g_nStringIdDigitalOutput_9, g_nStringIdDigitalOutput_10, g_nStringIdDigitalOutput_11, g_nStringIdDigitalOutput_12, g_nStringIdDigitalOutput_13, g_nStringIdDigitalOutput_14, g_nStringIdDigitalOutput_15, g_nStringIdDigitalOutput_16};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};
const TDataIOID FORTE_Wago1504_5::scmEIWith[] = {1, 2, 5, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0, scmWithListDelimiter};
const TForteInt16 FORTE_Wago1504_5::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmEventInputNames[] = {g_nStringIdMAP};
const TDataIOID FORTE_Wago1504_5::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_Wago1504_5::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmEventOutputNames[] = {g_nStringIdMAPO, g_nStringIdIND};
const SAdapterInstanceDef FORTE_Wago1504_5::scmAdapterInstances[] = {
  {g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterOut, true},
  {g_nStringIdWagoBusAdapter, g_nStringIdBusAdapterIn, false}
};
const SFBInterfaceSpec FORTE_Wago1504_5::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  17, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  2, scmAdapterInstances
};

FORTE_Wago1504_5::FORTE_Wago1504_5(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    WagoSlaveBase(36866, paContainer, &scmFBInterfaceSpec, paInstanceNameId),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_MAPO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_DigitalOutput_1(nullptr),
    conn_DigitalOutput_2(nullptr),
    conn_DigitalOutput_3(nullptr),
    conn_DigitalOutput_4(nullptr),
    conn_DigitalOutput_5(nullptr),
    conn_DigitalOutput_6(nullptr),
    conn_DigitalOutput_7(nullptr),
    conn_DigitalOutput_8(nullptr),
    conn_DigitalOutput_9(nullptr),
    conn_DigitalOutput_10(nullptr),
    conn_DigitalOutput_11(nullptr),
    conn_DigitalOutput_12(nullptr),
    conn_DigitalOutput_13(nullptr),
    conn_DigitalOutput_14(nullptr),
    conn_DigitalOutput_15(nullptr),
    conn_DigitalOutput_16(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

void FORTE_Wago1504_5::setInitialValues() {
  var_QI = 0_BOOL;
  var_DigitalOutput_1 = ""_STRING;
  var_DigitalOutput_2 = ""_STRING;
  var_DigitalOutput_3 = ""_STRING;
  var_DigitalOutput_4 = ""_STRING;
  var_DigitalOutput_5 = ""_STRING;
  var_DigitalOutput_6 = ""_STRING;
  var_DigitalOutput_7 = ""_STRING;
  var_DigitalOutput_8 = ""_STRING;
  var_DigitalOutput_9 = ""_STRING;
  var_DigitalOutput_10 = ""_STRING;
  var_DigitalOutput_11 = ""_STRING;
  var_DigitalOutput_12 = ""_STRING;
  var_DigitalOutput_13 = ""_STRING;
  var_DigitalOutput_14 = ""_STRING;
  var_DigitalOutput_15 = ""_STRING;
  var_DigitalOutput_16 = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_Wago1504_5::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventMAPID: {
      readData(1, var_DigitalOutput_1, conn_DigitalOutput_1);
      readData(2, var_DigitalOutput_2, conn_DigitalOutput_2);
      readData(5, var_DigitalOutput_5, conn_DigitalOutput_5);
      readData(3, var_DigitalOutput_3, conn_DigitalOutput_3);
      readData(4, var_DigitalOutput_4, conn_DigitalOutput_4);
      readData(6, var_DigitalOutput_6, conn_DigitalOutput_6);
      readData(7, var_DigitalOutput_7, conn_DigitalOutput_7);
      readData(8, var_DigitalOutput_8, conn_DigitalOutput_8);
      readData(9, var_DigitalOutput_9, conn_DigitalOutput_9);
      readData(10, var_DigitalOutput_10, conn_DigitalOutput_10);
      readData(11, var_DigitalOutput_11, conn_DigitalOutput_11);
      readData(12, var_DigitalOutput_12, conn_DigitalOutput_12);
      readData(13, var_DigitalOutput_13, conn_DigitalOutput_13);
      readData(14, var_DigitalOutput_14, conn_DigitalOutput_14);
      readData(15, var_DigitalOutput_15, conn_DigitalOutput_15);
      readData(16, var_DigitalOutput_16, conn_DigitalOutput_16);
      readData(0, var_QI, conn_QI);
      break;
    }
    default:
      break;
  }
}

void FORTE_Wago1504_5::writeOutputData(const TEventID paEIID) {
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

CIEC_ANY *FORTE_Wago1504_5::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_DigitalOutput_1;
    case 2: return &var_DigitalOutput_2;
    case 3: return &var_DigitalOutput_3;
    case 4: return &var_DigitalOutput_4;
    case 5: return &var_DigitalOutput_5;
    case 6: return &var_DigitalOutput_6;
    case 7: return &var_DigitalOutput_7;
    case 8: return &var_DigitalOutput_8;
    case 9: return &var_DigitalOutput_9;
    case 10: return &var_DigitalOutput_10;
    case 11: return &var_DigitalOutput_11;
    case 12: return &var_DigitalOutput_12;
    case 13: return &var_DigitalOutput_13;
    case 14: return &var_DigitalOutput_14;
    case 15: return &var_DigitalOutput_15;
    case 16: return &var_DigitalOutput_16;
  }
  return nullptr;
}

CIEC_ANY *FORTE_Wago1504_5::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_Wago1504_5::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_MAPO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_Wago1504_5::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_DigitalOutput_1;
    case 2: return &conn_DigitalOutput_2;
    case 3: return &conn_DigitalOutput_3;
    case 4: return &conn_DigitalOutput_4;
    case 5: return &conn_DigitalOutput_5;
    case 6: return &conn_DigitalOutput_6;
    case 7: return &conn_DigitalOutput_7;
    case 8: return &conn_DigitalOutput_8;
    case 9: return &conn_DigitalOutput_9;
    case 10: return &conn_DigitalOutput_10;
    case 11: return &conn_DigitalOutput_11;
    case 12: return &conn_DigitalOutput_12;
    case 13: return &conn_DigitalOutput_13;
    case 14: return &conn_DigitalOutput_14;
    case 15: return &conn_DigitalOutput_15;
    case 16: return &conn_DigitalOutput_16;
  }
  return nullptr;
}

CDataConnection *FORTE_Wago1504_5::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
