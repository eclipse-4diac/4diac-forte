/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
 *               2022 Primetals Technologies Austria GmbH
 *               2023 Martin Erich Jobst
 *               2024 Samator Indo Gas
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
 *  Ketut Kumajaya - Use the standard C++ time functions, fix pre Windows 8/
 *                   Server 2012 compatibility issue
 *                 - Move the standard C++ time functions as a common time
 *                   implementation
 *******************************************************************************/

#include "forte_architecture_time.h"
#include "forte_constants.h"

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
