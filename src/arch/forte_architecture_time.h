/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH, 2019 TU Wien/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *  Martin Melik-Merkumians - adds function for getting monotonic time in nanoseconds
 *******************************************************************************/

#ifndef SRC_ARCH_FORTE_ARCHITECTURE_TIME_H_
#define SRC_ARCH_FORTE_ARCHITECTURE_TIME_H_

#include <time.h>

#include "forte_constants.h"

#if defined(WINCE)
#include <wce_time.h>

inline
struct tm* forte_localtime(const time_t* pa_time){
  return wceex_localtime(pa_time);
}

inline
time_t forte_mktime(struct tm* pa_tm){
  return wceex_mktime(pa_tm);
}

inline
struct tm* forte_gmtime(const time_t* pa_time){
  return wceex_gmtime(pa_time);
}

inline
time_t forte_time(time_t* pa_time){
  return wceex_time(pa_time);
}

inline uint_fast64_t getNanoSecondsMonotonic() {
  LARGE_INTEGER performance_counter;
  LARGE_INTEGER performance_frequency;

  QueryPerformanceCounter(&performance_counter);
  QueryPerformanceFrequency(&performance_frequency);

  return static_cast<uint_fast64_t>(performance_counter.QuadPart * forte::core::constants::cNanosecondsPerSecond
                         / performance_frequency.QuadPart);
}
#else

inline
struct tm* forte_localtime(const time_t* pa_time){
  return localtime(pa_time);
}

inline
time_t forte_mktime(struct tm* pa_tm){
  return mktime(pa_tm);
}

inline
struct tm* forte_gmtime(const time_t* pa_time){
  return gmtime(pa_time);
}

inline
time_t forte_time(time_t* pa_time){
  return time(pa_time);
}

inline uint_fast64_t getNanoSecondsMonotonic() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_nsec + now.tv_sec * forte::core::constants::cNanosecondsPerSecond;
}

#endif

#endif /* SRC_ARCH_FORTE_ARCHITECTURE_TIME_H_ */
