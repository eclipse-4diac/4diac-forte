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

#include "fileres.h"
#include <cstring>
#include <cstdlib>
#include <cstdarg>


CFileResource::CFileResource(const char* pszFileName, const char* pszOpenMode /*= "a+"*/) : mFileHandle(std::fopen(pszFileName, pszOpenMode)) {

  m_ReadOnly = checkModeReadOnly(pszOpenMode);

}

CFileResource::~CFileResource() {

  if (mFileHandle != nullptr) {
    std::fclose(mFileHandle);
  }
}

void CFileResource::writeLine(const char* pszData) const {

  if (pszData == nullptr) {
    return; // bad parameter, just return and do nothing
  }
  if (!m_ReadOnly && (mFileHandle != nullptr)) {
    std::fputs(pszData, mFileHandle);

    if (ferror(mFileHandle)) {
      // TODO: add logging or trace function like "Error during write to file operation! Check file permissions!";
    }
    // flushing or repositioning is required if the file is opened in update mode ("a"), so we do this always
    // following read operations on the same file will get the updated value for sure
    std::fflush(mFileHandle);
  }
}

void CFileResource::writeFormattedLine(const char* pszFormat, ...) const {

  if (pszFormat == nullptr) {
    return; // bad parameter, just return and do nothing
  }
  if (!m_ReadOnly && (mFileHandle != nullptr)) {
    char     buffer[256]; // buffer to hold formatted string including the arguments
    va_list   args;

    // start to deal with the arguments
    va_start (args, pszFormat);
    if (vsprintf (buffer,pszFormat, args) > 0) {
      // formatted string was assembled properly
      std::fputs(buffer, mFileHandle);

      if (ferror(mFileHandle)) {
        // TODO: add logging or trace function like "Error during write to file operation! Check file permissions!";
      }
      // flushing or repositioning is required if the file is opened in update mode ("a"), so we do this always
      // following read operations on the same file will get the updated value for sure
      std::fflush(mFileHandle);
    }
    va_end(args);
  }
}

void CFileResource::readLine(char* pszData, std::size_t nLength) const {

  if ((pszData == nullptr) || (nLength < 1)) {
    return; // bad parameters, just return and do nothing
  }

  if (mFileHandle != nullptr) {
    char *pszLine = nullptr;
    std::size_t nLen = 0;
    ssize_t nNrCharsRead;

    // set file position to the beginning
    std::rewind(mFileHandle);

    nNrCharsRead = getline(&pszLine, &nLen, mFileHandle);
    if (nNrCharsRead != -1) {
      strncpy(pszData, pszLine, nLength);
    }
    else {
      // TODO: add logging or trace function like "Error during write to file operation! Check file permissions!";
    }

    // getline allocates memory if needed, we need to release this memory again
    free(pszLine);
  }
}

bool checkModeReadOnly (const char* pszMode) {

  bool    bIsReadOnly;

  if (pszMode == nullptr) {
    return false;
  }

  // there needs to be a "r"
  bIsReadOnly = (strstr(pszMode, "r") != nullptr);

  // and we don't want to have a "+"
  return (bIsReadOnly && (strstr(pszMode, "+") == nullptr));

}

