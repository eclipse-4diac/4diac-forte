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

#include "common.h"

namespace forte {
  GENERATE_APPLY_FUNCTION(func_MOD)
  template<typename T, typename U>
  auto func_MOD(const T &paIN1, const U &paIN2) -> typename mpl::get_castable_type_t<T, U> {
    return APPLY<T, U, func_MOD_Function, CIEC_ANY_INT>(paIN1, paIN2);
  }

  template<typename T>
  T func_MOD(const T &paIN1, const T &paIN2) {
    using ValueType = typename T::TValueType;
    if (0 == static_cast<typename T::TValueType>(paIN2)) {
      return T(0);
    }
    return T(static_cast<ValueType>(paIN1.getSignedValue()) % static_cast<ValueType>(paIN2.getSignedValue()));
  }

} // namespace forte
