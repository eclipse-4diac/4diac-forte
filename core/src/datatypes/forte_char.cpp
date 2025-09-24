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
 *               - initial implementation and rework communication infrastructure
 *   Alois Zoitl - migrated data type toString to std::string
 *   Martin Jobst - fix line feed and newline escape sequences
 *******************************************************************************/
#include "forte/datatypes/forte_char.h"
#include "forte/util/string_utils.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte {

  DEFINE_FIRMWARE_DATATYPE(CHAR, "CHAR"_STRID)

  void CIEC_CHAR::toString(std::string &paTargetBuf) const {
    paTargetBuf += "CHAR#'"s;
    dollarEscapeChar(paTargetBuf, static_cast<char>(operator TForteChar()), e_STRING);
    paTargetBuf += '\'';
  }

  int CIEC_CHAR::fromString(const char *paValue) {
    int bufferCount;
    if ('\'' != paValue[0] && 'C' != paValue[0]) { // Not a valid literal start
      return -1;
    } else if ('C' == paValue[0] && 'H' == paValue[1] && 'A' == paValue[2] && 'R' == paValue[3] && '#' == paValue[4] &&
               '\'' == paValue[5]) { // qualifier start
      bufferCount = 6; // Start of char with paValue
    } else if ('\'' == paValue[0]) {
      bufferCount = 1; // Start of char without qualifier
    } else {
      return -1; // Type qualifier was wrong
    }

    if ('\'' == paValue[bufferCount]) { // Char is immediatly closed
      *this = CIEC_CHAR('\0');
      return bufferCount + 1; // closing '
    }

    if ('$' != paValue[bufferCount] &&
        '\'' == paValue[bufferCount + 1]) { // No escape sequence, next symbol is the char
      *this = CIEC_CHAR(paValue[bufferCount]);
      return bufferCount + 2; // including consumed symbol and closing '
    }

    if ('$' ==
        paValue[bufferCount]) { // Escape sequence, so the next symbol must either be a hex number or a special symbol
      if ('\'' == paValue[bufferCount + 2]) { // if there is only one symbol it will get considered as special symbol
        switch (paValue[bufferCount + 1]) {
          case '$': *this = CIEC_CHAR('$'); break;
          case '\'': *this = CIEC_CHAR('\''); break;
          case 'N': // Newline is an implementation-independent alias for the end of a line
          case 'n': // FORTE uses LF on all platforms
          case 'L':
          case 'l': *this = CIEC_CHAR('\n'); break;
          case 'P':
          case 'p': *this = CIEC_CHAR('\f'); break;
          case 'R':
          case 'r': *this = CIEC_CHAR('\r'); break;
          case 'T':
          case 't': *this = CIEC_CHAR('\t'); break;
          default: return -1;
        }
        return bufferCount + 3; // $ + control symbol + '
      }

      if (util::isHexDigit(paValue[bufferCount + 1]) && util::isHexDigit(paValue[bufferCount + 2]) &&
          '\'' == paValue[bufferCount + 3]) { // if there are two symbols it is a hex code
        const TForteChar codePoint = static_cast<TForteChar>((util::charHexDigitToInt(paValue[bufferCount + 1]) << 4) +
                                                             util::charHexDigitToInt(paValue[bufferCount + 2]));
        *this = CIEC_CHAR(codePoint);
        return bufferCount + 4; // Three symbols for code point and closing '
      }
    }

    return -1; // no match so something must be wrong
  }

  const StringId CDataTypeTrait<CIEC_CHAR>::scmDataTypeName = "CHAR"_STRID;
} // namespace forte
