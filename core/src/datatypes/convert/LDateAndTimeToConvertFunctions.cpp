/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_ldate.h"
#include "forte/datatypes/forte_ldate_and_time.h"
#include "forte/datatypes/forte_ltime_of_day.h"

#include "forte/datatypes/convert/LDateAndTimeToConvertFunctions.h"

#include "forte/arch/forte_architecture_time.h"

namespace forte {
  CIEC_LDATE func_LDT_TO_LDATE(const CIEC_LDATE_AND_TIME &paVal) {
    TForteUInt64 nBuffer = paVal;
    time_t t = static_cast<time_t>(nBuffer / 1000000000ULL);
    struct tm ptm;

    if (nullptr == arch::forte_gmtime(&t, &ptm)) {
      return CIEC_LDATE(0);
    }

    ptm.tm_hour = 0;
    ptm.tm_min = 0;
    ptm.tm_sec = 0;

    t = arch::forte_timegm(&ptm);
    if (static_cast<time_t>(-1) == t) {
      return CIEC_LDATE(0);
    }

    return CIEC_LDATE(t * 1000000000ULL);
  }

  CIEC_LTIME_OF_DAY func_LDT_TO_LTOD(const CIEC_LDATE_AND_TIME &paValue) {
    TForteUInt64 nBuffer = paValue;
    time_t t = static_cast<time_t>(nBuffer / 1000000000ULL);
    struct tm ptm;
    if (nullptr == arch::forte_gmtime(&t, &ptm)) {
      return CIEC_LTIME_OF_DAY(0);
    }
    return CIEC_LTIME_OF_DAY(static_cast<TForteUInt64>(
        (ptm.tm_hour * UINT64_C(3600) + ptm.tm_min * UINT64_C(60) + ptm.tm_sec) * UINT64_C(1000000000) +
        (nBuffer % UINT64_C(1000000000))));
  }
} // namespace forte
