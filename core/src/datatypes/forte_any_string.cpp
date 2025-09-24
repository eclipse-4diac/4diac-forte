/*******************************************************************************
 * Copyright (c) 2011, 2025 nxtControl GmbH, ACIN, fortiss GmbH
 *                          TU Wien/ACIN, Martin Erich Jobst
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
 *    Martin Jobst - fix line feed and newline escape sequences
 *******************************************************************************/

#include "forte/datatypes/forte_any_string.h"

#include <cstring>

#include "forte/util/string_utils.h"
#include "unicode_utils.h"

namespace forte {
  char CIEC_ANY_STRING::smNullString[1] = {'\0'};

  CIEC_ANY_STRING::~CIEC_ANY_STRING() {
    if (getGenData()) {
      operator delete(getGenData());
    }
  }

  void CIEC_ANY_STRING::assign(const char *paData, TForteUInt16 paLen) {
    if (nullptr != paData) {
      if (0 != paLen && paData != getValue()) {
        reserve(paLen);
        memcpy(getValue(), paData, paLen);
      }
      if (nullptr != getValue()) {
        setLength(paLen);
        getValue()[paLen] =
            '\0'; // not really necessary, but is a stop if someone forgets that this is not a textual string
      }
    }
  }

  void CIEC_ANY_STRING::append(const char *paData) {
    if (nullptr != paData) {
      append(paData, static_cast<TForteUInt16>(strlen(paData)));
    }
  }

  void CIEC_ANY_STRING::append(const char *paData, TForteUInt16 paLen) {
    if (nullptr != paData) {
      TForteUInt16 nLen = length();
      if (0 != paLen) {
        if ((getCapacity() - nLen) < paLen) {
          reserve(static_cast<TForteUInt16>(nLen + paLen));
        }
        if (nullptr != getValue()) {
          memcpy(getValue() + nLen, paData, paLen);
          setLength(static_cast<TForteUInt16>(nLen + paLen));
          getValue()[nLen + paLen] =
              '\0'; // not really necessary, but is a stop if someone forgets that this is not a textual string
        }
      }
    }
  }

  void CIEC_ANY_STRING::reserve(const TForteUInt16 paRequestedSize) {
    if (getCapacity() < paRequestedSize + 1) {
      bool firstAlloc = (getGenData() == nullptr);
      TForteUInt16 nLength = length();
      TForteUInt16 nNewLength = static_cast<TForteUInt16>((getCapacity() * 3) >> 1);
      if (nNewLength < paRequestedSize) {
        nNewLength = paRequestedSize;
      }

      TForteByte *newMemory = static_cast<TForteByte *>(operator new(
          nNewLength + 5)); // the plus five are 2 bytes for length, 2 bytes for capacity and one for a backup \0
      TForteByte *oldMemory = getGenData();
      if (nullptr != oldMemory) {
        memcpy(newMemory, oldMemory, getCapacity() + 5);
        operator delete(oldMemory);
      }
      setGenData(newMemory);
      setAllocatedLength(static_cast<TForteUInt16>(
          nNewLength)); // only newLength is useable for strings and should be considered in the size checks
      if (firstAlloc) {
        setLength(nLength); // necessary to initialize the length if this is the first reserve call
        getValue()[nLength] = '\0';
      }
    }
  }

  int CIEC_ANY_STRING::determineEscapedStringLength(const char *paValue, char paDelimiter) {
    if (*paValue != paDelimiter) {
      return static_cast<unsigned int>(strlen(paValue));
    }

    const char *pacRunner;
    for (pacRunner = paValue + 1; *pacRunner != paDelimiter && *pacRunner; ++pacRunner) {
      if ('$' == *pacRunner) {
        TForteUInt16 nDummy;
        ++pacRunner;
        if (*pacRunner == '\0') {
          break;
        }
        if (!handleDollarEscapedChar(&pacRunner, (paDelimiter == '"'), nDummy)) {
          continue; // It is invalid but we need the real end
        }
      }
    }

    return (paDelimiter == *pacRunner) ? static_cast<unsigned int>(pacRunner + 1 - paValue) : -1;
  }

  bool CIEC_ANY_STRING::handleDollarEscapedChar(const char **paSymbol, bool paWide, TForteUInt16 &paValue) {
    switch ((*paSymbol)[0]) {
      case '\'':
      case '\"': paValue = static_cast<unsigned char>((*paSymbol)[0]); break;
      case 'N': // Newline is an implementation-independent alias for the end of a line
      case 'n': // FORTE uses LF on all platforms
      case 'L':
      case 'l': paValue = '\n'; break;
      case 'P':
      case 'p': paValue = '\f'; break;
      case 'R':
      case 'r': paValue = '\r'; break;
      case 'T':
      case 't': paValue = '\t'; break;
      case '$': paValue = '$'; break;
      default: return parseEscapedHexNum(paSymbol, paWide, paValue);
    }
    return true;
  }

  bool CIEC_ANY_STRING::parseEscapedHexNum(const char **paSymbol, bool paWide, TForteUInt16 &paValue) {
    bool bRetVal = false;

    paValue = 0;
    if (util::isHexDigit((*paSymbol)[0])) {
      paValue = static_cast<TForteUInt16>(util::charHexDigitToInt((*paSymbol)[0]) << 4);
      if (util::isHexDigit((*paSymbol)[1])) {
        paValue = TForteUInt16(
            paValue | util::charHexDigitToInt((*paSymbol)[1])); // operator | promotes operator uint16_t to int.

        if (paWide) {
          paValue = TForteUInt16(paValue << 8);

          if (util::isHexDigit((*paSymbol)[2])) {
            paValue = TForteUInt16(paValue | util::charHexDigitToInt((*paSymbol)[2]) << 4);
          } else {
            return false;
          }
          if (util::isHexDigit((*paSymbol)[3])) {
            paValue = TForteUInt16(paValue | util::charHexDigitToInt((*paSymbol)[3]));
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

    if (*runner == paDelimiter) {
      // remove leading string delimiter char
      ++runner;
    }

    char *value = getValue();
    while ((*runner != '\0') && (nLen != scmMaxStringLen)) {
      if ('$' == *runner) {
        ++runner;
        if (*runner == '\0') {
          break;
        }
        if (!handleDollarEscapedChar(&runner, bWide, nValue)) {
          return -1;
        }

        if (!bWide) {
          value[nLen] = (char) nValue;
        } else {
          int nEncLen = CUnicodeUtilities::encodeUTF8Codepoint(reinterpret_cast<TForteByte *>(value + nLen), 3, nValue);
          if (nEncLen < 0) {
            return -1;
          }
          nLen = static_cast<TForteUInt16>(nLen + nEncLen - 1);
        }
      } else {
        if (paDelimiter == *runner) {
          // we are at the end of the string
          ++runner;
          break;
        } else {
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
} // namespace forte
