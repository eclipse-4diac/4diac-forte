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

#include "forte/datatypes/forte_any_string.h"

namespace forte {
  template<typename T>
  T func_TOLOWER(const T &paIn) {
    static_assert(std::is_base_of_v<CIEC_ANY_STRING, T>, "Operand must be of type ANY_STRING!");
    if constexpr (std::is_base_of_v<CIEC_STRING, T>) {
      CIEC_STRING::storage_type buffer(paIn.getStorage());
      std::transform(buffer.begin(), buffer.end(), buffer.begin(), tolower);
      return CIEC_STRING(buffer);
    } else {
      T temp(paIn);
      char *current = temp.getValue();
      for (size_t i = 0; i <= paIn.length(); ++i) {
        current[i] = static_cast<char>(tolower(current[i]));
      }
      return temp;
    }
  }

} // namespace forte
