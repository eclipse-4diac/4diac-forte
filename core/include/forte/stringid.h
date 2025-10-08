/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
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

#pragma once

#include <cstddef>
#include <cstring>
#include <string_view>

#include "forte/util/fixed_string.h"

namespace forte {
  class StringId {
      template<util::fixed_string A>
      struct Register {
          Register() {
            intern(A);
          }
          static const Register scmRegister;
      };

    public:
      constexpr StringId() = default;

      [[nodiscard]] constexpr const char *data() const {
        return mString.data();
      }

      [[nodiscard]] constexpr std::size_t size() const {
        return mString.size();
      }

      [[nodiscard]] constexpr bool empty() const {
        return mString.empty();
      }

      [[nodiscard]] constexpr std::string_view get() const {
        return mString;
      }

      [[nodiscard]] constexpr operator bool() const {
        return mString.data() != nullptr;
      }

      [[nodiscard]] constexpr operator std::string_view() const {
        return mString;
      }

      [[nodiscard]] friend constexpr bool operator==(const StringId &paLeft, const StringId &paRight) {
        return paLeft.data() == paRight.data();
      }

      [[nodiscard]] friend constexpr auto operator<=>(const StringId &paLeft, const StringId &paRight) {
        return paLeft.data() <=> paRight.data();
      }

      static StringId lookup(std::string_view paString);

      static StringId insert(std::string_view paString);

      template<util::fixed_string A>
      static constexpr StringId fixed() {
        (void) &Register<A>::scmRegister;
#ifdef _WIN32
        return StringId{intern(A)};
#else
        return StringId{A};
#endif
      }

    private:
      constexpr explicit StringId(const std::string_view paString) : mString(paString) {
      }

      static std::string_view intern(std::string_view paString);

      std::string_view mString;
  };

  template<util::fixed_string A>
  const StringId::Register<A> StringId::Register<A>::scmRegister;

  namespace literals {
    template<util::fixed_string A>
    constexpr StringId operator""_STRID() {
      return StringId::fixed<A>();
    }
  } // namespace literals
} // namespace forte

template<>
struct std::hash<forte::StringId> {
    [[nodiscard]] std::size_t operator()(const forte::StringId &paStringId) const noexcept {
      return std::hash<const char *>{}(paStringId.data());
    }
};
