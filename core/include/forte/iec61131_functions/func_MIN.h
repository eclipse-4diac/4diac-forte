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

#include "forte/datatypes/forte_any_elementary.h"

#include "common.h"

namespace forte {
  template<typename T, typename U, typename... Args>
  auto func_MIN(const T &paIN1, const U &paIN2, const Args &...args) {
    return func_MIN(func_MIN(paIN1, paIN2), args...);
  }

  GENERATE_APPLY_FUNCTION(func_MIN)
  template<typename T, typename U>
  auto func_MIN(const T &paIN1, const U &paIN2) -> typename mpl::get_castable_type_t<T, U> {
    return APPLY<T, U, func_MIN_Function, CIEC_ANY_ELEMENTARY>(paIN1, paIN2);
  }

  template<typename T>
  T func_MIN(const T &paIN1, const T &paIN2) {
    if (static_cast<typename T::TValueType>(paIN1) < static_cast<typename T::TValueType>(paIN2)) {
      return paIN1;
    } else {
      return paIN2;
    }
  }

} // namespace forte
