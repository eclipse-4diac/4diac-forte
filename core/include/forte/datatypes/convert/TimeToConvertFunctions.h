/*******************************************************************************
 * Copyright (c) 2010-2019 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Monika Wenger, Carolyn Oates, Patrick Smejkal,
 *    Matthias Plasch,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - fixes DT_TO_TOD
 *    Martin Melik-Merkumians - removes invalid casts, update implementation
 *     to use new cast function
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_ltime.h"

#include "convert_function_utils.h"

namespace forte {
  //********************************************************************************************
  //   L/TIME_TO_*  functions
  //********************************************************************************************

  inline CIEC_TIME func_LTIME_TO_TIME(const CIEC_LTIME &paValue) {
    return CIEC_TIME(static_cast<CIEC_LTIME::TValueType>(paValue));
  }

  inline CIEC_LTIME func_TIME_TO_LTIME(const CIEC_TIME &paValue) {
    return CIEC_ANY::cast<CIEC_TIME>(paValue);
  }

  /** Non-standard functions */
  inline CIEC_STRING func_TIME_AS_STRING(const CIEC_TIME &paVal) {
    CIEC_STRING string;
    stringConverter(string, paVal);
    return string;
  }

  inline CIEC_WSTRING func_TIME_AS_WSTRING(const CIEC_TIME &paVal) {
    CIEC_WSTRING string;
    stringConverter(string, paVal);
    return string;
  }

  inline CIEC_STRING func_LTIME_AS_STRING(const CIEC_LTIME &paVal) {
    CIEC_STRING string;
    stringConverter(string, paVal);
    return string;
  }

  inline CIEC_WSTRING func_LTIME_AS_WSTRING(const CIEC_LTIME &paVal) {
    CIEC_WSTRING string;
    stringConverter(string, paVal);
    return string;
  }
} // namespace forte
