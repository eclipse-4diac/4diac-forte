/*******************************************************************************
 * Copyright (c) 2019, 2025 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/io/device/io_controller_poll.h"
#include <linux/spi/spidev.h>

namespace forte::eclipse4diac::io::plc01a1 {

  class PLC01A1Controller : public forte::io::IODevicePollController {
    public:
      explicit PLC01A1Controller(CDeviceExecution &paDeviceExecution);

      struct Config : IODeviceController::Config {
          unsigned int mUpdateInterval; //!< Sets the frequency for the data update cycle. The default value is 25 Hz.
      };

      class HandleDescriptor : public IODeviceController::HandleDescriptor {
        public:
          uint8_t mOffset;
          uint8_t mPosition;

          HandleDescriptor(std::string const &paId,
                           forte::io::IOMapper::Direction paDirection,
                           uint8_t paOffset,
                           uint8_t paPosition) :
              IODeviceController::HandleDescriptor(paId, paDirection),
              mOffset(paOffset),
              mPosition(paPosition) {
          }
      };

      void setConfig(struct IODeviceController::Config *paConfig) override;

      bool isHandleValueEqual(forte::io::IOHandle &paHandle) override;

      forte::io::IOHandle *createIOHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) override;

    protected:
      const char *init() override;
      void deInit() override;

      void poll() override;

    private:
      int mSPIInputFd;
      int mSPIOutputFd;

      static const size_t scmInputArrayLenght = 2;
      static const size_t scmOutputArrayLenght = 2;

      uint8_t mInputArrayOld[scmInputArrayLenght];
      uint8_t mInputArray[scmInputArrayLenght];
      uint8_t mOutputArray[scmOutputArrayLenght];

      uint8_t mInputTX[scmInputArrayLenght];
      uint8_t mOutputRX[scmInputArrayLenght];

      struct spi_ioc_transfer mInputTR;
      struct spi_ioc_transfer mOutputTR;

      void output_parity_bits();
  };

} // namespace forte::eclipse4diac::io::plc01a1
