/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *               
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_char.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_char_gen.cpp"
#endif

#include "unicode_utils.h"

#include <limits>
#include <stdio.h>

DEFINE_FIRMWARE_DATATYPE(CHAR, g_nStringIdCHAR)

const CIEC_CHAR::TValueType CIEC_CHAR::scmMaxVal = std::numeric_limits<CIEC_CHAR::TValueType>::max();

int CIEC_CHAR::toString(char *paValue, size_t paBufferSize) const {
  const char longestStringSerialization[] = "CHAR#'$00'";
  const size_t longestStringSize = sizeof(longestStringSerialization);
  if(paBufferSize >= longestStringSize) { //sizeof is char string + \0
  const unsigned char symbol = this->operator TForteChar();
    switch(symbol) {
      case '\0': return snprintf(paValue, longestStringSize, "CHAR#''");
      case '$': return snprintf(paValue, longestStringSize, "CHAR#'$$'");
      case '\'': return snprintf(paValue, longestStringSize, "CHAR#'$''");
      case '\n': return snprintf(paValue, longestStringSize, "CHAR#'$N'");
      case '\f': return snprintf(paValue, longestStringSize, "CHAR#'$P'"); // page aka form feed
      case '\r': return snprintf(paValue, longestStringSize, "CHAR#'$R'");
      case '\t': return snprintf(paValue, longestStringSize, "CHAR#'$T'");
      default: return snprintf(paValue, longestStringSize, "CHAR#'%c'", symbol);
    }
  }
  return -1;
}

int CIEC_CHAR::fromString(const char *paValue) {
  int bufferCount;    
  if ('\'' != paValue[0] && 'C' != paValue[0]) { // Not a valid literal start
    return -1;
  } else if('C' == paValue[0] && 'H' == paValue[1] && 'A' == paValue[2]
       && 'R' == paValue[3] && '#' == paValue[4] && '\'' == paValue[5]) { // qualifier start
    bufferCount = 6; // Start of char with paValue
  } else if ('\'' == paValue[0]) {
    bufferCount = 1 ; // Start of char without qualifier
  } else {
    return -1; // Type qualifier was wrong
  }

  if('\'' == paValue[bufferCount]) { // Char is immediatly closed
    *this = CIEC_CHAR('\0');
    return bufferCount + 1; // closing '
  }

  if('$' != paValue[bufferCount] && '\'' == paValue[bufferCount + 1]) { // No escape sequence, next symbol is the char
    *this = CIEC_CHAR(paValue[bufferCount]);
    return bufferCount + 2; //including consumed symbol and closing '
  }

  if('$' == paValue[bufferCount]) { // Escape sequence, so the next symbol must either be a hex number or a special symbol
    if('\'' == paValue[bufferCount + 2]) { // if there is only one symbol it will get considered as special symbol
      const char controlSymbol = static_cast<char>(toupper(static_cast<unsigned char>(paValue[bufferCount + 1])));
      switch(controlSymbol) {
        case '$': *this = CIEC_CHAR('$'); break;
        case '\'': *this = CIEC_CHAR('\''); break;
        case 'L': *this = CIEC_CHAR('\n'); break;
        case 'N': *this = CIEC_CHAR('\n'); break;
        case 'P': *this = CIEC_CHAR('\f'); break;
        case 'R': *this = CIEC_CHAR('\r'); break;
        case 'T': *this = CIEC_CHAR('\t'); break;
        default: return -1;
      }
      return bufferCount + 3; // $ + control symbol + '
    }

    if(forte::core::util::isHexDigit(paValue[bufferCount + 1]) && forte::core::util::isHexDigit(paValue[bufferCount + 2]) 
      && '\'' == paValue[bufferCount + 3]) { // if there are two symbols it is a hex code
      const TForteChar codePoint = static_cast<TForteChar>(
              (forte::core::util::charHexDigitToInt(paValue[bufferCount + 1]) << 4) +
              forte::core::util::charHexDigitToInt(paValue[bufferCount + 2]));
      *this = CIEC_CHAR(codePoint);
      return bufferCount + 4; // Three symbols for code point and closing '
    }
  }

  return -1; //no match so something must be wrong
}

size_t CIEC_CHAR::getToStringBufferSize() const {
  return sizeof("'$00'");
}
