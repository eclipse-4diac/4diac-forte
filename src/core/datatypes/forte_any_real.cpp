/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN, Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *     Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *     Martin Jobst
 *      - add support for infinity and nan values
 *******************************************************************************/
#include "forte_any_real.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_any_real_gen.cpp"
#endif

#include <cctype>

int CIEC_ANY_REAL::normalizeToStringRepresentation(char *paValue, size_t paBufferSize, int paUsedBytes) const {
  // look for decimal point or alphabetic character (indicating exponent, infinity, or nan)
  for (int i = 0; i < paUsedBytes; ++i) {
    if (paValue[i] == '.' || std::isalpha(paValue[i])) {
      return paUsedBytes;
    }
  }
  // found none, check if a decimal point can be added
  if (paUsedBytes + 2 >= static_cast<int>(paBufferSize)) {
    return -1;
  }
  // add it
  paValue[paUsedBytes++] = '.';
  paValue[paUsedBytes++] = '0';
  paValue[paUsedBytes] = '\0';
  return paUsedBytes;
}
