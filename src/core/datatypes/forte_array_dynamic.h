/*******************************************************************************
 * Copyright (c) 2007 - 2013 ACIN, nxtControl GmbH, fortiss GmbH
 *               2022 Primetals Technologies Austria GmbH
 *               2022 - 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Stansilav Meduna, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians, Martin Jobst - added ctors for copying one type
 *      to another and iterators
 *    Martin Jobst - add support for repeat syntax
 *                 - add equals function
 *                 - collapse identical consecutive elements
 *                 - add support for data types with different size
 *                 - refactored for elements with dynamic size
 *                 - refactored for lower and upper bounds
 *                 - refactored array type structure
 *******************************************************************************/

#pragma once

#include "forte_array.h"
#include <stdexcept>
#include <devlog.h>
#include <utility>

template<typename T>
class CIEC_ARRAY_COMMON;

template<typename T, intmax_t lowerBound, intmax_t upperBound>
class CIEC_ARRAY_FIXED;

template<typename T>
class CIEC_ARRAY_VARIABLE;

/*!\ingroup COREDTS CIEC_ARRAY_DYNAMIC represents the array data type according to IEC 61131.
 */
class CIEC_ARRAY_DYNAMIC : public CIEC_ARRAY {
DECLARE_FIRMWARE_DATATYPE(ARRAY_DYNAMIC)
public:
    using difference_type = std::ptrdiff_t;
    using value_type = typename CIEC_ARRAY::value_type;
    using pointer = typename CIEC_ARRAY::pointer;
    using const_pointer = typename CIEC_ARRAY::const_pointer;
    using reference = typename CIEC_ARRAY::reference;
    using const_reference = typename CIEC_ARRAY::const_reference;

    using CIEC_ARRAY::at;
    using CIEC_ARRAY::operator[];
    using CIEC_ARRAY::operator=;

    class iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = typename CIEC_ARRAY_DYNAMIC::value_type;
        using pointer = typename CIEC_ARRAY_DYNAMIC::pointer;
        using reference = typename CIEC_ARRAY_DYNAMIC::reference;
        using iterator_category = std::random_access_iterator_tag;

        constexpr iterator(size_t paElementSize, void *paData) : mElementSize(paElementSize), mData(paData) {}

        reference operator*() const {
          return *reinterpret_cast<pointer>(mData);
        }

        pointer operator->() const {
          return reinterpret_cast<pointer>(mData);
        }

        reference operator[](difference_type paValue) const {
          return *reinterpret_cast<pointer>(reinterpret_cast<TForteByte *>(mData) + mElementSize * paValue);
        }

        iterator &operator++() {
          mData = reinterpret_cast<TForteByte *>(mData) + mElementSize;
          return *this;
        }

        iterator &operator--() {
          mData = reinterpret_cast<TForteByte *>(mData) - mElementSize;
          return *this;
        }

        iterator operator++(int) {
          iterator temp = *this;
          ++(*this);
          return temp;
        }

        iterator operator--(int) {
          iterator temp = *this;
          --(*this);
          return temp;
        }

        iterator &operator+=(difference_type paValue) {
          mData = reinterpret_cast<TForteByte *>(mData) + mElementSize * paValue;
          return *this;
        }

        iterator &operator-=(difference_type paValue) {
          mData = reinterpret_cast<TForteByte *>(mData) - static_cast<difference_type >(mElementSize) * paValue;
          return *this;
        }

        friend iterator operator+(const iterator &paIter, difference_type paValue) {
          return iterator(paIter.mElementSize,
                          reinterpret_cast<TForteByte *>(paIter.mData) +
                          static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend iterator operator+(difference_type paValue, const iterator &paIter) {
          return iterator(paIter.mElementSize,
                          reinterpret_cast<TForteByte *>(paIter.mData) +
                          static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend iterator operator-(const iterator &paIter, difference_type paValue) {
          return iterator(paIter.mElementSize,
                          reinterpret_cast<TForteByte *>(paIter.mData) -
                          static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend difference_type operator-(const iterator &paIter, const iterator &paOther) {
          return (reinterpret_cast<TForteByte *>(paIter.mData) - reinterpret_cast<TForteByte *>(paOther.mData)) /
                 static_cast<difference_type>(paIter.mElementSize);
        }

        friend constexpr bool operator==(const iterator &paValue, const iterator &paOther) {
          return paValue.mData == paOther.mData;
        }

        friend constexpr bool operator!=(const iterator &paValue, const iterator &paOther) {
          return paValue.mData != paOther.mData;
        }

        friend constexpr bool operator<(const iterator &paValue, const iterator &paOther) {
          return paValue.mData < paOther.mData;
        }

        friend constexpr bool operator<=(const iterator &paValue, const iterator &paOther) {
          return paValue.mData <= paOther.mData;
        }

        friend constexpr bool operator>(const iterator &paValue, const iterator &paOther) {
          return paValue.mData > paOther.mData;
        }

        friend constexpr bool operator>=(const iterator &paValue, const iterator &paOther) {
          return paValue.mData >= paOther.mData;
        }

    private:
        size_t mElementSize;
        void *mData;
    };

    static_assert(std::is_copy_constructible_v<iterator>);
    static_assert(std::is_trivially_copy_constructible_v<iterator>);
    static_assert(std::is_copy_assignable_v<iterator>);
    static_assert(std::is_trivially_copy_assignable_v<iterator>);
    static_assert(std::is_destructible_v<iterator>);
    static_assert(std::is_trivially_destructible_v<iterator>);
    static_assert(std::is_swappable_v<iterator>);

    class const_iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = typename CIEC_ARRAY_DYNAMIC::value_type;
        using pointer = typename CIEC_ARRAY_DYNAMIC::const_pointer;
        using reference = typename CIEC_ARRAY_DYNAMIC::const_reference;
        using iterator_category = std::random_access_iterator_tag;

        constexpr const_iterator(size_t paElementSize, const void *paData) : mElementSize(paElementSize), mData(paData) {}

        reference operator*() const {
          return *reinterpret_cast<pointer>(mData);
        }

        pointer operator->() const {
          return reinterpret_cast<pointer>(mData);
        }

        reference operator[](difference_type paValue) const {
          return *reinterpret_cast<pointer>(reinterpret_cast<const TForteByte *>(mData) + mElementSize * paValue);
        }

        const_iterator &operator++() {
          mData = reinterpret_cast<const TForteByte *>(mData) + mElementSize;
          return *this;
        }

        const_iterator &operator--() {
          mData = reinterpret_cast<const TForteByte *>(mData) - mElementSize;
          return *this;
        }

        const_iterator operator++(int) {
          const_iterator temp = *this;
          ++(*this);
          return temp;
        }

        const_iterator operator--(int) {
          const_iterator temp = *this;
          --(*this);
          return temp;
        }

        const_iterator &operator+=(difference_type paValue) {
          mData = reinterpret_cast<const TForteByte *>(mData) + mElementSize * paValue;
          return *this;
        }

        const_iterator &operator-=(difference_type paValue) {
          mData = reinterpret_cast<const TForteByte *>(mData) - mElementSize * paValue;
          return *this;
        }

        friend const_iterator operator+(const const_iterator &paIter, difference_type paValue) {
          return const_iterator(paIter.mElementSize,
                                reinterpret_cast<const TForteByte *>(paIter.mData) +
                                static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend const_iterator operator+(difference_type paValue, const const_iterator &paIter) {
          return const_iterator(paIter.mElementSize,
                                reinterpret_cast<const TForteByte *>(paIter.mData) +
                                static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend const_iterator operator-(const const_iterator &paIter, difference_type paValue) {
          return const_iterator(paIter.mElementSize,
                                reinterpret_cast<const TForteByte *>(paIter.mData) -
                                static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend difference_type operator-(const const_iterator &paIter, const const_iterator &paOther) {
          return (reinterpret_cast<const TForteByte *>(paIter.mData) -
                  reinterpret_cast<const TForteByte *>(paOther.mData)) /
                 static_cast<difference_type>(paIter.mElementSize);
        }

        friend constexpr bool operator==(const const_iterator &paValue, const const_iterator &paOther) {
          return paValue.mData == paOther.mData;
        }

        friend constexpr bool operator!=(const const_iterator &paValue, const const_iterator &paOther) {
          return paValue.mData != paOther.mData;
        }

        friend constexpr bool operator<(const const_iterator &paValue, const const_iterator &paOther) {
          return paValue.mData < paOther.mData;
        }

        friend constexpr bool operator<=(const const_iterator &paValue, const const_iterator &paOther) {
          return paValue.mData <= paOther.mData;
        }

        friend constexpr bool operator>(const const_iterator &paValue, const const_iterator &paOther) {
          return paValue.mData > paOther.mData;
        }

        friend constexpr bool operator>=(const const_iterator &paValue, const const_iterator &paOther) {
          return paValue.mData >= paOther.mData;
        }

    private:
        size_t mElementSize;
        const void *mData;
    };

    static_assert(std::is_copy_constructible_v<const_iterator>);
    static_assert(std::is_trivially_copy_constructible_v<const_iterator>);
    static_assert(std::is_copy_assignable_v<const_iterator>);
    static_assert(std::is_trivially_copy_assignable_v<const_iterator>);
    static_assert(std::is_destructible_v<const_iterator>);
    static_assert(std::is_trivially_destructible_v<const_iterator>);
    static_assert(std::is_swappable_v<const_iterator>);

    /**
     * @brief Construct an array based on length and provided element type
     * @param paLength The array length
     * @param paArrayType The element type
     */
    CIEC_ARRAY_DYNAMIC(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType)
            : mSize(0), mElementSize(0), mLowerBound(0), mUpperBound(paLength - 1), mData(nullptr),
              mElementDataTypeEntry(nullptr) {
      setup(paLength, paArrayType);
    }

    /**
     * @brief Construct an array based on lower and upper bound and provided element type
     * @param paLowerBound The lower bound
     * @param paUpperBound The upper bound
     * @param paArrayType The element type
     */
    CIEC_ARRAY_DYNAMIC(intmax_t paLowerBound, intmax_t paUpperBound, CStringDictionary::TStringId paArrayType)
            : mSize(0), mElementSize(0), mLowerBound(paLowerBound), mUpperBound(paUpperBound), mData(nullptr),
              mElementDataTypeEntry(nullptr) {
      setup(paLowerBound, paUpperBound, paArrayType);
    }

    /**
     * @brief Copy constructor
     * @param paValue The original array
     */
    CIEC_ARRAY_DYNAMIC(const CIEC_ARRAY_DYNAMIC &paValue)
            : mSize(0), mElementSize(paValue.mElementSize), mLowerBound(paValue.mLowerBound),
              mUpperBound(paValue.mUpperBound), mData(nullptr), mElementDataTypeEntry(paValue.mElementDataTypeEntry) {
      setup(paValue);
    }

    /**
     * @brief Move constructor
     * @param paValue The original array
     */
    CIEC_ARRAY_DYNAMIC(CIEC_ARRAY_DYNAMIC &&paValue)
            : mSize(paValue.mSize), mElementSize(paValue.mElementSize), mLowerBound(paValue.mLowerBound),
              mUpperBound(paValue.mUpperBound), mData(std::exchange(paValue.mData, nullptr)),
              mElementDataTypeEntry(paValue.mElementDataTypeEntry) {
    }

    /**
     * @brief Copy constructor from common array
     * @tparam U The element type of the original array
     * @param paValue The original array
     */
    template<typename U>
    CIEC_ARRAY_DYNAMIC(const CIEC_ARRAY &paSource)
            : mSize(0), mElementSize(paSource[paSource.getLowerBound()].getSizeof()),
              mLowerBound(paSource.getLowerBound()), mUpperBound(paSource.getUpperBound()), mData(nullptr) {
      mElementDataTypeEntry = static_cast<CTypeLib::CDataTypeEntry *>(CTypeLib::findType(
              paSource.getElementTypeNameID(),
              CTypeLib::getDTLibStart()));
      setup(paSource);
    }
    
    /**
     * @brief Copy constructor from common typed array
     * @tparam U The element type of the original array
     * @param paValue The original array
     */
    template<typename U>
    CIEC_ARRAY_DYNAMIC(const CIEC_ARRAY_COMMON<U> &paSource)
            : mSize(0), mElementSize(paSource[paSource.getLowerBound()].getSizeof()),
              mLowerBound(paSource.getLowerBound()), mUpperBound(paSource.getUpperBound()), mData(nullptr) {
      mElementDataTypeEntry = static_cast<CTypeLib::CDataTypeEntry *>(CTypeLib::findType(
              paSource.getElementTypeNameID(),
              CTypeLib::getDTLibStart()));
      setup(paSource);
    }

    /**
     * @brief Copy constructor from variable array
     * @tparam U The element type of the original array
     * @param paValue The original array
     */
    template<typename U>
    CIEC_ARRAY_DYNAMIC(const CIEC_ARRAY_VARIABLE<U> &paSource)
            : mSize(0), mElementSize(paSource[paSource.getLowerBound()].getSizeof()),
              mLowerBound(paSource.getLowerBound()), mUpperBound(paSource.getUpperBound()), mData(nullptr) {
      mElementDataTypeEntry = static_cast<CTypeLib::CDataTypeEntry *>(CTypeLib::findType(
              paSource.getElementTypeNameID(),
              CTypeLib::getDTLibStart()));
      setup(paSource);
    }

    /**
     * @brief Copy constructor from fixed array
     * @tparam U The element type of the original array
     * @tparam lowerBound The lower bound of the original array
     * @tparam upperBound The upper bound of the original array
     * @param paValue The original array
     */
    template<typename U, intmax_t lowerBound, intmax_t upperBound>
    CIEC_ARRAY_DYNAMIC(const CIEC_ARRAY_FIXED<U, lowerBound, upperBound> &paSource)
            : mSize(0), mElementSize(paSource[lowerBound].getSizeof()), mLowerBound(lowerBound),
              mUpperBound(upperBound), mData(nullptr) {
      mElementDataTypeEntry = static_cast<CTypeLib::CDataTypeEntry *>(CTypeLib::findType(
              paSource.getElementTypeNameID(),
              CTypeLib::getDTLibStart()));
      setup(paSource);
    }

    /**
     * @brief Copy assignment operator
     * @param paSource The original array
     * @return The assigned array
     */
    CIEC_ARRAY_DYNAMIC &operator=(const CIEC_ARRAY_DYNAMIC &paSource) {
      assignDynamic(paSource, paSource.mLowerBound, paSource.mUpperBound);
      return *this;
    }

    CIEC_ARRAY_DYNAMIC &operator=(CIEC_ARRAY_DYNAMIC &&paSource) = delete; // no support for move assignment

    /**
     * @brief Copy assignment operator from common typed array
     * @tparam U The element type of the original array
     * @param paSource The original array
     * @return The assigned array
     */
    template<typename U>
    CIEC_ARRAY_DYNAMIC &operator=(const CIEC_ARRAY_COMMON<U> &paSource) {
      assignDynamic(paSource, paSource.getLowerBound(), paSource.getUpperBound());
      return *this;
    }

    /**
     * @brief Copy assignment operator from variable array
     * @tparam U The element type of the original array
     * @param paSource The original array
     * @return The assigned array
     */
    template<typename U>
    CIEC_ARRAY_DYNAMIC &operator=(const CIEC_ARRAY_VARIABLE<U> &paSource) {
      assignDynamic(paSource, paSource.getLowerBound(), paSource.getUpperBound());
      return *this;
    }

    /**
     * @brief Copy assignment operator from fixed array
     * @tparam U The element type of the original array
     * @tparam sourceLowerBound The lower bound of the original array
     * @tparam sourceUpperBound The upper bound of the original array
     * @param paSource The original array
     * @return The assigned array
     */
    template<typename U, intmax_t sourceLowerBound, intmax_t sourceUpperBound>
    CIEC_ARRAY_DYNAMIC &operator=(const CIEC_ARRAY_FIXED<U, sourceLowerBound, sourceUpperBound> &paSource) {
      assignDynamic(paSource, sourceLowerBound, sourceUpperBound);
      return *this;
    }

    /**
     * @brief Destructor
     */
    ~CIEC_ARRAY_DYNAMIC() override {
      clear();
    }

    //!Function to configure the array if it is created via the typelib
    void setup(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType);

    //!Function to configure the array if it is created via the typelib
    void setup(intmax_t paLowerBound, intmax_t paUpperBound, CStringDictionary::TStringId paArrayType);

    //!Function to configure the array if it is created via the typelib
    void setup(const CStringDictionary::TStringId *paParameters);

    [[nodiscard]] size_t size() const override {
      return mSize;
    }

    [[nodiscard]] intmax_t getLowerBound() const override {
      return mLowerBound;
    }

    [[nodiscard]] intmax_t getUpperBound() const override {
      return mUpperBound;
    }

    [[nodiscard]] constexpr iterator begin() {
      return iterator(mElementSize, mData);
    }

    [[nodiscard]] iterator end() {
      return iterator(mElementSize, reinterpret_cast<TForteByte *>(mData) + mElementSize * mSize);
    }

    [[nodiscard]] constexpr const_iterator begin() const {
      return const_iterator(mElementSize, mData);
    }

    [[nodiscard]] const_iterator end() const {
      return const_iterator(mElementSize, reinterpret_cast<TForteByte *>(mData) + mElementSize * mSize);
    }

    [[nodiscard]] constexpr const_iterator cbegin() const {
      return const_iterator(mElementSize, mData);
    }

    [[nodiscard]] const_iterator cend() const {
      return const_iterator(mElementSize, reinterpret_cast<TForteByte *>(mData) + mElementSize * mSize);
    }

    [[nodiscard]] reference at(intmax_t paIndex) override {
      if (paIndex < mLowerBound || paIndex > mUpperBound || !mData) {
#ifdef FORTE_RTTI_AND_EXCEPTIONS
        throw std::out_of_range("array::at: Index: " + std::to_string(paIndex) + " >=  size: " + std::to_string(size()));
#else // FORTE_RTTI_AND_EXCEPTIONS
        std::abort();
#endif // FORTE_RTTI_AND_EXCEPTIONS
      }
      return *reinterpret_cast<pointer>(reinterpret_cast<TForteByte *>(mData) +
                                        getDataArrayIndex(paIndex) * mElementSize);
    }

    [[nodiscard]] const_reference at(intmax_t paIndex) const override {
      if (paIndex < mLowerBound || paIndex > mUpperBound || !mData) {
#ifdef FORTE_RTTI_AND_EXCEPTIONS
        throw std::out_of_range("array::at: Index: " + std::to_string(paIndex) + " >=  size: " + std::to_string(size()));
#else // FORTE_RTTI_AND_EXCEPTIONS
        std::abort();
#endif // FORTE_RTTI_AND_EXCEPTIONS
      }
      return *reinterpret_cast<const_pointer>(reinterpret_cast<const TForteByte *>(mData) +
                                              getDataArrayIndex(paIndex) * mElementSize);
    }

    [[nodiscard]] reference operator[](intmax_t paIndex) override {
      return at(paIndex);
    }

    [[nodiscard]] const_reference operator[](intmax_t paIndex) const override {
      return at(paIndex);
    }

    [[nodiscard]] CIEC_ANY::EDataTypeID getElementDataTypeID() const override {
      return mData != nullptr ? reinterpret_cast<CIEC_ANY *>(mData)->getDataTypeID() : CIEC_ANY::e_ANY;
    }

    [[nodiscard]] CStringDictionary::TStringId getElementTypeNameID() const override {
      return mElementDataTypeEntry != nullptr ? mElementDataTypeEntry->getTypeNameId()
                                              : CStringDictionary::scmInvalidStringId;
    }

    [[nodiscard]] int fromString(const char *paValue) override;

protected:
    // This constructor is only to be used by the create instance method
    CIEC_ARRAY_DYNAMIC() : mSize(0), mElementSize(0), mLowerBound(0), mUpperBound(0), mData(nullptr),
                   mElementDataTypeEntry(nullptr) {
    };

private:
    template<typename U>
    inline void setup(const U &paArray) {
      if (mElementSize && mElementDataTypeEntry) { // check if initialized
        mData = operator new(paArray.size() * mElementSize);
        auto *dest = static_cast<TForteByte *>(mData);
        for (intmax_t i = paArray.getLowerBound(), end = paArray.getUpperBound(); i <= end; ++i) {
          paArray[i].clone(dest);
          dest += mElementSize;
          mSize++; // increment size one-by-one to track allocated elements for destruction
        }
      }
    }

    template<typename U>
    inline void assignDynamic(const U &paArray, intmax_t sourceLowerBound, intmax_t sourceUpperBound) {
      if (mSize && paArray.size()) { // check if initialized
        intmax_t begin = std::max(mLowerBound, sourceLowerBound);
        intmax_t end = std::min(mUpperBound, sourceUpperBound);
        for (intmax_t i = begin; i <= end; ++i) {
          (*this)[i].setValue(paArray[i]);
        }
      }
    }

    inline void clear() {
      if (mData) {
        std::destroy_n(CIEC_ARRAY_DYNAMIC::begin(), mSize);
      }
      operator delete(mData);
      mData = nullptr;
      mSize = 0;
    }

    [[nodiscard]] int initializeFromString(iterator &paPosition, const char *paSrcString, CIEC_ANY **paBufVal);

    [[nodiscard]] int initializeSimpleFromString(iterator &paPosition, const char *paSrcString, CIEC_ANY **paBufVal);

    [[nodiscard]] constexpr size_t getDataArrayIndex(intmax_t paIndex) const {
      return static_cast<size_t>(paIndex - mLowerBound);
    }

    size_t mSize; ///< The size of the array
    size_t mElementSize; ///< The size of each element in bytes
    intmax_t mLowerBound; ///< The lower bound of the array
    intmax_t mUpperBound; ///< The upper bound of the array
    void *mData; ///< The data pointer (with mSize * mElementSize bytes capacity)
    CTypeLib::CDataTypeEntry *mElementDataTypeEntry; ///< The element data type entry
};

static_assert(std::is_copy_constructible_v<CIEC_ARRAY_DYNAMIC>);
static_assert(std::is_move_constructible_v<CIEC_ARRAY_DYNAMIC>);
static_assert(std::is_copy_assignable_v<CIEC_ARRAY_DYNAMIC>);
static_assert(std::is_destructible_v<CIEC_ARRAY_DYNAMIC>);

static_assert(std::is_constructible_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_COMMON<CIEC_ANY> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_COMMON<CIEC_UINT> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);

static_assert(std::is_assignable_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_COMMON<CIEC_ANY> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_COMMON<CIEC_UINT> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY_DYNAMIC, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);

