/*******************************************************************************
 * Copyright (c) 2026 Aimirim STI
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * AI Disclosure: This file was largely AI-generated. The AI-generated
 * portions are made available under CC0-1.0 and not subject to the
 * project's licence. The human contributor has reviewed and verified
 * that the code is correct.
 *
 * SPDX-License-Identifier: EPL-2.0 AND CC0-1.0
 * Assisted-by: Anthropic Claude Opus 5.5 (claude-opus-5-5)
 *
 * Contributors:
 *   Pedro Ricardo C. Souza - initial implementation
 *******************************************************************************/

#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "forte/stringid.h"

namespace forte::lua {

  /*!\brief The version of a Lua type: FNV-1a (64 bit) over the bytes of its source
   *
   * @return exactly 16 lowercase hexadecimal digits
   */
  inline std::string computeTypeHash(std::string_view paSource) {
    constexpr std::uint64_t cOffsetBasis = 0xcbf29ce484222325ULL;
    constexpr std::uint64_t cPrime = 0x100000001b3ULL;
    constexpr char cDigits[] = "0123456789abcdef";

    std::uint64_t hash = cOffsetBasis;
    for (const char character : paSource) {
      hash ^= static_cast<unsigned char>(character);
      hash *= cPrime;
    }
    std::string text(16, '0');
    for (auto digit = text.rbegin(); digit != text.rend(); ++digit) {
      *digit = cDigits[hash & 0xFU];
      hash >>= 4;
    }
    return text;
  }

  /*!\brief The hash of a source as a string that lives as long as the process
   *
   * A type entry only keeps a view on its hash. Interned strings are never freed, so the view stays valid even
   * after the entry has been replaced.
   */
  inline std::string_view getInternedTypeHash(std::string_view paSource) {
    return StringId::insert(computeTypeHash(paSource)).get();
  }

} // namespace forte::lua
