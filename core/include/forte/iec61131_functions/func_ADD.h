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

#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_ldate_and_time.h"
#include "forte/datatypes/forte_ltime.h"
#include "forte/datatypes/forte_ltime_of_day.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_time_of_day.h"

#include "func_ADD_TOD_TIME.h"
#include "func_ADD_DT_TIME.h"
#include "func_ADD_LTOD_LTIME.h"
#include "func_ADD_LDT_LTIME.h"
#include "forte/datatypes/forte_any_num.h"

namespace forte {
  template<class T, class U>
  class AddOperation {
      using resultType = typename mpl::get_add_operator_result_type_t<T, U>;

    public:
      static resultType call(const T &paIN1, const U &paIN2) {
        return resultType(static_cast<typename T::TValueType>(paIN1) + static_cast<typename U::TValueType>(paIN2));
      }
      AddOperation() = delete;
  };

  template<>
  class AddOperation<CIEC_TIME_OF_DAY, CIEC_TIME> {
      using resultType = typename mpl::get_add_operator_result_type_t<CIEC_TIME_OF_DAY, CIEC_TIME>;

    public:
      static resultType call(const CIEC_TIME_OF_DAY &paIN1, const CIEC_TIME &paIN2) {
        return func_ADD_TOD_TIME(paIN1, paIN2);
      }
      AddOperation() = delete;
  };

  template<>
  class AddOperation<CIEC_DATE_AND_TIME, CIEC_TIME> {
      using resultType = typename mpl::get_add_operator_result_type_t<CIEC_DATE_AND_TIME, CIEC_TIME>;

    public:
      static resultType call(const CIEC_DATE_AND_TIME &paIN1, const CIEC_TIME &paIN2) {
        return func_ADD_DT_TIME(paIN1, paIN2);
      }
      AddOperation() = delete;
  };

  template<>
  class AddOperation<CIEC_LTIME_OF_DAY, CIEC_LTIME> {
      using resultType = typename mpl::get_add_operator_result_type_t<CIEC_LTIME_OF_DAY, CIEC_LTIME>;

    public:
      static resultType call(const CIEC_LTIME_OF_DAY &paIN1, const CIEC_LTIME &paIN2) {
        return func_ADD_LTOD_LTIME(paIN1, paIN2);
      }
      AddOperation() = delete;
  };

  template<>
  class AddOperation<CIEC_LDATE_AND_TIME, CIEC_LTIME> {
      using resultType = typename mpl::get_add_operator_result_type_t<CIEC_LDATE_AND_TIME, CIEC_LTIME>;

    public:
      static resultType call(const CIEC_LDATE_AND_TIME &paIN1, const CIEC_LTIME &paIN2) {
        return func_ADD_LDT_LTIME(paIN1, paIN2);
      }
      AddOperation() = delete;
  };

  template<typename T, typename U, typename... Args>
  auto func_ADD(const T &paIN1, const U &paIN2, const Args &...args) {
    static_assert(std::is_base_of_v<CIEC_ANY_NUM, T>, "Variadic ADD only allowed with ANY_NUMs");
    static_assert(std::is_base_of_v<CIEC_ANY_NUM, U>, "Variadic ADD only allowed with ANY_NUMs");
    return func_ADD(func_ADD(paIN1, paIN2), args...);
  }

  template<typename R = mpl::NullType, typename T, typename U>
  auto func_ADD(const T &paIN1, const U &paIN2) -> typename mpl::get_add_operator_result_type_t<T, U> {
    using deductedType = typename mpl::get_add_operator_result_type_t<T, U>;
    if constexpr (!std::is_same_v<R, mpl::NullType>) {
      static_assert(std::is_same_v<deductedType, R>, "Deducted type and requested type do not match!\n");
    }
    return AddOperation<T, U>::call(paIN1, paIN2);
  }

} // namespace forte
