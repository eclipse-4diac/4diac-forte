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

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_time_of_day.h"
#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_ltime_of_day.h"

namespace forte {
  //********************************************************************************************
  //   L/TOD_TO_*  functions
  //********************************************************************************************

  inline CIEC_TIME_OF_DAY func_LTOD_TO_TOD(const CIEC_LTIME_OF_DAY &paValue) {
    return CIEC_TIME_OF_DAY(static_cast<CIEC_LTIME_OF_DAY::TValueType>(paValue));
  }

  inline CIEC_LTIME_OF_DAY func_TOD_TO_LTOD(const CIEC_TIME_OF_DAY &paValue) {
    return CIEC_ANY::cast<CIEC_TIME_OF_DAY>(paValue);
  }
} // namespace forte
