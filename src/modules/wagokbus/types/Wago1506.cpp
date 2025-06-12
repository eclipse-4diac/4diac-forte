/*************************************************************************
 * Copyright (c) 2016, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#include "Wago1506.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterIn);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(DigitalInput_1);
USE_STRING_ID(DigitalInput_2);
USE_STRING_ID(DigitalInput_3);
USE_STRING_ID(DigitalInput_4);
USE_STRING_ID(DigitalInput_5);
USE_STRING_ID(DigitalInput_6);
USE_STRING_ID(DigitalInput_7);
USE_STRING_ID(DigitalInput_8);
USE_STRING_ID(DigitalOutput_1);
USE_STRING_ID(DigitalOutput_2);
USE_STRING_ID(DigitalOutput_3);
USE_STRING_ID(DigitalOutput_4);
USE_STRING_ID(DigitalOutput_5);
USE_STRING_ID(DigitalOutput_6);
USE_STRING_ID(DigitalOutput_7);
USE_STRING_ID(DigitalOutput_8);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(MAP);
USE_STRING_ID(MAPO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(Wago1506);
USE_STRING_ID(WagoBusAdapter);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_Wago1506, STRID(Wago1506))

namespace {
  const auto cDataInputNames = std::array{STRID(QI),
                                          STRID(DigitalInput_1),
                                          STRID(DigitalInput_2),
                                          STRID(DigitalInput_3),
                                          STRID(DigitalInput_4),
                                          STRID(DigitalInput_5),
                                          STRID(DigitalInput_6),
                                          STRID(DigitalInput_7),
                                          STRID(DigitalInput_8),
                                          STRID(DigitalOutput_1),
                                          STRID(DigitalOutput_2),
                                          STRID(DigitalOutput_3),
                                          STRID(DigitalOutput_4),
                                          STRID(DigitalOutput_5),
                                          STRID(DigitalOutput_6),
                                          STRID(DigitalOutput_7),
                                          STRID(DigitalOutput_8)};
  const auto cDataOutputNames = std::array{STRID(QO), STRID(STATUS)};
  const auto cEventInputNames = std::array{STRID(MAP)};
  const auto cEventOutputNames = std::array{STRID(MAPO), STRID(IND)};
  const auto cSocketNameIds = std::array{STRID(BusAdapterIn)};
  const auto cPlugNameIds = std::array{STRID(BusAdapterOut)};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = {},
      .mEONames = cEventOutputNames,
      .mEOTypeNames = {},
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = cSocketNameIds,
      .mPlugNames = cPlugNameIds,
  };
} // namespace

FORTE_Wago1506::FORTE_Wago1506(const CStringDictionary::TStringId paInstanceNameId,
                               forte::core::CFBContainer &paContainer) :
    WagoSlaveBase(34831, paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_DigitalInput_1(""_STRING),
    var_DigitalInput_2(""_STRING),
    var_DigitalInput_3(""_STRING),
    var_DigitalInput_4(""_STRING),
    var_DigitalInput_5(""_STRING),
    var_DigitalInput_6(""_STRING),
    var_DigitalInput_7(""_STRING),
    var_DigitalInput_8(""_STRING),
    var_DigitalOutput_1(""_STRING),
    var_DigitalOutput_2(""_STRING),
    var_DigitalOutput_3(""_STRING),
    var_DigitalOutput_4(""_STRING),
    var_DigitalOutput_5(""_STRING),
    var_DigitalOutput_6(""_STRING),
    var_DigitalOutput_7(""_STRING),
    var_DigitalOutput_8(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    conn_MAPO(*this, 0),
    conn_IND(*this, 1),
    conn_QI(nullptr),
    conn_DigitalInput_1(nullptr),
    conn_DigitalInput_2(nullptr),
    conn_DigitalInput_3(nullptr),
    conn_DigitalInput_4(nullptr),
    conn_DigitalInput_5(nullptr),
    conn_DigitalInput_6(nullptr),
    conn_DigitalInput_7(nullptr),
    conn_DigitalInput_8(nullptr),
    conn_DigitalOutput_1(nullptr),
    conn_DigitalOutput_2(nullptr),
    conn_DigitalOutput_3(nullptr),
    conn_DigitalOutput_4(nullptr),
    conn_DigitalOutput_5(nullptr),
    conn_DigitalOutput_6(nullptr),
    conn_DigitalOutput_7(nullptr),
    conn_DigitalOutput_8(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {};

void FORTE_Wago1506::setInitialValues() {
  var_QI = 0_BOOL;
  var_DigitalInput_1 = ""_STRING;
  var_DigitalInput_2 = ""_STRING;
  var_DigitalInput_3 = ""_STRING;
  var_DigitalInput_4 = ""_STRING;
  var_DigitalInput_5 = ""_STRING;
  var_DigitalInput_6 = ""_STRING;
  var_DigitalInput_7 = ""_STRING;
  var_DigitalInput_8 = ""_STRING;
  var_DigitalOutput_1 = ""_STRING;
  var_DigitalOutput_2 = ""_STRING;
  var_DigitalOutput_3 = ""_STRING;
  var_DigitalOutput_4 = ""_STRING;
  var_DigitalOutput_5 = ""_STRING;
  var_DigitalOutput_6 = ""_STRING;
  var_DigitalOutput_7 = ""_STRING;
  var_DigitalOutput_8 = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_Wago1506::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPID: {
      readData(1, var_DigitalInput_1, conn_DigitalInput_1);
      readData(2, var_DigitalInput_2, conn_DigitalInput_2);
      readData(5, var_DigitalInput_5, conn_DigitalInput_5);
      readData(3, var_DigitalInput_3, conn_DigitalInput_3);
      readData(4, var_DigitalInput_4, conn_DigitalInput_4);
      readData(6, var_DigitalInput_6, conn_DigitalInput_6);
      readData(7, var_DigitalInput_7, conn_DigitalInput_7);
      readData(8, var_DigitalInput_8, conn_DigitalInput_8);
      readData(9, var_DigitalOutput_1, conn_DigitalOutput_1);
      readData(10, var_DigitalOutput_2, conn_DigitalOutput_2);
      readData(11, var_DigitalOutput_3, conn_DigitalOutput_3);
      readData(12, var_DigitalOutput_4, conn_DigitalOutput_4);
      readData(13, var_DigitalOutput_5, conn_DigitalOutput_5);
      readData(14, var_DigitalOutput_6, conn_DigitalOutput_6);
      readData(15, var_DigitalOutput_7, conn_DigitalOutput_7);
      readData(16, var_DigitalOutput_8, conn_DigitalOutput_8);
      readData(0, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_Wago1506::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPOID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      break;
    }
    case scmEventINDID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_Wago1506::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_DigitalInput_1;
    case 2: return &var_DigitalInput_2;
    case 3: return &var_DigitalInput_3;
    case 4: return &var_DigitalInput_4;
    case 5: return &var_DigitalInput_5;
    case 6: return &var_DigitalInput_6;
    case 7: return &var_DigitalInput_7;
    case 8: return &var_DigitalInput_8;
    case 9: return &var_DigitalOutput_1;
    case 10: return &var_DigitalOutput_2;
    case 11: return &var_DigitalOutput_3;
    case 12: return &var_DigitalOutput_4;
    case 13: return &var_DigitalOutput_5;
    case 14: return &var_DigitalOutput_6;
    case 15: return &var_DigitalOutput_7;
    case 16: return &var_DigitalOutput_8;
  }
  return nullptr;
}

CIEC_ANY *FORTE_Wago1506::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_Wago1506::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MAPO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_Wago1506::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_DigitalInput_1;
    case 2: return &conn_DigitalInput_2;
    case 3: return &conn_DigitalInput_3;
    case 4: return &conn_DigitalInput_4;
    case 5: return &conn_DigitalInput_5;
    case 6: return &conn_DigitalInput_6;
    case 7: return &conn_DigitalInput_7;
    case 8: return &conn_DigitalInput_8;
    case 9: return &conn_DigitalOutput_1;
    case 10: return &conn_DigitalOutput_2;
    case 11: return &conn_DigitalOutput_3;
    case 12: return &conn_DigitalOutput_4;
    case 13: return &conn_DigitalOutput_5;
    case 14: return &conn_DigitalOutput_6;
    case 15: return &conn_DigitalOutput_7;
    case 16: return &conn_DigitalOutput_8;
  }
  return nullptr;
}

CDataConnection *FORTE_Wago1506::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
