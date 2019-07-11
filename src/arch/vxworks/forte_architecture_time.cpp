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

#include <timer.h>

#include "forte_architecture_time.h"
#include "forte_constants.h"

uint_fast64_t getNanoSecondsMonotonic() {
  return (forte::core::constants::cNanosecondsPerSecond > timerFreq()) ?
      sysTimestamp() / (timerFreq() / forte::core::constants::cNanosecondsPerSecond) :
      (sysTimestamp() / timerFreq()) * forte::core::constants::cNanosecondsPerSecond; /// forte::core::constants::cNanosecondsPerSecond) * sysClkRateGet();
}
