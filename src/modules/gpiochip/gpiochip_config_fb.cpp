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

using namespace forte::core::literals;

#include "gpiochip_controller.h"

DEFINE_FIRMWARE_FB(GPIOChipConfigFB, "GPIOChip"_STRID)

namespace {
  const auto cDataInputNames =
      std::array{"QI"_STRID,       "VALUE"_STRID,    "ChipNumber"_STRID, "LineNumber"_STRID, "ReadWriteMode"_STRID,
                 "BiasMode"_STRID, "ActiveLow"_STRID};
  const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
  const auto cEventInputNames = std::array{"INIT"_STRID};
  const auto cEventInputTypeIds = std::array{"EInit"_STRID};
  const auto cEventOutputNames = std::array{"INITO"_STRID, "IND"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
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
