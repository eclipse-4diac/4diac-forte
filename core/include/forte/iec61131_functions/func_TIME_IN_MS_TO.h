/*******************************************************************************
 * Copyright (c) 2019, 2024 TU Vienna/ACIN
 *                          Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians - adds non standard time to number conversions
 *    Martin Erich Jobst - add templated versions for ANY_DURATION
 *******************************************************************************/

#pragma once

#include <type_traits>
#include "forte/util/forte_constants.h"
#include "forte/datatypes/forte_any_real.h"
#include "forte/datatypes/forte_any_num.h"
#include "forte/datatypes/forte_any_duration.h"

namespace forte {
  template<typename T, typename U>
  inline T func_TIME_IN_MS_TO(const U &paValue) {
    static_assert((std::is_base_of_v<CIEC_ANY_NUM, T>), "T not of ANY_NUM");
    static_assert((std::is_base_of_v<CIEC_ANY_DURATION, U>), "U not of ANY_DURATION");
    if (std::is_base_of_v<CIEC_ANY_REAL, T>) {
      return T(static_cast<typename T::TValueType>(paValue.getInNanoSeconds()) *
               (static_cast<typename T::TValueType>(util::constants::cMillisecondsPerSecond) /
                static_cast<typename T::TValueType>(util::constants::cNanosecondsPerSecond)));
    } else {
      return T(static_cast<typename T::TValueType>(paValue.getInMilliSeconds()));
    }
  }
} // namespace forte
