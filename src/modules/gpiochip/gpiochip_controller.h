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

#pragma once

#include <io/device/io_controller.h>

class GPIOChipController : public forte::core::io::IODeviceController {
public:
  explicit GPIOChipController(CDeviceExecution &paDeviceExecution) :
    forte::core::io::IODeviceController(paDeviceExecution) {};

  struct Config : forte::core::io::IODeviceController::Config {
      unsigned int mChipNumber = 0;
      unsigned int mLineNumber = 0;
      enum ReadWriteMode {
        Input,
        PushPull,
        OpenDrain,
        OpenSource
      } mReadWriteMode = Input;
      enum BiasMode {
        None,
        PullUp,
        PullDown
      } mBiasMode = None;
      bool mActiveLow = false;
  };

  using forte::core::io::IODeviceController::HandleDescriptor;

  void setConfig(struct forte::core::io::IODeviceController::Config* paConfig) override {
    mConfig = *static_cast<Config*>(paConfig);
  }

  bool isHandleValueEqual(forte::core::io::IOHandle*) override {
    return false;
  }

  void handleChangeEvent(forte::core::io::IOHandle *paHandle) override;
  forte::core::io::IOHandle* initHandle(forte::core::io::IODeviceController::HandleDescriptor *paHandleDescriptor) override;

protected:
  const char* init() override;
  void deInit() override;
  void runLoop() override;

private:

  int mFd = -1;
  uint8_t mValue = 0;
  Config mConfig;

  static const char *const scmFailedToAccessChip;
  static const char *const scmFailedToAcquireLine;
  static const char *const scmFailedToWatchLine;
  static const char *const scmFailedToWriteLine;
  static const char *const scmFailedToReadLine;
  static const char *const scmOK;
};
