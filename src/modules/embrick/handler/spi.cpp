/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "spi.h"
#include <sstream>
#include <string>

unsigned long const EmbrickSPIHandler::scmDefaultSpiSpeed = 300000;
unsigned long const EmbrickSPIHandler::scmMaxSpiSpeed = 700000;
char const EmbrickSPIHandler::mSpiMode = SPI_CPHA;
char const EmbrickSPIHandler::mSpiBitOrder = 0; // MSB first

const char * const EmbrickSPIHandler::scmFailedToInitHandler = "Failed to init spidev handler. Check if spi is enabled.";
const char * const EmbrickSPIHandler::scmFailedToConfigMode = "Failed to config spi write mode.";
const char * const EmbrickSPIHandler::scmFailedToConfigBitOrder = "Failed to config spi bit order.";
const char * const EmbrickSPIHandler::scmFailedToConfigSpeed = "Failed to config spi speed.";
const char * const EmbrickSPIHandler::scmFailedToTestBus = "Failed to send test byte to spi.";
const char * const EmbrickSPIHandler::scmFailedToTransferBuffer = "Failed to transfer buffer via spi.";

EmbrickSPIHandler::EmbrickSPIHandler(unsigned int paInterface) :
    mError(0) {
  // Convert int to string
  std::ostringstream interfaceStream;
  interfaceStream << paInterface;
  std::string interfaceStr = interfaceStream.str();

  std::string spidev = "/dev/spidev" + interfaceStr + ".0";
  init(spidev.c_str());
}

EmbrickSPIHandler::~EmbrickSPIHandler() {
  deInit();
}

void EmbrickSPIHandler::init(const char* paSpidev) {
  // Init spidev
  DEVLOG_DEBUG("emBrick[SPIHandler]: Open spidev '%s'\n", paSpidev);
  mFd = open(paSpidev, O_RDWR);

  if(mFd < 0) {
    return fail(scmFailedToInitHandler);
  }
  // Set mode to SPI_CPOL and SPI_CPHA
  if(!config(SPI_IOC_WR_MODE, SPI_IOC_RD_MODE, mSpiMode)) {
    return fail(scmFailedToConfigMode);
  }
  // Set MSB (Most Significant Bit First)
  if(!config(SPI_IOC_WR_LSB_FIRST, SPI_IOC_RD_LSB_FIRST, mSpiBitOrder)) {
    return fail(scmFailedToConfigBitOrder);
  }
  // Set speed
  if(!config(SPI_IOC_WR_MAX_SPEED_HZ, SPI_IOC_RD_MAX_SPEED_HZ, scmMaxSpiSpeed)) {
    return fail(scmFailedToConfigSpeed);
  }
  mSpiSpeed = scmDefaultSpiSpeed;

  // Test
  unsigned char testByte[1] = { 0 };
  if(!transfer(testByte, testByte, 1)) {
    return fail(scmFailedToTestBus);
  }
  DEVLOG_INFO("emBrick[SPIHandler]: Ready.\n");
}

void EmbrickSPIHandler::deInit() {
  // Close handler if open
  if(mFd >= 0) {
    close(mFd);
  }
}

template<typename T> bool EmbrickSPIHandler::config(unsigned int paConfig, unsigned int paConfigVerify, T paValue) {
  int status;

  // Set config via ioctl
  status = ioctl(mFd, paConfig, &paValue);
  if(status < 0) {
    return false;
  }
  // Verify config
  T valueCheck;
  status = ioctl(mFd, paConfigVerify, &valueCheck);
  if(status < 0) {
    return false;
  }
  return valueCheck == paValue;
}

void EmbrickSPIHandler::fail(const char* paReason) {
  mError = paReason;
  DEVLOG_ERROR("emBrick[SPIHandler]: %s\n", mError);
}

bool EmbrickSPIHandler::transfer(unsigned char* paSendBuffer, unsigned char* paReceiveBuffer, int paLength) {

  struct spi_ioc_transfer msg;

  msg.tx_buf = (unsigned long) paSendBuffer;
  msg.rx_buf = (unsigned long) paReceiveBuffer;
  msg.len = paLength;
  msg.delay_usecs = 0;
  msg.speed_hz = mSpiSpeed;
  msg.bits_per_word = 8;
  msg.cs_change = 0;
  msg.pad = 0;

  // Send data to spi bus
  int status = ioctl(mFd, SPI_IOC_MESSAGE(1), &msg);
  if(status < 0) {
    fail(scmFailedToTransferBuffer);
    return false;
  }

  return true;
}

void EmbrickSPIHandler::setSpeed(const unsigned long paSpeed) {
  mSpiSpeed = paSpeed;
}

