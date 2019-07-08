/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

