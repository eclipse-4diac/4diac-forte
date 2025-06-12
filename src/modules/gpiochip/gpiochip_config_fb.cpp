/*******************************************************************************
 * Copyright (c) 2023 OFFIS e.V.
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

USE_STRING_ID(ActiveLow);
USE_STRING_ID(BiasMode);
USE_STRING_ID(BOOL);
USE_STRING_ID(ChipNumber);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(GPIOChip);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(LineNumber);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(ReadWriteMode);
USE_STRING_ID(STATUS);
USE_STRING_ID(UINT);
USE_STRING_ID(VALUE);
USE_STRING_ID(WSTRING);

#include "gpiochip_controller.h"

DEFINE_FIRMWARE_FB(GPIOChipConfigFB, STRID(GPIOChip))

namespace {
  const auto cDataInputNames =
      std::array{STRID(QI),       STRID(VALUE),    STRID(ChipNumber), STRID(LineNumber), STRID(ReadWriteMode),
                 STRID(BiasMode), STRID(ActiveLow)};
  const auto cDataOutputNames = std::array{STRID(QO), STRID(STATUS)};
  const auto cEventInputNames = std::array{STRID(INIT)};
  const auto cEventInputTypeIds = std::array{STRID(EInit)};
  const auto cEventOutputNames = std::array{STRID(INITO), STRID(IND)};
  const auto cEventOutputTypeIds = std::array{STRID(Event), STRID(Event)};
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

void GPIOChipConfigFB::setInitialValues() {
  ChipNumber() = 0_UINT;
  LineNumber() = 0_UINT;
  ReadWriteMode() = 0_UINT;
  BiasMode() = 0_UINT;
  ActiveLow() = 0_BOOL;
}

void GPIOChipConfigFB::readInputData(const TEventID paEIID) {
  for (auto i = 0; i < 7; i++) {
    readData(i, *mDIs[i], mDIConns[i]);
  }
}

void GPIOChipConfigFB::writeOutputData(const TEventID paEIID) {
  for (auto i = 0; i < 2; i++) {
    writeData(cFBInterfaceSpec.getNumDIs() + i, *mDOs[i], mDOConns[i]);
  }
}

forte::core::io::IODeviceController *GPIOChipConfigFB::createDeviceController(CDeviceExecution &paDeviceExecution) {
  return new GPIOChipController(paDeviceExecution);
}

void GPIOChipConfigFB::setConfig() {
  GPIOChipController::Config config;
  config.mChipNumber = TForteUInt16(ChipNumber());
  config.mLineNumber = TForteUInt16(LineNumber());
  config.mReadWriteMode = GPIOChipController::Config::ReadWriteMode(TForteUInt16(ReadWriteMode()));
  config.mBiasMode = GPIOChipController::Config::BiasMode(TForteUInt16(BiasMode()));
  config.mActiveLow = ActiveLow();
  getDeviceController()->setConfig(&config);
}

void GPIOChipConfigFB::onStartup(CEventChainExecutionThread *const paECET) {
  GPIOChipController::HandleDescriptor desc = GPIOChipController::HandleDescriptor(
      VALUE().getValue(),
      TForteUInt16(ReadWriteMode()) ? forte::core::io::IOMapper::Out : forte::core::io::IOMapper::In);
  initHandle(&desc);

  forte::core::io::IOConfigFBController::started(paECET);
}
