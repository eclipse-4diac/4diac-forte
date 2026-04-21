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
#include <limits>
#include <charconv>
#include <system_error>

namespace forte::util {
  template<typename T>
    requires std::same_as<T, TForteFloat> || std::same_as<T, TForteDFloat>
  void appendFloat(std::string &paTargetBuf, const T paValue) {
    const std::size_t start = paTargetBuf.size();
    // account for sign, decimal point, and exponent
    paTargetBuf.resize(start + std::numeric_limits<T>::max_digits10 + 8);
    auto [ptr, ec] = std::to_chars(paTargetBuf.data() + start, paTargetBuf.data() + paTargetBuf.size(), paValue,
                                   std::chars_format::general, std::numeric_limits<T>::max_digits10);
    if (ec != std::errc()) {
#if __cpp_exceptions
      throw std::system_error(std::make_error_code(ec), "Failed to convert float to string");
#else
      std::abort();
#endif
    }
    paTargetBuf.resize(ptr - paTargetBuf.data());
    normalizeToStringRepresentation(paTargetBuf, start);
  }

  template void appendFloat(std::string &, TForteFloat);
  template void appendFloat(std::string &, TForteDFloat);
} // namespace forte::util
