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
 *  This code uses sysfs to access GPIO pins of linux powered hardware devices.
 *  sysfs exports linux kernel driver functionality to user space with simple file access
 *
 *  advantage:
 *  easy to use file access and sysfs is already part of the linux kernel
 *
 *  disadvantage:
 *  speed, it is slower than direct hardware register access and memory mapping
 *  runs in user space and is therefore not time deterministic
 *
 *  the following comments in gpiopin.h are written to support automatic doc creation with doxygen
 *******************************************************************************/

#ifndef GPIO_GPIOPIN_H_
#define GPIO_GPIOPIN_H_

#include "../util/uncopyable.h"
#include "../../../arch/posix/forte_sync.h"
#include <cstdio>
#include <fstream>
#include <mutex>

namespace CONMELEON {
  //! sets the direction of the GPIO pin
  enum EPinDirection {
    input,     /*!< pin is configured as input */
    output    /*!< pin is configured as output */
  };

  //! sets the state of the GPIO pin
  enum EPinState {
    unused,   /*!< pin is not in use and can be configured by the user */
    used,     /*!< pin is in use and cannot be changed */
    reserved  /*!< pin is reserved by the system and cannot be configured by the user */
  };

  /** gives the full path to the gpio pin export file */
  const char* const ExportFilePath = "/sys/class/gpio/export";

  /** gives the full path to the gpio pin unexport file */
  const char* const UnexportFilePath = "/sys/class/gpio/unexport";

  /** gives the the first part of the path to the gpio pin directory, the pin number needs to be appended */
  const char* const SignalFilePathPrefix = "/sys/class/gpio/gpio";

  /** gives the the last part of the path to the gpio pin direction file */
  /*! The full path needs to be created like this: SignalFilePathPrefix + Pinnumber + DirFilePathPostfix
   *
   */
  const char* const DirFilePathPostfix = "/direction";

  /** gives the the last part of the path to the gpio pin value file */
  /*! The full path needs to be created like this: SignalFilePathPrefix + Pinnumber + ValFilePathPostfix
   *
   */
  const char* const ValFilePathPostfix = "/value";

  /*! \class CGpioPin
   *  \brief CGpioPin is a class for handling GPIO pins of an embedded Linux system (e.g. Raspberry Pi) via SYSFS files
   *
   *  CGpioPin is using the SYSFS kernel driver
   *  This approach is  slower than the direct memory access of the GPIO chip registers, but it is available almost on every Linux based embedded device without changes
   *  SYSFS files follow a given file path structure, please note that you normally need root privileges to access the SYSFS files.
   *   to use a GPIO pin you need to write its number to the /sys/class/gpio/export file
   *  to stop using a pin you need to write its number to the /sys/class/gpio/unexport file
   *  to set the direction of pin 4 (input or output) you need to write 'in' or 'out' to /sys/class/gpio/gpio4/direction file
   *  to set the value of pin 15 (high or low) you need to write '0' or '1' to /sys/class/gpio/gpio15/value file
   *  Please note that the pin numbers are referring to the GPIO chip IO numbers.
   *
   *  A pin itself normally is unique, so the CGpioPin class is derived from CUncopyable
   *   to prevent the compiler from creating default copy constructor and assignment operator and to disable these functions in derived classes as well.
   */
  class CGpioPin : private CUncopyable {
  public:
    /*! \fn CGpioPin(int iPinNr)
     * \brief creates a pin with \a iPinNr
     * \param iPinNr the pin to create
     * \param enDir the pin direction, can be either input or output
     *
     * The constructor checks for valid pin numbers ( > 0 ) and writes the pin number to /sys/class/gpio/export to be able to use the pin
     */
    CGpioPin(int iPinNr, EPinDirection enDir);

    /*! \fn ~CGpioPin()
     * \brief releases the current pin
     *
     * The destructor writes the pin number to /sys/class/gpio/unexport to release the GPIO pin
     */
    ~CGpioPin();

    /*! \fn bool isValid()
     * \brief returns true if the pin is valid or false if not
     *
     *  A pin is valid if its number is > 0 and the export (writing the pin number to the export file) was successful
     */
    bool isValid() const { return m_Valid; }

    /*! \fn bool isAvailable()
     * \brief returns true if the pin is available for use or false if not
     *
     *  A pin is available if it is valid and has not been configured as input or output yet
     */
    bool isAvailable() const { return (m_Valid && (m_State == unused)); }

    /*! \fn bool isInput()
     * \brief returns true if the pin is configured as an input or false if not
     *
     * The pin is configured as an input or not, this function also returns false if the pin is undefined (default setting of the constructor)
     */
    bool isInput() const { return (m_Direction == input); }

    /*! \fn bool isOutput()
     * \brief returns true if the pin is configured as an output or false if not
     *
     * The pin is configured as an output or not, this function also returns false if the pin is undefined (default setting of the constructor)
     */
    bool isOutput() const { return (m_Direction == output); }

    /*! \fn bool isInverted()
     * \brief returns true if the pin is inverted at the logic level
     *
     * The pin is configured as inverted logic or not (input  voltage level is high, but pin value is "0"
     * This is the case if the electronic circuit uses pull up resistors connected to the gpio pin.
     */
    bool isInverted() const { return m_Inverted; }

    /*! \fn void setInverted(bool bInverted)
     * \brief sets the pins logic behaviour to inverted according to the value of \a bInverted
     * \param bInverted is true in the pin should be active low or false if the pin is active high
     *
     * This setting is working for inputs only
     */
    void setInverted(bool bInverted);

    /*! \fn bool read()
     * \brief reads the current pin value
     *
     * Read the current pin value from the /sys/class/gpio/gpioxx/value file, this only works if the pin has been configured as an input
     */
    bool read() const;

    /*! \fn void write(bool bValue)
     * \brief writes the current pin value
     * \param bValue
     *
     * Write bValue to the /sys/class/gpio/gpioxx/value file, this only works if the pin has been configured as an output
     * SYSFS expects "1" for TRUE and "0" for FALSE
     */
    void write(bool bValue);

  private:

    bool sysfsExportPin() const;
    bool sysfsUnexportPin() const;
    bool sysfsSetPinDirection(EPinDirection enDir);
    bool sysfsOpenValueFileStream(EPinDirection enDir);

    int            m_Nr;
    bool          m_Valid;
    bool          m_Inverted;
    EPinDirection       m_Direction;
    EPinState        m_State;
    mutable std::fstream  m_PinValueStream; /* used for faster access to pin value file */
    static CPCSyncObject    m_GlobalFileMutex;
    mutable CPCSyncObject    m_LocalFileMutex;
  };

  /*! \fn bool readFromFile(const char* pszFileName, char* pszContent, std::size_t nBufferLength)
   * \brief reads the contents of a text file
   * \param pszFileName  character buffer holding the filename including path to read from
   * \param pszContent character buffer to hold content of the file
   * \param nBufferLength number of characters to read
   *
   * The caller needs to make sure that \a pszContent is large enough to hold \a nBufferlength characters
   * This function is an external helper function an is not a class member of CGpioPin
   * The function returns true, if the read operation was successful and false the other way round
   */
  bool readFromFile(const char* pszFileName, char* pszContent, std::size_t nBufferLength);

  /*! \fn bool bool writeToFile(const char* pszFileName, const char* pszContent)
   * \brief writes the contents to a text file
   * \param pszFileName  character buffer holding the filename including path to write to
   * \param pszContent character buffer to hold content that needs to be written
   *
   * This function is an external helper function an is not a class member of CGpioPin
   * The function returns true, if the write operation was successful and false the other way round
   */
  bool writeToFile(const char* pszFileName, const char* pszContent);
}

#endif /* GPIO_GPIOPIN_H_ */
