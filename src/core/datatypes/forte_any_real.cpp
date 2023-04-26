/*******************************************************************************
 * Copyright (c) 2011 ACIN
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
 *******************************************************************************/
#include "forte_any_real.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_any_real_gen.cpp"
#endif

int CIEC_ANY_REAL::normalizeToStringRepresentation(char *paValue, size_t paBufferSize, int paUsedBytes) const {
  size_t i = 0;
  for (; i < paBufferSize; ++i) {
    if (paValue[i] != '\0') {
      // Found one
      if (paValue[i] == '.' || tolower(paValue[i]) == 'e') {
        return paUsedBytes;
      }
    } else {
      break;
    }
  }
    // Found none, check if it can be added
  if (paUsedBytes + 2 >= static_cast<int>(paBufferSize)) {
    return -1;
  } else {
    // Add it
    paValue[i] = '.';
    paValue[i + 1] = '0';
    paValue[i + 2] = '\0';
    paUsedBytes += 2;
  }
  return paUsedBytes;
}
