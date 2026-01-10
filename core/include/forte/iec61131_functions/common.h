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

#include "forte/iec61131_cast_helper.h"

#define GENERATE_APPLY_FUNCTION(func)                                                                                  \
  template<typename T>                                                                                                 \
  struct func##_Function {                                                                                             \
      static T call(const T &paIN1, const T &paIN2) {                                                                  \
        return func(paIN1, paIN2);                                                                                     \
      };                                                                                                               \
  };

namespace forte {
  template<typename T, typename U, template<typename A> class F, typename C>
  auto APPLY(const T &paIN1, const U &paIN2) -> typename mpl::get_castable_type_t<T, U> {
    static_assert(forte::mpl::are_of_subtype_v<C, T, U>, "Template instantiation with incompatible types");
    using tImplicitCastType = typename mpl::get_castable_type_t<T, U>;
    static_assert(!(std::is_same_v<tImplicitCastType, mpl::NullType>), "No implicit cast possible");
    const tImplicitCastType Result(
        F<tImplicitCastType>::call(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2)));
    return Result;
  }

} // namespace forte
