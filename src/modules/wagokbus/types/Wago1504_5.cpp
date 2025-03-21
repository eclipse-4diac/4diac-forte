/*************************************************************************
 * Copyright (c) 2016, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#include "Wago1504_5.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterIn);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(DigitalOutput_1);
USE_STRING_ID(DigitalOutput_10);
USE_STRING_ID(DigitalOutput_11);
USE_STRING_ID(DigitalOutput_12);
USE_STRING_ID(DigitalOutput_13);
USE_STRING_ID(DigitalOutput_14);
USE_STRING_ID(DigitalOutput_15);
USE_STRING_ID(DigitalOutput_16);
USE_STRING_ID(DigitalOutput_2);
USE_STRING_ID(DigitalOutput_3);
USE_STRING_ID(DigitalOutput_4);
USE_STRING_ID(DigitalOutput_5);
USE_STRING_ID(DigitalOutput_6);
USE_STRING_ID(DigitalOutput_7);
USE_STRING_ID(DigitalOutput_8);
USE_STRING_ID(DigitalOutput_9);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(MAP);
USE_STRING_ID(MAPO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(Wago1504_5);
USE_STRING_ID(WagoBusAdapter);
USE_STRING_ID(WSTRING);


DEFINE_FIRMWARE_FB(FORTE_Wago1504_5, STRID(Wago1504_5))

const CStringDictionary::TStringId FORTE_Wago1504_5::scmDataInputNames[] = {STRID(QI), STRID(DigitalOutput_1), STRID(DigitalOutput_2), STRID(DigitalOutput_3), STRID(DigitalOutput_4), STRID(DigitalOutput_5), STRID(DigitalOutput_6), STRID(DigitalOutput_7), STRID(DigitalOutput_8), STRID(DigitalOutput_9), STRID(DigitalOutput_10), STRID(DigitalOutput_11), STRID(DigitalOutput_12), STRID(DigitalOutput_13), STRID(DigitalOutput_14), STRID(DigitalOutput_15), STRID(DigitalOutput_16)};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING)};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_Wago1504_5::scmEIWith[] = {1, 2, 5, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0, scmWithListDelimiter};
const TForteInt16 FORTE_Wago1504_5::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmEventInputNames[] = {STRID(MAP)};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_Wago1504_5::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_Wago1504_5::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmEventOutputNames[] = {STRID(MAPO), STRID(IND)};
const CStringDictionary::TStringId FORTE_Wago1504_5::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};
const SAdapterInstanceDef FORTE_Wago1504_5::scmAdapterInstances[] = {
  {STRID(WagoBusAdapter), STRID(BusAdapterOut), true},
  {STRID(WagoBusAdapter), STRID(BusAdapterIn), false}
};
const SFBInterfaceSpec FORTE_Wago1504_5::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  17, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  2, scmAdapterInstances
};

FORTE_Wago1504_5::FORTE_Wago1504_5(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    WagoSlaveBase(36866, paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_DigitalOutput_1(""_STRING),
    var_DigitalOutput_2(""_STRING),
    var_DigitalOutput_3(""_STRING),
    var_DigitalOutput_4(""_STRING),
    var_DigitalOutput_5(""_STRING),
    var_DigitalOutput_6(""_STRING),
    var_DigitalOutput_7(""_STRING),
    var_DigitalOutput_8(""_STRING),
    var_DigitalOutput_9(""_STRING),
    var_DigitalOutput_10(""_STRING),
    var_DigitalOutput_11(""_STRING),
    var_DigitalOutput_12(""_STRING),
    var_DigitalOutput_13(""_STRING),
    var_DigitalOutput_14(""_STRING),
    var_DigitalOutput_15(""_STRING),
    var_DigitalOutput_16(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
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
    conn_QO(this, 0, var_QO),
    conn_STATUS(this, 1, var_STATUS) {
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
