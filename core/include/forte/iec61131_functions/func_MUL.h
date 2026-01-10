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

#include "forte/datatypes/forte_ltime.h"
#include "forte/datatypes/forte_time.h"

#include "func_MUL_TIME.h"
#include "func_MUL_LTIME.h"
#include "forte/datatypes/forte_any_num.h"

namespace forte {
  template<class T, class U>
  class MulOperation {
      using resultType = typename mpl::get_mul_operator_result_type_t<T, U>;

    public:
      static resultType call(const T &paIN1, const U &paIN2) {
        return resultType(static_cast<typename T::TValueType>(paIN1) * static_cast<typename U::TValueType>(paIN2));
      }
      MulOperation() = delete;
  };

  template<class U>
  class MulOperation<CIEC_TIME, U> {
      using resultType = typename mpl::get_mul_operator_result_type_t<CIEC_TIME, U>;

    public:
      static resultType call(const CIEC_TIME &paIN1, const U &paIN2) {
        return func_MUL_TIME(paIN1, paIN2);
      }
      MulOperation() = delete;
  };

  template<class U>
  class MulOperation<CIEC_LTIME, U> {
      using resultType = typename mpl::get_mul_operator_result_type_t<CIEC_LTIME, U>;

    public:
      static resultType call(const CIEC_LTIME &paIN1, const U &paIN2) {
        return func_MUL_LTIME(paIN1, paIN2);
      }
      MulOperation() = delete;
  };

  template<typename T, typename U, typename... Args>
  auto func_MUL(const T &paIN1, const U &paIN2, const Args &...args) {
    static_assert(std::is_base_of_v<CIEC_ANY_NUM, T>, "Variadic MUL only allowed with ANY_NUMs");
    static_assert(std::is_base_of_v<CIEC_ANY_NUM, U>, "Variadic MUL only allowed with ANY_NUMs");
    return func_MUL(func_MUL(paIN1, paIN2), args...);
  }

  template<typename R = mpl::NullType, typename T, typename U>
  auto func_MUL(const T &paIN1, const U &paIN2) -> typename mpl::get_mul_operator_result_type_t<T, U> {
    using deductedType = typename mpl::get_mul_operator_result_type_t<T, U>;
    if constexpr (!std::is_same_v<R, mpl::NullType>) {
      static_assert(std::is_same_v<deductedType, R>, "Deducted type and requested type do not match!\n");
    }
    return MulOperation<T, U>::call(paIN1, paIN2);
  }

} // namespace forte
