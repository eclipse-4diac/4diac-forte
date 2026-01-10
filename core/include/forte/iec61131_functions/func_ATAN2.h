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

#include <cmath>

#include "forte/datatypes/forte_any_real.h"

namespace forte {
  template<typename T, typename U>
  inline auto func_ATAN2(const T &paY, const U &paX) -> typename mpl::get_castable_type_t<T, U> {
    static_assert(std::is_base_of_v<CIEC_ANY_REAL, T>, "T not of ANY_REAL");
    static_assert(std::is_base_of_v<CIEC_ANY_REAL, U>, "U not of ANY_REAL");
    using ReturnType = typename mpl::get_castable_type_t<T, U>;
    using XValueType = typename U::TValueType;
    using YValueType = typename T::TValueType;
    using ReturnValueType = typename ReturnType::TValueType;
    const ReturnValueType xValue = static_cast<ReturnValueType>(static_cast<XValueType>(paX));
    const ReturnValueType yValue = static_cast<ReturnValueType>(static_cast<YValueType>(paY));
    return ReturnType(static_cast<ReturnValueType>(std::atan2(yValue, xValue)));
  }

} // namespace forte
