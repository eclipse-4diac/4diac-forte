/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH, Martin Erich Jobst,
 *                    Primetals Technologies Austria GmbH, Insolsoft
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *   Martin Jobst - add string join function
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Martin Jobst - move normalizeToStringRepresentation from CIEC_ANY_REAL
 *                - add floating-point formatting function
 *   Anton Gusev  - add appendInt
 *******************************************************************************/

#pragma once

#include "forte/datatype.h"
#include "forte/stringid.h"
#include "forte/datatypes/forte_any.h"

#include <concepts>
#include <string>

namespace forte::util {

  inline bool isDigit(char paValue) {
    return ((paValue >= '0') && (paValue <= '9'));
  }

  bool isAtoFChar(char paValue);

  inline bool isHexDigit(char paValue) {
    return isDigit(paValue) || isAtoFChar(paValue);
  }

  inline TForteInt8 charDigitToInt(char paValue) {
    return static_cast<TForteInt8>(paValue - '0');
  }

  char hexChar(unsigned char charCode);

  TForteInt8 charAtoFToInt(char paValue);

  inline TForteInt8 charHexDigitToInt(char paValue) {
    return isDigit(paValue) ? charDigitToInt(paValue) : charAtoFToInt(paValue);
  }

  long int strtol(const char *nptr, char **endptr, int base);

  unsigned long int strtoul(const char *nptr, char **endptr, int base);

  long long int strtoll(const char *nptr, char **endptr, int base);

  unsigned long long int strtoull(const char *nptr, char **endptr, int base);

  /**
   * Gets the extra space needed in a string for escaped chars.
   * @param paString The null ended C-string that's being checked for chars that need to be escaped
   * @return Extra number of chars needed to allocated the chars once they are escaped
   */
  size_t getExtraSizeForXMLEscapedChars(const char *const paString);

  /**
   * Transforms a string with escaped chars (&apos;) into a string with the original chars (').
   * @param paSrc    src string to be unescaped
   * @param paTarget the string where the unescaped string will be put to
   */
  void transformEscapedXMLToNonEscapedText(std::string_view paSrc, std::string &paTarget);

  /**
   * Transforms a string with chars to be scaped (') into a string with the escaped chars (&apos;)
   * @param paString The null ended C-string to be transformed. It's changed after calling this function, and it
   * should be big enough to hold the extra chars
   */
  void transformNonEscapedToEscapedXMLText(std::string &paString, size_t paStart);

  char *lookForNonEscapedChar(char **paString, char paChar, char paEscapingChar);
  bool isEscaped(char *paChar, char *paBeginLimit, char paEscapingChar);
  void removeEscapedSigns(char **paString, char paEscapingChar);

  template<typename I, typename O = std::identity>
  void join(I paFirst, I paLast, O paOp, const char paDelimiter, std::string &paResult) {
    if (paFirst == paLast) {
      return;
    }
    for (; paFirst != paLast; ++paFirst) {
      paResult += paOp(*paFirst);
      paResult += paDelimiter;
    }
    paResult.pop_back(); // remove extra delimiter at end
  }

  void
  writeToStringNameValuePair(std::string &paTargetBuf, const StringId variableNameId, const CIEC_ANY *const variable);

  template<typename T>
    requires std::same_as<T, TForteFloat> || std::same_as<T, TForteDFloat>
  void appendFloat(std::string &paTargetBuf, T paValue);

  /**
   * Append integer to string (zero-padded to width N )
   */
  template<size_t N>
  void appendInt(std::string &paTargetBuf, int paValue) {
    paTargetBuf.resize(paTargetBuf.size() + N);
    auto lastPos = paTargetBuf.size() - 1;
    for (auto i = 0u; i < N; i++) {
      paTargetBuf[lastPos - i] = '0' + (paValue % 10);
      paValue /= 10;
    }
  }

  void normalizeToStringRepresentation(std::string &paTargetBuf, size_t paStartPos);
} // namespace forte::util
