/*******************************************************************************
 * Copyright (c) 2010,2025 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH
 *                         Primetals Technologies Austria GmbH
 *                         Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Monika Wenger, Ingo Hegny, Patrick Smejkal, Gerhard Ebenhofer,
 *    Matthias Plasch, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians
 *      - added different type templates for IEC 61131-3 functions and adds several type guards
 *      - Reworked binary operator templates to create correctly calculated and typed results
 *      - reworked and fixes ADD and SUB for time types
 *      - Added variadic comparison, MIN, MAX, ADD, MUL functions
 *      - Added unary plus function
 *    Martin Erich Jobst
 *      - added lower and upper bound functions
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_array_dynamic.h"

#include "endianess.h"
#include "forte/datatypes/forte_struct.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_any_string.h"
#include "forte/datatypes/forte_any_magnitude.h"
#include "forte/datatypes/forte_any_date.h"
#include "forte/datatypes/forte_any_char.h"
#include "forte/datatypes/forte_any_bit.h"

namespace forte {
  template<typename T>
  auto func_TO_BIG_ENDIAN(const T &paValue) ->
      typename std::enable_if_t<std::endian::native == std::endian::little && std::is_base_of_v<CIEC_ANY_MAGNITUDE, T>,
                                T> {
    return swapEndianess(paValue);
  }

  template<typename T>
  auto func_TO_BIG_ENDIAN(const T &paValue) ->
      typename std::enable_if_t<std::endian::native == std::endian::little && std::is_base_of_v<CIEC_ANY_BIT, T>, T> {
    return swapEndianess(paValue);
  }

  template<typename T>
  auto func_TO_BIG_ENDIAN(const T &paValue) ->
      typename std::enable_if_t<std::endian::native == std::endian::little && std::is_base_of_v<CIEC_ANY_CHAR, T>, T> {
    return swapEndianess(paValue);
  }

  template<typename T>
  auto func_TO_BIG_ENDIAN(const T &paValue) ->
      typename std::enable_if_t<std::endian::native == std::endian::little && std::is_base_of_v<CIEC_ANY_DATE, T>, T> {
    return swapEndianess(paValue);
  }

  template<typename T>
  auto func_TO_BIG_ENDIAN(const T &paValue) ->
      typename std::enable_if_t<std::endian::native == std::endian::little && std::is_base_of_v<CIEC_ANY_STRING, T>,
                                T> {
    return swapEndianess(paValue);
  }

  CIEC_ARRAY_DYNAMIC func_TO_BIG_ENDIAN(const CIEC_ARRAY_DYNAMIC &paValue);

  template<typename T>
  auto func_TO_BIG_ENDIAN(const T &paValue) ->
      typename std::enable_if_t<std::endian::native == std::endian::little && std::is_base_of_v<CIEC_STRUCT, T>, T> {
    return swapEndianess(paValue);
  }

  template<template<typename, intmax_t, intmax_t> typename T, typename U, intmax_t lowerBound, intmax_t upperBound>
  auto func_TO_BIG_ENDIAN(const T<U, lowerBound, upperBound> &paValue) -> typename std::enable_if_t<
      std::endian::native == std::endian::little &&
          std::is_base_of_v<CIEC_ARRAY_FIXED<U, lowerBound, upperBound>, T<U, lowerBound, upperBound>>,
      T<U, lowerBound, upperBound>> {
    return swapEndianess(paValue);
  }

  template<template<typename> typename T, typename U>
  auto func_TO_BIG_ENDIAN(const T<U> &paValue) -> typename std::
      enable_if_t<std::endian::native == std::endian::little && std::is_base_of_v<CIEC_ARRAY_VARIABLE<U>, T<U>>, T<U>> {
    return swapEndianess(paValue);
  }
  template<typename T>
  auto func_TO_BIG_ENDIAN(const T &paValue) ->
      typename std::enable_if_t<std::endian::native == std::endian::big && std::endian::native == std::endian::big, T> {
    return paValue;
  }

} // namespace forte
