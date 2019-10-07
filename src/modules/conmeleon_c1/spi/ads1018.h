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

#ifndef SPI_ADS1018_H_
#define SPI_ADS1018_H_

#include "../spi/spidevice.h"

namespace CONMELEON {

//! sets the conversion mode of the ADS1018
/*! Continuous means that the device will be sampling continuously and the current value will be available
 *  in the data register, if it is not read it will be overwritten.
 *  SingleShot means that the device will do a single sample and will be powered down afterwards. The sampling
 *  needs to be triggered manually by setting the most significant bit (Bit 15) in the configuration register to "1"
 */
enum EConversionMode {
  Continuous,   /*!< the ADS1018 will start a new conversion, when the last one is finished */
  SingleShot     /*!< the ADS1018 will sample once if the trigger bit is set in the configuration register and will power down afterwards */
};

//! sets the data rate of the ADS1018
enum EDataRate {
  SPS128,   /*!< 128 samples per second */
  SPS250,   /*!< 250 samples per second */
  SPS490,   /*!< 490 samples per second */
  SPS920,   /*!< 920 samples per second */
  SPS1600,   /*!< 1600 samples per second (default setting) */
  SPS2400,   /*!< 2400 samples per second */
  SPS3300   /*!< 3300 samples per second */
};

//! sets the conversion source of the ADS1018
enum EDataSource {
  AnalogInput,   /*!< an analog input channel will be sampled, according to the channel configuration */
  Temperature   /*!< the internal temperature sensor will be sampled */
};

//! sets the full scale voltage of the internal gain amplifier of the ADS1018
/*!  Please note that the maximum input voltage must not exceed the device supply voltage Vdd + 0,3V.
 *   If your device is supplied with 3,3V you will have to use a setting of FS4096.
 */
enum EPgaSetting {
  FS6144,   /*!< full scale voltage is +-6,144V */
  FS4096,   /*!< full scale voltage is +-4,096V */
  FS2048,   /*!< full scale voltage is +-2,048V (default setting) */
  FS1024,   /*!< full scale voltage is +-1,024V */
  FS0512,   /*!< full scale voltage is +-0,512V */
  FS0256     /*!< full scale voltage is +-0,256V */
};

//! sets the input channel configuration of the ADS1018
enum EChannelConfig {
  DIF01,   /*!< AIN0 and AIN1 of the ADS1018 will be a single channel in differential mode (AIN0 + and AIN1 -, default setting) */
  DIF03,   /*!< AIN0 and AIN3 of the ADS1018 will be a single channel in differential mode (AIN0 + and AIN3 -) */
  DIF13,   /*!< AIN1 and AIN3 of the ADS1018 will be a single channel in differential mode (AIN1 + and AIN3 -) */
  DIF23,   /*!< AIN2 and AIN3 of the ADS1018 will be a single channel in differential mode (AIN2 + and AIN3 -) */
  SE0,   /*!< AIN0 of the ADS1018 will be in single ended mode mode (AIN0 + and GND -) */
  SE1,   /*!< AIN1 of the ADS1018 will be in single ended mode mode (AIN1 + and GND -) */
  SE2,   /*!< AIN2 of the ADS1018 will be in single ended mode mode (AIN2 + and GND -) */
  SE3   /*!< AIN0 of the ADS1018 will be in single ended mode mode (AIN3 + and GND -) */
};

/*! \class CAds1018
 *  \brief CAds1018 represents a 4 channel 12 bit analog to digital converter ADS1018 from Texas Instruments
 *
 *  The datasheet is available at http://www.ti.com/lit/ds/symlink/ads1018.pdf
 *  The ADS1018 can perform conversions up to a speed of 3300 samples per second and has got a built in programmble gain amplifier
 *  and a temperature sensor.
 *  The CAds1018 class is derived from the CSpiDevice base class
 */
class CAds1018 : public CSpiDevice {

public:

  /*! \fn CAds1018(const char* sDevice)
   * \brief creates an ADS1018  device according to the  \a sDevice parameter
   * \param sDevice gives the device path e.g. /dev/spidev0.1 in case of the CONMELEON C1 board
   *
   * The constructor sets the default parameters to m_nBitsPerWord = 8, m_enMode = SPIMODE1 and m_nSpeed to 1 MHz.
   */
  explicit CAds1018(const char* sDevice);

  /*! \fn CAds1018(const char* sDevice, unsigned int nSpeed, CONMELEON::ESpiMode enMode)
   * \brief creates a SPI device according to the  \a sDevice parameter
   * \param sDevice gives the device path e.g. /dev/spidev0.1
   * \param nSpeed gives the bus clock frequency e.g. 1000000 for 1 MHz
   * \param enMode gives the SPI bus mode setting e.g. SPIMODE1
   *
   * The constructor sets the default parameters to m_nBitsPerWord = 8
   */
  CAds1018(const char* sDevice, unsigned int nSpeed, CONMELEON::ESpiMode enMode);

  /*! \fn EChannelConfig   getChannelConfig()
   * \brief reads the current channel configuration of the ADS1018
   *
   * The channel configuration is stored in bits [14:12] in the configuration register.
   */
  EChannelConfig   getChannelConfig() { return static_cast<EChannelConfig>((m_ConfigRegister[0] & 0b01110000) >> 4); };

  /*! \fn EConversionMode getConversionMode()
   * \brief reads the current conversion mode setting of the ADS1018
   *
   * The conversion mode is stored in bit [8] in the configuration register, which is the first bit in the MSB
   */
  EConversionMode getConversionMode() { return static_cast<EConversionMode>(m_ConfigRegister[0] & 0b00000001); };

  /*! \fn EPgaSetting getPgaSetting()
   * \brief reads the current internal programmable gain amplifier setting of the ADS1018
   *
   * The pga setting is stored in bits [11:9] in the configuration register
   */
  EPgaSetting   getPgaSetting() { return static_cast<EPgaSetting>((m_ConfigRegister[0] & 0b00001110) >> 1); };

  /*! \fn EDataRate getDataRate()
   * \brief reads the current data rate of the ADS1018
   *
   * The data rate speed setting is stored in bits [7:5] in the configuration register
   */
  EDataRate     getDataRate() { return static_cast<EDataRate>((m_ConfigRegister[1] & 0b11100000) >> 5); };

  /*! \fn EDataSource  getDataSource()
   * \brief reads the current data source of the ADS1018
   *
   * The data source setting for analog input or temperature readout is stored in bit [4] of the configuration register
   */
  EDataSource    getDataSource() { return static_cast<EDataSource>((m_ConfigRegister[1] & 0b00010000) >> 4); };

  /*! \fn float getReferenceVoltage()
   * \brief reads the current reference voltage of the ADS1018
   *
   * The reference voltage of the ADS1018 normally is the supply voltage (3,3V in case of the CONMELEON C1 board).
   */
  double      getReferenceVoltage() { return m_ReferenceVoltage; };

  /*! \fn float getVoltageDivider()
   * \brief reads the current voltage divider factor
   *
   * The factor gives the voltage divider, which might be installed at the input of the ADS1018.
   * The value is 3,0 in case of the CONMELEON C1 board, because the nominal input voltage range is 10V DC and the maximum input voltage at the ADS1018 is 3,3V.
   * This value is needed to calculate the input voltage from the integer value read from the ADS1018 data register.
   */
  double      getVoltageDivider() { return m_VoltageDivider; };

  /*! \fn unsigned int getConversionDuration()
   * \brief reads the conversion duration in microseconds
   *
   * The conversion duration is calculated from the data rate automatically.
   * If you have configured a data rate of 3300 samples per second, this function will return 303 microseconds.
   */
  unsigned int  getConversionDuration() { return m_ConversionDuration; };

  /*! \fn void setChannelConfig (EChannelConfig enConfig)
   * \brief sets the input channel configuration of the ADS1018
   * \param enConfig gives the input channel configuration to set
   *
   *   It tells if the channel is differential or single ended and
   *   and which channel should be selected by the internal multiplexer of the ADS108.
   *   If not set explicitly, the default value is AIN0 and AIN1 in differential mode.
   */
  void setChannelConfig (EChannelConfig enConfig);

  /*! \fn void setConversionMode (EConversionMode enMode)
   * \brief sets the conversion mode of the ADS1018
   * \param enMode gives the conversion mode to set
   *
   *   Sets the conversion mode either to continuous or single shot.
   *   Default value is single shot for power consumption reasons.
   */
  void setConversionMode (EConversionMode enMode);

  /*! \fn void setPGA (EPgaSetting enPga)
   * \brief sets the internal programmable gain amplifier configuration of the ADS1018
   * \param enPga gives the full scale voltage to set
   *
   *   Default value is full scale range at +-2.048V.
   *  Attention:
   *  The input voltage level must not exceed the level of the device power supply voltage VDD + 0,3V.
   *  In case of the CONMELEON C1 board VDD is 3,3V.
   */
  void setPGA (EPgaSetting enPga);

  /*! \fn void setSampleSpeed (EDataRate enSpeed)
   * \brief sets the conversion speed of the ADS1018
   * \param enSpeed gives the conversion speed to set
   *
   *   Default value is 1600 samples per second.
   */
  void setSampleSpeed (EDataRate enSpeed);

  /*! \fn void setDataSource (EDataSource enSrc)
   * \brief sets the data source for conversion
   * \param enSrc gives the conversion source to set
   *
   *   Sets the data source to sample data from.
   *   Possible values are a normal voltage input channel or the internal temperature sensor
   */
  void setDataSource (EDataSource enSrc);

  /*! \fn void setReferenceVoltage (const float fRefVoltage)
   * \brief sets the reference voltage of the ADS1018
   * \param fRefVoltage gives the reference voltage value to set
   *
   *   Sets the reference voltage, which is needed to calculate the input channel voltage from the received 16bit data register.
   *   The ADS1018 uses an internal reference voltage, which is the same than the supply voltage applied to the device.
   *   According to the datasheet of the ADS1018 supply voltage range is between 2.0V and 5.5V, so the same values are tolerated for the reference voltage  value.
   */
  void setReferenceVoltage (const float fRefVoltage);

  /*! \fn void setVoltageDivider (const float fDivider)
   * \brief sets the factor of an external voltage divider circuit
   * \param fDivider gives the divider factor to set
   *
   *   Sets the value of an external voltage divider, which might be necessary to measure voltages larger then the supply voltage.
   *   The voltage divider value needs to be > 1.0
   */
  void setVoltageDivider (const float fDivider);

  /*! \fn int readVoltage(const unsigned int iChannelNr)
   * \brief reads the current voltage at the analog input channel given by \a iChannelNr in mV
   * \param iChannelNr gives the number of the channel to read from, valid numbers are between 0 and 3
   *
   *   If the conversion mode is set to "Continuous" just the data is read from the conversion register and scaled to a voltage value.
   *  If the conversion mode is set to "SingleShot" this function sets the MSB in the configuration register
   *  bit [15] to "1" to trigger a single shot conversion and reads the data afterwards.
   *  The function will wait for the number of microseconds given in the conversion duration setting to retrieve the data from the configuration register.
   *  In case of an error (e.g. invalid channel number) the function will return a value of 0,0.
   */
  int readVoltage(const unsigned int iChannelNr);

  /*! \fn int readTemperature()
   * \brief reads the current temperature in 10th of degree celsius from the internal temperature sensor.
   *
   */
  int readTemperature();

  unsigned char* getConfigRegister(void) { return &m_ConfigRegister[0]; }
  unsigned char* getDataRegister(void) { return &m_DataRegister[0]; }

private:
  //   Factor used to calculate temperature readout value, see datasheet of ADS1018
  static constexpr double scm_TemperatureFactor = 0.125;

  double      m_ReferenceVoltage;
  double      m_VoltageDivider;
  unsigned int  m_ConversionDuration;

  // m_ConfigRegister is used to keep a copy of the current configuration register settings
  //   according to the ADS1018 datasheet, the default config register setting is 0x058B
  unsigned char  m_ConfigRegister[2];
  unsigned char  m_DataRegister[2];

  // this function just reads a 16 bit value from the ADS1018 according to the configuration registers
  // it makes no difference if it is a voltage from an input or the internal temperature sensor
  unsigned int readRawValue();

};

}

#endif /* SPI_ADS1018_H_ */
