/*******************************************************************************
 * Copyright (c) 2011 - 2015 nxtControl GmbH, ACIN, fortiss GmbH
 *               2018 TU Wien/ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Stansilav Meduna, Alois Zoitl, Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians
 *      - fixes behavior for getToStringBufferSize
 *******************************************************************************/
#include <fortenew.h>
#include "forte_any_string.h"
#include "unicode_utils.h"
#include <string.h>
#include <stdlib.h>
#include <devlog.h>

DEFINE_FIRMWARE_DATATYPE(ANY_STRING, g_nStringIdANY_STRING)

char CIEC_ANY_STRING::sm_acNullString[1] = {'\0'};

CIEC_ANY_STRING::~CIEC_ANY_STRING(){
  if(getGenData()){
    forte_free(getGenData());
  }
}

CIEC_ANY_STRING& CIEC_ANY_STRING::operator =(const char* const pa_pacValue){
  if(0 != pa_pacValue){
    size_t nLen = strlen(pa_pacValue);
    if (nLen > scm_unMaxStringLen) {
      //If we get a to large string we will truncate it
      nLen = scm_unMaxStringLen;
      DEVLOG_WARNING("Too large string given in assignment, destination will be truncated!\n");
    }
    assign(pa_pacValue, static_cast<TForteUInt16>(nLen));
  }
  return *this;
}

void CIEC_ANY_STRING::assign(const char *pa_poData, TForteUInt16 pa_nLen) {
  if (0 != pa_poData){
    if(0 != pa_nLen && pa_poData != getValue()) {
      reserve(pa_nLen);
      memcpy(getValue(), pa_poData, pa_nLen);
    }
    if(0 != getValue()){
      setLength(pa_nLen);
      getValue()[pa_nLen] = '\0'; //not really necessary, but is a stop if someone forgets that this is not a textual string
    }
  }
}

void CIEC_ANY_STRING::append(const char *pa_poData) {
  if (0 != pa_poData){
    append(pa_poData, static_cast<TForteUInt16>(strlen(pa_poData)));
  }
}

void CIEC_ANY_STRING::append(const char *pa_poData, TForteUInt16 pa_nLen) {
  if (0 != pa_poData){
    TForteUInt16 nLen = length();
    if(0 != pa_nLen){
      if((getCapacity() - nLen) < pa_nLen){
        reserve(static_cast<TForteUInt16>(nLen + pa_nLen));
      }
      if(0 != getValue()){
        memcpy(getValue() + nLen, pa_poData, pa_nLen);
        setLength(static_cast<TForteUInt16>(nLen + pa_nLen));
        getValue()[nLen + pa_nLen] = '\0'; //not really necessary, but is a stop if someone forgets that this is not a textual string
      }
    }
  }
}

void CIEC_ANY_STRING::reserve(TForteUInt16 pa_nRequestedSize){
  if(getCapacity() < pa_nRequestedSize + 1){
    bool firstAlloc = (getGenData() == 0);
    TForteUInt16 nLength = length();
    TForteUInt16 nNewLength = static_cast<TForteUInt16>((getCapacity() * 3) >> 1);
    if(nNewLength < pa_nRequestedSize){
      nNewLength = pa_nRequestedSize;
    }

    TForteByte *newMemory = (TForteByte *) forte_malloc(nNewLength + 5);  // the plus five are 2 bytes for length, 2 bytes for capacity and one for a backup \0
    TForteByte *oldMemory = getGenData();
    if(0 != oldMemory){
      memcpy(newMemory, oldMemory, getCapacity() + 5);
      forte_free(oldMemory);
    }
    setGenData(newMemory);
    setAllocatedLength(static_cast<TForteUInt16>(nNewLength));  //only newLength is useable for strings and should be considered in the size checks
    if (firstAlloc) {
      setLength(nLength);  //necessary to initialize the length if this is the first reserve call
      getValue()[nLength] = '\0';
    }
  }
}

int CIEC_ANY_STRING::determineEscapedStringLength(const char *pa_pacValue, char pa_cDelimiter){
  if (*pa_pacValue != pa_cDelimiter){
    return static_cast<unsigned int>(strlen(pa_pacValue));
  }

  const char *pacRunner;
  for (pacRunner = pa_pacValue+1; *pacRunner != pa_cDelimiter && *pacRunner; ++pacRunner) {
    if('$' == *pacRunner){
      TForteUInt16 nDummy;
      ++pacRunner;
      if(*pacRunner == '\0') {
        break;
      }
      if(!handleDollarEscapedChar(&pacRunner, (pa_cDelimiter == '"'), nDummy)){
        continue; // It is invalid but we need the real end
      }
    }
  }

  return (pa_cDelimiter == *pacRunner) ? static_cast<unsigned int>(pacRunner + 1 - pa_pacValue) : -1;
}

bool CIEC_ANY_STRING::handleDollarEscapedChar(const char **pa_pacValue, bool pa_bWide, TForteUInt16 &pa_rnValue){
  bool bRetVal = true;
  switch((*pa_pacValue)[0]){
    case '\'':
    case '\"':
      pa_rnValue = (*pa_pacValue)[0];
      break;
    case 'L':
    case 'l':
      pa_rnValue = 0x10;   //ASCI 0x10 is the line feed character
      break;
    case 'N':
    case 'n':
      pa_rnValue ='\n';
      break;
    case 'P':
    case 'p':
      pa_rnValue = '\f';
      break;
    case 'R':
    case 'r':
      pa_rnValue = '\r';
      break;
    case 'T':
    case 't':
      pa_rnValue = '\t';
      break;
    case '$':
      pa_rnValue = '$';
      break;
    default:
      bRetVal = parseEscapedHexNum(pa_pacValue, pa_bWide, pa_rnValue);
      break;
  }
  return bRetVal;
}

int CIEC_ANY_STRING::dollarEscapeChar(char *pa_pacValue, char pa_cValue, unsigned int pa_nBufferSize) {
  unsigned int nUsedBytes = 1;
  char cVal = pa_cValue;
  switch(pa_cValue){
    case '$':
      ++nUsedBytes;
      cVal = '$';
      break;
    case '\'':
      ++nUsedBytes;
      cVal = '\'';
      break;
    case 0x10:   // line feed
      ++nUsedBytes;
      cVal = 'l';
      break;
    case '\n':
      ++nUsedBytes;
      cVal = 'n';
      break;
    case '\f':
      ++nUsedBytes;
      cVal = 'p';
      break;
    case '\r':
      ++nUsedBytes;
      cVal = 'r';
      break;
    case '\t':
      ++nUsedBytes;
      cVal = 't';
      break;
    case '\"':
      ++nUsedBytes;
      cVal = '\"';
      break;
    default:
      break;
  }

  if (pa_pacValue == 0)
    return nUsedBytes;

  if (nUsedBytes > pa_nBufferSize)
    return -1;

  if (nUsedBytes == 2)
    *pa_pacValue++ = '$';

  *pa_pacValue = cVal;

  return nUsedBytes;
}

bool CIEC_ANY_STRING::parseEscapedHexNum(const char **pa_pacValue, bool pa_bWide, TForteUInt16 &pa_rnValue){
  bool bRetVal = false;

  pa_rnValue = 0;
  if (forte::core::util::isHexDigit((*pa_pacValue)[0])){
    pa_rnValue = static_cast<TForteUInt16>(forte::core::util::charHexDigitToInt((*pa_pacValue)[0]) << 4);
    if (forte::core::util::isHexDigit((*pa_pacValue)[1])){
      pa_rnValue = TForteUInt16(pa_rnValue | forte::core::util::charHexDigitToInt((*pa_pacValue)[1])); //operator | promotes operator uint16_t to int.

      if (pa_bWide) {
        pa_rnValue = TForteUInt16(pa_rnValue << 8);

        if(forte::core::util::isHexDigit((*pa_pacValue)[2])) {
          pa_rnValue = TForteUInt16(pa_rnValue | forte::core::util::charHexDigitToInt((*pa_pacValue)[2]) << 4);
        } else {
          return false;
        }
        if(forte::core::util::isHexDigit((*pa_pacValue)[3])) {
          pa_rnValue = TForteUInt16(pa_rnValue | forte::core::util::charHexDigitToInt((*pa_pacValue)[3]));
        } else {
          return false;
        }
      }

      *pa_pacValue += pa_bWide ? 3 : 1;
      bRetVal = true;
    }
  }
  return bRetVal;
}

int CIEC_ANY_STRING::unescapeFromString(const char *pa_pacValue, char pa_cDelimiter) {
  TForteUInt16 nLen = 0;
  int nRetVal = -1;
  const char *pacRunner = pa_pacValue;
  TForteUInt16 nValue;
  bool bWide = (pa_cDelimiter == '"');

  if(*pacRunner == pa_cDelimiter){
    //remove leading string delimiter char
    ++pacRunner;
  }

  char *acValue = getValue();
  while((*pacRunner != '\0') && (nLen != scm_unMaxStringLen)){
    if('$' == *pacRunner){
      ++pacRunner;
      if(*pacRunner == '\0') {
        break;
      }
      if(!handleDollarEscapedChar(&pacRunner, bWide, nValue)){
        return -1;
      }

#ifdef FORTE_USE_WSTRING_DATATYPE
      if (! bWide)
#endif //FORTE_USE_WSTRING_DATATYPE
        acValue[nLen] = (char) nValue;
#ifdef FORTE_USE_WSTRING_DATATYPE
      else {
        int nEncLen = CUnicodeUtilities::encodeUTF8Codepoint(reinterpret_cast<TForteByte *>(acValue + nLen), 3, nValue);
        if (nEncLen < 0) {
          return -1;
        }
        nLen = static_cast<TForteUInt16>(nLen + nEncLen - 1);
      }
#endif //FORTE_USE_WSTRING_DATATYPE
    }
    else{
      if(pa_cDelimiter == *pacRunner){
        //we are at the end of the string
        ++pacRunner;
        break;
      }
      else{
        acValue[nLen] = *pacRunner;
      }
    }
    ++pacRunner;
    ++nLen;
  }

  getValue()[nLen] = '\0';
  setLength(static_cast<TForteUInt16>(nLen));
  nRetVal = static_cast<TForteUInt16>(pacRunner - pa_pacValue);

  return nRetVal;
}
