/*******************************************************************************
 * Copyright (c) 2010,2025 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH
 *                         Primetals Technologies Austria GmbH
 *                         Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Monika Wenger, Ingo Hegny, Patrick Smejkal, Gerhard Ebenhofer,
 *    Matthias Plasch, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians
 *      - added different type templates for IEC 61131-3 functions and adds several type guards
 *      - Reworked binary operator templates to create correctly calculated and typed results
 *      - reworked and fixes ADD and SUB for time types
 *      - Added variadic comparison, MIN, MAX, ADD, MUL functions
 *      - Added unary plus function
 *    Martin Erich Jobst
 *      - added lower and upper bound functions
 *******************************************************************************/

#pragma once

#include "forte/util/devlog.h"

#include "forte/datatypes/forte_any_int.h"

#include "forte/datatypes/forte_int.h"

#include "forte/datatypes/forte_any_string.h"

#include "forte/iec61131_functions/func_LEFT.h"
#include "forte/iec61131_functions/func_RIGHT.h"

namespace forte {
  template<typename T>
  T func_MID(const T &paIn, const CIEC_ANY_INT &paL, const CIEC_ANY_INT &paP) {
    static_assert(std::is_base_of_v<CIEC_ANY_STRING, T>, "T not of ANY_STRING");
    if (true == paP.isSigned() && 0 > paP.getSignedValue()) {
      DEVLOG_ERROR("value of input P is less than zero\n");
      return paIn;
    } else {
      CIEC_INT lenRight = CIEC_INT(static_cast<CIEC_INT::TValueType>(paIn.length() - paP.getUnsignedValue() + 1));
      CIEC_INT lenLeft = CIEC_INT(static_cast<CIEC_INT::TValueType>(paL.getUnsignedValue()));
      return func_LEFT(func_RIGHT(paIn, lenRight), lenLeft);
    }
  }

} // namespace forte
