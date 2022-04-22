/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Martin Melik Merkumians - Adds getNanoSecondsMonotonic
 *                          - Optimize order of operations for time result
 *******************************************************************************/

#include <windows.h>

#include "forte_architecture_time.h"
#include "forte_constants.h"

uint_fast64_t getNanoSecondsMonotonic() {
  LARGE_INTEGER performance_counter;
  LARGE_INTEGER performance_frequency;

  QueryPerformanceCounter(&performance_counter);
  QueryPerformanceFrequency(&performance_frequency);

  // The clock frequency for Win10 on the Performance Timer is 10Mhz or less
  // in previous versions accoording to
  // https://stackoverflow.com/questions/63205226/better-than-100ns-resolution-timers-in-windows
  // so it is safe to assume that forte::core::constants::cNanosecondsPerSecond >> performance_frequency.QuadPart
  const uint_fast64_t timeFactor = static_cast<uint_fast64_t>(forte::core::constants::cNanosecondsPerSecond) / static_cast<uint_fast64_t>(performance_frequency.QuadPart);
  return static_cast<uint_fast64_t>(performance_counter.QuadPart) * timeFactor;
}


