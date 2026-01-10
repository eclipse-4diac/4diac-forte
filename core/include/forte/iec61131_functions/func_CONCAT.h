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

#include "forte/datatypes/forte_string.h"

#include "forte/datatypes/forte_any_chars.h"

namespace forte {
  template<typename T, typename U>
  auto func_CONCAT(const T &paIn1, const U &paIn2) -> typename mpl::get_concat_result_type_t<T, U> {
    using Returntype = typename mpl::get_concat_result_type_t<T, U>;
    static_assert((std::is_base_of_v<CIEC_ANY_CHARS, T>), "T not of ANY_CHARS");
    static_assert((std::is_base_of_v<CIEC_ANY_CHARS, U>), "U not of ANY_CHARS");
    static_assert((std::is_base_of_v<CIEC_ANY_CHARS, Returntype>), "Returntype not of ANY_CHARS");
    Returntype temp(paIn1);
    if constexpr (std::is_base_of_v<CIEC_STRING, Returntype>) {
      temp.append(paIn2);
      return temp;
    } else {
      temp.reserve(static_cast<TForteUInt16>(paIn1.length() + paIn2.length()));
      temp.append(paIn2.getValue(), paIn2.length());
      return temp;
    }
  }

  template<typename T, typename... Args>
  auto func_CONCAT(const T &paIn1, const Args &...args) {
    return func_CONCAT(paIn1, func_CONCAT(args...));
  }

} // namespace forte
