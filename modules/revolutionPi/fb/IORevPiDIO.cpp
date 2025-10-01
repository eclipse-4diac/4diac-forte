/*******************************************************************************
 * Copyright (c) 2017, 2024 fortiss GmbH, Monika Wenger, Johannes Kepler University Linz
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v20.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Monika Wenger, Alois Zoitl - migration to current runtime
 *******************************************************************************/

#include "IORevPiDIO.h"

using namespace forte::literals;

#include "../RevPiController.h"

namespace forte::eclipse4diac::io::revpi {

  DEFINE_FIRMWARE_FB(FORTE_IORevPiDIO, "eclipse4diac::io::revpi::IORevPiDIO"_STRID)

  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID,
                                            "DigitalInput_1"_STRID,
                                            "DigitalInput_2"_STRID,
                                            "DigitalInput_3"_STRID,
                                            "DigitalInput_4"_STRID,
                                            "DigitalInput_5"_STRID,
                                            "DigitalInput_6"_STRID,
                                            "DigitalInput_7"_STRID,
                                            "DigitalInput_8"_STRID,
                                            "DigitalInput_9"_STRID,
                                            "DigitalInput_10"_STRID,
                                            "DigitalInput_11"_STRID,
                                            "DigitalInput_12"_STRID,
                                            "DigitalInput_13"_STRID,
                                            "DigitalInput_14"_STRID,
                                            "DigitalOutput_1"_STRID,
                                            "DigitalOutput_2"_STRID,
                                            "DigitalOutput_3"_STRID,
                                            "DigitalOutput_4"_STRID,
                                            "DigitalOutput_5"_STRID,
                                            "DigitalOutput_6"_STRID,
                                            "DigitalOutput_7"_STRID,
                                            "DigitalOutput_8"_STRID,
                                            "DigitalOutput_9"_STRID,
                                            "DigitalOutput_10"_STRID,
                                            "DigitalOutput_11"_STRID,
                                            "DigitalOutput_12"_STRID,
                                            "DigitalOutput_13"_STRID,
                                            "DigitalOutput_14"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
    const auto cEventInputNames = std::array{"MAP"_STRID};
    const auto cEventOutputNames = std::array{"MAPO"_STRID, "IND"_STRID};
    const auto cSocketNameIds = std::array{"BusAdapterIn"_STRID};
    const auto cPlugNameIds = std::array{"BusAdapterOut"_STRID};

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

  const TForteUInt8 FORTE_IORevPiDIO::scmSlaveConfigurationIO[] = {};
  const TForteUInt8 FORTE_IORevPiDIO::scmSlaveConfigurationIONum = 0;

  FORTE_IORevPiDIO::FORTE_IORevPiDIO(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      ::forte::io::IOConfigFBMultiSlave(
          scmSlaveConfigurationIO, scmSlaveConfigurationIONum, 96, paContainer, cFBInterfaceSpec, paInstanceNameId),
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
      conn_DigitalInput_9(nullptr),
      conn_DigitalInput_10(nullptr),
      conn_DigitalInput_11(nullptr),
      conn_DigitalInput_12(nullptr),
      conn_DigitalInput_13(nullptr),
      conn_DigitalInput_14(nullptr),
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
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      var_BusAdapterOut("BusAdapterOut"_STRID, *this, 0),
      var_BusAdapterIn("BusAdapterIn"_STRID, *this, 0) {};

  void FORTE_IORevPiDIO::setInitialValues() {
    var_QI = 0_BOOL;
    var_DigitalInput_1 = ""_STRING;
    var_DigitalInput_2 = ""_STRING;
    var_DigitalInput_3 = ""_STRING;
    var_DigitalInput_4 = ""_STRING;
    var_DigitalInput_5 = ""_STRING;
    var_DigitalInput_6 = ""_STRING;
    var_DigitalInput_7 = ""_STRING;
    var_DigitalInput_8 = ""_STRING;
    var_DigitalInput_9 = ""_STRING;
    var_DigitalInput_10 = ""_STRING;
    var_DigitalInput_11 = ""_STRING;
    var_DigitalInput_12 = ""_STRING;
    var_DigitalInput_13 = ""_STRING;
    var_DigitalInput_14 = ""_STRING;
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
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_IORevPiDIO::readInputData(const TEventID paEIID) {
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
        readData(15, var_DigitalOutput_1, conn_DigitalOutput_1);
        readData(16, var_DigitalOutput_2, conn_DigitalOutput_2);
        readData(17, var_DigitalOutput_3, conn_DigitalOutput_3);
        readData(18, var_DigitalOutput_4, conn_DigitalOutput_4);
        readData(19, var_DigitalOutput_5, conn_DigitalOutput_5);
        readData(20, var_DigitalOutput_6, conn_DigitalOutput_6);
        readData(21, var_DigitalOutput_7, conn_DigitalOutput_7);
        readData(22, var_DigitalOutput_8, conn_DigitalOutput_8);
        readData(0, var_QI, conn_QI);
        readData(14, var_DigitalInput_14, conn_DigitalInput_14);
        readData(13, var_DigitalInput_13, conn_DigitalInput_13);
        readData(12, var_DigitalInput_12, conn_DigitalInput_12);
        readData(11, var_DigitalInput_11, conn_DigitalInput_11);
        readData(10, var_DigitalInput_10, conn_DigitalInput_10);
        readData(9, var_DigitalInput_9, conn_DigitalInput_9);
        readData(23, var_DigitalOutput_9, conn_DigitalOutput_9);
        readData(24, var_DigitalOutput_10, conn_DigitalOutput_10);
        readData(25, var_DigitalOutput_11, conn_DigitalOutput_11);
        readData(26, var_DigitalOutput_12, conn_DigitalOutput_12);
        readData(27, var_DigitalOutput_13, conn_DigitalOutput_13);
        readData(28, var_DigitalOutput_14, conn_DigitalOutput_14);
        break;
      }
      default: break;
    }
  }

  void FORTE_IORevPiDIO::writeOutputData(const TEventID paEIID) {
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

  CIEC_ANY *FORTE_IORevPiDIO::getDI(const size_t paIndex) {
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
      case 9: return &var_DigitalInput_9;
      case 10: return &var_DigitalInput_10;
      case 11: return &var_DigitalInput_11;
      case 12: return &var_DigitalInput_12;
      case 13: return &var_DigitalInput_13;
      case 14: return &var_DigitalInput_14;
      case 15: return &var_DigitalOutput_1;
      case 16: return &var_DigitalOutput_2;
      case 17: return &var_DigitalOutput_3;
      case 18: return &var_DigitalOutput_4;
      case 19: return &var_DigitalOutput_5;
      case 20: return &var_DigitalOutput_6;
      case 21: return &var_DigitalOutput_7;
      case 22: return &var_DigitalOutput_8;
      case 23: return &var_DigitalOutput_9;
      case 24: return &var_DigitalOutput_10;
      case 25: return &var_DigitalOutput_11;
      case 26: return &var_DigitalOutput_12;
      case 27: return &var_DigitalOutput_13;
      case 28: return &var_DigitalOutput_14;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_IORevPiDIO::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  forte::IPlugPin *FORTE_IORevPiDIO::getPlugPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
  }

  forte::ISocketPin *FORTE_IORevPiDIO::getSocketPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterIn : nullptr;
  }

  CEventConnection *FORTE_IORevPiDIO::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_MAPO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_IORevPiDIO::getDIConUnchecked(const TPortId paIndex) {
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
      case 9: return &conn_DigitalInput_9;
      case 10: return &conn_DigitalInput_10;
      case 11: return &conn_DigitalInput_11;
      case 12: return &conn_DigitalInput_12;
      case 13: return &conn_DigitalInput_13;
      case 14: return &conn_DigitalInput_14;
      case 15: return &conn_DigitalOutput_1;
      case 16: return &conn_DigitalOutput_2;
      case 17: return &conn_DigitalOutput_3;
      case 18: return &conn_DigitalOutput_4;
      case 19: return &conn_DigitalOutput_5;
      case 20: return &conn_DigitalOutput_6;
      case 21: return &conn_DigitalOutput_7;
      case 22: return &conn_DigitalOutput_8;
      case 23: return &conn_DigitalOutput_9;
      case 24: return &conn_DigitalOutput_10;
      case 25: return &conn_DigitalOutput_11;
      case 26: return &conn_DigitalOutput_12;
      case 27: return &conn_DigitalOutput_13;
      case 28: return &conn_DigitalOutput_14;
    }
    return nullptr;
  }

  CDataConnection *FORTE_IORevPiDIO::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  void FORTE_IORevPiDIO::initHandles() {
    // Initialize handles
    int iCount = 14;
    int oCount = 14;
    int iOffset = 1;
    int oOffset = iOffset + iCount;

    for (int i = 0; i < iCount; i++) {
      RevPiController::HandleDescriptor desc(static_cast<CIEC_STRING *>(getDI(iOffset + i))->getStorage(),
                                             ::forte::io::IOMapper::In, mIndex, CIEC_ANY::e_BOOL, (uint8_t) (i / 8),
                                             (uint8_t) (i % 8));
      initHandle(desc);
    }

    for (int i = 0; i < oCount; i++) {
      RevPiController::HandleDescriptor desc(static_cast<CIEC_STRING *>(getDI(oOffset + i))->getStorage(),
                                             ::forte::io::IOMapper::Out, mIndex, CIEC_ANY::e_BOOL, (uint8_t) (i / 8),
                                             (uint8_t) (i % 8));
      initHandle(desc);
    }
  }

} // namespace forte::eclipse4diac::io::revpi
