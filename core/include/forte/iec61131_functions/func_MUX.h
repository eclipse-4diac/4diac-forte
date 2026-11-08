/*******************************************************************************
 * Copyright (c) 2026 Martin Melik Merkumians
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * https://www.eclipse.org/legal/epl-2.0.
 *
 * AI Disclosure:
 * This file was largely AI-generated. AI-generated portions are made
 * available under CC0-1.0 and are not subject to the project's license.
 * Human contributions are made available under EPL-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0 AND CC0-1.0
 * Assisted-by: GitHub Copilot
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation of variadic MUX and shared runtime selection
 *******************************************************************************/

#pragma once

#include <array>
#include <span>
#include <type_traits>
#include <utility>

#include "common_type.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_any_int.h"
#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_struct.h"
#include "forte/iec61131_cast_helper.h"

namespace forte {
  /**
   * @brief Internal shared MUX helpers, not additional public IEC function signatures.
   */
  namespace mux_detail {
    /**
     * @brief Compare datatype IDs and derived-type identity metadata.
     * @param paLeft First value to compare.
     * @param paRight Second value to compare.
     * @return Whether datatype IDs, structure names, and array bounds/element
     *         metadata match, recursively for nested arrays.
     */
    inline bool sameDerivedType(const CIEC_ANY &paLeft, const CIEC_ANY &paRight) {
      if (paLeft.getDataTypeID() != paRight.getDataTypeID()) {
        return false;
      }

      if (paLeft.getDataTypeID() == CIEC_ANY::e_STRUCT) {
        return static_cast<const CIEC_STRUCT &>(paLeft).getStructTypeNameID() ==
               static_cast<const CIEC_STRUCT &>(paRight).getStructTypeNameID();
      }

      if (paLeft.getDataTypeID() == CIEC_ANY::e_ARRAY) {
        const auto &leftArray = static_cast<const CIEC_ARRAY &>(paLeft);
        const auto &rightArray = static_cast<const CIEC_ARRAY &>(paRight);
        if (leftArray.getLowerBound() != rightArray.getLowerBound() ||
            leftArray.getUpperBound() != rightArray.getUpperBound() ||
            leftArray.getElementDataTypeID() != rightArray.getElementDataTypeID() ||
            leftArray.getElementTypeNameID() != rightArray.getElementTypeNameID()) {
          return false;
        }
        if (leftArray.getElementDataTypeID() == CIEC_ANY::e_ARRAY) {
          return leftArray.size() != 0 && rightArray.size() != 0 &&
                 sameDerivedType(leftArray[leftArray.getLowerBound()], rightArray[rightArray.getLowerBound()]);
        }
      }

      return true;
    }

    /**
     * @brief Try one common result candidate and convert only the selected value.
     * @tparam Candidate IEC value type or derived-value pointer alternative.
     * @param paInputs Complete operand range used to authorize implicit casts.
     * @param paIndex Zero-based index of the selected operand.
     * @param paOutput Receives the converted scalar or copied derived value on success.
     * @pre paIndex is within paInputs; derived-type compatibility has been checked.
     * @return True if every input implicitly casts to Candidate, otherwise false
     *         without changing paOutput.
     */
    template<typename Candidate>
    bool selectAs(const std::span<const CIEC_ANY_VARIANT> paInputs, const size_t paIndex, CIEC_ANY_VARIANT &paOutput) {
      for (const CIEC_ANY_VARIANT &input : paInputs) {
        if (!std::visit(
                [](const auto &paValue) {
                  return mpl::is_implicitly_castable_v<std::decay_t<decltype(paValue)>, Candidate>;
                },
                static_cast<const CIEC_ANY_VARIANT::variant &>(input))) {
          return false;
        }
      }

      return std::visit(
          [&paOutput](const auto &paSelectedValue) {
            using SelectedType = std::decay_t<decltype(paSelectedValue)>;
            if constexpr (!mpl::is_implicitly_castable_v<SelectedType, Candidate>) {
              return false;
            } else if constexpr (std::is_base_of_v<CIEC_ANY, Candidate>) {
              paOutput.setValue(static_cast<Candidate>(paSelectedValue));
              return true;
            } else {
              paOutput.setValue(*paSelectedValue);
              return true;
            }
          },
          static_cast<const CIEC_ANY_VARIANT::variant &>(paInputs[paIndex]));
    }

    /**
     * @brief Search variant alternatives for a common runtime result type.
     * @tparam Indices Candidate indices in TIecAnyVariantType, tried in order.
     * @param paInputs Complete operand range.
     * @param paIndex Zero-based index of the selected operand.
     * @param paOutput Receives the selection on success; unchanged on failure.
     * @pre paIndex is within paInputs; derived-type compatibility has been checked.
     * @return Whether an alternative accepts every operand implicitly.
     */
    template<size_t... Indices>
    bool selectBuiltin(const std::span<const CIEC_ANY_VARIANT> paInputs,
                       const size_t paIndex,
                       CIEC_ANY_VARIANT &paOutput,
                       std::index_sequence<Indices...>) {
      return (selectAs<std::variant_alternative_t<Indices, TIecAnyVariantType>>(paInputs, paIndex, paOutput) || ...);
    }

    /**
     * @brief Select from a runtime-sized IEC variant range for both MUX call paths.
     * @param paK Signed or unsigned IEC integer selector, indexed from zero.
     * @param paInputs Operand range determining the common concrete result type.
     * @param paOutput Receives the converted selection on success; reset on failure.
     * @return False for an empty range, out-of-range selector, incompatible
     *         derived types, or absence of a common implicitly castable IEC type.
     *
     * Prefer an operand type accepting all inputs, then try the variant catalog.
     * Only the selected value is converted. Structure names and array metadata
     * must match for derived operands. Failure handling is FORTE implementation
     * behavior, not an IEC-specified result value; this helper does not log errors.
     */
    inline bool
    select(const CIEC_ANY_INT &paK, const std::span<const CIEC_ANY_VARIANT> paInputs, CIEC_ANY_VARIANT &paOutput) {
      if (paInputs.empty()) {
        paOutput.reset();
        return false;
      }

      size_t selectedIndex;
      if (paK.isSigned()) {
        const auto selector = paK.getSignedValue();
        if (selector < 0 || static_cast<CIEC_ANY_INT::TLargestUIntValueType>(selector) >= paInputs.size()) {
          paOutput.reset();
          return false;
        }
        selectedIndex = static_cast<size_t>(selector);
      } else {
        const auto selector = paK.getUnsignedValue();
        if (selector >= paInputs.size()) {
          paOutput.reset();
          return false;
        }
        selectedIndex = static_cast<size_t>(selector);
      }

      const CIEC_ANY &firstInput = paInputs.front().unwrap();
      for (const CIEC_ANY_VARIANT &input : paInputs.subspan(1)) {
        const auto commonDataType = firstInput.getDataTypeID();
        const auto inputDataType = input.unwrap().getDataTypeID();
        if ((commonDataType == CIEC_ANY::e_ARRAY || commonDataType == CIEC_ANY::e_STRUCT ||
             inputDataType == CIEC_ANY::e_ARRAY || inputDataType == CIEC_ANY::e_STRUCT) &&
            !sameDerivedType(firstInput, input.unwrap())) {
          paOutput.reset();
          return false;
        }
      }

      for (const CIEC_ANY_VARIANT &input : paInputs) {
        if (std::visit(
                [&](const auto &paCandidate) {
                  return selectAs<std::decay_t<decltype(paCandidate)>>(paInputs, selectedIndex, paOutput);
                },
                static_cast<const CIEC_ANY_VARIANT::variant &>(input))) {
          return true;
        }
      }
      if (selectBuiltin(paInputs, selectedIndex, paOutput,
                        std::make_index_sequence<std::variant_size_v<TIecAnyVariantType>>{})) {
        return true;
      }
      paOutput.reset();
      return false;
    }
  } // namespace mux_detail

  /**
   * @brief Select an operand by IEC integer index and return its common IEC type.
   * @tparam T Concrete IEC type of the first value input.
   * @tparam Args Concrete IEC types of the remaining value inputs.
   * @param paK Zero-based selector: zero selects paIN0, one the next input, etc.
   * @param paIN0 First value input.
   * @param paInputs Remaining value inputs; at least one is required.
   * @return Selected value converted to mpl::common_type_t<T, Args...>.
   * @pre All value inputs implicitly cast to a common concrete IEC type.
   * @pre Derived operands have matching structure names or array bounds and
   *      element metadata, recursively for nested arrays.
   *
   * Incompatible compile-time types and fewer than two value inputs are rejected
   * during instantiation. Only the selected operand is converted, although all
   * supplied operands are evaluated before the C++ call. For an out-of-range
   * selector or failed runtime derived-type check, FORTE returns a default value
   * of the result type without logging. An out-of-range selector is an IEC error;
   * the default return is implementation behavior, not a standardized IEC result.
   * @see mpl::common_type_t
   * @see func_SEL
   */
  template<typename T, typename... Args>
  auto func_MUX(const CIEC_ANY_INT &paK, const T &paIN0, const Args &...paInputs) -> mpl::common_type_t<T, Args...> {
    using ResultType = mpl::common_type_t<T, Args...>;
    static_assert(sizeof...(Args) >= 1, "MUX requires at least two value inputs");
    static_assert(!std::is_same_v<ResultType, mpl::NullType>, "MUX inputs must have a common IEC type");
    static_assert(std::is_base_of_v<CIEC_ANY, ResultType>, "MUX result must be an IEC data type");
    static_assert(!generic_datatype<ResultType>, "MUX result must be a concrete IEC data type");
    static_assert(
        (mpl::is_implicitly_castable_v<T, ResultType> && ... && mpl::is_implicitly_castable_v<Args, ResultType>),
        "MUX inputs must be implicitly castable to their common IEC type");

    const std::array<CIEC_ANY_VARIANT, sizeof...(Args) + 1> inputs{CIEC_ANY_VARIANT(paIN0),
                                                                   CIEC_ANY_VARIANT(paInputs)...};
    CIEC_ANY_VARIANT selectedValue;
    if (!mux_detail::select(paK, std::span<const CIEC_ANY_VARIANT>(inputs), selectedValue)) {
      return ResultType{};
    }

    ResultType result;
    result.setValue(selectedValue.unwrap());
    return result;
  }
} // namespace forte
