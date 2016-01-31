/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

    }
  }
}

#endif
