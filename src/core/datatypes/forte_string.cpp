/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
 *               2018 TU Wien/ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Rene Smodic, Alois Zoitl,
 *    Ingo Hegny, Martin Melik Merkumians, Stanislav Meduna
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - fixes toString behavior for 0 size buffer
 *    Martin Melik Merkumians - fixes behavior for getToStringBufferSize
 *******************************************************************************/
#include "forte_string.h"
#include <devlog.h>
#include "forte_uint.h"
#include "unicode_utils.h"

DEFINE_FIRMWARE_DATATYPE(STRING, g_nStringIdSTRING);

int CIEC_STRING::fromString(const char *pa_pacValue){
  int nSrcLen = determineEscapedStringLength(pa_pacValue, '\'');
  int nSrcCappedLength = nSrcLen;

  if(0 < nSrcLen){
    if((nSrcLen >= 7) && (0 == strncmp(pa_pacValue, "STRING#", 7))){
      pa_pacValue += 7;
      nSrcCappedLength -= 7;
    }

    if(static_cast<unsigned int>(nSrcLen) > scm_unMaxStringLen) {
      nSrcCappedLength = scm_unMaxStringLen;
    }


    if (*pa_pacValue == '\'') {
      reserve(static_cast<TForteUInt16>(nSrcCappedLength));
      if(unescapeFromString(pa_pacValue, '\'') < 0) {
        return -1;
      }
    } else {
      assign(pa_pacValue, static_cast<TForteUInt16>(nSrcCappedLength));
    }
  }

  return nSrcLen;
}

int CIEC_STRING::toString(char* paValue, size_t paBufferSize) const {
  int nRetVal = -1;
  if(0 != paValue && getToStringBufferSize() <= paBufferSize){
    *paValue = '\'';
    paValue++;
    TForteUInt16 nLen = length();
    size_t nUsedBytes = 0;
    paBufferSize -= 2;
    if(0 < nLen){
      const char * acValue = getValue();
      for(unsigned int i = 0; i < nLen; ++i){
        if(nUsedBytes >= paBufferSize) {
          return -1;
        }
        nUsedBytes += dollarEscapeChar(paValue+nUsedBytes, acValue[i], 2);
      }

    } else{
      *paValue = '\0';
    }
    nRetVal = static_cast<int>(nUsedBytes);
    paValue[nRetVal] = '\'';
    paValue[nRetVal + 1] = '\0';
    nRetVal += 2;
  }
  return nRetVal;
}

size_t CIEC_STRING::getToStringBufferSize() const {
  const char * const stringValue = getValue();
  size_t neededBufferSize = 0;
  for(size_t i = 0; i < length(); ++i){
    if(isprint(static_cast<unsigned char>(stringValue[i])) && '$' != stringValue[i] && '\'' != stringValue[i]){
      ++neededBufferSize;
    }
    else{
      switch (stringValue[i]){
        case '$':
        case '\'':
        case 0x10: // line feed
        case '\n':
        case '\f':
        case '\r':
        case '\t':
          neededBufferSize += 2;
          break;
        default:
          neededBufferSize += 3;
          break;
      }
    }
  }
  return neededBufferSize + 2 + 1; // For Quotes and \0
}

#ifdef FORTE_UNICODE_SUPPORT
int CIEC_STRING::fromUTF8(const char *pa_pacValue, int pa_nLen, bool pa_bUnescape) {

  int nSrcLen = pa_nLen >= 0 ? pa_nLen : (pa_bUnescape ? determineEscapedStringLength(pa_pacValue, '\'') : static_cast<int>(strlen(pa_pacValue)));
  int nSrcCappedLength = nSrcLen;

  if(0 <= nSrcLen){
    if (nSrcLen == 0) {
      assign("", 0);
      return 0;
    }

    if (nSrcLen > static_cast<int>(scm_unMaxStringLen)) {
      // If we get a to large string we will truncate it
      // This is a conservative guess
      nSrcCappedLength = scm_unMaxStringLen;
      DEVLOG_WARNING("Too large string, destination will be truncated!\n");
    }

    unsigned int nMaxWidth;
    int nLength = CUnicodeUtilities::checkUTF8(pa_pacValue, nSrcCappedLength, nMaxWidth);
    if (nLength < 0) {
      DEVLOG_WARNING("Invalid UTF-8 string given to fromString!\n");
      *this = "***INVALID UTF-8***";
      return -1;
    } else if (nMaxWidth > 8) {
      DEVLOG_WARNING("UTF-8 string with non-representable characters given to fromString!\n");
    }

    reserve(static_cast<TForteUInt16>(nLength));
    if (0 == getGenData()) {
      return -1;
    }

    TForteUInt32 nCodepoint;
    const char *pRunner = pa_pacValue;
    TForteByte *pEncBuffer = (TForteByte *) getValue();
    TForteByte *pEncRunner = pEncBuffer;

    while (*pRunner && (pRunner-pa_pacValue) < nSrcCappedLength && (pEncRunner-pEncBuffer) < nLength) {
      int nRes;
      nRes = CUnicodeUtilities::parseUTF8Codepoint((const TForteByte *) pRunner, nCodepoint);
      pRunner += nRes;
      if (nCodepoint == CUnicodeUtilities::scm_unBOMMarker) {
        continue;
      }
      if (nCodepoint >= 0x100) {
        nCodepoint = '?';
      }
      *pEncRunner++ = (TForteByte) nCodepoint;
    }

    *pEncRunner = '\0';
    setLength(static_cast<TForteUInt16>(pEncRunner - pEncBuffer));

    if (pa_bUnescape) {
      nLength = unescapeFromString(getValue(), '\'');
      if (nLength < 0) {
        return -1;
      }
    }
  }
  return nSrcLen;
}

int CIEC_STRING::toUTF8(char* paBuffer, size_t paBufferSize, bool paEscape) const {
  // Count the needed space
  size_t nNeededLength = paEscape ? 2 : 0; // Leading and trailing delimiter
  int nRes;

  const unsigned char *pRunner = (const unsigned char *) getValue();
  while (*pRunner) {
    nRes = CUnicodeUtilities::encodeUTF8Codepoint(0, 0, (TForteUInt32) *pRunner);
    if (nRes < 0) {
      return -1;
    }
    nNeededLength += nRes;
    if (nRes == 1 && paEscape && dollarEscapeChar(0, *pRunner, 0) == 2) {
      nNeededLength++;
    }
    ++pRunner;
  }

  if(0 == paBuffer) {
    return static_cast<int>(nNeededLength);
  }

  if(nNeededLength + 1 > paBufferSize) {
    return -1;
  }

  char *pEncRunner = paBuffer;
  char *pDataEnd = paBuffer + nNeededLength;

  if(paEscape) {
    *pEncRunner++ = '\'';
  }

  pRunner = (const unsigned char *) getValue();
  while (*pRunner) {
    nRes = CUnicodeUtilities::encodeUTF8Codepoint((TForteByte *) pEncRunner, static_cast<unsigned int>(pDataEnd - pEncRunner), (TForteUInt32) *pRunner);
    if(nRes == 1 && paEscape) {
      nRes = dollarEscapeChar(pEncRunner, *pRunner, static_cast<unsigned int>(pDataEnd - pEncRunner));
    }
    if(nRes < 0) {
      return -1;
    }

    pEncRunner += nRes;
    ++pRunner;
  }

  if(paEscape) {
    *pEncRunner++ = '\'';
  }
  *pEncRunner = '\0';

  return static_cast<int>(pEncRunner - paBuffer);
}

#endif
