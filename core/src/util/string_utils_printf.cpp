/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#include "forte/util/string_utils.h"

#include <concepts>
#include <cstdio>
#include <limits>

namespace forte::util {
  template<typename T>
    requires std::same_as<T, TForteFloat> || std::same_as<T, TForteDFloat>
  void appendFloat(std::string &paTargetBuf, const T paValue) {
    const std::size_t start = paTargetBuf.size();
    // account for sign, decimal point, and exponent
    paTargetBuf.resize(start + std::numeric_limits<T>::max_digits10 + 8);
    const int written = std::snprintf(paTargetBuf.data() + start, paTargetBuf.size() - start + 1, "%.*g",
                                      std::numeric_limits<T>::max_digits10, paValue);
    if (written < 0) {
      std::abort();
    }
    paTargetBuf.resize(start + static_cast<std::size_t>(written));
    normalizeToStringRepresentation(paTargetBuf, start);
  }

  template void appendFloat(std::string &, TForteFloat);
  template void appendFloat(std::string &, TForteDFloat);
} // namespace forte::util
