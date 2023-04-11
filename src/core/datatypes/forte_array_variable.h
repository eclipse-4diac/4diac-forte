/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *               2022 - 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians, Martin Jobst
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst
 *      - add support for data types with different size
 *******************************************************************************/
#pragma once

#include "forte_array_common.h"
#include <vector>

template <typename T>
class CIEC_ARRAY_VARIABLE : public CIEC_ARRAY_COMMON<T>
{
public:
using difference_type = std::ptrdiff_t;
using value_type = T;
using pointer = value_type *;
using const_pointer = const value_type *;
using reference = value_type &;
using const_reference = const value_type &;

using CIEC_ARRAY_COMMON<T>::operator[];

CIEC_ARRAY_VARIABLE(intmax_t paLowerBound, intmax_t paUpperBound) : cmLowerBound(paLowerBound), cmUpperBound(paUpperBound),
    cmSize(static_cast<size_t>(paUpperBound - paLowerBound + 1)), data(cmSize) {
}

CIEC_ARRAY_VARIABLE(std::initializer_list<T> init) : cmLowerBound(0), cmUpperBound(init.size() - 1), cmSize(init.size()), data(init) {
}

CIEC_ARRAY_VARIABLE(const CIEC_ARRAY_VARIABLE &paSource) : cmLowerBound(paSource.cmLowerBound), cmUpperBound(paSource.cmUpperBound), cmSize(paSource.cmSize), data(paSource.data) {
}

template <typename U, intmax_t sourceLowerBound, intmax_t sourceUpperBound>
CIEC_ARRAY_VARIABLE(const CIEC_ARRAY_FIXED<U, sourceLowerBound, sourceUpperBound> &paSource)
    : cmLowerBound(paSource.getLowerBound()), cmUpperBound(paSource.getUpperBound()), cmSize(paSource.size()), data(paSource.cbegin(), paSource.cend()) {
}

CIEC_ARRAY_VARIABLE(const CIEC_ARRAY<T> &paSource)
    : cmLowerBound(paSource.getLowerBound()), cmUpperBound(paSource.getUpperBound()), cmSize(paSource.size()) {
  for (auto element = paSource.cbegin(); element != paSource.cend(); ++element) {
      data.push_back(*static_cast<const T *>(element));
  }
}

CIEC_ARRAY_VARIABLE(const CIEC_ARRAY_TYPELIB &paSource)
    : cmLowerBound(paSource.getLowerBound()), cmUpperBound(paSource.getUpperBound()), cmSize(paSource.size()) {
  for (auto element = paSource.cbegin(); element != paSource.cend(); ++element) {
    data.push_back(*static_cast<const T *>(element));
  }
}

[[nodiscard]] size_t getSizeof() const override {
  return sizeof(CIEC_ARRAY_VARIABLE<T>);
}

[[nodiscard]] CIEC_ANY* clone(TForteByte *paDataBuf) const override {
  return (nullptr != paDataBuf) ? new (paDataBuf) CIEC_ARRAY_VARIABLE<T>(*this) : new CIEC_ARRAY_VARIABLE<T>(*this);
}

[[nodiscard]] reference at(intmax_t index) override {
  return data.at(getDataArrayIndex(index));
}

// PLC-like systems always want range checks
[[nodiscard]] reference operator[](intmax_t index) override {
  return data[getDataArrayIndex(index)];
}

[[nodiscard]] const_reference at(intmax_t index) const override {
  return data.at(getDataArrayIndex(index));
}

// PLC-like systems always want range checks
[[nodiscard]] const_reference operator[](intmax_t index) const override {
  return data[getDataArrayIndex(index)];
}

CIEC_ARRAY_VARIABLE<T> &operator=(const CIEC_ARRAY_COMMON<T> &paSource) {
  const intmax_t sourceLowerBound = paSource.getLowerBound();
  const intmax_t sourceUpperBound = paSource.getUpperBound();
  const intmax_t lowerBoundOffset = getLowerBound() - sourceLowerBound;
  const intmax_t upperBoundOffset = getUpperBound() - sourceUpperBound;
  
  auto sourceIteratorBegin = paSource.cbegin();
  auto sourceIteratorEnd = paSource.cend();
  auto targetIteratorBegin = begin();
  if ((sourceLowerBound <= getUpperBound()) && (sourceUpperBound >= getLowerBound())) {
  // Target lowerBound is a bigger number than the source, so all elements below the target lowerbound cannot be copied, so shift start to first element which will be copied
    if (getLowerBound() >= sourceLowerBound) {
      sourceIteratorBegin += lowerBoundOffset;
    } else {
      targetIteratorBegin -= lowerBoundOffset; // lowerBoundOffset is negative here
    }

    if (getUpperBound() < sourceUpperBound) {
      sourceIteratorEnd += upperBoundOffset; // upperBoundOffset is negative here
    }

    for (auto element = sourceIteratorBegin; element != sourceIteratorEnd; ++element, ++targetIteratorBegin) {
      *targetIteratorBegin = *static_cast<const T *>(element);
    }
  }
  return *this;
}

CIEC_ARRAY_VARIABLE<T> &operator=(const std::initializer_list<T> paSource) {
  data = paSource;
  return *this;
}

CIEC_ARRAY_VARIABLE<T> &operator=(const CIEC_ARRAY_VARIABLE<T> &paSource) {
  data = paSource.data;
  return *this;
}

[[nodiscard]] intmax_t getLowerBound() const override {
  return cmLowerBound;
}

[[nodiscard]] intmax_t getUpperBound() const override {
  return cmUpperBound;
}

[[nodiscard]] size_t size() const override {
  return cmUpperBound - cmLowerBound + 1; // e.g., from 0 to 0 is still 1 element
}

[[nodiscard]] constexpr typename CIEC_ARRAY_COMMON<T>::iterator begin() override {
  return data.data();
}

[[nodiscard]] constexpr typename CIEC_ARRAY_COMMON<T>::iterator end() override {
  return data.data() + data.size();
}

[[nodiscard]] constexpr typename CIEC_ARRAY_COMMON<T>::const_iterator cbegin() const override {
  return data.data();
}

[[nodiscard]] constexpr typename CIEC_ARRAY_COMMON<T>::const_iterator cend() const override {
  return data.data() + data.size();
}

[[nodiscard]] CIEC_ANY::EDataTypeID getElementDataTypeID() const override {
  return data[0].getDataTypeID();
}
~CIEC_ARRAY_VARIABLE() = default;

private:
  [[nodiscard]] size_t getDataArrayIndex(intmax_t paSTIndex) const {
     return paSTIndex - cmLowerBound;
  }

   const intmax_t cmLowerBound;
   const intmax_t cmUpperBound;
   const size_t cmSize;
   std::vector<T> data;
};