/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "EBSlave2181.h"
#include "../handler/bus.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::embrick {

  DEFINE_FIRMWARE_FB(FORTE_EBSlave2181, "eclipse4diac::io::embrick::EBSlave2181"_STRID)

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
                                            "DigitalOutput_1"_STRID,
                                            "DigitalOutput_2"_STRID,
                                            "DigitalOutput_3"_STRID,
                                            "DigitalOutput_4"_STRID,
                                            "DigitalOutput_5"_STRID,
                                            "DigitalOutput_6"_STRID,
                                            "DigitalOutput_7"_STRID,
                                            "DigitalOutput_8"_STRID,
                                            "UpdateInterval"_STRID};
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

  FORTE_EBSlave2181::FORTE_EBSlave2181(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      EmbrickSlave(scmSlaveConfigurationIO,
                   scmSlaveConfigurationIONum,
                   EmbrickSlaveHandler::G_8Di8Do,
                   paContainer,
                   cFBInterfaceSpec,
                   paInstanceNameId),
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
      conn_UpdateInterval(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {};

  void FORTE_EBSlave2181::setInitialValues() {
    var_QI = 0_BOOL;
    var_DigitalInput_1 = u""_WSTRING;
    var_DigitalInput_2 = u""_WSTRING;
    var_DigitalInput_3 = u""_WSTRING;
    var_DigitalInput_4 = u""_WSTRING;
    var_DigitalInput_5 = u""_WSTRING;
    var_DigitalInput_6 = u""_WSTRING;
    var_DigitalInput_7 = u""_WSTRING;
    var_DigitalInput_8 = u""_WSTRING;
    var_DigitalOutput_1 = u""_WSTRING;
    var_DigitalOutput_2 = u""_WSTRING;
    var_DigitalOutput_3 = u""_WSTRING;
    var_DigitalOutput_4 = u""_WSTRING;
    var_DigitalOutput_5 = u""_WSTRING;
    var_DigitalOutput_6 = u""_WSTRING;
    var_DigitalOutput_7 = u""_WSTRING;
    var_DigitalOutput_8 = u""_WSTRING;
    var_UpdateInterval = 0_UINT;
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_EBSlave2181::readInputData(const TEventID paEIID) {
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

  void FORTE_EBSlave2181::writeOutputData(const TEventID paEIID) {
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

  CIEC_ANY *FORTE_EBSlave2181::getDI(const size_t paIndex) {
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
      case 17: return &var_UpdateInterval;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_EBSlave2181::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_EBSlave2181::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_MAPO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_EBSlave2181::getDIConUnchecked(const TPortId paIndex) {
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
      case 17: return &conn_UpdateInterval;
    }
    return nullptr;
  }

  CDataConnection *FORTE_EBSlave2181::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  const TForteUInt8 FORTE_EBSlave2181::scmSlaveConfigurationIO[] = {17};
  const TForteUInt8 FORTE_EBSlave2181::scmSlaveConfigurationIONum = 1;

  void FORTE_EBSlave2181::initHandles() {
    // Initialize handles
    int iCount = 8;
    int oCount = 8;
    int iOffset = 1;
    int oOffset = iOffset + iCount;

    for (int i = 0; i < iCount; i++) {
      EmbrickBusHandler::HandleDescriptor desc = EmbrickBusHandler::HandleDescriptor(
          static_cast<CIEC_WSTRING *>(getDI(iOffset + i))->getValue(), forte::io::IOMapper::In, mIndex,
          EmbrickBusHandler::Bit, (uint8_t) (i / 8), (uint8_t) (i % 8));
      initHandle(desc);
    }

    for (int i = 0; i < oCount; i++) {
      EmbrickBusHandler::HandleDescriptor desc = EmbrickBusHandler::HandleDescriptor(
          static_cast<CIEC_WSTRING *>(getDI(oOffset + i))->getValue(), forte::io::IOMapper::Out, mIndex,
          EmbrickBusHandler::Bit, (uint8_t) (i / 8), (uint8_t) (i % 8));
      initHandle(desc);
    }
  }

} // namespace forte::eclipse4diac::io::embrick
