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

#include "MCC152.h"

#include "../boards/MCC152Board.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::mccdaqhat {

  DEFINE_FIRMWARE_FB(FORTE_MCC152, "eclipse4diac::io::mccdaqhat::MCC152"_STRID)

  namespace {
    const auto cDataInputNames =
        std::array{"QI"_STRID,   "Address"_STRID, "PullEnable"_STRID, "PullConfig"_STRID, "DI_0"_STRID, "DI_1"_STRID,
                   "DI_2"_STRID, "DI_3"_STRID,    "DI_4"_STRID,       "DI_5"_STRID,       "DI_6"_STRID, "DI_7"_STRID,
                   "DO_0"_STRID, "DO_1"_STRID,    "DO_2"_STRID,       "DO_3"_STRID,       "DO_4"_STRID, "DO_5"_STRID,
                   "DO_6"_STRID, "DO_7"_STRID,    "AO_0"_STRID,       "AO_1"_STRID};
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

  FORTE_MCC152::FORTE_MCC152(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      MCCIODevice(HatType::MCC152, paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_PullEnable(CIEC_BYTE(0xFF)),
      var_PullConfig(CIEC_BYTE(0xFF)),
      conn_MAPO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_Address(nullptr),
      conn_PullEnable(nullptr),
      conn_PullConfig(nullptr),
      conn_DI{},
      conn_DO{},
      conn_AO{},
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {
  }

  void FORTE_MCC152::setInitialValues() {
    var_QI = 0_BOOL;
    var_Address = CIEC_USINT(0);
    // Matches the MCC 152 state after mcc152_dio_reset(): all pull resistors enabled as pull-ups.
    var_PullEnable = CIEC_BYTE(0xFF);
    var_PullConfig = CIEC_BYTE(0xFF);
    for (auto &id : var_DI) {
      id = ""_STRING;
    }
    for (auto &id : var_DO) {
      id = ""_STRING;
    }
    for (auto &id : var_AO) {
      id = ""_STRING;
    }
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_MCC152::readInputData(const TEventID paEIID) {
    if (paEIID == scmEventMAPID) {
      for (TPortId port = 0; port < scmNumDIs; ++port) {
        readData(port, *getDI(port), *getDIConUnchecked(port));
      }
    }
  }

  void FORTE_MCC152::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventMAPOID: writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO); break;
      case scmEventINDID:
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        break;
      default: break;
    }
  }

  CIEC_ANY *FORTE_MCC152::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_Address;
      case 2: return &var_PullEnable;
      case 3: return &var_PullConfig;
    }
    if (paIndex < scmFirstDOPortDI) {
      return &var_DI[paIndex - scmFirstDIPortDI];
    }
    if (paIndex < scmFirstAOPortDI) {
      return &var_DO[paIndex - scmFirstDOPortDI];
    }
    return (paIndex < scmNumDIs) ? &var_AO[paIndex - scmFirstAOPortDI] : nullptr;
  }

  CIEC_ANY *FORTE_MCC152::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_MCC152::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_MAPO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_MCC152::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_Address;
      case 2: return &conn_PullEnable;
      case 3: return &conn_PullConfig;
    }
    if (paIndex < scmFirstDOPortDI) {
      return &conn_DI[paIndex - scmFirstDIPortDI];
    }
    if (paIndex < scmFirstAOPortDI) {
      return &conn_DO[paIndex - scmFirstDOPortDI];
    }
    return (paIndex < scmNumDIs) ? &conn_AO[paIndex - scmFirstAOPortDI] : nullptr;
  }

  CDataConnection *FORTE_MCC152::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  unsigned int FORTE_MCC152::getConfiguredAddress() {
    return static_cast<CIEC_USINT::TValueType>(var_Address);
  }

  TChannelUsage FORTE_MCC152::usageOf(const std::array<CIEC_STRING, scmMCC152DioChannels> &paIds) {
    TChannelUsage used{};
    for (size_t bit = 0; bit < scmMCC152DioChannels; ++bit) {
      used[bit] = paIds[bit].length() > 0;
    }
    return used;
  }

  const char *FORTE_MCC152::init() {
    uint8_t directionMask = 0;
    mInitError = computeMCC152DirectionMask(usageOf(var_DI), usageOf(var_DO), directionMask);
    if (!mInitError.empty()) {
      return mInitError.c_str();
    }
    MCCDaqHatController &controller = getMCCController();
    // The framework calls init() before dropSlaveHandles(); handles of a previous MAP still point at the old board
    // object, which createBoard() replaces, so they have to go first.
    controller.dropSlaveHandles(mIndex);
    MCC152Board &board = controller.createBoard<MCC152Board>(mIndex);
    mInitError = board.open(directionMask, static_cast<TForteByte>(var_PullEnable),
                            static_cast<TForteByte>(var_PullConfig));
    return mInitError.empty() ? nullptr : mInitError.c_str();
  }

  void FORTE_MCC152::initHandles() {
    for (size_t bit = 0; bit < scmMCC152DioChannels; ++bit) {
      addChannel(var_DI[bit], forte::io::IOMapper::In, MCCDaqHatController::HandleKind::DigitalIn,
                 static_cast<uint8_t>(bit));
      addChannel(var_DO[bit], forte::io::IOMapper::Out, MCCDaqHatController::HandleKind::DigitalOut,
                 static_cast<uint8_t>(bit));
    }
    for (size_t channel = 0; channel < scmMCC152AoChannels; ++channel) {
      addChannel(var_AO[channel], forte::io::IOMapper::Out, MCCDaqHatController::HandleKind::AnalogOut,
                 static_cast<uint8_t>(channel));
    }
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
