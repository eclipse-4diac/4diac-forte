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

#ifndef SPI_SPIDEVICE_H_
#define SPI_SPIDEVICE_H_

#include "../util/uncopyable.h"
#include <string>

namespace CONMELEON {

//! sets the mode of the SPI bus
/*! Clock polarity (CPOL) and clock phase (CPHA) determine the edges of the clock signal on which the data is driven and sampled (latched).
 *   Each of the two parameters has two possible states, which allows for four possible combinations, all of which are incompatible with one another.
 *
 *   - SPI_MODE_0   CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
 *  - SPI_MODE_1   CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
 *  - SPI_MODE_2   CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
 *  - SPI_MODE_3   CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge
 *
 *  Most of the devices use SPI_MODE_0 and SPI_MODE_1, but the datasheet of the device should always be checked.
 *  You can find a nice paper based on the Linux kernel documentation here <http://www.abcelectronique.com/forum/attachment.php?attachmentid=28879>
 */
enum ESpiMode {
  SPIMODE0, /*!< clock polarity (CPOL) is low and clock phase (CPHA) is low */
  SPIMODE1, /*!< clock polarity (CPOL) is low and clock phase (CPHA) is high */
  SPIMODE2, /*!< clock polarity (CPOL) is high and clock phase (CPHA) is low */
  SPIMODE3  /*!< clock polarity (CPOL) is high and clock phase (CPHA) is high */
};

// the following enums are not yet used, but might be necessary to allow only certain values for en_nSpeed and en_nBitsPerWord

//! sets the clock divider setting used for calculate the SPI bus clock from the internal system clock
/*! in case of the Raspberry Pi the system clock is 250MHz
 *
 */
enum EClockDevider {
  MHz125 = 2,    /*!< clock divider 2 gives SPI clock of 125 MHz */
  MHz62 = 4,    /*!< clock divider 4 gives SPI clock of 62.5 MHz */
  MHz31 = 8,    /*!< clock divider 8 gives SPI clock of 31.2 MHz */
  MHz15 = 16,    /*!< clock divider 16 gives SPI clock of 15.6 MHz */
  MHz8 = 32,    /*!< clock divider 32 gives SPI clock of 7.8 MHz */
  MHz4 = 64,    /*!< clock divider 64 gives SPI clock of 3.9 MHz */
  MHz2 = 128,    /*!< clock divider 128 gives SPI clock of 1953 kHz */
  MHz1 = 256,    /*!< clock divider 256 gives SPI clock of 976 kHz */
  kHz488 = 512,  /*!< clock divider 512 gives SPI clock of 488 kHz */
  kHz244 = 1024,  /*!< clock divider 1024 gives SPI clock of 244 kHz */
  kHz122 = 2048,  /*!< clock divider 2048 gives SPI clock of 122 kHz */
  kHz61 = 4096,  /*!< clock divider 4096 gives SPI clock of 61 kHz */
  kHz30 = 8192,  /*!< clock divider 8192 gives SPI clock of 30.5 kHz */
  kHz15 = 16384,  /*!< clock divider 16384 gives SPI clock of 15.2 kHz */
  Hz7629 = 32768  /*!< clock divider 32768 gives SPI clock of 7629 Hz */
};

//! sets the bits used per word for the SPI bus data transfer
enum EBitsPerWord {
  standard = 8,   /*!< 8 bits per word, which is the default for most applications */
  lcd = 9     /*!< 9 bits per word, used for LoSSI bus mode devices like LCD displays for instance */
};

/*! \class CSpiDevice
 *  \brief CSpiDevice represents a device (slave) connected via SPI bus and provides basic configuration and data processing
 *
 *  CSpiDevice needs the Linux SPI kernel driver to be loaded (e.g the spi-bcm2708 module on the Raspberry Pi)
 *  A SPI device (e.g. ADC) on bus X with chip select line Y will be available as /dev/spidevX.Y
 *  If you don't see this device in your file system, the spidev kernel driver module might not be loaded.
 *  SPI devices have a limited userspace API, supporting basic half-duplex
 *  read() and write() access to SPI slave devices.  Using ioctl() requests,
 *  full duplex transfers and device I/O configuration are also available.
 *
 *  disadvantage:
 *  - speed, it is slower than direct hardware register access and memory mapping
 *  - runs in user space and is therefore not time deterministic
 *
 *  A SPI device itself normally is unique, so the CSpiDevice class is derived from CUncopyable
 *   to prevent the compiler from creating default copy constructor and assignment operator and to disable these functions in derived classes as well.
 */
class CSpiDevice : private CUncopyable {

public:

  /*! \fn CSpiDevice(const char* sDevice)
   * \brief creates a SPI device according to the  \a sDevice parameter
   * \param sDevice gives the device path e.g. /dev/spidev0.0
   *
   * The constructor sets the default parameters to m_nBitsPerWord = 8, m_enMode = SPIMODE0 and m_nSpeed to 1 MHz.
   * It configures the bus via file access to /dev/spidevx.y and using ioctl() calls to the kernel driver
   */
  explicit CSpiDevice(const char* sDevice);

  /*! \fn CSpiDevice(const char* sDevice, unsigned int nSpeed, ESpiMode enMode)
   * \brief creates a SPI device according to the  \a sDevice parameter
   * \param sDevice gives the device path e.g. /dev/spidev0.0
   * \param nSpeed gives the bus clock frequency e.g. 1000000 for 1 MHz
   * \param enMode gives the SPI bus mode setting e.g. SPIMODE0
   *
   * The constructor sets the default parameters to m_nBitsPerWord = 8
   */
  CSpiDevice(const char* sDevice, unsigned int nSpeed, ESpiMode enMode);

  /*! \fn ~CSpiDevice()
   * \brief releases the current current SPI device and closing the file descriptor to /dev/spidevx.y
   *
   */
  ~CSpiDevice();

  /*! \fn bool isOpen()
   * \brief returns true if the device has been opened and configured successfully, false will be returned if something went wrong
   *
   */
  bool isOpen() const { return m_bValid; };

  /*! \fn ESpiMode getMode()
   * \brief returns the currently configured SPI mode
   *
   */
  ESpiMode getMode() const { return m_enMode; };

  /*! \fn void setMode(ESpiMode newMode)
   * \brief sets the SPI mode to \a newMode
   * \param newMode gives the SPI mode to set (SPIMODE0 to SPIMODE3)
   *
   */
  void setMode(ESpiMode newMode) { m_enMode = newMode; };

  /*! \fn bool write(const unsigned char* paData, int nLength)
   * \brief writes \a nLength bytes of the \a paData buffer to the device
   * \param paData buffer holding the bytes to write
   * \param nLength number of bytes to write to the device
   *
   * The function writes data e.g. the configuration register settings, which needs to be provided in paData, to the SPI device e.g. the ADC.
   * The answer sent from the device is ignored.
   * TRUE is returned if the operation was successful, FALSE if not.
   * The caller of the function needs to make sure, that \a paData is not smaller than \a nLength.
   */
  bool write(const unsigned char* paData, int nLength);

  /*! \fn bool read(unsigned char* paData, int nLength)
   * \brief reads \a nLength bytes of the \a paData buffer to the device
   * \param paData buffer holding the bytes read from the device
   * \param nLength number of bytes to read from the device
   *
   * The function reads data from the SPI device, e.g. an eeprom.
   * TRUE is returned if the operation was successful, FALSE if not.
   * The caller of the function needs to make sure, that \a paData is big enough to hold \a nLength bytes.
   */
  bool read(unsigned char* paData, int nLength);

  /*! \fn bool transfer(const unsigned char* paTxData, unsigned char* paRxData, unsigned int nLength)
   * \brief writes and reads \a nLength bytes in a single communication cycle
   * \param paTxData buffer holding the bytes to send to the device
   * \param paRxData buffer holding the bytes read back from the device
   * \param nLength number of bytes to read and write
   *
   * The function transfer() sends the bytes stored in the paTxData buffer to the SPI device and stores the answer in the paRxData buffer.
   * Both buffers need to have the same \a nLength in bytes.
   * This function is useful for SPI devices, which respond to a write operation of the SPI master immediately (e.g an ADC or an EEPROM)
   * TRUE is returned if the operation was successful, FALSE if not.
   * The caller of the function needs to make sure, that both \a paTxData and \a paRxData are big enough to hold \a nLength bytes.
   */
  bool transfer(const unsigned char* paTxData, unsigned char* paRxData, unsigned int nLength);


private:
  /*! \fn bool openAndConfigureBus(const char* sDevice)
   * \brief does the initial setup of the SPI device bus
   * \param sDevice gives the device path e.g. /dev/spidev0.0
   *
   * The function opens the device, sets read and write mode, speed and so on.
   * If something is wrong during the setup and configuration it returns FALSE, TRUE is returned if everything is fine.
   * All the member variables m_enMode, m_nBitsPerWord and m_nSpeed need to be initialized properly before this function is called.
   * This is normally done by the constructors of CSpiDevice.
   */
  bool openAndConfigureBus(const char* sDevice);

  // TODO: make the data type of m_BitsPerWord bullet proof to allow only 8 (standard) or 9 (used by some LCD displays in LoSSI mode) bits
  unsigned char  m_nBitsPerWord;

   // TODO: make the data type of m_Speed bullet proof, e.g. enum
  unsigned int  m_nSpeed;

  ESpiMode    m_enMode;

  // file descriptor used for the SPI device
  int        m_nFileDescriptor;

  // m_Valid is used by other member functions to check if the SPI device bus has been initialized properly
  bool      m_bValid;

};


}

#endif /* SPI_SPIDEVICE_H_ */
