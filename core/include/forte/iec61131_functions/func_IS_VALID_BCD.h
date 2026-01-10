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
  CIEC_BOOL func_IS_VALID_BCD(const T &paValue) {
    static_assert(std::is_base_of_v<CIEC_ANY_BIT, T>,
                  "Operand must be of ANY_BIT (excluding BOOL)"); // any bit type is allowed
    static_assert(!std::is_same_v<CIEC_BOOL, T>, "Operand must be of ANY_BIT, but not BOOL"); // but not BOOL
    using ValueType = typename T::TValueType;
    constexpr size_t valueTypeSize = sizeof(ValueType);
    constexpr ValueType mask = 0x0F;
    constexpr size_t nibble = 4;
    const ValueType value = static_cast<ValueType>(paValue);
    for (size_t i = 0U; i < valueTypeSize; i++) {
      const size_t bitShift = i * 8;
      if (((value >> bitShift) & mask) > 9U || ((value >> (bitShift + nibble)) & mask) > 9U) {
        return false_BOOL;
      }
    }
    return true_BOOL;
  }

} // namespace forte
