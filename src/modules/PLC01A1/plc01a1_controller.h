/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_MODULES_FESTO_CECC_FESTO_CONTROLLER_H_
#define SRC_MODULES_FESTO_CECC_FESTO_CONTROLLER_H_

#include <io/device/io_controller_poll.h>
#include <linux/spi/spidev.h>

class PLC01A1Controller : public forte::core::io::IODevicePollController {
  public:
    explicit PLC01A1Controller(CDeviceExecution &paDeviceExecution);

    struct Config : forte::core::io::IODeviceController::Config {
        unsigned int mUpdateInterval; //!< Sets the frequency for the data update cycle. The default value is 25 Hz.
    };

    class HandleDescriptor : public forte::core::io::IODeviceController::HandleDescriptor {
      public:
        uint8_t mOffset;
        uint8_t mPosition;

        HandleDescriptor(CIEC_WSTRING const &paId, forte::core::io::IOMapper::Direction paDirection, uint8_t paOffset, uint8_t paPosition) :
            forte::core::io::IODeviceController::HandleDescriptor(paId, paDirection), mOffset(paOffset), mPosition(paPosition) {

        }
    };

    void setConfig(struct forte::core::io::IODeviceController::Config* paConfig);

    virtual bool isHandleValueEqual(forte::core::io::IOHandle* paHandle);

    forte::core::io::IOHandle* initHandle(forte::core::io::IODeviceController::HandleDescriptor *paHandleDescriptor);

  protected:
    const char* init();
    void deInit();

    void poll();

  private:

    static const char *const scmSPIInputDevice;
    static const char *const scmSPIOutputDevice;

    int mSPIInputFd;
    int mSPIOutputFd;

    static const char * const scmFailedToOpenInputControlFile;
    static const char *const scmFailedToOpenOutputControlFile;

    static const uint32_t scmSPIMode;

    static const char *const scmFailedToSetInputMode;
    static const char *const scmFailedToSetOutputMode;

    static const uint8_t scmSPIBits;

    static const char *const scmFailedToSetInputBits;
    static const char *const scmFailedToSetOutputBits;

    static const uint32_t scmSPIInputMaxSpeed;
    static const uint32_t scmSPIOutputMaxSpeed;

    static const char *const scmFailedToSetInputSpeed;
    static const char *const scmFailedToSetOutputSpeed;

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

#endif /* SRC_MODULES_FESTO_CECC_FESTO_CONTROLLER_H_ */
