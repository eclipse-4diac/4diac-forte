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

#include <stdint.h>

#include "forte_architecture_time.h"

#include "forte_constants.h"

uint_fast64_t getNanoSecondsMonotonic() {
  return forte::core::constants::cNanosecondsPerSecond < BSP_TICKS_PER_SECOND ?
      tx_time_get() / (BSP_TICKS_PER_SECOND / forte::core::constants::cNanosecondsPerSecond) :
      (tx_time_get() / BSP_TICKS_PER_SECOND) * forte::core::constants::cNanosecondsPerSecond;
}


