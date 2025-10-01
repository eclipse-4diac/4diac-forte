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

#include "EBSlave2301.h"
#include "../handler/bus.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::embrick {

  DEFINE_FIRMWARE_FB(FORTE_EBSlave2301, "eclipse4diac::io::embrick::EBSlave2301"_STRID)

  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID,      "Relay_1"_STRID, "Relay_2"_STRID, "Relay_3"_STRID,
                                            "Relay_4"_STRID, "Relay_5"_STRID, "Relay_6"_STRID, "UpdateInterval"_STRID};
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

  FORTE_EBSlave2301::FORTE_EBSlave2301(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      EmbrickSlave(scmSlaveConfigurationIO,
                   scmSlaveConfigurationIONum,
                   EmbrickSlaveHandler::G_2RelNo4RelCo,
                   paContainer,
                   cFBInterfaceSpec,
                   paInstanceNameId),
      conn_MAPO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_Relay_1(nullptr),
      conn_Relay_2(nullptr),
      conn_Relay_3(nullptr),
      conn_Relay_4(nullptr),
      conn_Relay_5(nullptr),
      conn_Relay_6(nullptr),
      conn_UpdateInterval(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {};

  void FORTE_EBSlave2301::setInitialValues() {
    var_QI = 0_BOOL;
    var_Relay_1 = u""_WSTRING;
    var_Relay_2 = u""_WSTRING;
    var_Relay_3 = u""_WSTRING;
    var_Relay_4 = u""_WSTRING;
    var_Relay_5 = u""_WSTRING;
    var_Relay_6 = u""_WSTRING;
    var_UpdateInterval = 0_UINT;
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_EBSlave2301::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventMAPID: {
        readData(1, var_Relay_1, conn_Relay_1);
        readData(2, var_Relay_2, conn_Relay_2);
        readData(3, var_Relay_3, conn_Relay_3);
        readData(4, var_Relay_4, conn_Relay_4);
        readData(5, var_Relay_5, conn_Relay_5);
        readData(6, var_Relay_6, conn_Relay_6);
        readData(0, var_QI, conn_QI);
        break;
      }
      default: break;
    }
  }

  void FORTE_EBSlave2301::writeOutputData(const TEventID paEIID) {
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

  CIEC_ANY *FORTE_EBSlave2301::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_Relay_1;
      case 2: return &var_Relay_2;
      case 3: return &var_Relay_3;
      case 4: return &var_Relay_4;
      case 5: return &var_Relay_5;
      case 6: return &var_Relay_6;
      case 7: return &var_UpdateInterval;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_EBSlave2301::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_EBSlave2301::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_MAPO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_EBSlave2301::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_Relay_1;
      case 2: return &conn_Relay_2;
      case 3: return &conn_Relay_3;
      case 4: return &conn_Relay_4;
      case 5: return &conn_Relay_5;
      case 6: return &conn_Relay_6;
      case 7: return &conn_UpdateInterval;
    }
    return nullptr;
  }

  CDataConnection *FORTE_EBSlave2301::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  const TForteUInt8 FORTE_EBSlave2301::scmSlaveConfigurationIO[] = {7};
  const TForteUInt8 FORTE_EBSlave2301::scmSlaveConfigurationIONum = 1;

  void FORTE_EBSlave2301::initHandles() {
    // Initialize handles
    int iCount = 0;
    int oCount = 6;
    int iOffset = 1;
    int oOffset = iOffset + iCount;

    for (int i = 0; i < oCount; i++) {
      EmbrickBusHandler::HandleDescriptor desc = EmbrickBusHandler::HandleDescriptor(
          static_cast<CIEC_WSTRING *>(getDI(oOffset + i))->getValue(), forte::io::IOMapper::Out, mIndex,
          EmbrickBusHandler::Bit, (uint8_t) (i / 8), (uint8_t) (i % 8));
      initHandle(desc);
    }
  }

} // namespace forte::eclipse4diac::io::embrick
