/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Martin Melik Merkumians - Adds getNanoSecondsMonotonic
 *******************************************************************************/

#include <time.h>

#include "forte_architecture_time.h"
#include "forte_constants.h"

#ifndef FORTE_FAKE_TIME
uint_fast64_t getNanoSecondsMonotonic() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return static_cast<uint_fast64_t>(now.tv_nsec) + static_cast<uint_fast64_t>(now.tv_sec) *
   static_cast<uint_fast64_t>(forte::core::constants::cNanosecondsPerSecond);
}
#endif
