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

#include "forte/datatypes/forte_bool.h"

#include "forte/datatypes/forte_any_bit.h"

namespace forte {
  template<typename T, typename U, typename... Args>
  CIEC_BOOL func_LE(const T &paIn1, const U &paIn2, const Args &...args) {
    return CIEC_BOOL(func_LE(paIn1, paIn2) && func_LE(paIn2, args...));
  }

  template<typename T, typename U>
  CIEC_BOOL func_LE(const T &paIN1, const U &paIN2) {
    static_assert(forte::mpl::are_of_subtype_v<CIEC_ANY_ELEMENTARY, T, U>,
                  "Template instantiation with incompatible types");
    if constexpr (forte::mpl::are_of_subtype_v<CIEC_ANY_BIT, T, U>) { // ANY_BITs can be also partial accesses
      using tTClass = typename mpl::get_equivalent_CIEC_class_t<T>;
      using tUClass = typename mpl::get_equivalent_CIEC_class_t<U>;
      using tImplicitCastType = typename mpl::get_castable_type_t<tTClass, tUClass>;
      static_assert(!(std::is_same_v<tImplicitCastType, mpl::NullType>), "No implicit cast possible");
      return func_LE<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
    } else {
      using tImplicitCastType = typename mpl::get_castable_type_t<T, U>;
      static_assert(!(std::is_same_v<tImplicitCastType, mpl::NullType>), "No implicit cast possible");
      return func_LE<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
    }
  }

  template<typename T>
  CIEC_BOOL func_LE(const T &paIN1, const T &paIN2) {
    if constexpr (forte::mpl::has_less_equal_v<T>) {
      return CIEC_BOOL(paIN1 <= paIN2);
    } else {
      return CIEC_BOOL(static_cast<typename T::TValueType>(paIN1) <= static_cast<typename T::TValueType>(paIN2));
    }
  }

} // namespace forte
