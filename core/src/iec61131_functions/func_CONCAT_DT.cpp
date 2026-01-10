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

#include "forte/iec61131_functions/func_CONCAT_DT.h"
#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_any_int.h"

namespace forte {
  CIEC_DATE_AND_TIME func_CONCAT_DT(const CIEC_ANY_INT &YEAR,
                                    const CIEC_ANY_INT &MONTH,
                                    const CIEC_ANY_INT &DAY,
                                    const CIEC_ANY_INT &HOUR,
                                    const CIEC_ANY_INT &MINUTE,
                                    const CIEC_ANY_INT &SECOND,
                                    const CIEC_ANY_INT &MILLISECOND) {
    struct tm concatTime{};
    if (YEAR.isSigned()) {
      concatTime.tm_year = static_cast<int>(YEAR.getSignedValue() - 1900);
    } else {
      concatTime.tm_year = static_cast<int>(YEAR.getUnsignedValue() - 1900);
    }

    if (MONTH.isSigned()) {
      concatTime.tm_mon = static_cast<int>(MONTH.getSignedValue() - 1);
    } else {
      concatTime.tm_mon = static_cast<int>(MONTH.getUnsignedValue() - 1);
    }

    if (DAY.isSigned()) {
      concatTime.tm_mday = static_cast<int>(DAY.getSignedValue());
    } else {
      concatTime.tm_mday = static_cast<int>(DAY.getUnsignedValue());
    }

    if (HOUR.isSigned()) {
      concatTime.tm_hour = static_cast<int>(HOUR.getSignedValue());
    } else {
      concatTime.tm_hour = static_cast<int>(HOUR.getUnsignedValue());
    }

    if (MINUTE.isSigned()) {
      concatTime.tm_min = static_cast<int>(MINUTE.getSignedValue());
    } else {
      concatTime.tm_min = static_cast<int>(MINUTE.getUnsignedValue());
    }

    if (SECOND.isSigned()) {
      concatTime.tm_sec = static_cast<int>(SECOND.getSignedValue());
    } else {
      concatTime.tm_sec = static_cast<int>(SECOND.getUnsignedValue());
    }

    unsigned int millisecond = 0;
    if (MILLISECOND.isSigned()) {
      millisecond = static_cast<unsigned int>(MILLISECOND.getSignedValue());
    } else {
      millisecond = static_cast<unsigned int>(MILLISECOND.getUnsignedValue());
    }
    CIEC_DATE_AND_TIME retVal;
    retVal.setDateAndTime(concatTime, millisecond);
    return retVal;
  }

} // namespace forte
