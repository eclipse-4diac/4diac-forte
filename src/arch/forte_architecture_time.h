/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH, 2019 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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
time_t forte_time(){
  return wceex_time(0);
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
time_t forte_time(){
  return time(0);
}

#endif

uint_fast64_t getNanoSecondsMonotonic();

#endif /* SRC_ARCH_FORTE_ARCHITECTURE_TIME_H_ */
