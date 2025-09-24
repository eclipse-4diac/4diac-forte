/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, TU Wien/ACIN, nxtControl GmbH,
 *                          fortiss GmbH, Primetals Technologies Austria GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Rene Smodic, Alois Zoitl,
 *   Ingo Hegny, Martin Melik Merkumians, Stanislav Meduna
 *      - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - fixes toString behavior for 0 size buffer
 *                           - fixes behavior for getToStringBufferSize
 *                           - changes storage to std::string
 *   Alois Zoitl - migrated data type toString to std::string
 *   Martin Jobst - fix line feed and newline escape sequences
 *******************************************************************************/

#include "forte/datatypes/forte_string.h"

#include <cstddef>
#include <charconv>
#include <string_view>

#include "forte/datatype.h"
#include "forte/util/devlog.h"
#include "forte/util/string_utils.h"

#include "forte/arch/forte_fileio.h"

#include "unicode_utils.h"

using namespace forte::literals;
using namespace std::string_literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(STRING, "STRING"_STRID);

  CIEC_STRING::CIEC_STRING(const char *paValue) : mValue(paValue, forte_strnlen_s(paValue, scmMaxStringLen)) {
  }

  CIEC_STRING::CIEC_STRING(const char *paValue, const size_t paLength) :
      mValue(paValue, std::min(paLength, scmMaxStringLen)) {
  }

  CIEC_STRING::storage_type &CIEC_STRING::getStorageMutable() {
    return mValue;
  }

  int CIEC_STRING::fromString(const char *paValue) {
    const std::string_view inputString(paValue);
    constexpr std::string_view typePrefix("STRING#"); // a C-String on purpose to avoid a dangling reference
    constexpr size_t typePrefixLength = typePrefix.length();

    size_t stringStart = 0;

    if (inputString.length() > typePrefixLength &&
        inputString.substr(0, typePrefixLength) ==
            typePrefix) { // There is a type prefix at the begin and not in string
      stringStart += typePrefixLength;
    }

    const size_t openingQuote = inputString.find('\'', stringStart);
    size_t closingQuote = std::string::npos;

    std::string unescapedString;
    unescapedString.reserve(inputString.length() - stringStart);

    for (size_t i = openingQuote + 1; i < inputString.length(); ++i) {
      if (inputString[i] == '$') {
        if (util::isHexDigit(inputString[i + 1]) && util::isHexDigit(inputString[i + 2])) {
          uint8_t hexNumber;
          auto result = std::from_chars(inputString.data() + (i + 1), inputString.data() + (i + 3), hexNumber, 16);
          if (result.ec == std::errc()) {
            unescapedString += hexNumber; // append hex code char
          }
          i += 2;
          continue;
        } else {
          char specialSymbol = 0;
          switch (inputString[i + 1]) {
            case '\'': specialSymbol = '\''; break;
            case 'N': // Newline is an implementation-independent alias for the end of a line
            case 'n': // FORTE uses LF on all platforms
            case 'L':
            case 'l': specialSymbol = '\n'; break;
            case 'P':
            case 'p': specialSymbol = '\f'; break;
            case 'R':
            case 'r': specialSymbol = '\r'; break;
            case 'T':
            case 't': specialSymbol = '\t'; break;
            case '$': specialSymbol = '$'; break;
            default: DEVLOG_ERROR("Unknown escape symbol %c encountered", inputString[i + 1]);
          }
          unescapedString += specialSymbol; // append escaped char
          i += 1;
          continue;
        }
      } else if (inputString[i] == '\'') { // In case of an escaped \' we would have passed it above
        closingQuote = i;
        break;
      }
      unescapedString += (inputString[i]); // append simple char
    }

    if (closingQuote != std::string::npos) {
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
    this->append(paData, static_cast<TForteUInt16>(strlen(paData)));
  }

  void CIEC_STRING::append(const CIEC_STRING &paValue) {
    this->append(paValue.getStorage());
  }

  void CIEC_STRING::append(const std::string &paValue) {
    mValue.append(paValue);
  }

  int CIEC_STRING::compare(const CIEC_STRING &paValue) const {
    return mValue.compare(paValue.getStorage());
  }

  void CIEC_STRING::toString(std::string &paTargetBuf) const {
    paTargetBuf += '\'';
    for (auto c : getStorage()) {
      dollarEscapeChar(paTargetBuf, c, getDataTypeID());
    }
    paTargetBuf += '\'';
  }

  void CIEC_STRING::toUTF8(std::string &paBuffer, bool paEscape) const {
    TForteByte utf8Buffer[4];

    if (paEscape) {
      paBuffer += '\'';
    }

    for (char c : getStorage()) {
      TForteByte uc = static_cast<TForteByte>(c);
      int nRes = CUnicodeUtilities::encodeUTF8Codepoint(utf8Buffer, 4, static_cast<TForteUInt32>(uc));
      if (nRes < 0) {
        break;
      }

      if (paEscape) {
        for (size_t i = 0; i < static_cast<size_t>(nRes); i++) {
          dollarEscapeChar(paBuffer, static_cast<char>(utf8Buffer[i]), getDataTypeID());
        }
      } else {
        paBuffer.append(reinterpret_cast<char *>(utf8Buffer), static_cast<size_t>(nRes));
      }
    }

    if (paEscape) {
      paBuffer += '\'';
    }
  }

  const StringId CDataTypeTrait<CIEC_STRING>::scmDataTypeName = "STRING"_STRID;
} // namespace forte
