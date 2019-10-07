/*******************************************************************************
 * Copyright (c) 2016 Herwig Eichler, www.conmeleon.org
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Herwig Eichler  - initial API and implementation and initial documentation
 *
 *  This code has been inspired by the work of Philippe Van Hecke <lemouchon(at).gmail.com>
 *
 *  This class uses the linux kernel spidev device driver.
 *
 *  A SPI device (e.g. ADC) on bus X with chip select line Y will be available as /dev/spidevX.Y
 *  If you don't see this device in your file system, the spidev kernel module might not be loaded
 *  SPI devices have a limited userspace API, supporting basic half-duplex
 *  read() and write() access to SPI slave devices.  Using ioctl() requests,
 *  full duplex transfers and device I/O configuration are also available.
 *
 *  advantage:
 *  - portability, because spidev is part of the linux kernel
 *
 *  disadvantage:
 *  - speed, it is slower than direct hardware register access and memory mapping
 *  - runs in user space and is therefore not time deterministic
 *******************************************************************************/

// the following includes are needed for the spi kernel driver interface

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

// other includes

#include <cstring>    // needed for memset
#include "spidevice.h"

namespace CONMELEON {

/*!
 *   spi_mode_0 is commonly used
 *   8 bits per word is fine on most systems except for some low speed devices like LCD displays
 *   1000000 bits per second is a good choice
 */
CSpiDevice::CSpiDevice(const char* sDevice) : m_nBitsPerWord(8), m_nSpeed(1000000), m_enMode(SPIMODE0) {

  m_bValid = openAndConfigureBus(sDevice);
}

CSpiDevice::CSpiDevice(const char* sDevice, unsigned int nSpeed, ESpiMode enMode)
  : m_nBitsPerWord(8), m_nSpeed(nSpeed), m_enMode(enMode) {

  m_bValid = openAndConfigureBus(sDevice);
}

CSpiDevice::~CSpiDevice() {

  close(m_nFileDescriptor);
}

bool CSpiDevice::openAndConfigureBus(const char* sDevice) {

  // open the SPI device, e.g. /dev/spidev0.0
  // in normal linux system configuration, you need to have root privileges to do that
  m_nFileDescriptor = open(sDevice, O_RDWR);
  if (m_nFileDescriptor < 0) {
    // opening the device failed, so there is no need to continue
    // TODO: add logging and error handling
    return false;
  }
  else {

    bool  bStateOk = false; //this value is not used until line 85 when is re-assigned, but not defined there because in 82 there's work to do.
    // setting the SPI mode and read back
    int nRetVal = ioctl(m_nFileDescriptor, SPI_IOC_WR_MODE, &m_enMode);

    if (nRetVal < 0) {
      // TODO: add logging and error handling
    }

    bStateOk = (nRetVal >= 0);

    nRetVal = ioctl(m_nFileDescriptor, SPI_IOC_RD_MODE, &m_enMode);

    if (nRetVal < 0) {
      // TODO: add logging and error handling
    }

    // preserve a false state of a previous function call with the AND operator
    bStateOk = bStateOk && (nRetVal >= 0);

    // setting the SPI bits per word
    nRetVal = ioctl(m_nFileDescriptor, SPI_IOC_WR_BITS_PER_WORD, &m_nBitsPerWord);

    if (nRetVal < 0) {
      // TODO: add logging and error handling
    }

    bStateOk = bStateOk && (nRetVal >= 0);

    nRetVal = ioctl(m_nFileDescriptor, SPI_IOC_RD_BITS_PER_WORD, &m_nBitsPerWord);

    if (nRetVal < 0) {
      // TODO: add logging and error handling
    }

    bStateOk = bStateOk && (nRetVal >= 0);

    // setting the SPI speed
    nRetVal = ioctl(m_nFileDescriptor, SPI_IOC_WR_MAX_SPEED_HZ, &m_nSpeed);

    if (nRetVal < 0) {
      // TODO: add logging and error handling
    }

    bStateOk = bStateOk && (nRetVal >= 0);

    nRetVal = ioctl(m_nFileDescriptor, SPI_IOC_RD_MAX_SPEED_HZ, &m_nSpeed);

    if (nRetVal < 0) {
      // TODO: add logging and error handling
    }

    bStateOk = bStateOk && (nRetVal >= 0);

    return bStateOk;
  }
}

bool CSpiDevice::write(const unsigned char* paData, int nLength) {

  // check input parameters before writing anything
  if ((paData != nullptr) && m_bValid && ( nLength > 0 )) {
    // call write() method of linux/spi/spidev.h
     ssize_t nRetVal = ::write(m_nFileDescriptor, paData, nLength);

    // no logging here, because write() might be called often and will flood the error output stream
    return (nRetVal >= 0);

  }
  else {
    return false;
  }
}

bool CSpiDevice::read(unsigned char* paData, int nLength) {

  // check input parameters before reading anything
  if ((paData != nullptr) && m_bValid && ( nLength > 0 )) {
    // call read() method of linux/spi/spidev.h
     ssize_t nRetVal = ::read(m_nFileDescriptor, paData, nLength);

    // no logging here, because read() might be called often and will flood the error output stream
    return (nRetVal >= 0);

  }
  else {
    return false;
  }
}

bool CSpiDevice::transfer(const unsigned char* paTxData, unsigned char* paRxData, unsigned int nLength) {

  if ( (nLength > 0) && m_bValid ){
    /* the struct spi_ios_transfer represents the data of a full SPI communication cycle
     * it is defined in linux/spi/spidev.h
     * paTxData is the buffer to hold the data sent and paRxData the data received
     */
    struct spi_ioc_transfer spiMsg;

    // initialize spi_ioc_transfer structure first, see description in linux/spi/spidev.h
    std::memset(&spiMsg,0,sizeof(spiMsg));

    spiMsg.tx_buf = (unsigned long)(paTxData);         // transmit a byte from paTxData buffer, static_cast<unsigned long> does not work here
    spiMsg.rx_buf = (unsigned long)(paRxData);         // receive response from device in the paRxData buffer
    spiMsg.len = nLength;                  // doesn't matter which of the buffers we use for len, they hold the same data type
    spiMsg.delay_usecs = 0;                  // if non zero, delay after the transmission of the last byte
                                // before the device will be deselected, in case cs_change is true
    spiMsg.cs_change = 0;                  // set to true, if device should be deselected before new transfer starts
    spiMsg.bits_per_word = m_nBitsPerWord;
    spiMsg.speed_hz = m_nSpeed;

    // now hand the data over to the kernel SPI driver and read back at the same time
    int nRetVal = ioctl (m_nFileDescriptor, SPI_IOC_MESSAGE(1), &spiMsg);

    // no logging here, because transfer() might be called often and will flood the error output stream
    return (nRetVal >= 0);
  }
  else {

    return false;
  }
}

}


