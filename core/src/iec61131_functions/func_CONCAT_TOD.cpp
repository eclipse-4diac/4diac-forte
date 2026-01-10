/*******************************************************************************
 * Copyright (c) 2011 - 2013, 2018 ACIN, fortiss GmbH, 2019 TU Wien/ACIN
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - adds NOW and NOW_MONOTONIC functions,
 *        changes for removed implicit constructor for primitive types
 *******************************************************************************/

#include "forte/iec61131_functions/func_CONCAT_TOD.h"
#include "forte/util/devlog.h"
#include "forte/datatypes/forte_time_of_day.h"
#include "forte/datatypes/forte_any_int.h"

namespace forte {
  CIEC_TIME_OF_DAY func_CONCAT_TOD(const CIEC_ANY_INT &HOUR,
                                   const CIEC_ANY_INT &MINUTE,
                                   const CIEC_ANY_INT &SECOND,
                                   const CIEC_ANY_INT &MILLISECOND) {
    // Only unsigned representations need to be checked, as negative signed number will be large positive unsigned
    // numbers, outside the allowed range
    if (HOUR.getUnsignedValue() > 23) {
      DEVLOG_ERROR("CONCAT_TOD HOUR outside of valid range");
      return CIEC_TIME_OF_DAY(0);
    }
    if (MINUTE.getUnsignedValue() > 59) {
      DEVLOG_ERROR("CONCAT_TOD MINUTE outside of valid range");
      return CIEC_TIME_OF_DAY(0);
    }
    if (SECOND.getUnsignedValue() > 59) {
      DEVLOG_ERROR("CONCAT_TOD SECOND outside of valid range");
      return CIEC_TIME_OF_DAY(0);
    }
    if (MILLISECOND.getUnsignedValue() > 999) {
      DEVLOG_ERROR("CONCAT_TOD MILLISECOND outside of valid range");
      return CIEC_TIME_OF_DAY(0);
    }
    uint_fast64_t hour = HOUR.getSignedValue() * 60ULL * 60ULL * 1000ULL * 1000000ULL;
    uint_fast64_t minute = MINUTE.getSignedValue() * 60ULL * 1000ULL * 1000000ULL;
    uint_fast64_t second = SECOND.getSignedValue() * 1000ULL * 1000000ULL;
    uint_fast64_t millisecond = MILLISECOND.getSignedValue() * 1000000ULL;
    return CIEC_TIME_OF_DAY(hour + minute + second + millisecond);
  }

} // namespace forte
