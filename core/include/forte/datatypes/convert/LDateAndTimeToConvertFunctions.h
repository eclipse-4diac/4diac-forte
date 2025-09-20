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

#pragma once

#include "forte/datatypes/forte_time_of_day.h"
#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_date.h"
#include "forte/datatypes/forte_ltime_of_day.h"
#include "forte/datatypes/forte_ldate_and_time.h"
#include "forte/datatypes/forte_ldate.h"

namespace forte {
  //********************************************************************************************
  //   LDT_TO_**  functions
  //********************************************************************************************

  inline CIEC_DATE_AND_TIME func_LDT_TO_DT(const CIEC_LDATE_AND_TIME &paValue) {
    return CIEC_DATE_AND_TIME(static_cast<CIEC_LDATE_AND_TIME::TValueType>(paValue));
  }

  CIEC_LDATE func_LDT_TO_LDATE(const CIEC_LDATE_AND_TIME &paVal);

  inline CIEC_DATE func_LDT_TO_DATE(const CIEC_LDATE_AND_TIME &paValue) {
    return CIEC_DATE(static_cast<CIEC_LDATE::TValueType>(func_LDT_TO_LDATE(paValue)));
  }

  CIEC_LTIME_OF_DAY func_LDT_TO_LTOD(const CIEC_LDATE_AND_TIME &paValue);

  inline CIEC_TIME_OF_DAY func_LDT_TO_TOD(const CIEC_LDATE_AND_TIME &paValue) {
    return CIEC_TIME_OF_DAY(static_cast<CIEC_LTIME_OF_DAY::TValueType>(func_LDT_TO_LTOD(paValue)));
  }
} // namespace forte
