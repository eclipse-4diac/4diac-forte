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
  template<class T, class U>
  class OrOperation {
      using resultType = typename mpl::get_castable_type_t<T, U>;

    public:
      static resultType call(const T &paIN1, const U &paIN2) {
        return resultType(static_cast<resultType>(paIN1) | static_cast<resultType>(paIN2));
      }

    private:
      OrOperation() = default;
  };

  template<class T>
  class OrOperation<T, CIEC_BOOL> {
      using resultType = typename mpl::get_castable_type_t<T, CIEC_BOOL>;

    public:
      static resultType call(const T &paIN1, const CIEC_BOOL &paIN2) {
        return resultType(paIN1 | (paIN2 ? 1 : 0));
      }

    private:
      OrOperation() = default;
  };

  template<class T>
  class OrOperation<CIEC_BOOL, T> {
      using resultType = typename mpl::get_castable_type_t<T, CIEC_BOOL>;

    public:
      static resultType call(const CIEC_BOOL &paIN1, const T &paIN2) {
        return resultType((paIN1 ? 1 : 0) | paIN2);
      }

    private:
      OrOperation() = default;
  };

  template<>
  class OrOperation<CIEC_BOOL, CIEC_BOOL> {
    public:
      static CIEC_BOOL call(const CIEC_BOOL &paIN1, const CIEC_BOOL &paIN2) {
        return CIEC_BOOL(paIN1 || paIN2);
      }

    private:
      OrOperation() = default;
  };

  template<typename T, typename U>
  auto func_OR(const T &paIN1, const U &paIN2) ->
      typename mpl::get_castable_type_t<typename mpl::get_equivalent_CIEC_class_t<T>,
                                        typename mpl::get_equivalent_CIEC_class_t<U>> {
    using tTClass = typename mpl::get_equivalent_CIEC_class_t<T>;
    using tUClass = typename mpl::get_equivalent_CIEC_class_t<U>;
    static_assert(forte::mpl::are_of_subtype_v<CIEC_ANY_BIT, tTClass, tUClass>,
                  "Template instantiation with incompatible types");
    using tImplicitCastType = typename mpl::get_castable_type_t<tTClass, tUClass>;
    static_assert(!(std::is_same_v<tImplicitCastType, mpl::NullType>), "No implicit cast possible");
    return OrOperation<tTClass, tUClass>::call(paIN1, paIN2);
  }

} // namespace forte
