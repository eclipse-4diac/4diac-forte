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

#include <tuple>

#include "forte/datatypes/forte_any_num_variant.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/datatypes/forte_struct.h"
#include "forte/iec61131_cast_helper.h"
#include "forte/util/devlog.h"

namespace forte {
  namespace mux_detail {
    using numeric_candidates = std::tuple<CIEC_SINT,
                                          CIEC_USINT,
                                          CIEC_INT,
                                          CIEC_UINT,
                                          CIEC_DINT,
                                          CIEC_UDINT,
                                          CIEC_LINT,
                                          CIEC_ULINT,
                                          CIEC_REAL,
                                          CIEC_LREAL>;

    template<typename Candidates, typename... Inputs>
    struct common_type;

    template<typename... Inputs>
    struct common_type<std::tuple<>, Inputs...> {
        using type = mpl::NullType;
    };

    template<typename Candidate, typename... Candidates, typename... Inputs>
    struct common_type<std::tuple<Candidate, Candidates...>, Inputs...> {
        using type = std::conditional_t<(mpl::is_implicitly_castable_v<Inputs, Candidate> && ...),
                                        Candidate,
                                        typename common_type<std::tuple<Candidates...>, Inputs...>::type>;
    };

    template<typename... Inputs>
    using common_type_t = typename common_type<decltype(std::tuple_cat(std::declval<std::tuple<Inputs...>>(),
                                                                       std::declval<numeric_candidates>())),
                                               Inputs...>::type;

    template<typename T>
    concept input = std::is_base_of_v<CIEC_ANY, T> && !is_generic_datatype_v<T> && !std::is_abstract_v<T> &&
                    std::is_copy_constructible_v<T>;

    inline bool same_type(const CIEC_ANY &paLeft, const CIEC_ANY &paRight) {
      if (paLeft.getDataTypeID() != paRight.getDataTypeID()) {
        return false;
      }
      if (paLeft.getDataTypeID() == CIEC_ANY::e_STRUCT) {
        return static_cast<const CIEC_STRUCT &>(paLeft).getStructTypeNameID() ==
               static_cast<const CIEC_STRUCT &>(paRight).getStructTypeNameID();
      }
      if (paLeft.getDataTypeID() == CIEC_ANY::e_ARRAY) {
        const auto &left = static_cast<const CIEC_ARRAY &>(paLeft);
        const auto &right = static_cast<const CIEC_ARRAY &>(paRight);
        // Empty type-erased variable arrays expose no safe element-type metadata.
        if (left.size() == 0 || right.size() == 0) {
          return false;
        }
        if (left.getLowerBound() != right.getLowerBound() || left.getUpperBound() != right.getUpperBound() ||
            left.getElementDataTypeID() != right.getElementDataTypeID() ||
            left.getElementTypeNameID() != right.getElementTypeNameID()) {
          return false;
        }
        for (auto i = left.getLowerBound(); i <= left.getUpperBound(); ++i) {
          if (!same_type(left[i], right[i])) {
            return false;
          }
        }
      }
      return true;
    }

    template<typename T>
    bool same_type(const CIEC_ARRAY_VARIABLE<T> &paLeft, const CIEC_ARRAY_VARIABLE<T> &paRight) {
      if (paLeft.size() == 0 && paRight.size() == 0) {
        return paLeft.getLowerBound() == paRight.getLowerBound() && paLeft.getUpperBound() == paRight.getUpperBound();
      }
      return same_type(static_cast<const CIEC_ANY &>(paLeft), static_cast<const CIEC_ANY &>(paRight));
    }
  } // namespace mux_detail

  template<mux_detail::input T, mux_detail::input U, mux_detail::input... Args>
    requires(!std::is_same_v<mux_detail::common_type_t<T, U, Args...>, mpl::NullType>)
  auto func_MUX(const CIEC_ANY_INT &paK, const T &paIN0, const U &paIN1, const Args &...paArgs)
      -> mux_detail::common_type_t<T, U, Args...> {
    using ReturnType = mux_detail::common_type_t<T, U, Args...>;
    const auto defaultResult = [&]() {
      ReturnType result(paIN0);
      result.reset();
      return result;
    };
    if constexpr (std::is_base_of_v<CIEC_ANY_DERIVED, ReturnType>) {
      if (!mux_detail::same_type(paIN0, paIN1) || !(mux_detail::same_type(paIN0, paArgs) && ...)) {
        DEVLOG_ERROR("MUX inputs have incompatible types.\n");
        return defaultResult();
      }
    }
    const auto index = paK.getUnsignedValue();
    if (index >= sizeof...(Args) + 2) {
      DEVLOG_ERROR("MUX selector is out of range.\n");
      return defaultResult();
    }
    const auto inputs = std::tie(paIN0, paIN1, paArgs...);
    const auto select = [&]<std::size_t I = 0>(auto &&paSelf) -> ReturnType {
      if (I == index) {
        return ReturnType(std::get<I>(inputs));
      }
      if constexpr (I + 1 < sizeof...(Args) + 2) {
        return paSelf.template operator()<I + 1>(paSelf);
      } else {
        return defaultResult();
      }
    };
    return select(select);
  }
} // namespace forte
