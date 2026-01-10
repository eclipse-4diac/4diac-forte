/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_ltime.h"

namespace forte {
  template<typename T>
  inline auto func_TO_LTIME(const T &paVal) {
    static_assert(forte::mpl::is_implicit_or_explicit_castable_v<T, CIEC_LTIME>);
    return CIEC_ANY::cast<CIEC_LTIME>(paVal);
  }
} // namespace forte
