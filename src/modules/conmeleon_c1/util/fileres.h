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
#ifndef UTIL_FILERES_H_
#define UTIL_FILERES_H_

#include <cstdio>
#include "../util/uncopyable.h"

/*! \class CFileResource
 *  \brief CFileResource is a class for ASCII file handling according to the RAII concept
 *
 *  CFileResource is meant to be a lightweight replacement for file streams, where a small footprint is necessary
 *  and the comfortable file streams of C++ standard library are an overkill.
 *  This is also the reason why char* is used instead of std::string
 *  This class provides basic file access functions only.
 */
class CFileResource : private CUncopyable {

public:
  /*! \fn CFileResource(const char*  pszFileName, const char* pszOpenMode)
   * \brief Opens the ASCII file, which name and full path is given in \a pszFileName
   * \param pszFileName the full path to the ASCII file
   * \param pszOpenMode the open mode that should be used, if not stated the default value is "a+"
   *
   * The valid openmode flags are the following, which are defined in cstdio:
   *  - "r"    file is opened for reading and must exist.
   *  - "w"    file is opened for writing and is created if it does not exist.
   *      If the file exists, it's contents will be erased.
   *  - "a"    file is opened for writing and the new data is written at the end of the file.
   *      The file is created if it does not exist.
   *  - "+"    a appended "+" sign opens the file for both reading and writing, e.g. "r+"
   *
   *  the other options "b" and "x" are ignored
   */
  CFileResource(const char*  pszFileName, const char* pszOpenMode = "a+");

  /*! \fn ~CFileResource()
   * \brief The destructor just closes the file and releases the internal file handle
   */
  ~CFileResource();

  /*! \fn bool isOpen()
   * \brief returns true if the file has been opened successfully
   */
  bool isOpen() { return (m_pFileHandle != nullptr); }

  /*! \fn bool isReadOnly()
   * \brief returns true if the file has been opened in read only mode
   */
  bool isReadOnly() { return m_ReadOnly; };

  /*! \fn void writeLine(const char* pszData)
   * \brief writes a string given by \a pszData to the ASCII file
   * \param pszData buffer holding the string to write
   *
   */
  void writeLine(const char* pszData) const;

  /*! \fn void writeFormattedLine(const char* pszFormat, ...)
   * \brief writes a formatted string given by \a pszFormat and all following parameters to the ASCII file
   * \param pszFormat buffer holding the formatted string to write, similar to printf() syntax
   *
   */
  void writeFormattedLine(const char* pszFormat, ...) const;

  /*! \fn void readLine(char* pszData, std::size_t nLength)
   * \brief reads the first line of an ASCII file into a buffer given by \a pszData
   * \param pszData buffer holding the string being read
   * \param nLength number of characters, which should be read
   *
   * The caller needs to provide the proper char buffer pszData and the number of chars to read via nLength parameter
   */
  void  readLine(char* pszData, std::size_t nLength) const;

private:
  FILE*  m_pFileHandle;
  bool  m_ReadOnly;
};


/*! \fn bool checkModeReadOnly (const char* pszMode)
 * \brief checks if the mode \a pszMode passed in is a read only mode
 * \param pszMode buffer holding the mode string
 *
 * The function returns true, if the the mode string is read only.
 * To be read only the string pszMode must contain "r" and must not contain "+"
 */
bool checkModeReadOnly (const char* pszMode);

#endif /* UTIL_FILERES_H_ */
