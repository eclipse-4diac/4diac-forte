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
#include "forte_wchar.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_wchar_gen.cpp"
#endif

#include "unicode_utils.h"

#include <limits>
#include <stdio.h>

DEFINE_FIRMWARE_DATATYPE(WCHAR, g_nStringIdWCHAR)

const CIEC_WCHAR::TValueType CIEC_WCHAR::scmMaxVal = std::numeric_limits<CIEC_WCHAR::TValueType>::max();

int CIEC_WCHAR::toString(char *paValue, size_t paBufferSize) const {
  const char longestStringSerialization[] = "WCHAR#\"$0000\"";
  const size_t longestStringSize = sizeof(longestStringSerialization);
  if(paBufferSize >= longestStringSize) { //sizeof is char string + \0
  const TForteWChar symbol = this->operator TForteWChar();
    switch(symbol) {
      case '\0': return snprintf(paValue, longestStringSize, "WCHAR#\"\"");
      case '$': return snprintf(paValue, longestStringSize, "WCHAR#\"$$\"");
      case '"': return snprintf(paValue, longestStringSize, "WCHAR#\"$\"\"");
      case '\n': return snprintf(paValue, longestStringSize, "WCHAR#\"$N\"");
      case '\f': return snprintf(paValue, longestStringSize, "WCHAR#\"$P\""); // page aka form feed
      case '\r': return snprintf(paValue, longestStringSize, "WCHAR#\"$R\"");
      case '\t': return snprintf(paValue, longestStringSize, "WCHAR#\"$T\"");
      default: {
        if(symbol < 256) {
            return snprintf(paValue, longestStringSize, "WCHAR#\"%c\"", symbol);
        }
        return snprintf(paValue, longestStringSize, "WCHAR#\"$%X\"", symbol);
      }
    }
  }
  return -1;
}

int CIEC_WCHAR::fromString(const char *paValue) {
  int bufferCount;    
  if ('"' != paValue[0] && 'W' != paValue[0]) { // Not a valid literal start
    return -1;
  }

  if('W' == paValue[0] && 'C' == paValue[1] && 'H' == paValue[2] && 'A' == paValue[3]
       && 'R' == paValue[4] && '#' == paValue[5] && '"' == paValue[6]) { // qualifier start
    bufferCount = 7; // Start of char with paValue
  } else if ('"' == paValue[0]) {
    bufferCount = 1 ; // Start of char without qualifier
  } else {
    return -1; // Type qualifier was wrong
  }

  if('"' == paValue[bufferCount]) { // Char is immediatly closed
    *this = CIEC_WCHAR('\0');
    return bufferCount + 1; // closing '
  }

  if('$' != paValue[bufferCount] && '"' == paValue[bufferCount + 1]) { // No escape sequence, next symbol is the char
    *this = CIEC_WCHAR(paValue[bufferCount]);
    return bufferCount + 2; //including consumed symbol and closing '
  }

  if('$' == paValue[bufferCount]) { // Escape sequence, so the next symbol must either be a hex number or a special symbol
    if('"' == paValue[bufferCount + 2]) { // if there is only one symbol it will get considered as special symbol
      const char controlSymbol = static_cast<char>(toupper(static_cast<unsigned char>(paValue[bufferCount + 1])));
      switch(controlSymbol) {
        case '$': *this = CIEC_WCHAR('$'); break;
        case '"': *this = CIEC_WCHAR('"'); break;
        case 'L': *this = CIEC_WCHAR('\n'); break;
        case 'N': *this = CIEC_WCHAR('\n'); break;
        case 'P': *this = CIEC_WCHAR('\f'); break;
        case 'R': *this = CIEC_WCHAR('\r'); break;
        case 'T': *this = CIEC_WCHAR('\t'); break;
        default: return -1;
      }
      return bufferCount + 3; // $ + control symbol + '
    }

    if(forte::core::util::isHexDigit(paValue[bufferCount + 1]) && forte::core::util::isHexDigit(paValue[bufferCount + 2]) 
      && forte::core::util::isHexDigit(paValue[bufferCount + 3]) && forte::core::util::isHexDigit(paValue[bufferCount + 4])
      && '"' == paValue[bufferCount + 5]) { // if there are two symbols it is a hex code
      TForteWChar codePoint = static_cast<TForteWChar>(
              (forte::core::util::charHexDigitToInt(paValue[bufferCount + 1]) << 12) +
              (forte::core::util::charHexDigitToInt(paValue[bufferCount + 2]) << 8) +
              (forte::core::util::charHexDigitToInt(paValue[bufferCount + 3]) << 4) +
              (forte::core::util::charHexDigitToInt(paValue[bufferCount + 4])));
      *this = CIEC_WCHAR(codePoint);
      return bufferCount + 6; // Three symbols for code point and closing '
    }
  }

  return -1; //no match so something must be wrong
}

size_t CIEC_WCHAR::getToStringBufferSize() const {
  return sizeof("$0000");
}
