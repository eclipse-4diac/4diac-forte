/*******************************************************************************
 * Copyright (c) 2022, 2025 Primetals Technologies Austria GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Martin Jobst - fix line feed and newline escape sequences
 *******************************************************************************/
#include "forte/datatypes/forte_wchar.h"
#include <cstdint>
#include <format>
#include "forte/util/string_utils.h"

using namespace forte::literals;
using namespace std::literals::string_literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(WCHAR, "WCHAR"_STRID)

  void CIEC_WCHAR::toString(std::string &paTargetBuf) const {
    const TForteWChar symbol = this->operator TForteWChar();
    switch (symbol) {
      case '\0': paTargetBuf += "WCHAR#\"$0000\""s; break;
      case '$': paTargetBuf += "WCHAR#\"$$\""s; break;
      case '"': paTargetBuf += "WCHAR#\"$\"\""s; break;
      case '\n': paTargetBuf += "WCHAR#\"$l\""s; break;
      case '\f':
        paTargetBuf += "WCHAR#\"$p\""s; // page aka form feed
        break;
      case '\r': paTargetBuf += "WCHAR#\"$r\""s; break;
      case '\t': paTargetBuf += "WCHAR#\"$t\""s; break;
      default:
        paTargetBuf += "WCHAR#\""s;
        if (symbol < 256 && isprint(static_cast<unsigned char>(symbol))) {
          paTargetBuf += static_cast<char>(symbol);
        } else {
          std::format_to(std::back_inserter(paTargetBuf), "${:04X}", static_cast<uint16_t>(symbol));
        }
        paTargetBuf += "\"";
        break;
    }
  }

  int CIEC_WCHAR::fromString(const char *paValue) {
    int bufferCount;
    if ('"' != paValue[0] && 'W' != paValue[0]) { // Not a valid literal start
      return -1;
    }

    if ('W' == paValue[0] && 'C' == paValue[1] && 'H' == paValue[2] && 'A' == paValue[3] && 'R' == paValue[4] &&
        '#' == paValue[5] && '"' == paValue[6]) { // qualifier start
      bufferCount = 7; // Start of char with paValue
    } else if ('"' == paValue[0]) {
      bufferCount = 1; // Start of char without qualifier
    } else {
      return -1; // Type qualifier was wrong
    }

    if ('"' == paValue[bufferCount]) { // Char is immediatly closed
      *this = CIEC_WCHAR('\0');
      return bufferCount + 1; // closing '
    }

    if ('$' != paValue[bufferCount] && '"' == paValue[bufferCount + 1]) { // No escape sequence, next symbol is the char
      *this = CIEC_WCHAR(paValue[bufferCount]);
      return bufferCount + 2; // including consumed symbol and closing '
    }

    if ('$' ==
        paValue[bufferCount]) { // Escape sequence, so the next symbol must either be a hex number or a special symbol
      if ('"' == paValue[bufferCount + 2]) { // if there is only one symbol it will get considered as special symbol
        switch (paValue[bufferCount + 1]) {
          case '$': *this = CIEC_WCHAR('$'); break;
          case '"': *this = CIEC_WCHAR('"'); break;
          case 'N': // Newline is an implementation-independent alias for the end of a line
          case 'n': // FORTE uses LF on all platforms
          case 'L':
          case 'l': *this = CIEC_WCHAR('\n'); break;
          case 'P':
          case 'p': *this = CIEC_WCHAR('\f'); break;
          case 'R':
          case 'r': *this = CIEC_WCHAR('\r'); break;
          case 'T':
          case 't': *this = CIEC_WCHAR('\t'); break;
          default: return -1;
        }
        return bufferCount + 3; // $ + control symbol + '
      }

      if (util::isHexDigit(paValue[bufferCount + 1]) && util::isHexDigit(paValue[bufferCount + 2]) &&
          util::isHexDigit(paValue[bufferCount + 3]) && util::isHexDigit(paValue[bufferCount + 4]) &&
          '"' == paValue[bufferCount + 5]) { // if there are two symbols it is a hex code
        TForteWChar codePoint = static_cast<TForteWChar>((util::charHexDigitToInt(paValue[bufferCount + 1]) << 12) +
                                                         (util::charHexDigitToInt(paValue[bufferCount + 2]) << 8) +
                                                         (util::charHexDigitToInt(paValue[bufferCount + 3]) << 4) +
                                                         (util::charHexDigitToInt(paValue[bufferCount + 4])));
        *this = CIEC_WCHAR(codePoint);
        return bufferCount + 6; // Three symbols for code point and closing '
      }
    }

    return -1; // no match so something must be wrong
  }

  const StringId CDataTypeTrait<CIEC_WCHAR>::scmDataTypeName = "WCHAR"_STRID;
} // namespace forte
