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
#include <AP_Sys.h>

#include "forte_architecture_time.h"

uint_fast64_t getNanoSecondsMonotonic() {
  return rX_SysGetSystemTicks() * rX_SysGetSystemCycletime() *
      (forte::core::constants::cNanosecondsPerSecond / forte::core::constants::cMicrosecondsPerSecond);
}

