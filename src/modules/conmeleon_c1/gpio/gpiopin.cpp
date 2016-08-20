/*******************************************************************************
 * Copyright (c) 2016 Herwig Eichler, www.conmeleon.org
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Herwig Eichler  - initial API and implementation and initial documentation
 *******************************************************************************/

#include "../gpio/gpiopin.h"
#include "../util/fileres.h"
#include <string>
#include <iostream>

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

	m_GlobalFileMutex.lock();

	return writeToFile(ExportFilePath, &szPinNr[0]);

	m_GlobalFileMutex.unlock();
	// TODO: if an exception is thrown within writeToFile(), the mutex might be left locked
}

bool CGpioPin::sysfsUnexportPin() const {

	char szPinNr[12] = {0};

	sprintf(szPinNr, "%d", m_Nr);

	m_GlobalFileMutex.lock(); /* protect sysfs unexport file from multiple access */

	bool bRet = writeToFile(UnexportFilePath, &szPinNr[0]);

	m_GlobalFileMutex.unlock();
	return bRet;

}

bool CGpioPin::sysfsSetPinDirection(EPinDirection enDir) {

	char szFilename[40] = {0};
	bool bRet = false;

	sprintf(szFilename, "%s%d%s", SignalFilePathPrefix, m_Nr, DirFilePathPostfix);

	/* only this class instance needs access to the sysfs direction file
	 * we can use the local mutex for protection
	 */
	m_LocalFileMutex.lock();

	if ((enDir == input) && (writeToFile(&szFilename[0], "in"))) {
		bRet = true;
	}

	if ((enDir == output) && (writeToFile(&szFilename[0], "out"))) {
		bRet = true;
	}

	m_LocalFileMutex.unlock();

	return bRet;
}

bool CGpioPin::sysfsOpenValueFileStream(EPinDirection enDir) {

	char szFilename[40] = {0};

	sprintf(szFilename, "%s%d%s", SignalFilePathPrefix, m_Nr, ValFilePathPostfix);

	/* only this class instance needs access to the sysfs value file
	 * we can use the local mutex for protection
	 */
	m_LocalFileMutex.lock();

	m_PinValueStream.open(szFilename, ((enDir == input) ? std::ios_base::in : std::ios_base::out));

	m_LocalFileMutex.unlock();

	return m_PinValueStream.is_open();
}

void CGpioPin::setInverted(bool bInverted) {

	m_Inverted = bInverted && (m_Direction == input);
}

bool CGpioPin::read() const {

	if (m_Valid && (m_Direction == input)) {

		if (m_PinValueStream.is_open()) {

			std::string sLine;

			m_LocalFileMutex.lock();

			/* set to beginning of file, just to be sure */
			m_PinValueStream.clear();
			m_PinValueStream.seekg(0, std::ios::beg);

			m_PinValueStream >> sLine;

			m_LocalFileMutex.unlock();

			return ((sLine != "0")^m_Inverted);
		}

	}
	return false;
}

void CGpioPin::write(bool bValue) {

	if (m_Valid && (m_Direction == output)) {

		m_LocalFileMutex.lock();

		if (m_PinValueStream.is_open()) {
			m_PinValueStream << (bValue ? "1" : "0") << std::flush;
		}

		m_LocalFileMutex.unlock();
	}
}

bool readFromFile(const char* pszFileName, char* pszContent, std::size_t nBufferLength ) {

	if ((pszFileName == nullptr) || (pszContent == nullptr) || (nBufferLength < 1)){
		return false;
	}
	CFileResource 	file(pszFileName, "r");

	if (file.isOpen()) {
		file.readLine(pszContent, nBufferLength);
// TODO:	add error handling
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
	CFileResource 	file(pszFileName, "a");

	if (file.isOpen() && (pszContent != nullptr)) {
		file.writeLine(pszContent);
// TODO:	add error handling
	}
	return true;
}

} // namespace conmeleon

