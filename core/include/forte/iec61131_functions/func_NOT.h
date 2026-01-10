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

#include "forte/datatypes/forte_bool.h"

#include "forte/datatypes/forte_any_bit.h"

namespace forte {
  template<typename T>
  auto func_NOT(const T &paIN) -> typename mpl::get_equivalent_CIEC_class_t<T> {
    static_assert((std::is_base_of_v<CIEC_ANY_BIT, T>), "NOT function only for ANY_BIT");
    using tResultType = typename mpl::get_equivalent_CIEC_class_t<T>;
    if constexpr (std::is_same_v<tResultType, CIEC_BOOL>) {
      return tResultType(!paIN);
    } else {
      return tResultType(~paIN);
    }
  }

} // namespace forte
