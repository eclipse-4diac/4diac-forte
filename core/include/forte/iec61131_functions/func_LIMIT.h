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

namespace forte {
  template<typename T, typename U, typename V>
  auto func_LIMIT(const T &paMN, const U &paIN, const V &paMX) ->
      typename mpl::get_castable_type_t<typename mpl::get_castable_type_t<T, U>, V> {
    using tImplicitType = typename mpl::get_castable_type_t<typename mpl::get_castable_type_t<T, U>, V>;
    return func_MIN(func_MAX(static_cast<tImplicitType>(paIN), static_cast<tImplicitType>(paMN)),
                    static_cast<tImplicitType>(paMX));
  }

} // namespace forte
