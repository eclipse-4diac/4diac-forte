/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *               2022 Martin Erich Jobst
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
 *******************************************************************************/
#pragma once

#include "forte_array_common.h"
#include <array>
#include <devlog.h>

template <typename T, intmax_t lowerBound, intmax_t upperBound>
class CIEC_ARRAY_FIXED : public CIEC_ARRAY_COMMON<T>
{
public:
    using difference_type = std::ptrdiff_t;
    using value_type = typename CIEC_ARRAY_COMMON<T>::value_type;
    using pointer = typename CIEC_ARRAY_COMMON<T>::pointer;
    using const_pointer = typename CIEC_ARRAY_COMMON<T>::const_pointer;
    using reference = typename CIEC_ARRAY_COMMON<T>::reference;
    using const_reference = typename CIEC_ARRAY_COMMON<T>::const_reference;
    using iterator = typename CIEC_ARRAY_COMMON<T>::iterator;
    using const_iterator = typename CIEC_ARRAY_COMMON<T>::const_iterator;

    CIEC_ARRAY_FIXED() = default;

    /** @brief constructor for initializer list */
    CIEC_ARRAY_FIXED(std::initializer_list<T> init) {
      auto copyEnd = init.end();
      if (init.size() > size()) {
        copyEnd -= init.size() - size();
        DEVLOG_WARNING("Initializer list longer than array\n");
      }
      std::copy(init.begin(), copyEnd, data.begin());
    }

    CIEC_ARRAY_FIXED(const CIEC_ARRAY_FIXED<T, lowerBound, upperBound> &paSource) : data(paSource.data) {

    }

    /** @brief copy constructor for same array type */
    template <typename U, intmax_t sourceLowerBound, intmax_t sourceUpperBound>
    CIEC_ARRAY_FIXED(const CIEC_ARRAY_FIXED<U, sourceLowerBound, sourceUpperBound> &paSource)
    {
        /* If lowerBound of the data source is within the range of the destination,
         * then the sources lowerBound must be smaller than the destination's upperBound.
         * and its upperBound higher than the destination's lowerBound
         */
        if constexpr ((sourceLowerBound <= upperBound) && (sourceUpperBound >= lowerBound)) {
          constexpr intmax_t lowerBoundOffset = lowerBound - sourceLowerBound;
          constexpr intmax_t upperBoundOffset = upperBound - sourceUpperBound;

          auto sourceIteratorBegin = paSource.begin();
          auto sourceIteratorEnd = paSource.end();
          auto targetIteratorBegin = begin();

          // Target lowerBound is a bigger number than the source, so all elements below
          // the target lowerbound cannot be copied, so shift start to first element which will be copied
          if constexpr (lowerBound >= sourceLowerBound) {
            sourceIteratorBegin += lowerBoundOffset;
          }
          else {
            targetIteratorBegin -= lowerBoundOffset; // lowerBoundOffset is negative here
          }

          if constexpr (upperBound < sourceUpperBound) {
            sourceIteratorEnd += upperBoundOffset; // upperBoundOffset is negative here
          }
          std::copy(sourceIteratorBegin, sourceIteratorEnd, targetIteratorBegin);
        }
    }

    CIEC_ARRAY_FIXED(const CIEC_ARRAY_COMMON<T> &paSource) {
      const intmax_t sourceLowerBound = paSource.getLowerBound();
      const intmax_t sourceUpperBound = paSource.getUpperBound();
      const intmax_t lowerBoundOffset = lowerBound - sourceLowerBound;
      const intmax_t upperBoundOffset = upperBound - sourceUpperBound;

      auto sourceIteratorBegin = paSource.begin();
      auto sourceIteratorEnd = paSource.end();
      auto targetIteratorBegin = begin();

      if ((sourceLowerBound <= upperBound) && (sourceUpperBound >= lowerBound)) {
      // Target lowerBound is a bigger number than the source, so all elements below the target lowerbound cannot be copied, so shift start to first element which will be copied
        if (lowerBound >= sourceLowerBound) {
          sourceIteratorBegin += lowerBoundOffset;
        } else {
          targetIteratorBegin -= lowerBoundOffset; // lowerBoundOffset is negative here
        }
  
        if (upperBound < sourceUpperBound) {
          sourceIteratorEnd += upperBoundOffset; // upperBoundOffset is negative here
        }
  
        for (auto element = sourceIteratorBegin; element != sourceIteratorEnd; ++element, ++targetIteratorBegin) {
          *targetIteratorBegin = *static_cast<const T *>(element);
        }
      }
    }

    CIEC_ARRAY_FIXED &operator=(const CIEC_ARRAY_COMMON<T> &paSource) override {
      const intmax_t sourceLowerBound = paSource.getLowerBound();
      const intmax_t sourceUpperBound = paSource.getUpperBound();
      const intmax_t lowerBoundOffset = lowerBound - sourceLowerBound;
      const intmax_t upperBoundOffset = upperBound - sourceUpperBound;

      auto sourceIteratorBegin = paSource.begin();
      auto sourceIteratorEnd = paSource.end();
      auto targetIteratorBegin = begin();

      if ((sourceLowerBound <= upperBound) && (sourceUpperBound >= lowerBound)) {
      // Target lowerBound is a bigger number than the source, so all elements below the target lowerbound cannot be copied, so shift start to first element which will be copied
        if (lowerBound >= sourceLowerBound) {
          sourceIteratorBegin += lowerBoundOffset;
        } else {
          targetIteratorBegin -= lowerBoundOffset; // lowerBoundOffset is negative here
        }
  
        if (upperBound < sourceUpperBound) {
          sourceIteratorEnd += upperBoundOffset; // upperBoundOffset is negative here
        }
  
        for (auto element = sourceIteratorBegin; element != sourceIteratorEnd; ++element, ++targetIteratorBegin) {
          *targetIteratorBegin = *static_cast<const T *>(element);
        }
      }
      return *this;
    }

    CIEC_ARRAY_FIXED &operator=(const CIEC_ARRAY_FIXED<T, lowerBound, upperBound> &paSource)
    {
        data = paSource.data;
        return *this;
    }

    CIEC_ARRAY_FIXED &operator=(std::initializer_list<T> init) override {
      auto copyEnd = init.end();
      if (init.size() > size()) {
        copyEnd -= init.size() - size();
        DEVLOG_WARNING("Initializer list longer than array\n");
      }
      std::copy(init.begin(), copyEnd, data.begin());
      return *this;
    }

    template <typename U, intmax_t sourceLowerBound, intmax_t sourceUpperBound>
    CIEC_ARRAY_FIXED &operator=(const CIEC_ARRAY_FIXED<U, sourceLowerBound, sourceUpperBound> &paSource)
    {
        if constexpr ((sourceLowerBound <= upperBound) && (sourceUpperBound >= lowerBound)) {
          constexpr intmax_t lowerBoundOffset = lowerBound - sourceLowerBound;
          constexpr intmax_t upperBoundOffset = upperBound - sourceUpperBound;

          auto sourceIteratorBegin = paSource.begin();
          auto sourceIteratorEnd = paSource.end();
          auto targetIteratorBegin = begin();

          // Target lowerBound is a bigger number than the source, so all elements below
          // the target lowerbound cannot be copied, so shift start to first element which will be copied
          if constexpr (lowerBound >= sourceLowerBound) {
            sourceIteratorBegin += lowerBoundOffset;
          } else {
            targetIteratorBegin -= lowerBoundOffset; // lowerBoundOffset is negative here
          }

          if constexpr (upperBound < sourceUpperBound) {
            sourceIteratorEnd += upperBoundOffset; // upperBoundOffset is negative here
          }

          std::copy(sourceIteratorBegin, sourceIteratorEnd, targetIteratorBegin);
        }
        return *this;
    }

    // Copy plain old CIEC_ARRAY
    template <typename U>
    CIEC_ARRAY_FIXED(const CIEC_ARRAY<U> &paSource) {
      const intmax_t sourceLowerBound = paSource.getLowerBound();
      const intmax_t sourceUpperBound = paSource.getUpperBound();
      const intmax_t lowerBoundOffset = lowerBound - sourceLowerBound;
      const intmax_t upperBoundOffset = upperBound - sourceUpperBound;

      auto sourceIteratorBegin = paSource.begin();
      auto sourceIteratorEnd = paSource.end();
      auto targetIteratorBegin = begin();

      if ((sourceLowerBound <= upperBound) && (sourceUpperBound >= lowerBound)) {
        // Target lowerBound is a bigger number than the source, so all elements below the target lowerbound cannot be copied, so shift start to first element which will be copied
        if (lowerBound >= sourceLowerBound) {
          sourceIteratorBegin += lowerBoundOffset;
        } else {
          targetIteratorBegin -= lowerBoundOffset; // lowerBoundOffset is negative here
        }
  
        if (upperBound < sourceUpperBound) {
          sourceIteratorEnd += upperBoundOffset; // upperBoundOffset is negative here
        }
  
        for (auto element = sourceIteratorBegin; element != sourceIteratorEnd; ++element, ++targetIteratorBegin) {
          *targetIteratorBegin = *static_cast<const T *>(element);
        }
      }
    }

    [[nodiscard]] intmax_t getLowerBound() const override
    {
        return lowerBound;
    }

    [[nodiscard]] intmax_t getUpperBound() const override
    {
        return upperBound;
    }

    [[nodiscard]] size_t size() const override
    {
        return data.size();
    }

    [[nodiscard]] reference at(intmax_t index) override
    {
        return data.at(getDataArrayIndex(index));
    }

    // PLC-like systems always want range checks
    [[nodiscard]] reference operator[](intmax_t index) override
    {
        return data[getDataArrayIndex(index)];
    }

    [[nodiscard]] const_reference at(intmax_t index) const override
    {
        return data.at(getDataArrayIndex(index));
    }

    // PLC-like systems always want range checks
    [[nodiscard]] const_reference operator[](intmax_t index) const override
    {
        return data[getDataArrayIndex(index)];
    }

    constexpr static const size_t cmSize = upperBound - lowerBound + 1;

    [[nodiscard]] constexpr iterator begin() override
    {
        return data.begin();
    }

    [[nodiscard]] constexpr iterator end() override
    {
        return data.end();
    }

    [[nodiscard]] constexpr const_iterator begin() const override
    {
        return data.begin();
    }

    [[nodiscard]] constexpr const_iterator end() const override
    {
        return data.end();
    }

    [[nodiscard]] CIEC_ANY::EDataTypeID getElementDataTypeID() const override
    {
        return data[0].getDataTypeID();
    }

    ~CIEC_ARRAY_FIXED() = default;

private:
    [[nodiscard]] size_t getDataArrayIndex(intmax_t paSTIndex) const
    {
        return paSTIndex - lowerBound;
    }

    std::array<T, cmSize> data;
};