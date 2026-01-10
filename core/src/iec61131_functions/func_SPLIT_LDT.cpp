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

#include "forte/iec61131_functions/func_SPLIT_LDT.h"
#include "forte/datatypes/forte_ldate_and_time.h"
#include "forte/datatypes/forte_any_int.h"

namespace forte {
  void func_SPLIT_LDT(const CIEC_LDATE_AND_TIME &paValue,
                      CIEC_ANY_INT &YEAR,
                      CIEC_ANY_INT &MONTH,
                      CIEC_ANY_INT &DAY,
                      CIEC_ANY_INT &HOUR,
                      CIEC_ANY_INT &MINUTE,
                      CIEC_ANY_INT &SECOND,
                      CIEC_ANY_INT &MILLISECOND) {
    struct tm timeStruct;
    paValue.getTimeStruct(&timeStruct);

    YEAR.setUnsignedValue(timeStruct.tm_year + 1900);
    MONTH.setUnsignedValue(timeStruct.tm_mon + 1);
    DAY.setUnsignedValue(timeStruct.tm_mday);
    HOUR.setUnsignedValue(timeStruct.tm_hour);
    MINUTE.setUnsignedValue(timeStruct.tm_min);
    SECOND.setUnsignedValue(timeStruct.tm_sec);
    MILLISECOND.setUnsignedValue(paValue.getMilliSeconds());
  }

} // namespace forte
