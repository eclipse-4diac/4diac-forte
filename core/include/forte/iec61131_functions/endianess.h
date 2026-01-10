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

#include "forte/datatypes/forte_struct.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/datatypes/forte_any_magnitude.h"
#include "forte/datatypes/forte_any_bit.h"
#include "forte/datatypes/forte_any_char.h"
#include "forte/datatypes/forte_any_date.h"
#include "forte/datatypes/forte_any_string.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

#include <type_traits>

namespace forte {
  template<typename T>
  T swapSimpleDataHelper(const T data) {
    constexpr size_t dataSize = sizeof(T);
    const char *const dataBytes = reinterpret_cast<const char *>(&data);
    T swappedData;
    char *const swappedDataBytes = reinterpret_cast<char *>(&swappedData);

    if constexpr (1 == dataSize) {
      return T(data);
    } else if constexpr (2 == dataSize) {
      *(swappedDataBytes) = *(dataBytes + 1);
      *(swappedDataBytes + 1) = *(dataBytes);
    } else if constexpr (4 == dataSize) {
      *(swappedDataBytes + 0) = *(dataBytes + 3);
      *(swappedDataBytes + 1) = *(dataBytes + 2);
      *(swappedDataBytes + 2) = *(dataBytes + 1);
      *(swappedDataBytes + 3) = *(dataBytes + 0);
    } else if constexpr (8 == dataSize) {
      *(swappedDataBytes + 0) = *(dataBytes + 7);
      *(swappedDataBytes + 1) = *(dataBytes + 6);
      *(swappedDataBytes + 2) = *(dataBytes + 5);
      *(swappedDataBytes + 3) = *(dataBytes + 4);
      *(swappedDataBytes + 4) = *(dataBytes + 3);
      *(swappedDataBytes + 5) = *(dataBytes + 2);
      *(swappedDataBytes + 6) = *(dataBytes + 1);
      *(swappedDataBytes + 7) = *(dataBytes + 0);
    } else {
      static_assert(true, "Size of data is unhandled");
    }
    return swappedData;
  }

  template<typename T>
  auto swapEndianess(const T &paValue) -> typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_MAGNITUDE, T>, T> {
    const typename T::TValueType data = static_cast<typename T::TValueType>(paValue);
    typename T::TValueType swappedData = swapSimpleDataHelper(data);
    return T(swappedData);
  }

  template<typename T>
  auto swapEndianess(const T &paValue) -> typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_BIT, T>, T> {
    const typename T::TValueType data = static_cast<typename T::TValueType>(paValue);
    typename T::TValueType swappedData = swapSimpleDataHelper(data);
    return T(swappedData);
  }

  template<typename T>
  auto swapEndianess(const T &paValue) -> typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_CHAR, T>, T> {
    const typename T::TValueType data = static_cast<typename T::TValueType>(paValue);
    typename T::TValueType swappedData = swapSimpleDataHelper(data);
    return T(swappedData);
  }

  template<typename T>
  auto swapEndianess(const T &paValue) -> typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, T> {
    const typename T::TValueType data = static_cast<typename T::TValueType>(paValue);
    typename T::TValueType swappedData = swapSimpleDataHelper(data);
    return T(swappedData);
  }

  template<typename T>
  auto swapEndianess(const T &paValue) -> typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_STRING, T>, T> {
    if constexpr (std::is_same_v<CIEC_STRING, T>) {
      return T(paValue);
    } else if constexpr (std::is_same_v<CIEC_WSTRING, T>) {
      T reversed(paValue);
      const TForteUInt16 length = reversed.length();
      for (TForteUInt16 i = 0; i < length; i += sizeof(typename T::TSymbolType)) {
        *(reversed.getValue() + i) = *(paValue.getValue() + (i + 1));
        *(reversed.getValue() + (i + 1)) = *(paValue.getValue() + i);
      }
      return reversed;
    } else {
      static_assert(true, "Unhandled/unknown subtype of CIEC_ANY_STRING");
    }
  }

  void swapEndianessInplace(CIEC_ARRAY &paValue);

  void swapEndianessInplace(CIEC_STRUCT &paValue);

  template<typename T>
  auto swapEndianess(const T &paValue) -> typename std::enable_if_t<std::is_base_of_v<CIEC_STRUCT, T>, T> {
    T reversed(paValue);
    swapEndianessInplace(reversed);
    return reversed;
  }

  template<template<typename, intmax_t, intmax_t> typename T, typename U, intmax_t lowerBound, intmax_t upperBound>
  auto swapEndianess(const T<U, lowerBound, upperBound> &paValue) -> typename std::enable_if_t<
      std::is_base_of_v<CIEC_ARRAY_FIXED<U, lowerBound, upperBound>, T<U, lowerBound, upperBound>>,
      T<U, lowerBound, upperBound>> {
    T<U, lowerBound, upperBound> reversed(paValue);
    for (auto iter = reversed.begin(); iter != reversed.end(); iter++) {
      *iter = swapEndianess(*iter);
    }
    return reversed;
  }

  template<template<typename> typename T, typename U>
  auto swapEndianess(const T<U> &paValue) ->
      typename std::enable_if_t<std::is_base_of_v<CIEC_ARRAY_VARIABLE<U>, T<U>>, T<U>> {
    T<U> reversed(paValue);
    for (auto iter = reversed.begin(); iter != reversed.end(); iter++) {
      *iter = swapEndianess(*iter);
    }
    return reversed;
  }

} // namespace forte
