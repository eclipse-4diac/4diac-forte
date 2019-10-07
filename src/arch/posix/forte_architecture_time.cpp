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

uint_fast64_t getNanoSecondsMonotonic() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_nsec + now.tv_sec * forte::core::constants::cNanosecondsPerSecond;
}


