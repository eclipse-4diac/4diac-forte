/*******************************************************************************
 * Copyright (c) 2023, 2025 OFFIS e.V.
 *
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

#include "forte/io/device/io_controller.h"

namespace forte::eclipse4diac::io::gpiochip {

  class GPIOChipController : public forte::io::IODeviceController {
    public:
      explicit GPIOChipController(CDeviceExecution &paDeviceExecution) : IODeviceController(paDeviceExecution) {};

      struct Config : IODeviceController::Config {
          unsigned int mChipNumber = 0;
          unsigned int mLineNumber = 0;
          enum ReadWriteMode { Input, PushPull, OpenDrain, OpenSource } mReadWriteMode = Input;
          enum BiasMode { None, PullUp, PullDown } mBiasMode = None;
          bool mActiveLow = false;
      };

      using IODeviceController::HandleDescriptor;

      void setConfig(struct IODeviceController::Config *paConfig) override {
        mConfig = *static_cast<Config *>(paConfig);
      }

      bool isHandleValueEqual(forte::io::IOHandle &) override {
        return false;
      }

      void handleChangeEvent(forte::io::IOHandle *paHandle) override;
      ::forte::io::IOHandle *createIOHandle(HandleDescriptor &paHandleDescriptor) override;

    protected:
      const char *init() override;
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

} // namespace forte::eclipse4diac::io::gpiochip
