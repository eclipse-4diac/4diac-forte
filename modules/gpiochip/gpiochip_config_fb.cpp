/*******************************************************************************
 * Copyright (c) 2023, 2025 OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jörg Walter - initial implementation
 *******************************************************************************/

#include "gpiochip_config_fb.h"
#include "gpiochip_controller.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::gpiochip {

  namespace {
    const auto cEventInputNames = std::array{"INIT"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "IND"_STRID};
    const auto cDataInputNames =
        std::array{"QI"_STRID,       "VALUE"_STRID,    "ChipNumber"_STRID, "LineNumber"_STRID, "ReadWriteMode"_STRID,
                   "BiasMode"_STRID, "ActiveLow"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_GPIOChip, "eclipse4diac::io::gpiochip::GPIOChip"_STRID)

  FORTE_GPIOChip::FORTE_GPIOChip(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      IOConfigFBController(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QI(0_BOOL),
      var_VALUE(u""_WSTRING),
      var_ChipNumber(0_UINT),
      var_LineNumber(0_UINT),
      var_ReadWriteMode(0_UINT),
      var_BiasMode(0_UINT),
      var_ActiveLow(0_BOOL),
      var_QO(0_BOOL),
      var_STATUS(u""_WSTRING),
      conn_INITO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_VALUE(nullptr),
      conn_ChipNumber(nullptr),
      conn_LineNumber(nullptr),
      conn_ReadWriteMode(nullptr),
      conn_BiasMode(nullptr),
      conn_ActiveLow(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {
  }

  void FORTE_GPIOChip::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_QI = 0_BOOL;
    var_VALUE = u""_WSTRING;
    var_ChipNumber = 0_UINT;
    var_LineNumber = 0_UINT;
    var_ReadWriteMode = 0_UINT;
    var_BiasMode = 0_UINT;
    var_ActiveLow = 0_BOOL;
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_GPIOChip::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_VALUE, conn_VALUE);
        readData(2, var_ChipNumber, conn_ChipNumber);
        readData(3, var_LineNumber, conn_LineNumber);
        readData(4, var_ReadWriteMode, conn_ReadWriteMode);
        readData(5, var_BiasMode, conn_BiasMode);
        readData(6, var_ActiveLow, conn_ActiveLow);
        break;
      }
      default: break;
    }
  }

  void FORTE_GPIOChip::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(7, var_QO, conn_QO);
        writeData(8, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventINDID: {
        writeData(7, var_QO, conn_QO);
        writeData(8, var_STATUS, conn_STATUS);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_GPIOChip::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_VALUE;
      case 2: return &var_ChipNumber;
      case 3: return &var_LineNumber;
      case 4: return &var_ReadWriteMode;
      case 5: return &var_BiasMode;
      case 6: return &var_ActiveLow;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_GPIOChip::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_GPIOChip::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_GPIOChip::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_VALUE;
      case 2: return &conn_ChipNumber;
      case 3: return &conn_LineNumber;
      case 4: return &conn_ReadWriteMode;
      case 5: return &conn_BiasMode;
      case 6: return &conn_ActiveLow;
    }
    return nullptr;
  }

  CDataConnection *FORTE_GPIOChip::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  forte::io::IODeviceController *FORTE_GPIOChip::createDeviceController(CDeviceExecution &paDeviceExecution) {
    return new GPIOChipController(paDeviceExecution);
  }

  void FORTE_GPIOChip::setConfig() {
    GPIOChipController::Config config;
    config.mChipNumber = TForteUInt16(var_ChipNumber);
    config.mLineNumber = TForteUInt16(var_LineNumber);
    config.mReadWriteMode = GPIOChipController::Config::ReadWriteMode(TForteUInt16(var_ReadWriteMode));
    config.mBiasMode = GPIOChipController::Config::BiasMode(TForteUInt16(var_BiasMode));
    config.mActiveLow = var_ActiveLow;
    getDeviceController()->setConfig(&config);
  }

  void FORTE_GPIOChip::onStartup(CEventChainExecutionThread *const paECET) {
    GPIOChipController::HandleDescriptor desc = GPIOChipController::HandleDescriptor(
        var_VALUE.getValue(), TForteUInt16(var_ReadWriteMode) ? forte::io::IOMapper::Out : forte::io::IOMapper::In);
    initHandle(desc);
    started(paECET);
  }

} // namespace forte::eclipse4diac::io::gpiochip
