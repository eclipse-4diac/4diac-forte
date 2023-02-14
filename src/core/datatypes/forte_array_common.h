/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#pragma once

#include <stddef.h>
#include <inttypes.h>
#include <initializer_list>

#include "forte_any_derived.h"
#include "forte_any_int.h"

/** \brief A common supertype for all CIEC_ARRAY variants, providing the minimal interface an array must provide
 */
template <typename T>
class CIEC_ARRAY_COMMON : public CIEC_ANY_DERIVED
{
public:
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = pointer;
  using const_iterator = const_pointer;

  [[nodiscard]] virtual intmax_t getLowerBound() const = 0;

  [[nodiscard]] virtual intmax_t getUpperBound() const = 0;

  [[nodiscard]] virtual size_t size() const = 0;

  [[nodiscard]] virtual CIEC_ANY::EDataTypeID getElementDataTypeID() const = 0;

  virtual CIEC_ARRAY_COMMON &operator=(std::initializer_list<T> paSource) = 0;
  virtual CIEC_ARRAY_COMMON &operator=(const CIEC_ARRAY_COMMON<T> &paSource) = 0;

  [[nodiscard]] virtual reference at(intmax_t index) = 0;

  [[nodiscard]] virtual reference operator[](intmax_t index) = 0;

  [[nodiscard]] virtual const_reference at(intmax_t index) const = 0;

  [[nodiscard]] virtual const_reference operator[](intmax_t index) const = 0;

  [[nodiscard]] reference at(const CIEC_ANY_INT &index) {
    const intmax_t indexValue = index.getSignedValue();
    return at(indexValue);
  }

  [[nodiscard]] reference operator[](const CIEC_ANY_INT &index) {
    const intmax_t indexValue = index.getSignedValue();
    return operator[](indexValue);
  }

  [[nodiscard]] const_reference at(const CIEC_ANY_INT &index) const {
    const intmax_t indexValue = index.getSignedValue();
    return at(indexValue);
  }

  [[nodiscard]] const_reference operator[](const CIEC_ANY_INT &index) const {
    const intmax_t indexValue = index.getSignedValue();
    return operator[](indexValue);
  }

  [[nodiscard]] virtual iterator begin() = 0;

  [[nodiscard]] virtual iterator end() = 0;

  [[nodiscard]] virtual const_iterator cbegin() const = 0;

  [[nodiscard]] virtual const_iterator cend() const = 0;

  [[nodiscard]] EDataTypeID getDataTypeID() const final {
    return CIEC_ANY::e_ARRAY;
  }

  ~CIEC_ARRAY_COMMON() override = default;

  protected:
    CIEC_ARRAY_COMMON() = default;
};