/*************************************************************************
 *** Copyright (c) 2022 Peirlberger Juergen
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: PLCnextAXLSEDO16
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2022-04-07/Peirlberger Juergen -  - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "PLCnextAXLSEDO16.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterIn);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(DO_1);
USE_STRING_ID(DO_10);
USE_STRING_ID(DO_11);
USE_STRING_ID(DO_12);
USE_STRING_ID(DO_13);
USE_STRING_ID(DO_14);
USE_STRING_ID(DO_15);
USE_STRING_ID(DO_16);
USE_STRING_ID(DO_2);
USE_STRING_ID(DO_3);
USE_STRING_ID(DO_4);
USE_STRING_ID(DO_5);
USE_STRING_ID(DO_6);
USE_STRING_ID(DO_7);
USE_STRING_ID(DO_8);
USE_STRING_ID(DO_9);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(PLCnextAXLSEDO16);
USE_STRING_ID(PLCnextBusAdapter);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(WSTRING);

#include "PLCnextBusAdapter.h"
#include "core/datatypes/forte_string.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_PLCnextAXLSEDO16, STRID(PLCnextAXLSEDO16))

namespace {
  const auto cDataInputNames =
      std::array{STRID(QI),    STRID(DO_1),  STRID(DO_2),  STRID(DO_3),  STRID(DO_4),  STRID(DO_5),
                 STRID(DO_6),  STRID(DO_7),  STRID(DO_8),  STRID(DO_9),  STRID(DO_10), STRID(DO_11),
                 STRID(DO_12), STRID(DO_13), STRID(DO_14), STRID(DO_15), STRID(DO_16)};
  const auto cDataOutputNames = std::array{STRID(QO), STRID(STATUS)};
  const auto cEventInputNames = std::array{STRID(INIT)};
  const auto cEventInputTypeIds = std::array{STRID(EInit)};
  const auto cEventOutputNames = std::array{STRID(INITO), STRID(IND)};
  const auto cEventOutputTypeIds = std::array{STRID(EInit), STRID(Event)};
  const auto cSocketNameIds = std::array{STRID(BusAdapterIn)};
  const auto cPlugNameIds = std::array{STRID(BusAdapterOut)};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = cSocketNameIds,
      .mPlugNames = cPlugNameIds,
  };
} // namespace

FORTE_PLCnextAXLSEDO16::FORTE_PLCnextAXLSEDO16(const CStringDictionary::TStringId paInstanceNameId,
                                               forte::core::CFBContainer &paContainer) :
    PLCnextSlaveHandler(PLCnextSlaveHandler::Output, paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_DO_1(""_STRING),
    var_DO_2(""_STRING),
    var_DO_3(""_STRING),
    var_DO_4(""_STRING),
    var_DO_5(""_STRING),
    var_DO_6(""_STRING),
    var_DO_7(""_STRING),
    var_DO_8(""_STRING),
    var_DO_9(""_STRING),
    var_DO_10(""_STRING),
    var_DO_11(""_STRING),
    var_DO_12(""_STRING),
    var_DO_13(""_STRING),
    var_DO_14(""_STRING),
    var_DO_15(""_STRING),
    var_DO_16(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    conn_INITO(*this, 0),
    conn_IND(*this, 1),
    conn_QI(nullptr),
    conn_DO_1(nullptr),
    conn_DO_2(nullptr),
    conn_DO_3(nullptr),
    conn_DO_4(nullptr),
    conn_DO_5(nullptr),
    conn_DO_6(nullptr),
    conn_DO_7(nullptr),
    conn_DO_8(nullptr),
    conn_DO_9(nullptr),
    conn_DO_10(nullptr),
    conn_DO_11(nullptr),
    conn_DO_12(nullptr),
    conn_DO_13(nullptr),
    conn_DO_14(nullptr),
    conn_DO_15(nullptr),
    conn_DO_16(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {};

void FORTE_PLCnextAXLSEDO16::setInitialValues() {
  var_QI = 0_BOOL;
  var_DO_1 = ""_STRING;
  var_DO_2 = ""_STRING;
  var_DO_3 = ""_STRING;
  var_DO_4 = ""_STRING;
  var_DO_5 = ""_STRING;
  var_DO_6 = ""_STRING;
  var_DO_7 = ""_STRING;
  var_DO_8 = ""_STRING;
  var_DO_9 = ""_STRING;
  var_DO_10 = ""_STRING;
  var_DO_11 = ""_STRING;
  var_DO_12 = ""_STRING;
  var_DO_13 = ""_STRING;
  var_DO_14 = ""_STRING;
  var_DO_15 = ""_STRING;
  var_DO_16 = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_PLCnextAXLSEDO16::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_DO_1, conn_DO_1);
      readData(2, var_DO_2, conn_DO_2);
      readData(3, var_DO_3, conn_DO_3);
      readData(4, var_DO_4, conn_DO_4);
      readData(5, var_DO_5, conn_DO_5);
      readData(6, var_DO_6, conn_DO_6);
      readData(7, var_DO_7, conn_DO_7);
      readData(8, var_DO_8, conn_DO_8);
      readData(9, var_DO_9, conn_DO_9);
      readData(10, var_DO_10, conn_DO_10);
      readData(11, var_DO_11, conn_DO_11);
      readData(12, var_DO_12, conn_DO_12);
      readData(13, var_DO_13, conn_DO_13);
      readData(14, var_DO_14, conn_DO_14);
      readData(15, var_DO_15, conn_DO_15);
      readData(16, var_DO_16, conn_DO_16);
      break;
    }
    default: break;
  }
}

void FORTE_PLCnextAXLSEDO16::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
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

CIEC_ANY *FORTE_PLCnextAXLSEDO16::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_DO_1;
    case 2: return &var_DO_2;
    case 3: return &var_DO_3;
    case 4: return &var_DO_4;
    case 5: return &var_DO_5;
    case 6: return &var_DO_6;
    case 7: return &var_DO_7;
    case 8: return &var_DO_8;
    case 9: return &var_DO_9;
    case 10: return &var_DO_10;
    case 11: return &var_DO_11;
    case 12: return &var_DO_12;
    case 13: return &var_DO_13;
    case 14: return &var_DO_14;
    case 15: return &var_DO_15;
    case 16: return &var_DO_16;
  }
  return nullptr;
}

CIEC_ANY *FORTE_PLCnextAXLSEDO16::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_PLCnextAXLSEDO16::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_PLCnextAXLSEDO16::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_DO_1;
    case 2: return &conn_DO_2;
    case 3: return &conn_DO_3;
    case 4: return &conn_DO_4;
    case 5: return &conn_DO_5;
    case 6: return &conn_DO_6;
    case 7: return &conn_DO_7;
    case 8: return &conn_DO_8;
    case 9: return &conn_DO_9;
    case 10: return &conn_DO_10;
    case 11: return &conn_DO_11;
    case 12: return &conn_DO_12;
    case 13: return &conn_DO_13;
    case 14: return &conn_DO_14;
    case 15: return &conn_DO_15;
    case 16: return &conn_DO_16;
  }
  return nullptr;
}

CDataConnection *FORTE_PLCnextAXLSEDO16::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}

void FORTE_PLCnextAXLSEDO16::initHandles() {
  // Initialize handles
  for (int i = 0; i < 16; i++) {
    PLCnextDeviceController::HandleDescriptor desc = PLCnextDeviceController::HandleDescriptor(
        *static_cast<CIEC_WSTRING *>(getDI(i + 1)), forte::core::io::IOMapper::Out, mIndex, (uint16_t) i,
        PLCnextDeviceController::Bit);

    initHandle(desc);
  }
}

const char *FORTE_PLCnextAXLSEDO16::init() {
  PLCnextDeviceController &controller = *static_cast<PLCnextDeviceController *>(&getController());

  // initialize buffer with correct size
  initBufferImage(16);
  controller.registerSlaveHandler(this);

  return plcNextDevice.init(mIndex, "OUT01");
}
