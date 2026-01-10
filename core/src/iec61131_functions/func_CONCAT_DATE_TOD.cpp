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

#include "forte/iec61131_functions/func_CONCAT_DATE_TOD.h"
#include "forte/datatypes/forte_time_of_day.h"
#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_date.h"

namespace forte {
  CIEC_DATE_AND_TIME func_CONCAT_DATE_TOD(const CIEC_DATE &paIN1, const CIEC_TIME_OF_DAY &paIN2) {
    CIEC_DATE_AND_TIME retVal;

    struct tm pstTime;
    paIN1.getTimeStruct(&pstTime);

    time_t t = static_cast<time_t>(static_cast<TForteUInt64>(paIN2) / 1000ULL);
    pstTime.tm_hour = static_cast<int>(t / 3600);
    pstTime.tm_min = static_cast<int>((t % 3600) / 60);
    pstTime.tm_sec = static_cast<int>(t % 60);

    retVal.setDateAndTime(pstTime, paIN2.getMilliSeconds());
    return retVal;
  }

} // namespace forte
