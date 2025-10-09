/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - Adds getNanoSecondsMonotonic
 *  Martin Jobst - add high-resolution realtime clock fallback
 *******************************************************************************/

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "forte/arch/forte_architecture_time.h"
#include "forte/util/forte_constants.h"

uint_fast64_t getNanoSecondsMonotonicArch() {
  return (static_cast<uint_fast64_t>(xTaskGetTickCount()) *
          (forte::util::constants::cNanosecondsPerSecond / static_cast<uint_fast64_t>(configTICK_RATE_HZ)));
}

uint_fast64_t getNanoSecondsRealtimeArch() {
  return static_cast<uint_fast64_t>(forte::arch::forte_time()) * 1000000000LL;
}

time_t forte::arch::forte_timegm(struct tm *pa_tm) {
  time_t tm = mktime(pa_tm);
  return mktime(gmtime(&tm));
}

struct tm *forte::arch::forte_localtime(const time_t *paTime, struct tm *const paResult) {
  return localtime_r(paTime, paResult);
}

struct tm *forte::arch::forte_gmtime(const time_t *const paTime, struct tm *const paResult) {
  return gmtime_r(paTime, paResult);
}
