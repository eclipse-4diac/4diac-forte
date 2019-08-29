/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
 *   2018 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Ingo Hegny, Stanislav Meduna
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians
 *      - fixes behavior for getToStringBufferSize
 *******************************************************************************/
#include "forte_wstring.h"
#include <devlog.h>

#include "unicode_utils.h"

DEFINE_FIRMWARE_DATATYPE(WSTRING, g_nStringIdWSTRING)

bool CIEC_WSTRING::fromUTF16(const TForteByte *pa_pacBuffer, unsigned int pa_nBufferLen){
  bool bLittleEndian = false;
  TForteUInt32 nCodepoint;
  int nRes;
  unsigned int nRemLen = pa_nBufferLen;
  const TForteByte *pacBuffer = pa_pacBuffer;

  if(pa_nBufferLen == 0){
    assign("", 0);
    return true;
  }
  else if((pa_nBufferLen & 1) == 1){
    return false;
  }

  // Check the BOM, default to big endian
  nRes = CUnicodeUtilities::parseUTF16Codepoint(pa_pacBuffer, nCodepoint, false);
  if(nRes < 0) {
    return false;
  }
  if(nRes == 2){
    if(nCodepoint == CUnicodeUtilities::scm_unBOMMarkerSwapped){
      bLittleEndian = true;
      nRemLen -= 2;
      pacBuffer += 2;
    }
    else if(nCodepoint == CUnicodeUtilities::scm_unBOMMarker){
      nRemLen -= 2;
      pacBuffer += 2;
    }
  }

  // Count the needed space
  const TForteByte *pRunner = pacBuffer;
  unsigned int i = 0;
  unsigned int nNeededLength = 0;
  while(i < nRemLen){
    nRes = CUnicodeUtilities::parseUTF16Codepoint(pRunner, nCodepoint, bLittleEndian);
    if(nRes < 0 || nRes + i > nRemLen) {
      return false;
    }
    i += nRes;
    pRunner += nRes;
    nRes = CUnicodeUtilities::encodeUTF8Codepoint(0, 0, nCodepoint);
    if(nRes < 0) {
      return false;
    }
    nNeededLength += nRes;
  }

  if(nNeededLength > scm_unMaxStringLen) {
    return false;
  }

  // Reserve and encode
  reserve(static_cast<TForteUInt16>(nNeededLength));
  if(getGenData() == 0) {
    return false;
  }

  TForteByte *pEncRunner = (TForteByte *) getValue();
  TForteByte *pDataEnd = pEncRunner + nNeededLength;
  pRunner = pacBuffer;
  i = 0;
  while(i < nRemLen){
    nRes = CUnicodeUtilities::parseUTF16Codepoint(pRunner, nCodepoint, bLittleEndian);
    i += nRes;
    pRunner += nRes;
    nRes = CUnicodeUtilities::encodeUTF8Codepoint(pEncRunner, static_cast<unsigned int>(pDataEnd - pEncRunner), nCodepoint);
    pEncRunner += nRes;
  }
  setLength(static_cast<TForteUInt16>(nNeededLength));
  *pEncRunner = '\0';

  return true;
}

int CIEC_WSTRING::toUTF16(TForteByte* pa_pacBuffer, unsigned int pa_nBufferSize) const{
  // Count the needed space
  const TForteByte *pRunner = (const TForteByte *) getValue();
  unsigned int i = 0;
  unsigned int nNeededLength = 0;
  unsigned int nRemLen = length();
  TForteUInt32 nCodepoint;
  int nRes;

  while(i < nRemLen){
    nRes = CUnicodeUtilities::parseUTF8Codepoint(pRunner, nCodepoint);
    if(nRes < 0 || nRes + i > nRemLen) {
      return -1;
    }
    i += nRes;
    pRunner += nRes;
    // Skip the BOM if present
    if(nCodepoint != CUnicodeUtilities::scm_unBOMMarker){
      nRes = CUnicodeUtilities::encodeUTF16Codepoint(0, 0, nCodepoint, false);
      if(nRes < 0) {
        return -1;
      }
      nNeededLength += nRes;
    }
  }

  if(pa_pacBuffer == 0) {
    return nNeededLength;
  }

  if(nNeededLength > pa_nBufferSize) {
    return -1;
  }

  TForteByte *pEncRunner = pa_pacBuffer;
  TForteByte *pDataEnd = pa_pacBuffer + nNeededLength;

  pRunner = (const TForteByte *) getValue();
  nRemLen = length();
  i = 0;
  while(i < nRemLen){
    nRes = CUnicodeUtilities::parseUTF8Codepoint(pRunner, nCodepoint);
    if(nRes < 0 || nRes + i > nRemLen) {
      return -1;
    }
    i += nRes;
    pRunner += nRes;
    if(nCodepoint != CUnicodeUtilities::scm_unBOMMarker){
      nRes = CUnicodeUtilities::encodeUTF16Codepoint(pEncRunner, static_cast<unsigned int>(pDataEnd - pEncRunner), nCodepoint, false);
      if(nRes < 0) {
        return -1;
      }
      pEncRunner += nRes;
    }
  }

  return static_cast<unsigned int>(pEncRunner - pa_pacBuffer);
}

int CIEC_WSTRING::fromString(const char *pa_pacValue){
  int nRetVal = 0;
  if(0 == strncmp(pa_pacValue, "WSTRING#", 8)){
    pa_pacValue += 8;
    nRetVal = 8;
  }

  int nUTF8Result = fromUTF8(pa_pacValue, -1, (*pa_pacValue == '"'));

  return (-1 == nUTF8Result) ? -1 : (nRetVal + nUTF8Result);
}

int CIEC_WSTRING::toString(char* paValue, size_t paBufferSize) const {
  return toUTF8(paValue, paBufferSize, true);
}

int CIEC_WSTRING::fromUTF8(const char *pa_pacValue, int pa_nLen, bool pa_bUnescape){
  int nSrcLen = pa_nLen >= 0 ? pa_nLen : (pa_bUnescape ? determineEscapedStringLength(pa_pacValue, '"') : static_cast<int>(strlen(pa_pacValue)));
  int nSrcCappedLength = nSrcLen;

  if(0 <= nSrcLen){
    if((0 == pa_pacValue) || (pa_pacValue[0] == '\0') || (nSrcLen == 0)){
      assign("", 0);
      return 0;
    }

    if(nSrcLen > static_cast<int>(scm_unMaxStringLen)){
      // If we get a to large string we will truncate it
      // This is a conservative guess
      nSrcCappedLength = scm_unMaxStringLen;
      DEVLOG_WARNING("Too large string, destination will be truncated!\n");
    }

    // The needed space is surely not larger than original length - it can
    // only be smaller if there are chars outside the BMP
    reserve(static_cast<TForteUInt16>(nSrcCappedLength));
    if(0 == getGenData()){
      return -1;
    }

    unsigned int nMaxWidth;
    int nLength = CUnicodeUtilities::checkUTF8((const char *) pa_pacValue, nSrcCappedLength, nMaxWidth);

    // Only accept if this is valid UTF-8, otherwise we can get major
    // problems when serializing
    if(nLength < 0){
      DEVLOG_WARNING("Invalid UTF-8 string given to fromString!\n");
      *this = "***INVALID UTF-8***";
      return -1;
    }
    else if(nMaxWidth > 16){
      DEVLOG_WARNING("UTF-8 string with characters outside of BMP given to fromString!\n");
    }

    // If BMP, all is well - simply assign
    if(nMaxWidth <= 16){
      assign(pa_pacValue, static_cast<TForteUInt16>(nSrcCappedLength));
    }
    else{
      TForteUInt32 nCodepoint;
      const TForteByte *pRunner = (const TForteByte *) pa_pacValue;
      TForteByte *pEncBuffer = (TForteByte *) getValue();
      TForteByte *pEncRunner = pEncBuffer;

      while(*pRunner && (pRunner - (const TForteByte *) pa_pacValue) < nSrcCappedLength){
        int nRes;
        nRes = CUnicodeUtilities::parseUTF8Codepoint(pRunner, nCodepoint);
        pRunner += nRes;
        if(nCodepoint == CUnicodeUtilities::scm_unBOMMarker) {
          continue;
        }
        if(nCodepoint >= 0x10000) {
          nCodepoint = '?';
        }
        nRes = CUnicodeUtilities::encodeUTF8Codepoint(pEncRunner, static_cast<unsigned int>(nSrcCappedLength - (pEncRunner - pEncBuffer)), nCodepoint);
        if(nRes < 1) {
          break;
        }
        pEncRunner += nRes;
      }

      *pEncRunner = '\0';
      setLength(static_cast<TForteUInt16>(pEncRunner - pEncBuffer));
    }

    if(pa_bUnescape){
      nLength = unescapeFromString(getValue(), '"');
      if(-1 == nLength){
        return -1;
      }
    }
  }
  return nSrcLen;
}

int CIEC_WSTRING::toUTF8(char* paBuffer, size_t paBufferSize, bool paEscape) const {
  if((TForteUInt32) length() + (paEscape ? 2 : 0) + 1 > paBufferSize) {
    return -1;
  }

  if(!paEscape){
    memcpy(paBuffer, getValue(), length() + 1);
    return length();
  }

  const char *pRunner;
  char *pEncRunner = paBuffer;
  char *pDataEnd = paBuffer + paBufferSize;

  *pEncRunner++ = '\"';

  pRunner = getValue();
  while(*pRunner){
    int nRes = dollarEscapeChar(pEncRunner, *pRunner, static_cast<unsigned int>(pDataEnd - pEncRunner));

    if(nRes < 0) {
      return -1;
    }

    pEncRunner += nRes;
    ++pRunner;
  }

  if(pDataEnd - pEncRunner < 2) {
    return -1;
  }

  *pEncRunner++ = '\"';
  *pEncRunner = '\0';

  return static_cast<int>(pEncRunner - paBuffer);
}

size_t CIEC_WSTRING::getToStringBufferSize() const {
  const char * const stringValue = getValue();
  size_t neededBufferSize = 0;
  for(size_t i = 0; i < length(); ++i){
    if(isprint(static_cast<unsigned char>(stringValue[i])) && '$' != stringValue[i] && '\"' != stringValue[i]){
      ++neededBufferSize;
    }
    else{
      switch (stringValue[i]){
        case '$':
        case 0x10: // line feed
        case '\n':
        case '\f':
        case '\r':
        case '\t':
        case '\"':
          neededBufferSize += 2;
          break;
        default:
          // WSTRING are two-byte strings, so we only have to check for two byte UTF-8 symbols
          if((stringValue[i] & 0xe0) == 0xc0 && (stringValue[i + 1] & 0xc0) == 0x80){
            ++i;
          }
          neededBufferSize += 5;
          break;
      }
    }
  }
  return neededBufferSize + 2 + 1; // For Quotes and \0
}
