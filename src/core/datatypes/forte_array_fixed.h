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
 *      - refactored array type structure
 *******************************************************************************/
#pragma once

#include "forte_array_common.h"
#include <array>
#include <devlog.h>

#include "forte_ulint.h"

template<typename T>
class CIEC_ARRAY_VARIABLE;

template<typename T, intmax_t lowerBound, intmax_t upperBound>
class CIEC_ARRAY_FIXED : public CIEC_ARRAY_COMMON<T> {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = typename CIEC_ARRAY_COMMON<T>::value_type;
    using pointer = typename CIEC_ARRAY_COMMON<T>::pointer;
    using const_pointer = typename CIEC_ARRAY_COMMON<T>::const_pointer;
    using reference = typename CIEC_ARRAY_COMMON<T>::reference;
    using const_reference = typename CIEC_ARRAY_COMMON<T>::const_reference;
    using iterator = typename std::array<T, 0>::iterator;
    using const_iterator = typename std::array<T, 0>::const_iterator;

    using CIEC_ARRAY::at;
    using CIEC_ARRAY::operator[];
    using CIEC_ARRAY::operator=;

    using CIEC_ARRAY_COMMON<T>::at;
    using CIEC_ARRAY_COMMON<T>::operator[];
    using CIEC_ARRAY_COMMON<T>::operator=;

    /**
     * @brief Default constructor
     */
    CIEC_ARRAY_FIXED() = default;

    /**
     * @brief Construct an array from an initializer list
     * @param init The initializer list
     */
    CIEC_ARRAY_FIXED(std::initializer_list<T> init) {
      std::copy_n(init.begin(), std::min(init.size(), cmSize), data.begin());
    }

    /**
     * @brief Copy constructor
     * @param paSource The original array
     */
    CIEC_ARRAY_FIXED(const CIEC_ARRAY_FIXED &paSource) : data(paSource.data) {}

    /**
     * @brief Move constructor
     * @param paSource The original array
     */
    CIEC_ARRAY_FIXED(CIEC_ARRAY_FIXED &&paSource) : data(std::move(paSource.data)) {}

    /**
     * Copy constructor from common array
     * @tparam U The original element type
     * @param paSource The original array
     */
    CIEC_ARRAY_FIXED(const CIEC_ARRAY &paSource) {
      assignDynamic(paSource, paSource.getLowerBound(), paSource.getUpperBound());
    }

    /**
     * Copy constructor from common array
     * @tparam U The original element type
     * @param paSource The original array
     */
    template<typename U>
    CIEC_ARRAY_FIXED(const CIEC_ARRAY_COMMON<U> &paSource) {
      assignDynamic(paSource, paSource.getLowerBound(), paSource.getUpperBound());
    }

    /**
     * Copy constructor from variable array
     * @tparam U The original element type
     * @param paSource The original array
     */
    template<typename U, std::enable_if_t<std::is_assignable_v<T &, const U &>, bool> = true>
    CIEC_ARRAY_FIXED(const CIEC_ARRAY_VARIABLE<U> &paSource) {
      assign(paSource, paSource.getLowerBound(), paSource.getUpperBound());
    }

    /**
     * Copy constructor from fixed array with different type
     * @tparam U The original element type
     * @tparam sourceLowerBound The original lower bound
     * @tparam sourceUpperBound The original upper bound
     * @param paSource The original array
     */
    template<typename U, intmax_t sourceLowerBound, intmax_t sourceUpperBound,
            std::enable_if_t<std::is_assignable_v<T &, const U &>, bool> = true>
    CIEC_ARRAY_FIXED(const CIEC_ARRAY_FIXED<U, sourceLowerBound, sourceUpperBound> &paSource) {
      assign(paSource, sourceLowerBound, sourceUpperBound);
    }

    /**
     * @brief Copy assignment operator
     * @param paSource The original array
     * @return The assigned array
     */
    CIEC_ARRAY_FIXED &operator=(const CIEC_ARRAY_FIXED &paSource) {
      data = paSource.data;
      return *this;
    }

    /**
     * @brief Move assignment operator
     * @param paSource The original array
     * @return The assigned array
     */
    CIEC_ARRAY_FIXED &operator=(CIEC_ARRAY_FIXED &&paSource) {
      data = std::move(paSource.data);
      return *this;
    }

    /**
     * @brief Copy assignment operator from variable array
     * @tparam U The original element type
     * @param paSource The original array
     * @return The assigned array
     */
    template<typename U, std::enable_if_t<std::is_assignable_v<T &, const U &>, bool> = true>
    CIEC_ARRAY_FIXED &operator=(const CIEC_ARRAY_VARIABLE<U> &paSource) {
      assign(paSource, paSource.getLowerBound(), paSource.getUpperBound());
      return *this;
    }

    /**
     * @brief Copy assignment operator from fixed array
     * @tparam U The original element type
     * @tparam sourceLowerBound The original lower bound
     * @tparam sourceUpperBound The original upper bound
     * @param paSource The original array
     * @return The assigned array
     */
    template<typename U, intmax_t sourceLowerBound, intmax_t sourceUpperBound,
            std::enable_if_t<std::is_assignable_v<T &, const U &>, bool> = true>
    CIEC_ARRAY_FIXED &operator=(const CIEC_ARRAY_FIXED<U, sourceLowerBound, sourceUpperBound> &paSource) {
      assign(paSource, sourceLowerBound, sourceUpperBound);
      return *this;
    }

    [[nodiscard]] size_t getSizeof() const override {
      return sizeof(CIEC_ARRAY_FIXED<T, lowerBound, upperBound>);
    }

    [[nodiscard]] CIEC_ANY *clone(TForteByte *paDataBuf) const override {
      return (nullptr != paDataBuf) ? new(paDataBuf) CIEC_ARRAY_FIXED<T, lowerBound, upperBound>(*this)
                                    : new CIEC_ARRAY_FIXED<T, lowerBound, upperBound>(*this);
    }

    [[nodiscard]] constexpr intmax_t getLowerBound() const override {
      return lowerBound;
    }

    [[nodiscard]] constexpr intmax_t getUpperBound() const override {
      return upperBound;
    }

    [[nodiscard]] constexpr size_t size() const override {
      return data.size();
    }

    [[nodiscard]] constexpr reference at(intmax_t index) override {
      return data.at(getDataArrayIndex(index));
    }

    // PLC-like systems always want range checks
    [[nodiscard]] constexpr reference operator[](intmax_t index) override {
      return data[getDataArrayIndex(index)];
    }

    [[nodiscard]] constexpr const_reference at(intmax_t index) const override {
      return data.at(getDataArrayIndex(index));
    }

    // PLC-like systems always want range checks
    [[nodiscard]] constexpr const_reference operator[](intmax_t index) const override {
      return data[getDataArrayIndex(index)];
    }

    constexpr static const size_t cmSize = upperBound - lowerBound + 1;

    [[nodiscard]] constexpr iterator begin() {
      return data.begin();
    }

    [[nodiscard]] constexpr iterator end() {
      return data.end();
    }

    [[nodiscard]] constexpr const_iterator begin() const {
      return data.begin();
    }

    [[nodiscard]] constexpr const_iterator end() const {
      return data.end();
    }

    [[nodiscard]] constexpr const_iterator cbegin() const {
      return data.cbegin();
    }

    [[nodiscard]] constexpr const_iterator cend() const {
      return data.cend();
    }

    [[nodiscard]] CIEC_ANY::EDataTypeID getElementDataTypeID() const override {
      return data[0].getDataTypeID();
    }

    [[nodiscard]] CStringDictionary::TStringId getElementTypeNameID() const override {
      return data[0].getTypeNameID();
    }

    [[nodiscard]] int fromString(const char *paValue) override {
      int nRetVal = -1;
      const char *pcRunner = paValue;
      if ('[' == paValue[0]) {
        pcRunner++;

        value_type bufVal;
        iterator iter = begin();

        while (('\0' != *pcRunner) && (']' != *pcRunner)) {
          CIEC_ARRAY::findNextNonBlankSpace(&pcRunner);

          int valueLength = initializeFromString(iter, pcRunner, bufVal);
          if (0 > valueLength) {
            break;
          }
          pcRunner += valueLength;

          CIEC_ARRAY::findNextNonBlankSpace(&pcRunner);
          if (',' == *pcRunner) {
            pcRunner++;
          } else { //we have an error or the end bracket
            break;
          }
        }
        if (*pcRunner == ']') { //arrays have to and on a closing bracket
          nRetVal = static_cast<int>(pcRunner - paValue + 1); //+1 from the closing bracket
          // For the rest of the array size copy the default element
          std::fill(iter, end(), T());
        }
      }
      return nRetVal;
    }

    ~CIEC_ARRAY_FIXED() = default;

private:
    template<typename U>
    inline void assign(const U &paArray, intmax_t sourceLowerBound, intmax_t sourceUpperBound) {
      intmax_t begin = std::max(lowerBound, sourceLowerBound);
      intmax_t end = std::min(upperBound, sourceUpperBound);
      for (intmax_t i = begin; i <= end; ++i) {
        (*this)[i] = paArray[i];
      }
    }

    template<typename U>
    inline void assignDynamic(const U &paArray, intmax_t sourceLowerBound, intmax_t sourceUpperBound) {
      if(paArray.size()) { // check if initialized
        intmax_t begin = std::max(lowerBound, sourceLowerBound);
        intmax_t end = std::min(upperBound, sourceUpperBound);
        for (intmax_t i = begin; i <= end; ++i) {
          (*this)[i].setValue(paArray[i]);
        }
      }
    }

    [[nodiscard]] constexpr size_t getDataArrayIndex(intmax_t paSTIndex) const {
      return static_cast<size_t>(paSTIndex - lowerBound);
    }

    [[nodiscard]] int initializeFromString(iterator &paPosition, const char *paSrcString, reference paBufVal) {
      // check repeat syntax (e.g., "255(1, 2, 3)")
      CIEC_ULINT repeat;
      const char *pcRunner = paSrcString;
      iterator initialPosition = paPosition;
      int repeatLength = repeat.fromString(pcRunner);
      if (0 < repeatLength) {
        pcRunner += repeatLength;
        CIEC_ARRAY::findNextNonBlankSpace(&pcRunner);
        if ('(' == *pcRunner) {
          pcRunner++;
          while (('\0' != *pcRunner) && (')' != *pcRunner)) {
            CIEC_ARRAY::findNextNonBlankSpace(&pcRunner);

            int valueLength = initializeSimpleFromString(paPosition, pcRunner, paBufVal);
            if (0 > valueLength) {
              break;
            }
            pcRunner += valueLength;

            CIEC_ARRAY::findNextNonBlankSpace(&pcRunner);
            if (',' == *pcRunner) {
              pcRunner++;
            } else {
              //we have an error or the end parentheses
              break;
            }
          }
          if (*pcRunner == ')') { //repeat syntax elements have to and on a closing parentheses
            intmax_t repeatSequenceLength = paPosition - initialPosition;
            for (size_t rep = 1;
                 rep < repeat.getUnsignedValue() && paPosition != end(); ++rep) { // once added already
              for (intmax_t seqIndex = 0; seqIndex < repeatSequenceLength && paPosition != end(); ++seqIndex) {
                (paPosition++)->setValue(*(initialPosition + seqIndex));
              }
            }
            return static_cast<int>(pcRunner - paSrcString + 1); //+1 from the closing parentheses
          }
          return -1;
        }
      }
      return initializeSimpleFromString(paPosition, paSrcString, paBufVal);
    }

    [[nodiscard]] int initializeSimpleFromString(iterator &paPosition, const char *paSrcString, reference paBufVal) {
      if (paPosition != end()) {
        return (paPosition++)->fromString(paSrcString);
      } else {
        return paBufVal.fromString(paSrcString);
      }
    }

    std::array<T, cmSize> data;
};

static_assert(std::is_copy_constructible_v<CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0>>);
static_assert(std::is_move_constructible_v<CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0>>);
static_assert(std::is_copy_assignable_v<CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0>>);
static_assert(std::is_move_assignable_v<CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0>>);
static_assert(std::is_destructible_v<CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0>>);
static_assert(std::is_swappable_v<CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0>>);

static_assert(std::is_constructible_v<CIEC_ARRAY_FIXED<CIEC_ULINT, 0, 0>, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY_FIXED<CIEC_ULINT, 0, 0>, CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &&>);

static_assert(std::is_assignable_v<CIEC_ARRAY_FIXED<CIEC_ULINT, 0, 0>, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY_FIXED<CIEC_ULINT, 0, 0>, CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &&>);

static_assert(std::is_constructible_v<CIEC_ARRAY_FIXED<CIEC_ULINT, 0, 0>, const CIEC_ARRAY &>);
static_assert(std::is_constructible_v<CIEC_ARRAY_FIXED<CIEC_ULINT, 0, 0>, const CIEC_ARRAY_COMMON<CIEC_UINT> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY_FIXED<CIEC_ULINT, 0, 0>, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);

static_assert(std::is_assignable_v<CIEC_ARRAY_FIXED<CIEC_ULINT, 0, 0>, const CIEC_ARRAY &>);
static_assert(std::is_assignable_v<CIEC_ARRAY_FIXED<CIEC_ULINT, 0, 0>, const CIEC_ARRAY_COMMON<CIEC_UINT> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY_FIXED<CIEC_ULINT, 0, 0>, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);

