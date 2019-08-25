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
 *******************************************************************************/

#include "gpiopin.h"
#include "../util/fileres.h"
#include <string>
#include <iostream>
#include "../../../core/utils/criticalregion.h"

namespace CONMELEON {

CPCSyncObject CGpioPin::m_GlobalFileMutex;

CGpioPin::CGpioPin(int iPinNr, EPinDirection enDir) : m_Nr(iPinNr), m_Valid(iPinNr > 0), m_Inverted(false), m_Direction(enDir), m_State(unused){

  if (iPinNr > 0) {
    m_Valid = this->sysfsExportPin() && this->sysfsSetPinDirection(enDir) && this->sysfsOpenValueFileStream(enDir);
  }
}

CGpioPin::~CGpioPin() {

  this->sysfsUnexportPin();
  // the value file stream does not need to be closed here, this will be handled by the fstream destructor.
}

bool CGpioPin::sysfsExportPin() const {

  char szPinNr[12] = {0};

  sprintf(szPinNr, "%d", m_Nr);

  /* protect sysfs export file from multiple access
   * all class instances need access to the same export file, that's why a global class mutex needs to be used
   */
  bool bRet;
  {
      CCriticalRegion criticalRegion(m_GlobalFileMutex);
      bRet = writeToFile(ExportFilePath, &szPinNr[0]);
  }

  return bRet;

  // TODO: if an exception is thrown within writeToFile(), the mutex might be left locked
}

bool CGpioPin::sysfsUnexportPin() const {

  char szPinNr[12] = {0};

  sprintf(szPinNr, "%d", m_Nr);

  bool bRet;
  {
    CCriticalRegion criticalRegion(m_GlobalFileMutex);
    bRet = writeToFile(UnexportFilePath, &szPinNr[0]);
  }

  return bRet;
}

bool CGpioPin::sysfsSetPinDirection(EPinDirection enDir) {

  char szFilename[40] = {0};
  bool bRet = false;

  sprintf(szFilename, "%s%d%s", SignalFilePathPrefix, m_Nr, DirFilePathPostfix);

  /* only this class instance needs access to the sysfs direction file
   * we can use the local mutex for protection
   */
  {
    CCriticalRegion criticalRegion(m_LocalFileMutex);

    if((enDir == input) && (writeToFile(&szFilename[0], "in"))){
      bRet = true;
    }

    if((enDir == output) && (writeToFile(&szFilename[0], "out"))){
      bRet = true;
    }
  }

  return bRet;
}

bool CGpioPin::sysfsOpenValueFileStream(EPinDirection enDir) {

  char szFilename[40] = {0};

  sprintf(szFilename, "%s%d%s", SignalFilePathPrefix, m_Nr, ValFilePathPostfix);

  /* only this class instance needs access to the sysfs value file
   * we can use the local mutex for protection
   */
  {
    CCriticalRegion criticalRegion(m_LocalFileMutex);

    m_PinValueStream.open(szFilename, ((enDir == input) ? std::ios_base::in : std::ios_base::out));

  }

  return m_PinValueStream.is_open();
}

void CGpioPin::setInverted(bool bInverted) {

  m_Inverted = bInverted && (m_Direction == input);
}

bool CGpioPin::read() const {

  if (m_Valid && (m_Direction == input)) {

    if (m_PinValueStream.is_open()) {

      std::string sLine;

      CCriticalRegion criticalRegion(m_LocalFileMutex);

      /* set to beginning of file, just to be sure */
      m_PinValueStream.clear();
      m_PinValueStream.seekg(0, std::ios::beg);

      m_PinValueStream >> sLine;

      return ((sLine != "0") ^ (m_Inverted));
    }

  }
  return false;
}

void CGpioPin::write(bool bValue) {

  if (m_Valid && (m_Direction == output)) {
    CCriticalRegion criticalRegion(m_LocalFileMutex);
    if(m_PinValueStream.is_open()){
      m_PinValueStream << (bValue ? "1" : "0") << std::flush;
    }
  }
}

bool readFromFile(const char* pszFileName, char* pszContent, std::size_t nBufferLength ) {

  if ((pszFileName == nullptr) || (pszContent == nullptr) || (nBufferLength < 1)){
    return false;
  }
  CFileResource   file(pszFileName, "r");

  if (file.isOpen()) {
    file.readLine(pszContent, nBufferLength);
// TODO:  add error handling
    return true;
  }
  else {
    *pszContent = '\0';
    return false;
  }

  // no need to close the file manually, because this is handled by the destructor of CFileResource class (RAII)
}

bool writeToFile(const char* pszFileName, const char* pszContent) {

  if (pszFileName == nullptr) {
    return false;
  }
  CFileResource   file(pszFileName, "a");

  if (file.isOpen() && (pszContent != nullptr)) {
    file.writeLine(pszContent);
// TODO:  add error handling
  }
  return true;
}

} // namespace conmeleon

