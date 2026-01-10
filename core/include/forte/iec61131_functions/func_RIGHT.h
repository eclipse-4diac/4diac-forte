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

#include "forte/datatypes/forte_string.h"

#include "forte/datatypes/forte_any_string.h"

namespace forte {
  template<typename T>
  T func_RIGHT(const T &paIn, const CIEC_ANY_INT &paL) {
    static_assert(std::is_base_of_v<CIEC_ANY_STRING, T>, "T not of ANY_STRING");
    if (true == paL.isSigned() && 0 > paL.getSignedValue()) {
      DEVLOG_ERROR("value of input L is less than zero\n");
      return paIn;
    } else {
      if (paIn.length() < paL.getUnsignedValue()) {
        DEVLOG_ERROR("string shorter than input L\n");
        return paIn;
      } else {
        if constexpr (std::is_same_v<CIEC_STRING, T>) {
          const CIEC_STRING::storage_type &storage = paIn.getStorage();
          return CIEC_STRING(storage.substr(storage.length() - paL.getUnsignedValue()));
        } else {
          T temp;
          temp.reserve(static_cast<TForteUInt16>(paL.getUnsignedValue()));
          memcpy(temp.getValue(), paIn.getValue() + (paIn.length() - static_cast<TForteUInt16>(paL.getUnsignedValue())),
                 static_cast<TForteUInt16>(paL.getUnsignedValue()));
          temp.getValue()[paL.getUnsignedValue()] = '\0';
          temp.assign(temp.getValue(), static_cast<TForteUInt16>(paL.getUnsignedValue()));
          return temp;
        }
      }
    }
  }

} // namespace forte
