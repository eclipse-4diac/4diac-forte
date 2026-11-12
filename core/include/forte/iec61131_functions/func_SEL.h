/*******************************************************************************
 * Copyright (c) 2010 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH
 *                         Primetals Technologies Austria GmbH
 *                         Martin Erich Jobst, Martin Melik Merkumians
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
 *    Martin Melik Merkumians
 *      - use shared common IEC type resolution and convert the selected operand
 *******************************************************************************/

#pragma once

#include "common_type.h"
#include "forte/datatypes/forte_bool.h"

namespace forte {
  /**
   * @brief Select one of two values and convert it to their common IEC type.
   * @tparam T IEC type of IN0.
   * @tparam U IEC type of IN1.
   * @param G IEC BOOL selector: false selects IN0, true selects IN1.
   * @param IN0 Value selected when G is false.
   * @param IN1 Value selected when G is true.
   * @return Selected operand converted to mpl::common_type_t<T, U>.
   *
   * Both input types must implicitly cast to the common result type; incompatible
   * types are rejected during instantiation. Only the selected operand is
   * converted, although both operands are evaluated before the C++ call.
   * @see mpl::common_type_t
   * @see func_MUX
   */
  template<typename T, typename U>
  auto func_SEL(const CIEC_BOOL &G, const T &IN0, const U &IN1) -> mpl::common_type_t<T, U> {
    using ResultType = mpl::common_type_t<T, U>;
    static_assert(!std::is_same_v<ResultType, mpl::NullType>, "SEL inputs must have a common IEC type");
    return (G.operator bool()) ? static_cast<ResultType>(IN1) : static_cast<ResultType>(IN0);
  }

} // namespace forte
