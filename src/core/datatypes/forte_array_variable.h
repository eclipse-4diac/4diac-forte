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
#include <vector>

template<typename T>
class CIEC_ARRAY_VARIABLE : public CIEC_ARRAY_COMMON<T> {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    using CIEC_ARRAY::at;
    using CIEC_ARRAY::operator[];
    using CIEC_ARRAY::operator=;

    using CIEC_ARRAY_COMMON<T>::at;
    using CIEC_ARRAY_COMMON<T>::operator[];
    using CIEC_ARRAY_COMMON<T>::operator=;

    /**
     * @brief Construct an array based on lower and upper bound
     * @param paLowerBound The lower bound
     * @param paUpperBound The upper bound
     */
    CIEC_ARRAY_VARIABLE(intmax_t paLowerBound, intmax_t paUpperBound)
            : cmLowerBound(paLowerBound), cmUpperBound(paUpperBound),
              cmSize(static_cast<size_t>(paUpperBound - paLowerBound + 1)), data(cmSize) {
    }

    /**
     * @brief Construct an array from an initializer list
     * @param init The initializer list
     */
    CIEC_ARRAY_VARIABLE(std::initializer_list<T> init)
            : cmLowerBound(0), cmUpperBound(init.size() - 1), cmSize(init.size()), data(init) {
    }

    /**
     * @brief Copy constructor
     * @param paSource The original array
     */
    CIEC_ARRAY_VARIABLE(const CIEC_ARRAY_VARIABLE &paSource)
            : cmLowerBound(paSource.cmLowerBound), cmUpperBound(paSource.cmUpperBound), cmSize(paSource.cmSize),
              data(paSource.data) {
    }

    /**
     * @brief Move constructor
     * @param paSource The original array
     */
    CIEC_ARRAY_VARIABLE(CIEC_ARRAY_VARIABLE &&paSource)
            : cmLowerBound(paSource.cmLowerBound), cmUpperBound(paSource.cmUpperBound), cmSize(paSource.cmSize),
              data(std::move(paSource.data)) {
    }

    /**
     * @brief Copy constructor from common array
     * @tparam U The original element type
     * @param paSource The original array
     */
    CIEC_ARRAY_VARIABLE(const CIEC_ARRAY &paSource)
            : cmLowerBound(paSource.getLowerBound()), cmUpperBound(paSource.getUpperBound()), cmSize(paSource.size()),
              data(paSource.size()) {
      assignDynamic(paSource, paSource.getLowerBound(), paSource.getUpperBound());
    }

    /**
     * @brief Copy constructor from common array with different type
     * @tparam U The original element type
     * @param paSource The original array
     */
    template<typename U>
    CIEC_ARRAY_VARIABLE(const CIEC_ARRAY_COMMON<U> &paSource)
            : cmLowerBound(paSource.getLowerBound()), cmUpperBound(paSource.getUpperBound()), cmSize(paSource.size()),
              data(paSource.size()) {
      assignDynamic(paSource, paSource.getLowerBound(), paSource.getUpperBound());
    }

    /**
     * @brief Copy constructor from variable array with different type
     * @tparam U The original element type
     * @param paSource The original array
     */
    template<typename U, std::enable_if_t<std::is_assignable_v<T &, const U &>, bool> = true>
    CIEC_ARRAY_VARIABLE(const CIEC_ARRAY_VARIABLE<U> &paSource)
            : cmLowerBound(paSource.cmLowerBound), cmUpperBound(paSource.cmUpperBound), cmSize(paSource.cmSize),
              data(paSource.begin(), paSource.end()) {
    }

    /**
     * @brief Copy constructor from fixed array with different type
     * @tparam U The original element type
     * @tparam sourceLowerBound The original lower bound
     * @tparam sourceUpperBound The original upper bound
     * @param paSource The original array
     */
    template<typename U, intmax_t sourceLowerBound, intmax_t sourceUpperBound,
            std::enable_if_t<std::is_assignable_v<T &, const U &>, bool> = true>
    CIEC_ARRAY_VARIABLE(const CIEC_ARRAY_FIXED<U, sourceLowerBound, sourceUpperBound> &paSource)
            : cmLowerBound(sourceLowerBound), cmUpperBound(sourceUpperBound), cmSize(paSource.size()),
              data(paSource.begin(), paSource.end()) {
    }

    /**
     * @brief Copy assignment operator
     * @param paSource The original array
     * @return The assigned array
     */
    CIEC_ARRAY_VARIABLE &operator=(const CIEC_ARRAY_VARIABLE &paSource) {
      assign(paSource, paSource.cmLowerBound, paSource.cmUpperBound);
      return *this;
    }

    /**
     * @brief Move assignment operator
     * @param paSource The original array
     * @return The assigned array
     */
    CIEC_ARRAY_VARIABLE &operator=(CIEC_ARRAY_VARIABLE &&paSource) {
      assignMove(std::forward<CIEC_ARRAY_VARIABLE>(paSource), paSource.cmLowerBound, paSource.cmUpperBound);
      return *this;
    }

    /**
     * @brief Copy assignment operator from variable array
     * @tparam U The original element type
     * @param paSource The original array
     * @return The assigned array
     */
    template<typename U, std::enable_if_t<std::is_assignable_v<T &, const U &>, bool> = true>
    CIEC_ARRAY_VARIABLE &operator=(const CIEC_ARRAY_VARIABLE<U> &paSource) {
      assign(paSource, paSource.cmLowerBound, paSource.cmUpperBound);
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
    CIEC_ARRAY_VARIABLE &operator=(const CIEC_ARRAY_FIXED<U, sourceLowerBound, sourceUpperBound> &paSource) {
      assign(paSource, sourceLowerBound, sourceUpperBound);
      return *this;
    }

    [[nodiscard]] size_t getSizeof() const override {
      return sizeof(CIEC_ARRAY_VARIABLE<T>);
    }

    [[nodiscard]] CIEC_ANY *clone(TForteByte *paDataBuf) const override {
      return (nullptr != paDataBuf) ? new(paDataBuf) CIEC_ARRAY_VARIABLE<T>(*this) : new CIEC_ARRAY_VARIABLE<T>(*this);
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

    [[nodiscard]] constexpr intmax_t getLowerBound() const override {
      return cmLowerBound;
    }

    [[nodiscard]] constexpr intmax_t getUpperBound() const override {
      return cmUpperBound;
    }

    [[nodiscard]] constexpr size_t size() const override {
      return static_cast<size_t>(cmUpperBound - cmLowerBound + 1); // e.g., from 0 to 0 is still 1 element
    }

    [[nodiscard]] iterator begin() noexcept {
      return data.begin();
    }

    [[nodiscard]] iterator end() noexcept {
      return data.end();
    }

    [[nodiscard]] const_iterator begin() const noexcept {
      return data.begin();
    }

    [[nodiscard]] const_iterator end() const noexcept {
      return data.end();
    }

    [[nodiscard]] const_iterator cbegin() const noexcept {
      return data.cbegin();
    }

    [[nodiscard]] const_iterator cend() const noexcept {
      return data.cend();
    }

    [[nodiscard]] CIEC_ANY::EDataTypeID getElementDataTypeID() const override {
      return data[0].getDataTypeID();
    }

    [[nodiscard]] CStringDictionary::TStringId getElementTypeNameID() const override {
      return data[0].getTypeNameID();
    }

    [[nodiscard]] int fromString(const char *) override {
      DEVLOG_ERROR("Parsing variable-size array from string is not supported\n");
      return -1; // not supported
    }

    ~CIEC_ARRAY_VARIABLE() = default;

private:
    template<typename U>
    inline void assign(const U &paArray, intmax_t sourceLowerBound, intmax_t sourceUpperBound) {
      intmax_t begin = std::max(cmLowerBound, sourceLowerBound);
      intmax_t end = std::min(cmUpperBound, sourceUpperBound);
      for (intmax_t i = begin; i <= end; ++i) {
        (*this)[i] = paArray[i];
      }
    }

    template<typename U>
    inline void assignMove(U &&paArray, intmax_t sourceLowerBound, intmax_t sourceUpperBound) {
      intmax_t begin = std::max(cmLowerBound, sourceLowerBound);
      intmax_t end = std::min(cmUpperBound, sourceUpperBound);
      for (intmax_t i = begin; i <= end; ++i) {
        (*this)[i] = std::move(paArray[i]);
      }
    }

    template<typename U>
    inline void assignDynamic(const U &paArray, intmax_t sourceLowerBound, intmax_t sourceUpperBound) {
      if(paArray.size()) { // check if initialized
        intmax_t begin = std::max(cmLowerBound, sourceLowerBound);
        intmax_t end = std::min(cmUpperBound, sourceUpperBound);
        for (intmax_t i = begin; i <= end; ++i) {
          (*this)[i].setValue(paArray[i]);
        }
      }
    }

    [[nodiscard]] constexpr size_t getDataArrayIndex(intmax_t paSTIndex) const {
      return static_cast<size_t>(paSTIndex - cmLowerBound);
    }

    const intmax_t cmLowerBound;
    const intmax_t cmUpperBound;
    const size_t cmSize;
    std::vector<T> data;
};

static_assert(std::is_copy_constructible_v<CIEC_ARRAY_VARIABLE<CIEC_ANY>>);
static_assert(std::is_move_constructible_v<CIEC_ARRAY_VARIABLE<CIEC_ANY>>);
static_assert(std::is_copy_assignable_v<CIEC_ARRAY_VARIABLE<CIEC_ANY>>);
static_assert(std::is_move_assignable_v<CIEC_ARRAY_VARIABLE<CIEC_ANY>>);
static_assert(std::is_destructible_v<CIEC_ARRAY_VARIABLE<CIEC_ANY>>);
static_assert(std::is_swappable_v<CIEC_ARRAY_VARIABLE<CIEC_ANY>>);

static_assert(std::is_constructible_v<CIEC_ARRAY_VARIABLE<CIEC_ULINT>, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY_VARIABLE<CIEC_ULINT>, CIEC_ARRAY_VARIABLE<CIEC_UINT> &&>);

static_assert(std::is_assignable_v<CIEC_ARRAY_VARIABLE<CIEC_ULINT>, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY_VARIABLE<CIEC_ULINT>, CIEC_ARRAY_VARIABLE<CIEC_UINT> &&>);

static_assert(std::is_constructible_v<CIEC_ARRAY_VARIABLE<CIEC_ULINT>, const CIEC_ARRAY &>);
static_assert(std::is_constructible_v<CIEC_ARRAY_VARIABLE<CIEC_ULINT>, const CIEC_ARRAY_COMMON<CIEC_UINT> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY_VARIABLE<CIEC_ULINT>, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);

static_assert(std::is_assignable_v<CIEC_ARRAY_VARIABLE<CIEC_ULINT>, const CIEC_ARRAY &>);
static_assert(std::is_assignable_v<CIEC_ARRAY_VARIABLE<CIEC_ULINT>, const CIEC_ARRAY_COMMON<CIEC_UINT> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY_VARIABLE<CIEC_ULINT>, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);

