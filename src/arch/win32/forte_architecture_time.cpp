/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
 *               2022 Primetals Technologies Austria GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Martin Melik Merkumians - Adds getNanoSecondsMonotonic
 *                          - Optimize order of operations for time result
 *  Martin Jobst - add high-resolution realtime clock
 *******************************************************************************/

#include <windows.h>

#include "forte_architecture_time.h"
#include "forte_constants.h"

#ifndef FORTE_FAKE_TIME
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

uint_fast64_t getNanoSecondsRealtime() {
  FILETIME filetime;

  GetSystemTimePreciseAsFileTime(&filetime);

  // The FILETIME struct stores the number of 100-nanosecond intervals since January 1, 1601 (UTC).
  uint_fast64_t time = static_cast<uint_fast64_t>(filetime.dwHighDateTime) << 32 | static_cast<uint_fast64_t>(filetime.dwLowDateTime);
  return (time - 116444736000000000LL) * 100LL; // convert to Unix Epoch and nanoseconds
}
#endif

time_t forte_timegm(struct tm *pa_tm) {
  return _mkgmtime(pa_tm);
}

struct tm* forte_localtime(const time_t* paTime, struct tm* const paResult) {
  if(0 == localtime_s(paResult, paTime)) {
    return paResult;
  }
  return nullptr;
}

struct tm* forte_gmtime(const time_t* const paTime, struct tm* const paResult){
  if(0 == gmtime_s(paResult, paTime)) {
    return paResult;
  }
  return nullptr;
}
