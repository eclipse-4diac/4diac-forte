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

#include "forte/datatypes/forte_lint.h"
#include "forte/util/forte_constants.h"
#include "func_TIME_IN_NS_TO.h"

namespace forte {
  template<typename T>
  inline CIEC_LINT func_TIME_IN_NS_TO_LINT(const T &paValue) {
    return func_TIME_IN_NS_TO<CIEC_LINT>(paValue);
  }
} // namespace forte
