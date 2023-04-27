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
 *******************************************************************************/
#ifndef _FORTE_ARRAY_H_
#define _FORTE_ARRAY_H_

#include "forte_array_common.h"
#include "forte_ulint.h"
#include <stdexcept>
#include <devlog.h>
#include <utility>

#ifdef FORTE_SUPPORT_ARRAYS

#ifndef CIEC_ARRAY_COLLAPSE_MAX_SIZE
#define CIEC_ARRAY_COLLAPSE_MAX_SIZE 100
#endif

template<typename T, intmax_t lowerBound, intmax_t upperBound>
class CIEC_ARRAY_FIXED;

template<typename T>
class CIEC_ARRAY_VARIABLE;

/*!\ingroup COREDTS CIEC_ARRAY represents the array data type according to IEC 61131.
 */
template<typename T = CIEC_ANY>
class CIEC_ARRAY : public CIEC_ARRAY_COMMON<T> {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = typename CIEC_ARRAY_COMMON<T>::value_type;
    using pointer = typename CIEC_ARRAY_COMMON<T>::pointer;
    using const_pointer = typename CIEC_ARRAY_COMMON<T>::const_pointer;
    using reference = typename CIEC_ARRAY_COMMON<T>::reference;
    using const_reference = typename CIEC_ARRAY_COMMON<T>::const_reference;

    using CIEC_ARRAY_COMMON<T>::at;
    using CIEC_ARRAY_COMMON<T>::operator[];
    using CIEC_ARRAY_COMMON<T>::operator=;

    class iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = typename CIEC_ARRAY<T>::value_type;
        using pointer = typename CIEC_ARRAY<T>::pointer;
        using reference = typename CIEC_ARRAY<T>::reference;
        using iterator_category = std::random_access_iterator_tag;

        constexpr iterator(size_t paElementSize, void *paData) : mElementSize(paElementSize), mData(paData) {}

        constexpr reference operator*() const {
          return *reinterpret_cast<pointer>(mData);
        }

        constexpr pointer operator->() const {
          return reinterpret_cast<pointer>(mData);
        }

        constexpr reference operator[](difference_type paValue) const {
          return *reinterpret_cast<pointer>(reinterpret_cast<TForteByte *>(mData) + mElementSize * paValue);
        }

        constexpr iterator &operator++() {
          mData = reinterpret_cast<TForteByte *>(mData) + mElementSize;
          return *this;
        }

        constexpr iterator &operator--() {
          mData = reinterpret_cast<TForteByte *>(mData) - mElementSize;
          return *this;
        }

        constexpr iterator operator++(int) {
          iterator temp = *this;
          ++(*this);
          return temp;
        }

        constexpr iterator operator--(int) {
          iterator temp = *this;
          --(*this);
          return temp;
        }

        constexpr iterator &operator+=(difference_type paValue) {
          mData = reinterpret_cast<TForteByte *>(mData) + mElementSize * paValue;
          return *this;
        }

        constexpr iterator &operator-=(difference_type paValue) {
          mData = reinterpret_cast<TForteByte *>(mData) - static_cast<difference_type >(mElementSize) * paValue;
          return *this;
        }

        friend constexpr iterator operator+(const iterator &paIter, difference_type paValue) {
          return iterator(paIter.mElementSize,
                          reinterpret_cast<TForteByte *>(paIter.mData) +
                          static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend constexpr iterator operator+(difference_type paValue, const iterator &paIter) {
          return iterator(paIter.mElementSize,
                          reinterpret_cast<TForteByte *>(paIter.mData) +
                          static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend constexpr iterator operator-(const iterator &paIter, difference_type paValue) {
          return iterator(paIter.mElementSize,
                          reinterpret_cast<TForteByte *>(paIter.mData) -
                          static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend constexpr difference_type operator-(const iterator &paIter, const iterator &paOther) {
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
        using value_type = typename CIEC_ARRAY<T>::value_type;
        using pointer = typename CIEC_ARRAY<T>::const_pointer;
        using reference = typename CIEC_ARRAY<T>::const_reference;
        using iterator_category = std::random_access_iterator_tag;

        constexpr const_iterator(size_t paElementSize, void *paData) : mElementSize(paElementSize), mData(paData) {}

        constexpr reference operator*() const {
          return *reinterpret_cast<pointer>(mData);
        }

        constexpr pointer operator->() const {
          return reinterpret_cast<pointer>(mData);
        }

        constexpr reference operator[](difference_type paValue) const {
          return *reinterpret_cast<pointer>(reinterpret_cast<const TForteByte *>(mData) + mElementSize * paValue);
        }

        constexpr const_iterator &operator++() {
          mData = reinterpret_cast<const TForteByte *>(mData) + mElementSize;
          return *this;
        }

        constexpr const_iterator &operator--() {
          mData = reinterpret_cast<const TForteByte *>(mData) - mElementSize;
          return *this;
        }

        constexpr const_iterator operator++(int) {
          const_iterator temp = *this;
          ++(*this);
          return temp;
        }

        constexpr const_iterator operator--(int) {
          const_iterator temp = *this;
          --(*this);
          return temp;
        }

        constexpr const_iterator &operator+=(difference_type paValue) {
          mData = reinterpret_cast<const TForteByte *>(mData) + mElementSize * paValue;
          return *this;
        }

        constexpr const_iterator &operator-=(difference_type paValue) {
          mData = reinterpret_cast<const TForteByte *>(mData) - mElementSize * paValue;
          return *this;
        }

        friend constexpr const_iterator operator+(const const_iterator &paIter, difference_type paValue) {
          return const_iterator(paIter.mElementSize,
                                reinterpret_cast<const TForteByte *>(paIter.mData) +
                                static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend constexpr const_iterator operator+(difference_type paValue, const const_iterator &paIter) {
          return const_iterator(paIter.mElementSize,
                                reinterpret_cast<const TForteByte *>(paIter.mData) +
                                static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend constexpr const_iterator operator-(const const_iterator &paIter, difference_type paValue) {
          return const_iterator(paIter.mElementSize,
                                reinterpret_cast<const TForteByte *>(paIter.mData) -
                                static_cast<difference_type>(paIter.mElementSize) * paValue);
        }

        friend constexpr difference_type operator-(const const_iterator &paIter, const const_iterator &paOther) {
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
    CIEC_ARRAY(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType)
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
    CIEC_ARRAY(intmax_t paLowerBound, intmax_t paUpperBound, CStringDictionary::TStringId paArrayType)
            : mSize(0), mElementSize(0), mLowerBound(paLowerBound), mUpperBound(paUpperBound), mData(nullptr),
              mElementDataTypeEntry(nullptr) {
      setup(paLowerBound, paUpperBound, paArrayType);
    }

    /**
     * @brief Copy constructor
     * @param paValue The original array
     */
    CIEC_ARRAY(const CIEC_ARRAY &paValue)
            : mSize(0), mElementSize(paValue.mElementSize), mLowerBound(paValue.mLowerBound),
              mUpperBound(paValue.mUpperBound), mData(nullptr), mElementDataTypeEntry(paValue.mElementDataTypeEntry) {
      setup(paValue);
    }

    /**
     * @brief Move constructor
     * @param paValue The original array
     */
    constexpr CIEC_ARRAY(CIEC_ARRAY &&paValue)
            : mSize(paValue.mSize), mElementSize(paValue.mElementSize), mLowerBound(paValue.mLowerBound),
              mUpperBound(paValue.mUpperBound), mData(std::exchange(paValue.mData, nullptr)),
              mElementDataTypeEntry(paValue.mElementDataTypeEntry) {
    }

    /**
     * @brief Copy constructor from different element type
     * @tparam U The element type of the original array
     * @param paValue The original array
     */
    template<typename U>
    CIEC_ARRAY(const CIEC_ARRAY<U> &paValue)
            : mSize(0), mElementSize(paValue.mElementSize), mLowerBound(paValue.mLowerBound),
              mUpperBound(paValue.mUpperBound), mData(nullptr), mElementDataTypeEntry(paValue.mElementDataTypeEntry) {
      setup(paValue);
    }

    /**
     * @brief Move constructor from different element type
     * @tparam U The element type of the original array
     * @param paValue The original array
     */
    template<typename U>
    constexpr CIEC_ARRAY(CIEC_ARRAY<U> &&paValue)
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
    CIEC_ARRAY(const CIEC_ARRAY_COMMON<U> &paSource)
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
    CIEC_ARRAY(const CIEC_ARRAY_VARIABLE<U> &paSource)
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
    CIEC_ARRAY(const CIEC_ARRAY_FIXED<U, lowerBound, upperBound> &paSource)
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
    CIEC_ARRAY &operator=(const CIEC_ARRAY &paSource) {
      assignDynamic(paSource, paSource.mLowerBound, paSource.mUpperBound);
      return *this;
    }

    CIEC_ARRAY &operator=(CIEC_ARRAY &&paSource) = delete; // no support for move assignment

    /**
     * @brief Copy assignment operator from different type
     * @tparam U The element type of the original array
     * @param paSource The original array
     * @return The assigned array
     */
    template<typename U>
    CIEC_ARRAY &operator=(const CIEC_ARRAY<U> &paSource) {
      assignDynamic(paSource, paSource.mLowerBound, paSource.mUpperBound);
      return *this;
    }

    /**
     * @brief Copy assignment operator from variable array
     * @tparam U The element type of the original array
     * @param paSource The original array
     * @return The assigned array
     */
    template<typename U>
    CIEC_ARRAY &operator=(const CIEC_ARRAY_VARIABLE<U> &paSource) {
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
    CIEC_ARRAY &operator=(const CIEC_ARRAY_FIXED<U, sourceLowerBound, sourceUpperBound> &paSource) {
      assignDynamic(paSource, sourceLowerBound, sourceUpperBound);
      return *this;
    }

    /**
     * @brief Destructor
     */
    ~CIEC_ARRAY() override {
      clear();
    }

    //!Function to configure the array if it is created via the typelib
    void setup(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType) {
      clear();
      if (paLength) {
        mLowerBound = 0;
        mUpperBound = paLength - 1;

        mElementDataTypeEntry = static_cast<CTypeLib::CDataTypeEntry *>(CTypeLib::findType(paArrayType,
                                                                                           CTypeLib::getDTLibStart()));
        if (mElementDataTypeEntry) {
          mElementSize = mElementDataTypeEntry->getSize();
          mData = operator new(paLength * mElementSize);
          auto *dest = static_cast<TForteByte *>(mData);
          for (; mSize < paLength; ++mSize) { // increment size one-by-one to track allocated elements for destruction
            mElementDataTypeEntry->createDataTypeInstance(dest);
            dest += mElementSize;
          }
        }
      }
    }

    //!Function to configure the array if it is created via the typelib
    void setup(intmax_t paLowerBound, intmax_t paUpperBound, CStringDictionary::TStringId paArrayType) {
      clear();
      mLowerBound = paLowerBound;
      mUpperBound = paUpperBound;

      mElementDataTypeEntry = static_cast<CTypeLib::CDataTypeEntry *>(CTypeLib::findType(paArrayType,
                                                                                         CTypeLib::getDTLibStart()));
      if (mElementDataTypeEntry) {
        auto size = static_cast<size_t>(paUpperBound - paLowerBound + 1);
        mElementSize = mElementDataTypeEntry->getSize();
        mData = operator new(size * mElementSize);
        auto *dest = static_cast<TForteByte *>(mData);
        for (; mSize < size; ++mSize) { // increment size one-by-one to track allocated elements for destruction
          mElementDataTypeEntry->createDataTypeInstance(dest);
          dest += mElementSize;
        }
      }
    }

    [[nodiscard]] constexpr size_t size() const override {
      return mSize;
    }

    [[nodiscard]] constexpr intmax_t getLowerBound() const override {
      return mLowerBound;
    }

    [[nodiscard]] constexpr intmax_t getUpperBound() const override {
      return mUpperBound;
    }

    [[nodiscard]] constexpr iterator begin() {
      return iterator(mElementSize, mData);
    }

    [[nodiscard]] constexpr iterator end() {
      return iterator(mElementSize, reinterpret_cast<TForteByte *>(mData) + mElementSize * mSize);
    }

    [[nodiscard]] constexpr const_iterator begin() const {
      return const_iterator(mElementSize, mData);
    }

    [[nodiscard]] constexpr const_iterator end() const {
      return const_iterator(mElementSize, reinterpret_cast<TForteByte *>(mData) + mElementSize * mSize);
    }

    [[nodiscard]] constexpr const_iterator cbegin() const {
      return const_iterator(mElementSize, mData);
    }

    [[nodiscard]] constexpr const_iterator cend() const {
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
                                              : CStringDictionary::scm_nInvalidStringId;
    }

    void setValue(const CIEC_ANY &paValue) override {
      if (paValue.getDataTypeID() == CIEC_ANY::e_ARRAY) {
        *this = static_cast<const CIEC_ARRAY &>(paValue);
      }
    }

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
      if (paOther.getDataTypeID() == CIEC_ANY::e_ARRAY) {
        auto &otherArray = static_cast<const CIEC_ARRAY &>(paOther);
        if (mLowerBound != otherArray.mLowerBound || mUpperBound != otherArray.mUpperBound) {
          return false;
        }

        for (intmax_t i = mLowerBound; i <= mUpperBound; ++i) {
          if (!(*this)[i].equals(otherArray[i])) {
            return false;
          }
        }
        return true;
      }
      return false;
    }

    [[nodiscard]] int fromString(const char *paValue) override {
      int nRetVal = -1;
      const char *pcRunner = paValue;
      if ('[' == paValue[0]) {
        pcRunner++;

        CIEC_ANY *poBufVal = nullptr;
        iterator iter = begin();

        while (('\0' != *pcRunner) && (']' != *pcRunner)) {
          findNextNonBlankSpace(&pcRunner);

          int valueLength = initializeFromString(iter, pcRunner, &poBufVal);
          if (0 > valueLength) {
            break;
          }
          pcRunner += valueLength;

          findNextNonBlankSpace(&pcRunner);
          if (',' == *pcRunner) {
            pcRunner++;
          } else {
            //we have an error or the end bracket
            break;
          }
        }
        if (*pcRunner == ']') {
          //arrays have to and on a closing bracket
          nRetVal = static_cast<int>(pcRunner - paValue + 1); //+1 from the closing bracket
          // For the rest of the array size copy the default element
          std::destroy(iter, end());
          size_t size = mSize;
          mSize = static_cast<size_t>(iter - begin());
          auto *dest = reinterpret_cast<TForteByte *>(&*iter);
          for (; mSize < size; ++mSize) { // increment size one-by-one to track allocated elements for destruction
            mElementDataTypeEntry->createDataTypeInstance(dest);
            dest += mElementSize;
          }
        }
        delete poBufVal;
      }
      return nRetVal;
    }

    [[nodiscard]] int toString(char *paValue, size_t paBufferSize) const override {
      int nBytesUsed = -1;

      if (mSize > CIEC_ARRAY_COLLAPSE_MAX_SIZE) {
        return toCollapsedString(paValue, paBufferSize);
      }

      if (paBufferSize) {
        *paValue = '[';
        paValue++;
        paBufferSize--;
        nBytesUsed = 1;
        const_iterator iter = begin();
        for (size_t i = 0; i < mSize; ++i, ++iter) {
          int nUsedBytesByElement = iter->toString(paValue, paBufferSize);
          if (-1 == nUsedBytesByElement) {
            return -1;
          }
          paBufferSize -= static_cast<size_t>(nUsedBytesByElement);
          paValue += nUsedBytesByElement;
          if (!paBufferSize) {
            return -1;
          }

          nBytesUsed += nUsedBytesByElement;

          if (i != mSize - 1) {
            *paValue = ',';
            paValue++;

            ++nBytesUsed;
            paBufferSize--;
          }
        }
        if (paBufferSize < 2) {
          return -1;
        }
        *paValue = ']';
        paValue[1] = '\0';
        nBytesUsed++;
      }

      return nBytesUsed;
    }

    [[nodiscard]] int toCollapsedString(char *paValue, size_t paBufferSize) const {
      int nBytesUsed = -1;

      if (paBufferSize > 3) {
        *paValue = '[';
        paValue++;
        paBufferSize--;
        nBytesUsed = 1;
        size_t unSize = size();
        size_t count = 0;
        const_iterator iter = begin();
        const CIEC_ANY *lastElement = nullptr;
        for (size_t i = 0; i < unSize; ++i, ++iter) {
          if (lastElement != nullptr && !lastElement->equals(*iter)) {
            int usedBytesByElement = toCollapsedElementString(*lastElement, count, nBytesUsed > 1, paValue,
                                                              paBufferSize);
            if (usedBytesByElement < 0) {
              return -1;
            }
            paValue += usedBytesByElement;
            paBufferSize -= static_cast<size_t>(usedBytesByElement);
            nBytesUsed += usedBytesByElement;
            count = 0;
          }
          lastElement = &*iter;
          count++;
        }
        if (lastElement) {
          int usedBytesByElement = toCollapsedElementString(*lastElement, count, nBytesUsed > 1, paValue,
                                                            paBufferSize);
          if (usedBytesByElement < 0) {
            return -1;
          }
          paValue += usedBytesByElement;
          paBufferSize -= static_cast<size_t>(usedBytesByElement);
          nBytesUsed += usedBytesByElement;
        }

        if (paBufferSize < 2) {
          return -1;
        }
        *paValue = ']';
        paValue[1] = '\0';
        nBytesUsed++;
      }

      return nBytesUsed;
    }

    [[nodiscard]] int toCollapsedElementString(const CIEC_ANY &paElement, size_t paCount, bool paComma, char *paValue,
                                               size_t paBufferSize) const {
      int nBytesUsed = 0;

      if (paComma) {
        if (paBufferSize < 1) {
          return -1;
        }

        *(paValue++) = ',';
        paBufferSize--;
        nBytesUsed++;
      }

      if (paCount > 1) {
        int usedBytesByRepeat = CIEC_ULINT(paCount).toString(paValue, paBufferSize);
        if (usedBytesByRepeat < 0) {
          return -1;
        }
        paValue += usedBytesByRepeat;
        paBufferSize -= static_cast<size_t>(usedBytesByRepeat);
        nBytesUsed += usedBytesByRepeat;

        if (paBufferSize < 1) {
          return -1;
        }
        *(paValue++) = '(';
        paBufferSize--;
        nBytesUsed++;
      }

      int usedBytesByElement = paElement.toString(paValue, paBufferSize);
      if (usedBytesByElement < 0) {
        return -1;
      }
      paValue += usedBytesByElement;
      paBufferSize -= static_cast<size_t>(usedBytesByElement);
      nBytesUsed += usedBytesByElement;

      if (paCount > 1) {
        if (paBufferSize < 1) {
          return -1;
        }
        *(paValue++) = ')';
        // paBufferSize--; value unused
        nBytesUsed++;
      }
      return nBytesUsed;
    }

    [[nodiscard]] size_t getToStringBufferSize() const override {
      size_t retVal = 3; // 2 bytes for the open and closing brackets and one for the '\0'
      retVal += (mSize > 1) ? (mSize - 1) : 0; // for the commas between the elements

      if (nullptr != mData) {
        switch (getElementDataTypeID()) { // in these cases, the length of the elements are not always the same
          case CIEC_ANY::e_WSTRING:
          case CIEC_ANY::e_STRING: // quotes or double quotes are already counted in ANY_STRING
          case CIEC_ANY::e_ARRAY:
          case CIEC_ANY::e_STRUCT:
            for (auto &elem: *this) {
              retVal += elem.getToStringBufferSize() - 1;
              // -1 since the \0 of each element is counted as just one at the beginning
            }
            break;
          default:
            retVal += mSize * (reinterpret_cast<CIEC_ANY *>(mData)->getToStringBufferSize() -
                               1); //-1 since the \0 of each element is counted as just one at the beginning
            break;
        }
      }

      return retVal;
    }

    [[nodiscard]] size_t getSizeof() const override {
      return sizeof(CIEC_ARRAY);
    }

    [[nodiscard]] CIEC_ANY *clone(TForteByte *paDataBuf) const override {
      return (nullptr != paDataBuf) ? new(paDataBuf) CIEC_ARRAY(*this) : new CIEC_ARRAY(*this);
    }

protected:
    // This constructor is only to be used by the create instance method
    CIEC_ARRAY() : mSize(0), mElementSize(0), mLowerBound(0), mUpperBound(0), mData(nullptr),
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
        std::destroy_n(CIEC_ARRAY::begin(), mSize);
      }
      operator delete(mData);
      mData = nullptr;
      mSize = 0;
    }

    [[nodiscard]] int initializeFromString(iterator &paPosition, const char *paSrcString, CIEC_ANY **paBufVal) {
      // check repeat syntax (e.g., "255(1, 2, 3)")
      CIEC_ULINT repeat;
      const char *pcRunner = paSrcString;
      iterator initialPosition = paPosition;
      int repeatLength = repeat.fromString(pcRunner);
      if (0 < repeatLength) {
        pcRunner += repeatLength;
        findNextNonBlankSpace(&pcRunner);
        if ('(' == *pcRunner) {
          pcRunner++;
          while (('\0' != *pcRunner) && (')' != *pcRunner)) {
            findNextNonBlankSpace(&pcRunner);

            int valueLength = initializeSimpleFromString(paPosition, pcRunner, paBufVal);
            if (0 > valueLength) {
              break;
            }
            pcRunner += valueLength;

            findNextNonBlankSpace(&pcRunner);
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

    [[nodiscard]] int initializeSimpleFromString(iterator &paPosition, const char *paSrcString,
                                                 CIEC_ANY **paBufVal) {
      if (paPosition != end()) {
        return (paPosition++)->fromString(paSrcString);
      } else {
        if (nullptr == *paBufVal) {
          *paBufVal = mElementDataTypeEntry->createDataTypeInstance(nullptr);
        }
        return (*paBufVal)->fromString(paSrcString);
      }
    }

    static void findNextNonBlankSpace(const char **paRunner) {
      while (' ' == **paRunner) {
        (*paRunner)++;
      }
    }

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

static_assert(std::is_copy_constructible_v<CIEC_ARRAY<CIEC_ANY>>);
static_assert(std::is_move_constructible_v<CIEC_ARRAY<CIEC_ANY>>);
static_assert(std::is_copy_assignable_v<CIEC_ARRAY<CIEC_ANY>>);
static_assert(std::is_destructible_v<CIEC_ARRAY<CIEC_ANY>>);

static_assert(std::is_constructible_v<CIEC_ARRAY<CIEC_ANY>, const CIEC_ARRAY<CIEC_UINT> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY<CIEC_ULINT>, const CIEC_ARRAY<CIEC_UINT> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY<CIEC_ANY>, CIEC_ARRAY<CIEC_UINT> &&>);
static_assert(std::is_constructible_v<CIEC_ARRAY<CIEC_ULINT>, CIEC_ARRAY<CIEC_UINT> &&>);

static_assert(std::is_assignable_v<CIEC_ARRAY<CIEC_ANY>, const CIEC_ARRAY<CIEC_UINT> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY<CIEC_ULINT>, const CIEC_ARRAY<CIEC_UINT> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY<CIEC_ANY>, CIEC_ARRAY<CIEC_UINT> &&>);
static_assert(std::is_assignable_v<CIEC_ARRAY<CIEC_ULINT>, CIEC_ARRAY<CIEC_UINT> &&>);

static_assert(std::is_constructible_v<CIEC_ARRAY<CIEC_ANY>, const CIEC_ARRAY_COMMON<CIEC_ANY> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY<CIEC_ANY>, const CIEC_ARRAY_COMMON<CIEC_UINT> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY<CIEC_ANY>, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY<CIEC_ULINT>, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY<CIEC_ANY>, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);
static_assert(std::is_constructible_v<CIEC_ARRAY<CIEC_ULINT>, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);

static_assert(std::is_assignable_v<CIEC_ARRAY<CIEC_ANY>, const CIEC_ARRAY_COMMON<CIEC_ANY> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY<CIEC_ANY>, const CIEC_ARRAY_COMMON<CIEC_UINT> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY<CIEC_ANY>, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY<CIEC_ULINT>, const CIEC_ARRAY_VARIABLE<CIEC_UINT> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY<CIEC_ANY>, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);
static_assert(std::is_assignable_v<CIEC_ARRAY<CIEC_ULINT>, const CIEC_ARRAY_FIXED<CIEC_UINT, 0, 0> &>);

class CIEC_ARRAY_TYPELIB : public CIEC_ARRAY<CIEC_ANY> {
DECLARE_FIRMWARE_DATATYPE(ARRAY_TYPELIB)
public:
    CIEC_ARRAY_TYPELIB() = default;

    CIEC_ARRAY_TYPELIB(TForteUInt16 paLength, CStringDictionary::TStringId paArrayType)
            : CIEC_ARRAY<CIEC_ANY>(paLength, paArrayType) {}
};

static_assert(std::is_copy_constructible_v<CIEC_ARRAY_TYPELIB>);
static_assert(std::is_move_constructible_v<CIEC_ARRAY_TYPELIB>);
static_assert(std::is_copy_assignable_v<CIEC_ARRAY_TYPELIB>);
static_assert(std::is_destructible_v<CIEC_ARRAY_TYPELIB>);

static_assert(std::is_constructible_v<CIEC_ARRAY<CIEC_ULINT>, const CIEC_ARRAY_TYPELIB &>);

#endif /* FORTE_SUPPORT_ARRAYS */

#endif /*_FORTE_ARRAY_H_*/
