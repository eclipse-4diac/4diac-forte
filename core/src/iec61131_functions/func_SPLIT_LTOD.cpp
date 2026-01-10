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

#include "forte/iec61131_functions/func_SPLIT_LTOD.h"
#include "forte/datatypes/forte_ltime_of_day.h"
#include "forte/datatypes/forte_any_int.h"
#include "forte/datatypes/forte_any_duration.h"

namespace forte {
  void func_SPLIT_LTOD(const CIEC_LTIME_OF_DAY &paValue,
                       CIEC_ANY_INT &HOUR,
                       CIEC_ANY_INT &MINUTE,
                       CIEC_ANY_INT &SECOND,
                       CIEC_ANY_INT &MILLISECOND) {
    CIEC_LTIME_OF_DAY::TValueType nanoSeconds = static_cast<CIEC_LTIME_OF_DAY::TValueType>(paValue);
    CIEC_LTIME_OF_DAY::TValueType hours = nanoSeconds / (3600ULL * CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond);
    HOUR.setUnsignedValue(hours);

    nanoSeconds = nanoSeconds - hours * (3600ULL * CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond);
    CIEC_LTIME_OF_DAY::TValueType minutes = nanoSeconds / (60ULL * CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond);
    MINUTE.setUnsignedValue(minutes);

    nanoSeconds = nanoSeconds - minutes * (60ULL * CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond);
    CIEC_LTIME_OF_DAY::TValueType seconds = nanoSeconds / CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond;
    SECOND.setUnsignedValue(seconds);

    nanoSeconds = nanoSeconds - seconds * CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond;
    CIEC_LTIME_OF_DAY::TValueType milliSeconds =
        nanoSeconds / (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000ULL);
    MILLISECOND.setUnsignedValue(milliSeconds);
  }

} // namespace forte
