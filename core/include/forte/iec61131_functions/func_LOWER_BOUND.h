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

#include "forte/datatypes/forte_any_int.h"

#include "forte/datatypes/forte_array_common.h"

namespace forte {
  template<typename T, typename U>
  T func_LOWER_BOUND(const CIEC_ARRAY &paArray, const U &paDimension) {
    static_assert(std::is_base_of_v<CIEC_ANY_INT, T>, "T not of ANY_INT");
    static_assert(std::is_base_of_v<CIEC_ANY_INT, U>, "U not of ANY_INT");
    return T(
        static_cast<typename T::TValueType>(paArray.getLowerBound(static_cast<typename U::TValueType>(paDimension))));
  }

} // namespace forte
