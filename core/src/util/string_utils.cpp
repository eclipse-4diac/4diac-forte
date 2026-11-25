/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH, Johannes Kepler University Linz,
 *                    Primetals Technologies Austria GmbH,
 *                    Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Martin Jobst - move normalizeToStringRepresentation from CIEC_ANY_REAL
 *   Pedro Ricardo C. Souza - numeric character references in transformEscapedXMLToNonEscapedText
 *******************************************************************************/
#include "forte/util/string_utils.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_lint.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/util/devlog.h"

#include <errno.h>
#include <string.h>
#include <array>
#include <charconv>
#include <map>

using namespace std::string_literals;

namespace forte::util {
  namespace {

    constexpr auto scCharacters2Escape = std::array{"&quot;", "&apos;", "&amp;", "&lt;", "&gt;"};

    const std::map<char, std::tuple<const char *, size_t>> scEscapeMap = {
        {'"', {scCharacters2Escape[0], std::char_traits<char>::length(scCharacters2Escape[0])}},
        {'\'', {scCharacters2Escape[1], std::char_traits<char>::length(scCharacters2Escape[1])}},
        {'&', {scCharacters2Escape[2], std::char_traits<char>::length(scCharacters2Escape[2])}},
        {'<', {scCharacters2Escape[3], std::char_traits<char>::length(scCharacters2Escape[3])}},
        {'>', {scCharacters2Escape[4], std::char_traits<char>::length(scCharacters2Escape[4])}}};

    /*! Appends the character of the numeric character reference (&#NN; or &#xHH;) at the start of paSrc as UTF-8
     *
     * @return the length of the reference, 0 if paSrc does not start with a valid one
     */
    size_t appendNumericCharacterReference(std::string_view paSrc, std::string &paTarget) {
      const size_t end = paSrc.find(';');
      if (!paSrc.starts_with("&#") || end == std::string_view::npos) {
        return 0;
      }
      const bool hex = paSrc.size() > 2 && (paSrc[2] == 'x' || paSrc[2] == 'X');
      const char *const first = paSrc.data() + (hex ? 3 : 2);
      const char *const last = paSrc.data() + end;
      uint32_t codePoint = 0;
      const auto result = std::from_chars(first, last, codePoint, hex ? 16 : 10);
      if (first == last || result.ec != std::errc() || result.ptr != last || codePoint == 0 || codePoint > 0x10FFFF ||
          (codePoint >= 0xD800 && codePoint <= 0xDFFF)) {
        return 0;
      }
      if (codePoint < 0x80) {
        paTarget += static_cast<char>(codePoint);
      } else if (codePoint < 0x800) {
        paTarget += static_cast<char>(0xC0 | (codePoint >> 6));
        paTarget += static_cast<char>(0x80 | (codePoint & 0x3F));
      } else if (codePoint < 0x10000) {
        paTarget += static_cast<char>(0xE0 | (codePoint >> 12));
        paTarget += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        paTarget += static_cast<char>(0x80 | (codePoint & 0x3F));
      } else {
        paTarget += static_cast<char>(0xF0 | (codePoint >> 18));
        paTarget += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
        paTarget += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        paTarget += static_cast<char>(0x80 | (codePoint & 0x3F));
      }
      return end + 1;
    }

    constexpr std::string_view getEscapedXMLSequence(char paChar) noexcept {
      switch (paChar) {
        case '"': return scCharacters2Escape[0];
        case '\'': return scCharacters2Escape[1];
        case '&': return scCharacters2Escape[2];
        case '<': return scCharacters2Escape[3];
        case '>': return scCharacters2Escape[4];
        default: return {};
      }
    }
  } // namespace

  bool isAtoFChar(char paValue) {
    paValue = static_cast<char>(toupper(paValue));
    return ((paValue >= 'A') && (paValue <= 'F'));
  }

  char hexChar(unsigned char charCode) {
    charCode &= 0x0F;
    return charCode < 10 ? '0' + charCode : 'A' - 10 + charCode;
  }

  TForteInt8 charAtoFToInt(char paValue) {
    return static_cast<TForteInt8>((toupper(paValue) - 'A') + 10);
  }

  long int strtol(const char *nptr, char **endptr, int base) {
    long int nRetVal = 0;
    bool bNegativeNumber = false;
    errno = 0;

    // in difference to IEC 611313-3 literals base 16 literals may have a leading -
    if ((*nptr) == '-') {
      bNegativeNumber = true;
      ++nptr;
    }

    if (16 == base && (('0' == (*nptr)) && ('x' == nptr[1]))) {
      // we have a preceding 0x step over it
      nptr += 2;
    }

    long nLimit1 = (bNegativeNumber ? -(std::numeric_limits<CIEC_DINT::TValueType>::min() / base)
                                    : (std::numeric_limits<CIEC_DINT::TValueType>::max() / base));
    long nLimit2 = (bNegativeNumber ? -(std::numeric_limits<CIEC_DINT::TValueType>::min() % base)
                                    : (std::numeric_limits<CIEC_DINT::TValueType>::max() % base));
    if (nLimit2 < 0) // Rounding direction for negative numbers is implementation defined
    {
      nLimit1--;
      nLimit2 = base - nLimit2;
    }

    for (; *nptr; ++nptr) { // Do until '/0'
      if ((!isDigit(*nptr)) && (!((16 == base) && isAtoFChar(*nptr)))) {
        // we have an invalid character
        break;
      }
      TForteInt8 nCharVal = charHexDigitToInt(*nptr);
      if (nRetVal > nLimit1 || (nRetVal == nLimit1 && nCharVal > nLimit2)) {
        nRetVal = (bNegativeNumber) ? std::numeric_limits<CIEC_DINT::TValueType>::min()
                                    : std::numeric_limits<CIEC_DINT::TValueType>::max();
        errno = ERANGE;
        break;
      }
      nRetVal = base * nRetVal + nCharVal;
    }

    if ((bNegativeNumber) && (ERANGE != errno)) {
      nRetVal = -nRetVal;
    }

    if (nullptr != endptr) {
      *endptr = const_cast<char *>(nptr);
    }
    return nRetVal;
  }

  unsigned long int strtoul(const char *nptr, char **endptr, int base) {
    unsigned long int unRetVal = 0;
    unsigned long int unLimit1 = (std::numeric_limits<CIEC_UDINT::TValueType>::max() / base);
    unsigned long int unLimit2 = (std::numeric_limits<CIEC_UDINT::TValueType>::max() % base);
    errno = 0;

    if ((16 == base) && ('0' == (*nptr)) && ('x' == nptr[1])) {
      // we have a preceding 0x step over it
      nptr += 2;
    }

    for (; *nptr; ++nptr) { // Do until '/0'
      if ((!isDigit(*nptr)) && (!((16 == base) && isAtoFChar(*nptr)))) {
        // we have an invalid character
        break;
      }
      TForteInt8 nCharVal = charHexDigitToInt(*nptr);

      if (unRetVal > unLimit1 || (unRetVal == unLimit1 && (unsigned long int) nCharVal > unLimit2)) {
        // in this round we would exceed the limit of the data type
        unRetVal = std::numeric_limits<CIEC_UDINT::TValueType>::max();
        errno = ERANGE;
        break;
      }
      unRetVal = base * unRetVal + nCharVal;
    }
    if (nullptr != endptr) {
      *endptr = const_cast<char *>(nptr);
    }
    return unRetVal;
  }

  long long int strtoll(const char *nptr, char **endptr, int base) {
    long long int nRetVal = 0;
    bool bNegativeNumber = false;
    errno = 0;

    // in difference to IEC 611313-3 literals base 16 literals may have a leading -
    if ((*nptr) == '-') {
      bNegativeNumber = true;
      ++nptr;
    }

    if (16 == base && ('0' == (*nptr)) && ('x' == nptr[1])) {
      // we have a preceding 0x step over it
      nptr += 2;
    }

    long long nLimit1;
    long long nLimit2;

    if (bNegativeNumber) {
      volatile long long nLimMinDiv = std::numeric_limits<CIEC_LINT::TValueType>::min() / base;
      volatile long long nLimMinMod = std::numeric_limits<CIEC_LINT::TValueType>::min() % base;

      nLimit1 = -nLimMinDiv;
      nLimit2 = -nLimMinMod;

      if (nLimit2 < 0) // Rounding direction for negative numbers is implementation defined
      {
        nLimit1--;
        nLimit2 = base - nLimit2;
      }
    } else {
      nLimit1 = std::numeric_limits<CIEC_LINT::TValueType>::max() / base;
      nLimit2 = std::numeric_limits<CIEC_LINT::TValueType>::max() % base;
    }

    for (; *nptr; ++nptr) { // Do until '/0'
      if ((!isDigit(*nptr)) && (!((16 == base) && isAtoFChar(*nptr)))) {
        // we have an invalid character
        break;
      }
      TForteInt8 nCharVal = charHexDigitToInt(*nptr);
      if (nRetVal > nLimit1 || (nRetVal == nLimit1 && nCharVal > nLimit2)) {
        nRetVal = (bNegativeNumber) ? std::numeric_limits<CIEC_LINT::TValueType>::min()
                                    : std::numeric_limits<CIEC_LINT::TValueType>::max();
        errno = ERANGE;
        break;
      }
      nRetVal = base * nRetVal + nCharVal;
    }

    if ((bNegativeNumber) && (ERANGE != errno)) {
      nRetVal = -nRetVal;
    }
    if (nullptr != endptr) {
      *endptr = const_cast<char *>(nptr);
    }
    return nRetVal;
  }

  unsigned long long int strtoull(const char *nptr, char **endptr, int base) {
    unsigned long long int unRetVal = 0;
    unsigned long long int unLimit1 = (std::numeric_limits<CIEC_ULINT::TValueType>::max() / base);
    unsigned long long int unLimit2 = (std::numeric_limits<CIEC_ULINT::TValueType>::max() % base);
    errno = 0;

    if ((16 == base) && ('0' == (*nptr)) && ('x' == nptr[1])) {
      // we have a preceding 0x step over it
      nptr += 2;
    }

    for (; *nptr; ++nptr) { // Do until '/0'
      if ((!isDigit(*nptr)) && (!((16 == base) && isAtoFChar(*nptr)))) {
        // we have an invalid character
        break;
      }
      TForteInt8 nCharVal = charHexDigitToInt(*nptr);
      if (unRetVal > unLimit1 || (unRetVal == unLimit1 && (unsigned long long int) nCharVal > unLimit2)) {
        // in this round we would exceed the limit of the data type
        unRetVal = std::numeric_limits<CIEC_ULINT::TValueType>::max();
        errno = ERANGE;
        break;
      }
      unRetVal = base * unRetVal + nCharVal;
    }
    if (nullptr != endptr) {
      *endptr = const_cast<char *>(nptr);
    }
    return unRetVal;
  }

  void normalizeToStringRepresentation(std::string &paTargetBuf, size_t paStartPos) {
    if (std::isalpha(paTargetBuf[paStartPos]) || ((paTargetBuf[paStartPos] == '-' || paTargetBuf[paStartPos] == '+') &&
                                                  std::isalpha(paTargetBuf[paStartPos + 1]))) {
      // if first or (signed) second char is an alphabetic char we have nan or infinity
      // remove optional parenthesis after "nan" (e.g., "-nan(ind)")
      if (const auto parenPos = paTargetBuf.find("(", paStartPos); parenPos != std::string::npos) {
        paTargetBuf.resize(parenPos);
      }
      return;
    }

    auto dotPos = paTargetBuf.find('.', paStartPos);
    auto expPos = paTargetBuf.find('e', paStartPos);

    if (dotPos == std::string::npos) {
      if (expPos == std::string::npos) {
        paTargetBuf += ".0"s;
      } else {
        paTargetBuf.insert(expPos, ".0"s);
      }
    }
  }

  size_t getExtraSizeForXMLEscapedChars(const char *paString) {
    size_t retVal = 0;
    while (0 != *paString) {
      auto escapeChar = scEscapeMap.find(*paString);
      if (escapeChar != scEscapeMap.end()) {
        retVal += std::get<1>(escapeChar->second) - 1;
      }
      paString++;
    }
    return retVal;
  }

  void transformNonEscapedToEscapedXMLText(std::string &paString, size_t paStart) {
    if (paString.size() == 0) {
      return;
    }

    for (size_t runner = paString.size() - 1; runner >= paStart && runner != static_cast<size_t>(-1); runner--) {
      const std::string_view escapedSeq = getEscapedXMLSequence(paString[runner]);
      if (!escapedSeq.empty()) {
        paString.replace(runner, 1, escapedSeq.data(), escapedSeq.size());
      }
    }
  }

  void transformEscapedXMLToNonEscapedText(std::string_view paSrc, std::string &paTarget) {
    for (size_t i = 0; i < paSrc.size();) {
      if (paSrc[i] == '&') {
        auto rest = paSrc.substr(i);
        bool replaced = false;
        for (const auto &[characters2Escape, escapeSeqTuple] : scEscapeMap) {
          const char *escapSeq = std::get<0>(escapeSeqTuple);
          size_t escapeSequLen = std::get<1>(escapeSeqTuple);
          if (rest.starts_with(std::string_view(escapSeq, escapeSequLen))) {
            paTarget += characters2Escape;
            i += escapeSequLen;
            replaced = true;
            break;
          }
        }

        if (!replaced) {
          if (const size_t length = appendNumericCharacterReference(rest, paTarget); length != 0) {
            i += length;
            replaced = true;
          }
        }

        if (replaced) {
          continue;
        } else {
          DEVLOG_ERROR(
              "[XML Transformer] The given XML text has & character but it's none of the known escaped characters");
        }
      }
      paTarget += paSrc[i];
      i++;
    }
  }

  char *lookForNonEscapedChar(char **paString, char paChar, char paEscapingChar) {
    char *foundChar = nullptr;
    char *initialPosition = *paString;
    while (!foundChar && '\0' != **paString) {
      foundChar = strchr(*paString, paChar);
      if (nullptr != foundChar) {
        *paString = foundChar;
        if (isEscaped(foundChar, initialPosition, paEscapingChar)) {
          while ('\0' != *foundChar) { // move the rest of the string one char back
            *(foundChar - 1) = *foundChar;
            foundChar++;
          }
          *(foundChar - 1) = *foundChar; // move also the ending \0
          foundChar = nullptr; // reset to keep looking
        } else {
          *foundChar = '\0';
          (*paString)++;
        }
      } else { // there's no paChar in the string
        break;
      }
    }
    return foundChar;
  }

  bool isEscaped(char *paChar, char *paBeginLimit, char paEscapingChar) {
    size_t noOfScapingSigns = 0;
    while (paBeginLimit !=
           paChar) { // count the amount of \ signs before paChar to know if the \ signs was also escaped.
      paChar--;
      if (paEscapingChar == *paChar) {
        noOfScapingSigns++;
      } else {
        break;
      }
    }

    return (noOfScapingSigns % 2); // an even number of \ (or zero) means paChar was not escaped
  }

  void removeEscapedSigns(char **paString, char paEscapingChar) {
    char *runner = *paString;
    while ('\0' != *runner) {
      if (paEscapingChar == *runner && paEscapingChar == *(runner + 1)) {
        char *copier = runner + 1;
        while ('\0' != *copier) {
          *(copier - 1) = *copier;
          copier++;
        }
        *(copier - 1) = *copier; // move also the ending \0
      }
      runner++;
    }
  }

  void
  writeToStringNameValuePair(std::string &paTargetBuf, const StringId variableNameId, const CIEC_ANY *const variable) {
    paTargetBuf += variableNameId;
    paTargetBuf += ":="s;
    variable->toString(paTargetBuf);
  }
} // namespace forte::util
