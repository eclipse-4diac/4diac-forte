/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Hesam Rezaee
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_any_duration.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_any_duration_gen.cpp"
#endif
#include "../../arch/timerha.h"
#include <stdio.h>
#include <cinttypes>

// change time elements to string

int CIEC_ANY_DURATION::timeElementsToString(int64_t paTimeElement, char *paValue, int paSize, size_t paBufferSize, const char *paUnit) const {

  int size = 0;

  if(paSize < 0) {
      return -1;
  }

  if(static_cast<size_t>(paSize) > paBufferSize) {
      return -1;
  }

  if(0 != paTimeElement) {
    size = snprintf(paValue + paSize, paBufferSize - paSize, "%" PRId64 "%s", paTimeElement, paUnit);
    if((-1 == size) || (size >= static_cast<int>(paBufferSize) - paSize)) {
      return -1;
    }
  }

  return size;
}