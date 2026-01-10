/*******************************************************************************
 * Copyright (c) 2010 - 2015 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *                      2018-2019 TU Vienna/ACIN
 *               2022, 2023 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Monika Wenger, Carolyn Oates, Patrick Smejkal,
 *    Matthias Plasch,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - fixes DT_TO_TOD, removes invalid casts,
 *        update implementation to use new cast function, changes for removed
 *        implicit constructor for primitve types
 *      - adds more *_BCD_TO_** functions
 *******************************************************************************/

#pragma once

#include <type_traits>
#include "forte/datatypes/forte_any_int.h"
#include "forte/datatypes/forte_any_bit.h"

namespace forte {
  template<typename U, typename T>
  const U func_BCD_TO(const T &paVal) {
    static_assert(std::is_base_of_v<CIEC_ANY_BIT, T>);
    static_assert(std::is_base_of_v<CIEC_ANY_INT, U>);
    using SourcePrimitive = typename T::TValueType;
    using TargetPrimitive = typename U::TValueType;
    constexpr size_t sourceWidth = sizeof(SourcePrimitive);
    const SourcePrimitive sourceValue = static_cast<SourcePrimitive>(paVal);
    TargetPrimitive targetValue = 0;
    TargetPrimitive multiplicator = 1;
    for (size_t i = 0; i < sourceWidth; ++i) {
      targetValue += static_cast<TargetPrimitive>(((sourceValue >> (i * 8)) & 0x0F) * multiplicator);
      multiplicator *= 10;
      targetValue += static_cast<TargetPrimitive>(((sourceValue >> (i * 8 + 4)) & 0x0F) * multiplicator);
      multiplicator *= 10;
    }
    return U(targetValue);
  }
} // namespace forte
