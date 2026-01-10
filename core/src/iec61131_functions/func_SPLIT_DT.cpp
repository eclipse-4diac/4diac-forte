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

#include "forte/iec61131_functions/func_SPLIT_DT.h"
#include "forte/iec61131_functions/func_SPLIT_LDT.h"
#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_any_int.h"

namespace forte {
  void func_SPLIT_DT(const CIEC_DATE_AND_TIME &paValue,
                     CIEC_ANY_INT &YEAR,
                     CIEC_ANY_INT &MONTH,
                     CIEC_ANY_INT &DAY,
                     CIEC_ANY_INT &HOUR,
                     CIEC_ANY_INT &MINUTE,
                     CIEC_ANY_INT &SECOND,
                     CIEC_ANY_INT &MILLISECOND) {
    func_SPLIT_LDT(paValue, YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, MILLISECOND);
  }

} // namespace forte
