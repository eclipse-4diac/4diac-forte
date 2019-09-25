/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <datatype.h>

namespace forte {
  namespace core {
    namespace util {

      inline bool isDigit(char pa_cValue){
        return ((pa_cValue >= '0') && (pa_cValue <= '9'));
      }

      bool isAtoFChar(char pa_cValue);

      inline bool isHexDigit(char pa_cValue){
        return isDigit(pa_cValue) || isAtoFChar(pa_cValue);
      }

      inline TForteInt8 charDigitToInt(char pa_cValue){
        return static_cast<TForteInt8>(pa_cValue - '0');
      }

      TForteInt8 charAtoFToInt(char pa_cValue);

      inline TForteInt8 charHexDigitToInt(char pa_cValue){
        return isDigit(pa_cValue) ? charDigitToInt(pa_cValue) : charAtoFToInt(pa_cValue);
      }

      long int strtol(const char *nptr, char **endptr, int base);

      unsigned long int strtoul(const char *nptr, char **endptr, int base);

#ifdef FORTE_USE_64BIT_DATATYPES
      long long int strtoll(const char *nptr, char **endptr, int base);

      unsigned long long int strtoull(const char *nptr, char **endptr, int base);
#endif


      const char scXMLEscapedCharacters[] = { '"', '\'', '&', '<', '>'};

      const char* const scReplacementForXMLEscapedCharacters[sizeof(scXMLEscapedCharacters)] = {"&quot;", "&apos;", "&amp;",  "&lt;", "&gt;"};

      /**
       * Gets the extra space needed in a string for escaped chars.
       * @param paString The null ended C-string that's being checked for chars that need to be escaped
       * @return Extra number of chars needed to allocated the chars once they are escaped
       */
      size_t getExtraSizeForXMLEscapedChars(const char* const paString);

      /**
       * Transforms a string with escaped chars (&apos;) into a string with the original chars (').
       * @param paString The null ended C-string to be transformed. It's changed after calling this function
       * @return Extra number of chars which were present in the original string
       */
      size_t transformEscapedXMLToNonEscapedText(char* const paString);

      /**
       * Transforms a string with chars to be scaped (') into a string with the escaped chars (&apos;)
       * @param paString The null ended C-string to be transformed. It's changed after calling this function, and it should be big enough to hold the extra chars
       * @return Extra number of chars now present in the string comparing to the original one
       */
      size_t transformNonEscapedToEscapedXMLText(char* const paString);


      char *lookForNonEscapedChar(char **paString, char paChar, char paEscapingChar);
      bool isEscaped(char *paChar, char *paBeginLimit, char paEscapingChar);
      void removeEscapedSigns(char **paString, char paEscapingChar);

    }
  }
}

#endif
