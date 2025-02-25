/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
 *               TU Wien/ACIN, Primetals Technologies Austria GmbH
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
 *                            - fixes behavior for getToStringBufferSize
 *                            - changes storage to std::string
 *******************************************************************************/
#include "forte_string.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_string_gen.cpp"
#endif
#include <devlog.h>
#include "forte_uint.h"
#include "unicode_utils.h"
#include <string_view>
#include <charconv>

using namespace std::string_literals;

DEFINE_FIRMWARE_DATATYPE(STRING, g_nStringIdSTRING);

CIEC_STRING::CIEC_STRING(const char* paValue) : mValue(paValue, forte_strnlen_s(paValue, scmMaxStringLen)) {
}

CIEC_STRING::CIEC_STRING(const char* paValue, const size_t paLength) : mValue(paValue, std::min(paLength, scmMaxStringLen)) {
}

CIEC_STRING::storage_type &CIEC_STRING::getStorageMutable() {
  return mValue;
}

int CIEC_STRING::fromString(const char *paValue) {
  const std::string_view inputString(paValue);
  constexpr std::string_view typePrefix("STRING#"); // a C-String on purpose to avoid a dangling reference
  constexpr size_t typePrefixLength = typePrefix.length();

  size_t stringStart = 0;

  if(inputString.length() > typePrefixLength && inputString.substr(0, typePrefixLength) == typePrefix) { // There is a type prefix at the begin and not in string
    stringStart += typePrefixLength;
  }

  const size_t openingQuote = inputString.find('\'', stringStart);
  size_t closingQuote = std::string::npos;

  std::string unescapedString;
  unescapedString.reserve(inputString.length() - stringStart);

  for(size_t i = openingQuote + 1; i < inputString.length(); ++i) {
    if (inputString[i] == '$') {
      if(forte::core::util::isHexDigit(inputString[i + 1]) && forte::core::util::isHexDigit(inputString[i + 2])) {
        uint8_t hexNumber;
        auto result = std::from_chars(inputString.data() + (i + 1), inputString.data() + (i + 3), hexNumber, 16);
        if(result.ec == std::errc()) {
          unescapedString += hexNumber; // append hex code char
        }
        i += 2;
        continue;
      } else {
        char specialSymbol = 0;
        switch (inputString[i + 1]) {
          case '\'':
            specialSymbol = '\'';
            break;
          case 'L':
          case 'l':
            specialSymbol = '\x10';
            break;
          case 'N':
          case 'n':
            specialSymbol = '\n';
            break;
          case 'P':
          case 'p':
            specialSymbol = '\f';
            break;
          case 'R':
          case 'r':
            specialSymbol = '\r';
            break;
          case 'T':
          case 't':
            specialSymbol = '\t';
            break;
          case '$':
            specialSymbol = '$';
            break;
          default:
            DEVLOG_ERROR("Unknown escape symbol %c encountered", inputString[i + 1]);
        }
        unescapedString += specialSymbol; // append escaped char
        i += 1;
        continue;
      }
    }
    else if (inputString[i] == '\'') { // In case of an escaped \' we would have passed it above
      closingQuote = i;
      break;
    }
    unescapedString += (inputString[i]); // append simple char
  }

  if(closingQuote != std::string::npos) {
    mValue = unescapedString;
    return static_cast<int>(closingQuote + 1);
  }

  mValue.clear(); // Clear string in case of a failed fromString
  return -1;
}

void CIEC_STRING::reserve(const TForteUInt16 paRequestedSize) {
  mValue.reserve(paRequestedSize);
}

void CIEC_STRING::assign(const char *paData, const TForteUInt16 paLen) {
  mValue.assign(paData, paLen);
}

void CIEC_STRING::append(const char *paData, const TForteUInt16 paLen) {
  mValue.append(paData, paLen);
}

/*! Append data, cannot contain '0x00' as this is used to identify the end of the cstring
 */
void CIEC_STRING::append(const char *paData) {
  this->append(paData, static_cast < TForteUInt16>(strlen(paData)));
}

void CIEC_STRING::append(const CIEC_STRING &paValue) {
  this->append(paValue.getStorage());
}

void CIEC_STRING::append(const std::string &paValue) {
  mValue.append(paValue);
}

int CIEC_STRING::compare(const CIEC_STRING& paValue) const {
  return mValue.compare(paValue.getStorage());
}

int CIEC_STRING::determineEscapedStringLength(const char *paValue, char paDelimiter, const size_t paLength) {
  if (*paValue != paDelimiter){
    return static_cast<unsigned int>(paLength);
  }

  const char *pacRunner = paValue + 1;
  for (size_t i = 0; i < paLength; ++i, ++pacRunner) {
    if('$' == *pacRunner){
      TForteUInt16 nDummy;
      ++pacRunner;
      if(!handleDollarEscapedChar(&pacRunner, (paDelimiter == '"'), nDummy)){
        continue; // It is invalid but we need the real end
      }
    }
  }

  return (paDelimiter == *pacRunner) ? static_cast<unsigned int>(pacRunner + 1 - paValue) : -1;
}

int CIEC_STRING::toString(char* paValue, size_t paBufferSize) const {
  int nRetVal = -1;
  if(nullptr != paValue && getToStringBufferSize() <= paBufferSize){
    *paValue = '\'';
    paValue++;
    TForteUInt16 nLen = length();
    size_t nUsedBytes = 0;
    paBufferSize -= 2;
    if(0 < nLen){
      const char * acValue = getStorage().c_str();
      for(unsigned int i = 0; i < nLen; ++i){
        if(nUsedBytes >= paBufferSize) {
          return -1;
        }
        nUsedBytes += dollarEscapeChar(paValue+nUsedBytes, acValue[i], 3, getDataTypeID());
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
  size_t neededBufferSize = 0;
  for(auto iter = mValue.begin(); iter != mValue.end(); ++iter){
    if(isprint(static_cast<unsigned char>(*iter)) && '$' != *iter && '\'' != *iter){
      ++neededBufferSize;
    } else {
      switch (*iter){
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
int CIEC_STRING::toUTF8(char* paBuffer, size_t paBufferSize, bool paEscape) const {
  // Count the needed space
  size_t nNeededLength = paEscape ? 2 : 0; // Leading and trailing delimiter
  int nRes;

  const unsigned char *pRunner = reinterpret_cast<const unsigned char *>(getStorage().c_str());
  while (*pRunner) {
    nRes = CUnicodeUtilities::encodeUTF8Codepoint(nullptr, 0, (TForteUInt32) *pRunner);
    if (nRes < 0) {
      return -1;
    }
    nNeededLength += nRes;
    if (nRes == 1 && paEscape && dollarEscapeChar(nullptr, *pRunner, 0, getDataTypeID()) == 2) {
      nNeededLength++;
    }
    ++pRunner;
  }

  if(nullptr == paBuffer) {
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

  pRunner = reinterpret_cast<const unsigned char *>(getStorage().c_str());
  while (*pRunner) {
    nRes = CUnicodeUtilities::encodeUTF8Codepoint((TForteByte *) pEncRunner, static_cast<unsigned int>(pDataEnd - pEncRunner), (TForteUInt32) *pRunner);
    if(nRes == 1 && paEscape) {
      nRes = dollarEscapeChar(pEncRunner, *pRunner, static_cast<unsigned int>(pDataEnd - pEncRunner), getDataTypeID());
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

