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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_any_string_gen.cpp"
#endif
#include "unicode_utils.h"
#include <string.h>
#include <stdlib.h>
#include <devlog.h>
#include <cstdio>

char CIEC_ANY_STRING::smNullString[1] = {'\0'};

CIEC_ANY_STRING::~CIEC_ANY_STRING(){
  if(getGenData()){
    forte_free(getGenData());
  }
}

void CIEC_ANY_STRING::assign(const char *paData, TForteUInt16 paLen) {
  if (nullptr != paData){
    if(0 != paLen && paData != getValue()) {
      reserve(paLen);
      memcpy(getValue(), paData, paLen);
    }
    if(nullptr != getValue()){
      setLength(paLen);
      getValue()[paLen] = '\0'; //not really necessary, but is a stop if someone forgets that this is not a textual string
    }
  }
}

void CIEC_ANY_STRING::append(const char *paData) {
  if (nullptr != paData){
    append(paData, static_cast<TForteUInt16>(strlen(paData)));
  }
}

void CIEC_ANY_STRING::append(const char *paData, TForteUInt16 paLen) {
  if (nullptr != paData){
    TForteUInt16 nLen = length();
    if(0 != paLen){
      if((getCapacity() - nLen) < paLen){
        reserve(static_cast<TForteUInt16>(nLen + paLen));
      }
      if(nullptr != getValue()){
        memcpy(getValue() + nLen, paData, paLen);
        setLength(static_cast<TForteUInt16>(nLen + paLen));
        getValue()[nLen + paLen] = '\0'; //not really necessary, but is a stop if someone forgets that this is not a textual string
      }
    }
  }
}

void CIEC_ANY_STRING::reserve(const TForteUInt16 paRequestedSize){
  if(getCapacity() < paRequestedSize + 1){
    bool firstAlloc = (getGenData() == nullptr);
    TForteUInt16 nLength = length();
    TForteUInt16 nNewLength = static_cast<TForteUInt16>((getCapacity() * 3) >> 1);
    if(nNewLength < paRequestedSize){
      nNewLength = paRequestedSize;
    }

    TForteByte *newMemory = (TForteByte *) forte_malloc(nNewLength + 5);  // the plus five are 2 bytes for length, 2 bytes for capacity and one for a backup \0
    TForteByte *oldMemory = getGenData();
    if(nullptr != oldMemory){
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

int CIEC_ANY_STRING::determineEscapedStringLength(const char *paValue, char paDelimiter){
  if (*paValue != paDelimiter){
    return static_cast<unsigned int>(strlen(paValue));
  }

  const char *pacRunner;
  for (pacRunner = paValue+1; *pacRunner != paDelimiter && *pacRunner; ++pacRunner) {
    if('$' == *pacRunner){
      TForteUInt16 nDummy;
      ++pacRunner;
      if(*pacRunner == '\0') {
        break;
      }
      if(!handleDollarEscapedChar(&pacRunner, (paDelimiter == '"'), nDummy)){
        continue; // It is invalid but we need the real end
      }
    }
  }

  return (paDelimiter == *pacRunner) ? static_cast<unsigned int>(pacRunner + 1 - paValue) : -1;
}

bool CIEC_ANY_STRING::handleDollarEscapedChar(const char **paSymbol, bool paWide, TForteUInt16 &paValue) {
  bool bRetVal = true;
  switch((*paSymbol)[0]){
    case '\'':
    case '\"':
      paValue = (*paSymbol)[0];
      break;
    case 'L':
    case 'l':
      paValue = 0x10;   //ASCI 0x10 is the line feed character
      break;
    case 'N':
    case 'n':
      paValue ='\n';
      break;
    case 'P':
    case 'p':
      paValue = '\f';
      break;
    case 'R':
    case 'r':
      paValue = '\r';
      break;
    case 'T':
    case 't':
      paValue = '\t';
      break;
    case '$':
      paValue = '$';
      break;
    default:
      bRetVal = parseEscapedHexNum(paSymbol, paWide, paValue);
      break;
  }
  return bRetVal;
}

int CIEC_ANY_STRING::dollarEscapeChar(char *paValue, char paSymbol, unsigned int paBufferSize, const EDataTypeID paTypeID) {
  unsigned int nUsedBytes = 1;
  char cVal = paSymbol;
  switch(paSymbol){
    case '$':
      ++nUsedBytes;
      cVal = '$';
      break;
    case '\'':
      if(paTypeID == CIEC_ANY::e_STRING) {
        ++nUsedBytes;
        cVal = '\'';
      }
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
      if(paTypeID == CIEC_ANY::e_WSTRING) {
        ++nUsedBytes;
        cVal = '\"';
      }
      break;
    default:
      if(!isprint(paSymbol) && paTypeID == CIEC_ANY::e_STRING) {
        nUsedBytes += 2; // '$xx'
      }
      break;
  }

  if (paValue == nullptr) {
    return nUsedBytes;
  }

  if (nUsedBytes > paBufferSize) {
    return -1;
  }

  if(nUsedBytes == 3) {
    *paValue = '$';
    char hex[3];
    std::snprintf(hex, sizeof(hex), "%02X", paSymbol);
    *(++paValue) = hex[0];
    *(++paValue) = hex[1];
  } else if (nUsedBytes == 2) {
    *paValue = '$';
    *(++paValue) = cVal;
  } else {
    *paValue = cVal;
  }

  return nUsedBytes;
}

bool CIEC_ANY_STRING::parseEscapedHexNum(const char **paSymbol, bool paWide, TForteUInt16 &paValue){
  bool bRetVal = false;

  paValue = 0;
  if (forte::core::util::isHexDigit((*paSymbol)[0])){
    paValue = static_cast<TForteUInt16>(forte::core::util::charHexDigitToInt((*paSymbol)[0]) << 4);
    if (forte::core::util::isHexDigit((*paSymbol)[1])){
      paValue = TForteUInt16(paValue | forte::core::util::charHexDigitToInt((*paSymbol)[1])); //operator | promotes operator uint16_t to int.

      if (paWide) {
        paValue = TForteUInt16(paValue << 8);

        if(forte::core::util::isHexDigit((*paSymbol)[2])) {
          paValue = TForteUInt16(paValue | forte::core::util::charHexDigitToInt((*paSymbol)[2]) << 4);
        } else {
          return false;
        }
        if(forte::core::util::isHexDigit((*paSymbol)[3])) {
          paValue = TForteUInt16(paValue | forte::core::util::charHexDigitToInt((*paSymbol)[3]));
        } else {
          return false;
        }
      }

      *paSymbol += paWide ? 3 : 1;
      bRetVal = true;
    }
  }
  return bRetVal;
}

int CIEC_ANY_STRING::unescapeFromString(const char *paValue, char paDelimiter) {
  TForteUInt16 nLen = 0;
  int nRetVal = -1;
  const char *runner = paValue;
  TForteUInt16 nValue;
  bool bWide = (paDelimiter == '"');

  if(*runner == paDelimiter) {
    //remove leading string delimiter char
    ++runner;
  }

  char *value = getValue();
  while((*runner != '\0') && (nLen != scmMaxStringLen)){
    if('$' == *runner){
      ++runner;
      if(*runner == '\0') {
        break;
      }
      if(!handleDollarEscapedChar(&runner, bWide, nValue)){
        return -1;
      }

      if (! bWide) {
        value[nLen] = (char) nValue;
      } else {
        int nEncLen = CUnicodeUtilities::encodeUTF8Codepoint(reinterpret_cast<TForteByte *>(value + nLen), 3, nValue);
        if (nEncLen < 0) {
          return -1;
        }
        nLen = static_cast<TForteUInt16>(nLen + nEncLen - 1);
      }
    }
    else{
      if(paDelimiter == *runner){
        //we are at the end of the string
        ++runner;
        break;
      }
      else{
        value[nLen] = *runner;
      }
    }
    ++runner;
    ++nLen;
  }

  getValue()[nLen] = '\0';
  setLength(static_cast<TForteUInt16>(nLen));
  nRetVal = static_cast<TForteUInt16>(runner - paValue);

  return nRetVal;
}
