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

#include <io/mapper/io_handle_bit.h>
#include "plc01a1_controller.h"
#include <devlog.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

const char *const PLC01A1Controller::scmSPIInputDevice = "/dev/spidev0.0";
const char *const PLC01A1Controller::scmSPIOutputDevice = "/dev/spidev0.1";

const char *const PLC01A1Controller::scmFailedToOpenInputControlFile = "Failed to open input control file";
const char *const PLC01A1Controller::scmFailedToOpenOutputControlFile = "Failed to open output control file";

const char *const PLC01A1Controller::scmFailedToSetInputMode = "Failed to set input mode";
const char *const PLC01A1Controller::scmFailedToSetOutputMode = "Failed to set output mode";

const char *const PLC01A1Controller::scmFailedToSetInputBits = "Failed to set input bits";
const char *const PLC01A1Controller::scmFailedToSetOutputBits = "Failed to set output bits";

const char *const PLC01A1Controller::scmFailedToSetInputSpeed = "Failed to set input speed";
const char *const PLC01A1Controller::scmFailedToSetOutputSpeed = "Failed to set output speed";

const uint32_t PLC01A1Controller::scmSPIMode = 0;

const uint8_t PLC01A1Controller::scmSPIBits = 8;
const uint32_t PLC01A1Controller::scmSPIInputMaxSpeed = 6250000; //6.25 MHz
const uint32_t PLC01A1Controller::scmSPIOutputMaxSpeed = 5000000; //5 MHz



PLC01A1Controller::PLC01A1Controller(CDeviceExecution &paDeviceExecution) :
    forte::core::io::IODevicePollController(paDeviceExecution, 25), mSPIInputFd(0), mSPIOutputFd(0) {
  memset(mInputArrayOld, 0, scmInputArrayLenght);
  memset(mInputArray, 0, scmInputArrayLenght);
  memset(mOutputArray, 0, scmOutputArrayLenght);
  memset(mInputTX, 0, scmOutputArrayLenght);
  memset(mOutputRX, 0, scmOutputArrayLenght);

  memset(&mInputTR, 0, sizeof(struct spi_ioc_transfer));
  memset(&mOutputTR, 0, sizeof(struct spi_ioc_transfer));

  mInputTR.tx_buf = (unsigned long) mInputTX;
  mInputTR.rx_buf = (unsigned long) mInputArray;
  mInputTR.len = 2;
  mInputTR.speed_hz = scmSPIInputMaxSpeed;
  mInputTR.delay_usecs = 0;
  mInputTR.bits_per_word = scmSPIBits;

  mOutputTR.tx_buf = (unsigned long) mOutputArray;
  mOutputTR.rx_buf = (unsigned long) mOutputRX;
  mOutputTR.len = 2;
  mOutputTR.speed_hz = scmSPIOutputMaxSpeed;
  mOutputTR.delay_usecs = 0;
  mOutputTR.bits_per_word = scmSPIBits;

}

void PLC01A1Controller::setConfig(struct forte::core::io::IODeviceController::Config *paConfig) {
  Config newConfig = *static_cast<Config*>(paConfig);
  setPollInterval(static_cast<float>(newConfig.mUpdateInterval));
}

const char* PLC01A1Controller::init() {

  mSPIInputFd = open(scmSPIInputDevice, O_RDWR);
  if(mSPIInputFd < 0) {
    return scmFailedToOpenInputControlFile;
  }

  mSPIOutputFd = open(scmSPIOutputDevice, O_RDWR);
  if(mSPIOutputFd < 0) {
    return scmFailedToOpenOutputControlFile;
  }

  //SPI Mode
  int ret = ioctl(mSPIInputFd, SPI_IOC_WR_MODE32, &scmSPIMode);
  if(-1 == ret) {
    return scmFailedToSetInputMode;
  }

  ret = ioctl(mSPIOutputFd, SPI_IOC_WR_MODE32, &scmSPIMode);
  if(-1 == ret) {
    return scmFailedToSetOutputMode;
  }

  //bits per word
  ret = ioctl(mSPIInputFd, SPI_IOC_WR_BITS_PER_WORD, &scmSPIBits);
  if(-1 == ret) {
    return scmFailedToSetInputBits;
  }

  ret = ioctl(mSPIOutputFd, SPI_IOC_WR_BITS_PER_WORD, &scmSPIBits);
  if(-1 == ret) {
    return scmFailedToSetOutputBits;
  }

  // max speed hz
  ret = ioctl(mSPIInputFd, SPI_IOC_WR_MAX_SPEED_HZ, &scmSPIInputMaxSpeed);
    if(-1 == ret) {
    return scmFailedToSetInputSpeed;
  }

  ret = ioctl(mSPIOutputFd, SPI_IOC_WR_MAX_SPEED_HZ, &scmSPIOutputMaxSpeed);
  if(-1 == ret) {
    return scmFailedToSetOutputSpeed;
  }

  DEVLOG_INFO("[PLC01A1Controller]: Initialization Correct!\n");

  return 0;
}

void PLC01A1Controller::deInit() {
  if(mSPIInputFd != 0) {
    close(mSPIInputFd);
    mSPIInputFd = 0;
  }
  if(mSPIOutputFd != 0) {
    close(mSPIOutputFd);
    mSPIOutputFd = 0;
  }
}

void PLC01A1Controller::poll() {

  int ret = ioctl(mSPIInputFd, SPI_IOC_MESSAGE(1), &mInputTR);
  if(ret < 1) {
    DEVLOG_ERROR("[PLC01A1Controller]: Failed sending SPI message to input controller");
  }

  // Check for updates and fire events
  checkForInputChanges();

  // Copy image to old image
  memcpy(mInputArrayOld, mInputArray, scmInputArrayLenght);

  output_parity_bits();

  ret = ioctl(mSPIOutputFd, SPI_IOC_MESSAGE(1), &mOutputTR);
  if(ret < 1) {
    DEVLOG_ERROR("[PLC01A1Controller]: Failed sending SPI message to output controller");
  }

}

bool PLC01A1Controller::isHandleValueEqual(forte::core::io::IOHandle *paHandle) {
  return ((forte::core::io::IOHandleBit*) paHandle)->equal(mInputArrayOld);
}

forte::core::io::IOHandle* PLC01A1Controller::initHandle(forte::core::io::IODeviceController::HandleDescriptor *paHandleDescriptor) {
  HandleDescriptor desc = *static_cast<HandleDescriptor*>(paHandleDescriptor);

  return new forte::core::io::IOHandleBit(this, desc.mDirection, desc.mOffset, desc.mPosition,
    desc.mDirection == forte::core::io::IOMapper::In ? mInputArray : mOutputArray);
}

void PLC01A1Controller::output_parity_bits() {

  uint8_t outputBits[8] = { };
  uint8_t parityBits[4] = { };

  for(size_t i = 0; i < 8; i++) {
    outputBits[i] = mOutputArray[0] & (0x80 >> i);
    outputBits[i] = static_cast<uint8_t>(outputBits[i] >> (7 - i));
  }

  parityBits[3] = ((outputBits[7] ^ outputBits[5]) ^ outputBits[3]) ^ outputBits[1];
  parityBits[3] = (parityBits[3] == 0x01) ? 0x08 : 0x00;


  parityBits[2] = ((outputBits[6] ^ outputBits[4]) ^ outputBits[2]) ^ outputBits[0];
  parityBits[2] = (parityBits[2] == 0x01) ? 0x04 : 0x00;

  parityBits[1] = ((((((outputBits[7] ^ outputBits[6]) ^ outputBits[5]) ^ outputBits[4]) ^ outputBits[3])
    ^ outputBits[2])
    ^ outputBits[1]) ^ outputBits[0];
  parityBits[1] = (parityBits[1] == 0x01) ? 0x02 : 0x00;


  parityBits[0] = (parityBits[1] == 0x02) ? 0x00 : 0x01;

  mOutputArray[1] = parityBits[3] | parityBits[2] | parityBits[1] | parityBits[0];
}

