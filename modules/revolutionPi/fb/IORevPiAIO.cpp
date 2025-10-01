/*******************************************************************************
 * Copyright (c) 2018, 2024 fortiss GmbH, Monika Wenger, Johannes Kepler University Linz
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v20.html
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *   Monika Wenger, Alois Zoitl - migration to current runtime
 *******************************************************************************/

#include "IORevPiAIO.h"

using namespace forte::literals;

#include "../RevPiController.h"

namespace forte::eclipse4diac::io::revpi {

  DEFINE_FIRMWARE_FB(FORTE_IORevPiAIO, "eclipse4diac::io::revpi::IORevPiAIO"_STRID)

  namespace {
    const auto cDataInputNames = std::array{
        "QI"_STRID,    "AnalogInput_1"_STRID, "AnalogInput_2"_STRID,  "AnalogInput_3"_STRID, "AnalogInput_4"_STRID,
        "RTD_1"_STRID, "RTD_2"_STRID,         "AnalogOutput_1"_STRID, "AnalogOutput_2"_STRID};
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

  const TForteUInt8 FORTE_IORevPiAIO::scmSlaveConfigurationIO[] = {};
  const TForteUInt8 FORTE_IORevPiAIO::scmSlaveConfigurationIONum = 0;

  FORTE_IORevPiAIO::FORTE_IORevPiAIO(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      ::forte::io::IOConfigFBMultiSlave(
          scmSlaveConfigurationIO, scmSlaveConfigurationIONum, 103, paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_MAPO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_AnalogInput_1(nullptr),
      conn_AnalogInput_2(nullptr),
      conn_AnalogInput_3(nullptr),
      conn_AnalogInput_4(nullptr),
      conn_RTD_1(nullptr),
      conn_RTD_2(nullptr),
      conn_AnalogOutput_1(nullptr),
      conn_AnalogOutput_2(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      var_BusAdapterOut("BusAdapterOut"_STRID, *this, 0),
      var_BusAdapterIn("BusAdapterIn"_STRID, *this, 0) {};

  void FORTE_IORevPiAIO::initHandles() {
    uint8_t inputOffset = 0;
    uint8_t outputOffset = 0;

    for (int i = 1; i < 9; i++) {
      uint8_t *currentOffset = (i < 7) ? &inputOffset : &outputOffset;
      RevPiController::HandleDescriptor desc(static_cast<CIEC_STRING *>(getDI(i))->getStorage(),
                                             (i < 7) ? ::forte::io::IOMapper::In : ::forte::io::IOMapper::Out, mIndex,
                                             CIEC_ANY::e_WORD, *currentOffset, 0);
      initHandle(desc);
      *currentOffset = static_cast<uint8_t>(*currentOffset + 2);
    }
  }

  void FORTE_IORevPiAIO::setInitialValues() {
    var_QI = 0_BOOL;
    var_AnalogInput_1 = ""_STRING;
    var_AnalogInput_2 = ""_STRING;
    var_AnalogInput_3 = ""_STRING;
    var_AnalogInput_4 = ""_STRING;
    var_RTD_1 = ""_STRING;
    var_RTD_2 = ""_STRING;
    var_AnalogOutput_1 = ""_STRING;
    var_AnalogOutput_2 = ""_STRING;
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_IORevPiAIO::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventMAPID: {
        readData(1, var_AnalogInput_1, conn_AnalogInput_1);
        readData(2, var_AnalogInput_2, conn_AnalogInput_2);
        readData(5, var_RTD_1, conn_RTD_1);
        readData(3, var_AnalogInput_3, conn_AnalogInput_3);
        readData(4, var_AnalogInput_4, conn_AnalogInput_4);
        readData(6, var_RTD_2, conn_RTD_2);
        readData(7, var_AnalogOutput_1, conn_AnalogOutput_1);
        readData(8, var_AnalogOutput_2, conn_AnalogOutput_2);
        readData(0, var_QI, conn_QI);
        break;
      }
      default: break;
    }
  }

  void FORTE_IORevPiAIO::writeOutputData(const TEventID paEIID) {
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

  CIEC_ANY *FORTE_IORevPiAIO::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_AnalogInput_1;
      case 2: return &var_AnalogInput_2;
      case 3: return &var_AnalogInput_3;
      case 4: return &var_AnalogInput_4;
      case 5: return &var_RTD_1;
      case 6: return &var_RTD_2;
      case 7: return &var_AnalogOutput_1;
      case 8: return &var_AnalogOutput_2;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_IORevPiAIO::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  forte::IPlugPin *FORTE_IORevPiAIO::getPlugPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
  }

  forte::ISocketPin *FORTE_IORevPiAIO::getSocketPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterIn : nullptr;
  }

  CEventConnection *FORTE_IORevPiAIO::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_MAPO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_IORevPiAIO::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_AnalogInput_1;
      case 2: return &conn_AnalogInput_2;
      case 3: return &conn_AnalogInput_3;
      case 4: return &conn_AnalogInput_4;
      case 5: return &conn_RTD_1;
      case 6: return &conn_RTD_2;
      case 7: return &conn_AnalogOutput_1;
      case 8: return &conn_AnalogOutput_2;
    }
    return nullptr;
  }

  CDataConnection *FORTE_IORevPiAIO::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::io::revpi
