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

#include "forte/util/devlog.h"

#include "forte/datatypes/forte_any_int.h"

#include "forte/datatypes/forte_bool.h"

#include "forte/datatypes/forte_any_bit.h"

namespace forte {
  template<typename T>
  auto func_SHL(const T &paIn, const CIEC_ANY_INT &paN) -> typename mpl::get_equivalent_CIEC_class_t<T> {
    static_assert((std::is_base_of_v<CIEC_ANY_BIT, T>), "T not of ANY_BIT");
    using returnType = typename mpl::get_equivalent_CIEC_class_t<T>;
    if ((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
      return returnType(static_cast<typename returnType::TValueType>(paIn << paN.getUnsignedValue()));
    } else {
      DEVLOG_ERROR("value of input N is less than zero\n");
      return returnType(static_cast<typename T::TValueType>(0));
    }
  }
  template<>
  auto func_SHL(const CIEC_BOOL &paIn, const CIEC_ANY_INT &paN) -> CIEC_BOOL;

} // namespace forte
