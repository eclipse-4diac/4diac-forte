/*******************************************************************************
 * Copyright (c) 2026 Thomas Oellinger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Oellinger - initial implementation
 *******************************************************************************/

#include "MCC128.h"

#include "../boards/MCC128Board.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::mccdaqhat {

  DEFINE_FIRMWARE_FB(FORTE_MCC128, "eclipse4diac::io::mccdaqhat::MCC128"_STRID)

  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID,   "Address"_STRID, "InputMode"_STRID, "InputRange"_STRID,
                                            "AI_0"_STRID, "AI_1"_STRID,    "AI_2"_STRID,      "AI_3"_STRID,
                                            "AI_4"_STRID, "AI_5"_STRID,    "AI_6"_STRID,      "AI_7"_STRID};
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

  FORTE_MCC128::FORTE_MCC128(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      MCCIODevice(HatType::MCC128, paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_MAPO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_Address(nullptr),
      conn_InputMode(nullptr),
      conn_InputRange(nullptr),
      conn_AI{},
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {
  }

  void FORTE_MCC128::setInitialValues() {
    var_QI = 0_BOOL;
    var_Address = CIEC_USINT(0);
    var_InputMode = CIEC_USINT(0);
    var_InputRange = CIEC_USINT(0);
    for (auto &id : var_AI) {
      id = ""_STRING;
    }
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_MCC128::readInputData(const TEventID paEIID) {
    if (paEIID == scmEventMAPID) {
      for (TPortId port = 0; port < scmNumDIs; ++port) {
        readData(port, *getDI(port), *getDIConUnchecked(port));
      }
    }
  }

  void FORTE_MCC128::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventMAPOID: writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO); break;
      case scmEventINDID:
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        break;
      default: break;
    }
  }

  CIEC_ANY *FORTE_MCC128::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_Address;
      case 2: return &var_InputMode;
      case 3: return &var_InputRange;
    }
    return (paIndex < scmNumDIs) ? &var_AI[paIndex - scmFirstChannelDI] : nullptr;
  }

  CIEC_ANY *FORTE_MCC128::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_MCC128::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_MAPO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_MCC128::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_Address;
      case 2: return &conn_InputMode;
      case 3: return &conn_InputRange;
    }
    return (paIndex < scmNumDIs) ? &conn_AI[paIndex - scmFirstChannelDI] : nullptr;
  }

  CDataConnection *FORTE_MCC128::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  unsigned int FORTE_MCC128::getConfiguredAddress() {
    return static_cast<CIEC_USINT::TValueType>(var_Address);
  }

  TChannelUsage FORTE_MCC128::getChannelUsage() {
    TChannelUsage used{};
    for (size_t channel = 0; channel < scmMCC128Channels; ++channel) {
      used[channel] = var_AI[channel].length() > 0;
    }
    return used;
  }

  const char *FORTE_MCC128::init() {
    const unsigned int mode = static_cast<CIEC_USINT::TValueType>(var_InputMode);
    const unsigned int range = static_cast<CIEC_USINT::TValueType>(var_InputRange);
    const TChannelUsage used = getChannelUsage();
    mInitError = validateMCC128Config(mode, range, used);
    if (!mInitError.empty()) {
      return mInitError.c_str();
    }
    MCCDaqHatController &controller = getMCCController();
    // The framework calls init() before dropSlaveHandles(); handles of a previous MAP still point at the old board
    // object, which createBoard() replaces, so they have to go first.
    controller.dropSlaveHandles(mIndex);
    MCC128Board &board = controller.createBoard<MCC128Board>(mIndex);
    mInitError = board.open(static_cast<uint8_t>(mode), static_cast<uint8_t>(range), used);
    return mInitError.empty() ? nullptr : mInitError.c_str();
  }

  void FORTE_MCC128::initHandles() {
    for (size_t channel = 0; channel < scmMCC128Channels; ++channel) {
      addChannel(var_AI[channel], forte::io::IOMapper::In, MCCDaqHatController::HandleKind::AnalogIn,
                 static_cast<uint8_t>(channel));
    }
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
