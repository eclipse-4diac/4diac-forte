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

#include "forte/datatypes/forte_any_int.h"

#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

#include "forte/datatypes/forte_wchar.h"
#include "forte/datatypes/forte_char.h"
#include "forte/datatypes/forte_any_string.h"
#include "forte/datatypes/forte_any_chars.h"

namespace forte {
  template<typename T, typename U, typename V>
  T func_FIND(const U &paIn1, const V &paIn2) {
    static_assert(std::is_base_of_v<CIEC_ANY_INT, T>, "result must be of type CIEC_ANY_INT");
    static_assert(std::is_base_of_v<CIEC_ANY_STRING, U>, "paIn1 must be of type CIEC_ANY_STRING");
    static_assert(std::is_base_of_v<CIEC_ANY_CHARS, V>, "paIn2 must be of type CIEC_ANY_CHARS");
    if constexpr (std::is_base_of_v<U, CIEC_STRING>) {
      static_assert(std::is_base_of_v<V, CIEC_STRING> || std::is_same_v<V, CIEC_CHAR>,
                    "FIND in STRING only with STRING or CHAR");
      if constexpr (std::is_base_of_v<V, CIEC_STRING>) {
        const auto findIndex = paIn1.getStorage().find(paIn2.getStorage());
        if (findIndex != std::string::npos) {
          return T(static_cast<typename T::TValueType>(findIndex + 1));
        }
      } else if constexpr (std::is_same_v<V, CIEC_CHAR>) {
        const auto findIndex = paIn1.getStorage().find(static_cast<CIEC_CHAR::TValueType>(paIn2));
        if (findIndex != std::string::npos) {
          return T(static_cast<typename T::TValueType>(findIndex + 1));
        }
      }
      return T(0);
    } else if constexpr (std::is_same_v<U, CIEC_WSTRING>) {
      static_assert(std::is_same_v<V, CIEC_WSTRING> || std::is_same_v<V, CIEC_WCHAR>,
                    "FIND in WSTRING only with WSTRING or WCHAR");
      if constexpr (std::is_same_v<V, CIEC_WSTRING>) {
        const char *pc_Find = strstr(paIn1.getValue(), paIn2.getValue());
        if (nullptr != pc_Find) {
          return T(static_cast<typename T::TValueType>(pc_Find - paIn1.getValue() + 1));
        }
      } else if constexpr (std::is_same_v<V, CIEC_WCHAR>) {
        const char *pc_Find = strstr(paIn1.getValue(), static_cast<CIEC_WCHAR::TValueType>(paIn2));
        if (nullptr != pc_Find) {
          return T(static_cast<typename T::TValueType>(pc_Find - paIn1.getValue() + 1));
        }
      }
      return T(0);
    }
  }

} // namespace forte
