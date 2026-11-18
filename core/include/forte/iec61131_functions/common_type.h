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
 *      - initial implementation of common IEC type resolution
 *******************************************************************************/

/**
 * @file
 * @brief Common IEC result-type resolution for selection functions.
 */
#pragma once

#include <tuple>
#include <type_traits>

#include "forte/datatypes/forte_any_variant.h"
#include "forte/iec61131_cast_helper.h"

namespace forte::mpl {
  /**
   * @brief Find the first concrete IEC candidate accepting every input implicitly.
   * @tparam Candidates Ordered type list of candidate result types.
   * @tparam Inputs Operand types checked against each candidate.
   *
   * Checks the complete input set for each candidate using the IEC implicit-cast
   * traits. The nested type is NullType if no candidate accepts all inputs.
   */
  template<typename Candidates, typename... Inputs>
  struct find_common_type;

  /** @brief Terminate an exhausted candidate search with NullType. */
  template<template<typename...> class List, typename... Inputs>
  struct find_common_type<List<>, Inputs...> {
      /** @brief No acceptable candidate was found. */
      using type = NullType;
  };

  /** @brief Accept the current candidate or continue searching the remaining list. */
  template<template<typename...> class List, typename Candidate, typename... Candidates, typename... Inputs>
  struct find_common_type<List<Candidate, Candidates...>, Inputs...> {
      /** @brief First acceptable concrete IEC candidate, or NullType. */
      using type = std::conditional_t<std::is_base_of_v<CIEC_ANY, Candidate> && !generic_datatype<Candidate> &&
                                          (is_implicitly_castable_v<Inputs, Candidate> && ...),
                                      Candidate,
                                      typename find_common_type<List<Candidates...>, Inputs...>::type>;
  };

  /**
   * @brief Resolve a common concrete IEC type for the complete operand set.
   * @tparam Inputs Operand types whose implicit IEC conversions are considered.
   *
   * Prefer an operand type accepting all inputs; otherwise search the concrete
   * types in TIecAnyVariantType. Candidate order breaks ties between acceptable
   * types. The nested type is NullType when none accepts every operand.
   * This resolution does not use native C++ arithmetic promotion.
   * @see common_type_t
   */
  template<typename... Inputs>
  struct common_type {
      /** @brief First acceptable operand type, or NullType. */
      using InputType = typename find_common_type<std::tuple<Inputs...>, Inputs...>::type;
      /** @brief Common concrete IEC result type, or NullType if incompatible. */
      using type = std::conditional_t<!std::is_same_v<InputType, NullType>,
                                      InputType,
                                      typename find_common_type<TIecAnyVariantType, Inputs...>::type>;
  };

  /**
   * @brief Common concrete IEC type for all Inputs, or NullType if incompatible.
   * @tparam Inputs Operand types checked using IEC implicit-cast traits.
   * @see common_type
   */
  template<typename... Inputs>
  using common_type_t = typename common_type<Inputs...>::type;
} // namespace forte::mpl
