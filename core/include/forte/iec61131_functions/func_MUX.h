/*******************************************************************************
 * Copyright (c) 2026 Contributors to the Eclipse Foundation
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_ulint.h"
#include "forte/iec61131_cast_helper.h"
#include "forte/util/devlog.h"

namespace forte {
  template<typename T, typename U>
  auto func_MUX(const CIEC_ANY_INT &paK, const T &paIN0, const U &paIN1) -> mpl::get_castable_type_t<T, U> {
    using ReturnType = mpl::get_castable_type_t<T, U>;
    switch (paK.getUnsignedValue()) {
      case 0: return ReturnType(paIN0);
      case 1: return ReturnType(paIN1);
      default: DEVLOG_ERROR("MUX selector is out of range.\n"); return ReturnType();
    }
  }

  template<typename T, typename U, typename V, typename... Args>
  auto func_MUX(const CIEC_ANY_INT &paK, const T &paIN0, const U &paIN1, const V &paIN2, const Args &...paArgs) {
    using PromotedType = mpl::get_castable_type_t<T, U>;
    using ReturnType = decltype(func_MUX(paK, PromotedType(paIN0), paIN2, paArgs...));
    if (paK.getUnsignedValue() == 0) {
      return ReturnType(paIN0);
    }
    return ReturnType(func_MUX(CIEC_ULINT(paK.getUnsignedValue() - 1), PromotedType(paIN1), paIN2, paArgs...));
  }
} // namespace forte
