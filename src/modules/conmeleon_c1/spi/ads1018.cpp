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
 *  This class represents a 4 channel ADC from Texas Instruments Type ADS1018
 *  Please refer to the datasheet available at http://www.ti.com for details
 *  ADS1018 is derived from the CSpiDevice base class which uses the linux kernel spidev device driver.
 *
 *  Remark on SCLK timing:
 *  According to the timing requirements of the ADS1018 the minimum SCLK period duration is 250ns, which means
 *  the maximum SCLK frequency is 4MHz.
 *
 *  A SPI device (e.g. ADC) on bus X with chip select line Y will be available as /dev/spidevX.Y
 *  If you don't see this device in your file system, the spidev kernel module might not be loaded
 *  SPI devices have a limited userspace API, supporting basic half-duplex
 *  read() and write() access to SPI slave devices.  Using ioctl() requests,
 *  full duplex transfers and device I/O configuration are also available.
 *******************************************************************************/

#include "ads1018.h"
#include <unistd.h>

namespace CONMELEON {

/*! just call the constructor of the base class first and initialize own members afterwards
 *  0x05, 0x8B are the default configuration register settings according to the ADS1018 datasheet
 * SS  MUX2  MUX1  MUX0  PGA2  PGA1  PGA0  MODE  DR2  DR1  DR0  TS_MODE  PULL_UP_EN  NOP1  NOP0   NOT_USED
 * 0  0    0    0    0    1    0    1    1  0  0  0    1      0    1    1
 * -> MSB = 00000101 = 0x05, LSB = 10001011 = 0x8B
 *
 * AIN0 and AIN1 as differential input
 * PGA full scale voltage 2.048V
 * single shot conversion mode
 * data rate 1600 samples
 * analog input channel (internal temperature sensor disabled)
 * internal pull up resistors enabled
 * valid data, update config register
 */

CAds1018::CAds1018(const char* sDevice) : CSpiDevice(sDevice, 1000000, SPIMODE1) {

  m_ConfigRegister[0] = 0x05;     // MSB
  m_ConfigRegister[1] = 0x8B;     // LSB
  m_DataRegister[0] = 0x00;       // MSB
  m_DataRegister[1] = 0x00;       // LSB
  m_ReferenceVoltage = 3.3;
  m_VoltageDivider = 1.0;
  // default data rate is 1600 samples per second, so a single conversion takes around 625 microseconds
  m_ConversionDuration = 625;

}

CAds1018::CAds1018(const char* sDevice, unsigned int nSpeed, CONMELEON::ESpiMode enMode) : CSpiDevice(sDevice, nSpeed, enMode) {

  m_ConfigRegister[0] = 0x05;     // MSB
  m_ConfigRegister[1] = 0x8B;     // LSB
  m_DataRegister[0] = 0x00;       // MSB
  m_DataRegister[1] = 0x00;       // LSB
  m_ReferenceVoltage = 3.3;
  m_VoltageDivider = 1.0;
  m_ConversionDuration = 625;

}

void CAds1018::setChannelConfig (EChannelConfig enConfig) {

  unsigned char cBitMask = 0b01110000; // channel configuration is stored in bits [14:12] in the configuration register

  // most significant byte of configuration register is on index 0
  // reset bit [14:12] to "0" with the bitmask first and than "or" in the new bits
  m_ConfigRegister[0] = (m_ConfigRegister[0] & (~cBitMask)) | (enConfig << 4);

}

void CAds1018::setConversionMode (EConversionMode enMode) {

  unsigned char cBitMask = 0b00000001; // conversion mode is stored in bit [8] in the configuration register, which is the first bit in the MSB

  // most significant byte of configuration register is on index 0
  // reset bit [8] to "0" with the bitmask first and than "or" in the new bit
  m_ConfigRegister[0] = (m_ConfigRegister[0] & (~cBitMask)) | (enMode);

}

void CAds1018::setPGA (EPgaSetting enPga) {

  unsigned char cBitMask = 0b00001110; // pga setting is stored in bits [11:9] in the configuration register

  // most significant byte of configuration register is on index 0
  // reset bit [11:9] to "000" with the bitmask first and than "or" in the new bits
  m_ConfigRegister[0] = (m_ConfigRegister[0] & (~cBitMask)) | (enPga << 1);

}

void CAds1018::setSampleSpeed (EDataRate enSpeed) {

  unsigned char cBitMask = 0b11100000; // data rate speed setting is stored in bits [7:5] in the configuration register

  // least significant byte of configuration register is on index 1
  // reset bit [7:5] to "000" with the bitmask first and than "or" in the new bits
  m_ConfigRegister[1] = (m_ConfigRegister[1] & (~cBitMask)) | (enSpeed << 5);

  switch (enSpeed) {

  case SPS128:
        m_ConversionDuration = 7813;
        break;
  case SPS250:
        m_ConversionDuration = 4000;
        break;
  case SPS490:
        m_ConversionDuration = 2041;
        break;
  case SPS920:
        m_ConversionDuration = 1087;
        break;
  case SPS1600:
        m_ConversionDuration = 625;
        break;
  case SPS2400:
        m_ConversionDuration = 417;
        break;
  case SPS3300:
        m_ConversionDuration = 303;
        break;
  }

}

void CAds1018::setDataSource (EDataSource enSrc) {

  unsigned char cBitMask = 0b00010000; // setting for temperature readout is stored in bit [4] of the configuration register

  // least significant byte of configuration register is on index 1
  // reset bit [4] to "0" with the bitmask first and than "or" in the new bit
  m_ConfigRegister[1] = (m_ConfigRegister[1] & (~cBitMask)) | (enSrc << 4);
}

void CAds1018::setReferenceVoltage (const float fRefVoltage) {

  // check if fRevVoltage is within the range of the allowed supply voltages

  if (fRefVoltage < 2.0) {
    m_ReferenceVoltage = 2.0;
  }
  else if (fRefVoltage > 5.5) {
    m_ReferenceVoltage = 5.5;
  }
  else {
    m_ReferenceVoltage = fRefVoltage;
  }
}

void CAds1018::setVoltageDivider (const float fDivider) {

  // divider voltages must not be smaller than 1.0
  if (fDivider < 1.0) {
    m_VoltageDivider = 1.0;
  }
  else {
    m_VoltageDivider = fDivider;
  }
}

unsigned int CAds1018::readRawValue() {

  // first check if the ADS1018 is ready
  if (this->isOpen()) {

    // buffer to hold configuration, we operate in 32bit mode, initialize with the current configuration register settings
    unsigned char TX[4] = {m_ConfigRegister[0], m_ConfigRegister[1], m_ConfigRegister[0], m_ConfigRegister[1]};
    // buffer to hold received data, initialize with "0"
    unsigned char RX[4] = {0, };

    // set the single shot conversion start bit, if conversion mode is set to single shot
    TX[0] = TX[0] | (this->getConversionMode() << 7);
    TX[2] = TX[0];

    // we write the configuration register first without reading back anything, to trigger a single shot conversion if necessary
    if (CSpiDevice::write(TX, 4)) {

      // wait until the conversion is finished
      usleep(m_ConversionDuration);

      // reset the single shot conversion trigger bit again , because we don't want to trigger a new conversion
      TX[0] = TX[0] & 0b01111111;
      TX[2] = TX[0];

      // now we can read something back, data will be in the first two bytes MSB first
      if (CSpiDevice::transfer(TX, RX, 4)) {

        m_DataRegister[0] = RX[0];
        m_DataRegister[1] = RX[1];

        unsigned int  nRawVoltage = 0;   // used for bit shifting

        // TODO: handle negative voltages in case of differential inputs
        // copy data register bytes to 16 bit integer
        nRawVoltage = (RX[0] << 8) | RX[1];

        // now shift integer 4 bits to the right, because the 12 bit voltage value of the ADS1018 is left aligned
        nRawVoltage >>= 4;

        return nRawVoltage;
      }
    }
  }
  return 0;
}

int CAds1018::readVoltage(const unsigned int iChannelNr) {

  // iChannelNr is unsigned and valid channel numbers are 0..3
  // TODO: check for differential inputs as well, only single ended is handled now
  if (iChannelNr < 4) {

    // all channels are configured in single ended mode
    // TODO: also implement differential channels
    this->setChannelConfig(static_cast<EChannelConfig>(iChannelNr + 4));

    // set the data source to normal input channel first, just to be sure
    this->setDataSource(AnalogInput);

    unsigned int nRawVoltage = this->readRawValue();
    double fFullscaleVoltage = 0.0;

    // get current PGA setting to read current full scale voltage
    switch (this->getPgaSetting()) {
    case FS6144:
      fFullscaleVoltage = 6.144;
      break;
    case FS4096:
      fFullscaleVoltage = 4.096;
      break;
    case FS2048:
      fFullscaleVoltage = 2.048;
      break;
    case FS1024:
      fFullscaleVoltage = 1.024;
      break;
    case FS0512:
      fFullscaleVoltage = 0.512;
      break;
    case FS0256:
      fFullscaleVoltage = 0.256;
      break;
    }
    // we return the voltage in mV, that's why we use an additional factor of 1000.0
    return static_cast<int>(static_cast<double>(nRawVoltage)*m_VoltageDivider*fFullscaleVoltage*1000.0/2048.0);
  }
  else {
  // TODO: add logging or error handling
  }
  return 0.0;
}

int CAds1018::readTemperature() {

  // read internal temperature sensor of the ADS1018
  /*!
   *  Temperature readout of the ADS1018 internal temperature sensor:
   *   least significant bit equals 0.125 °C (temperature resolution)
   *   temperature is sampled in the same resolution (12bit) as the input channels
   *   if the most significant bit is "1" the temperature value is negative
   *   e.g. read value from the ADS1018 is 0x0F38 -> 111100111000
   *   1. check MSB bit [11] = 1 -> negative temperature
   *   2. generate the twos complement of 0x0F38 000011000111 + 000000000001 = 000011001000 -> 0x00C8
   *   readTemperature() will return 0x00C8 * (-0.125) = -25°C
   *
   *   e.g. read value from the ADS1018 is 0x0258 -> 001001011000
   *   1. check MSB bit [11] = 0 -> positive temperature
   *   2. no twos complement necessary
   *   readTemperature() will return 0x0258 * 0.125 = +75°C
   */

  // set the data source to internal temperature sensor
  this->setDataSource(Temperature);

  unsigned int   nRawTemperature = this->readRawValue();

  // check if MSB bit is "1", which means we have a negative temperature readout

  if (nRawTemperature & 0x8000) {
    // negative temperature, do a twos complement first, according to ADS1018 datasheet page 14
    nRawTemperature = ~nRawTemperature;
    nRawTemperature += 1;
  }

  // return 10th of degree, that's why we use a factor of 10.0
  return static_cast<int>(static_cast<double>(nRawTemperature) * scm_TemperatureFactor * 10.0);

}

}  // namespace CONMELEON

