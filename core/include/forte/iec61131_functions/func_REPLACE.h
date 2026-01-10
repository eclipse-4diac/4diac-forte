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

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_uint.h"

#include "forte/datatypes/forte_any_string.h"

#include "forte/iec61131_functions/func_CONCAT.h"
#include "forte/iec61131_functions/func_LEFT.h"
#include "forte/iec61131_functions/func_RIGHT.h"

namespace forte {
  template<typename T>
  T func_REPLACE(const T &paIn1, const T &paIn2, const CIEC_ANY_INT &paL, const CIEC_ANY_INT &paP) {
    static_assert(std::is_base_of_v<CIEC_ANY_STRING, T>);
    const CIEC_ANY::TLargestUIntValueType L =
        paL.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(
                             std::max(CIEC_ANY::TLargestIntValueType(0), paL.getSignedValue()))
                       : paL.getUnsignedValue();
    const CIEC_ANY::TLargestUIntValueType P =
        paP.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(
                             std::max(CIEC_ANY::TLargestIntValueType(0), paP.getSignedValue()))
                       : paP.getUnsignedValue();

    if (L == 0) {
      DEVLOG_ERROR("REPLACE called with L equal or less to 0!\n");
      return paIn1;
    }

    if (P == 0) {
      DEVLOG_ERROR("REPLACE called with P equal or less to 0!\n");
      return paIn1;
    }

    if ((P + L - 1) > paIn1.length()) { // STRINGs start at index 1, so we need to subtract 1
      DEVLOG_ERROR("REPLACE outside string boundaries!\n");
      return paIn1;
    }

    if ((P + L + paIn2.length()) > std::numeric_limits<CIEC_UINT::TValueType>::max()) {
      DEVLOG_ERROR("REPLACE exceeds length of string!\n");
    }

    CIEC_INT positionRight = CIEC_INT(static_cast<CIEC_INT::TValueType>(paIn1.length() - (L + P - 1)));
    CIEC_INT positionLeft = CIEC_INT(static_cast<CIEC_INT::TValueType>(P - 1));
    return func_CONCAT(func_CONCAT(func_LEFT(paIn1, positionLeft), paIn2), func_RIGHT(paIn1, positionRight));
  }

} // namespace forte
