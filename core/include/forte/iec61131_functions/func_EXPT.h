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

#include "forte/datatypes/forte_any_num.h"

namespace forte {
  template<typename T, typename U>
  T func_EXPT(const T &paIN1, const U &paIN2) {
    static_assert((std::is_base_of_v<CIEC_ANY_REAL, T>), "IN1 not of type ANY_REAL");
    static_assert((std::is_base_of_v<CIEC_ANY_NUM, U>), "IN2 not of type ANY_NUM");
    return T(static_cast<typename T::TValueType>(
        pow(static_cast<typename T::TValueType>(paIN1), static_cast<typename U::TValueType>(paIN2))));
  }

} // namespace forte
