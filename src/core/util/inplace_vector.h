/*******************************************************************************
 * Copyright (c) 2015, 2025 fortiss GmbH, Jörg Walter, Martin Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *   Jörg Walter
 *    - change to make API more like std::inplace_vector (C++26)
 *   Martin Jobst
 *    - add erase functions
 *    - add equality operators
 *******************************************************************************/
#pragma once

#include <array>
#include <cassert>
#include <algorithm>

namespace forte::core::util {

  /*!\brief A vector having a fixed size array as data storage.
   *
   *  Mimicks the upcoming std::inplace_vector as far as 4diac FORTE needs it.
   *
   */
  template<typename T, size_t Capacity>
  class inplace_vector {
    public:
      using value_type = T;
      using size_type = std::size_t;
      using difference_type = std::ptrdiff_t;
      using const_pointer = const T *;
      using pointer = T *;
      using const_reference = const T &;
      using reference = T &;
      using const_iterator = const T *;
      using iterator = T *;

      inplace_vector() : mNumElements(0) {
      }

      constexpr inplace_vector(std::initializer_list<value_type> il) {
        // TODO: improve this to eliminate all static initialization code in stringdict.cpp
        for (auto elt : il) {
          mDataStorage[mNumElements++] = elt;
        }
      }

      void clear() {
        mNumElements = 0;
      }

      void resize(size_type newsize, value_type initval) {
        assert(newsize <= capacity());
        for (; mNumElements < newsize; mNumElements++) {
          mDataStorage[mNumElements] = initval;
        }
        mNumElements = newsize;
      }

      pointer try_push_back(const_reference val) {
        if (size() >= capacity()) {
          return nullptr;
        }

        mDataStorage[mNumElements] = val;
        mNumElements++;
        return &mDataStorage[mNumElements - 1];
      }

      pointer push_back(const_reference val) {
        auto result = try_push_back(val);
        assert(result);
        return result;
      }

      iterator insert(iterator pos, const_reference val) {
        assert(size() < capacity());

        std::move_backward(pos, end(), end() + 1);
        *pos = val;
        mNumElements++;
        return pos;
      }

      iterator insert(iterator pos, const_iterator first, const_iterator last) {
        size_t count = std::distance(first, last);
        assert(size() + count <= capacity());

        std::move_backward(pos, end(), end() + count);
        std::copy(first, last, pos);
        mNumElements += count;
        return pos;
      }

      void pop_back() {
        if (!empty()) {
          mNumElements--;
        }
      }

      reference front() {
        assert(size());
        return mDataStorage[0];
      }

      reference back() {
        assert(size());
        return mDataStorage[size() - 1];
      }

      reference operator[](size_type index) {
        assert(index < capacity());
        return mDataStorage[index];
      }

      bool empty() const {
        return !size();
      }

      size_type size() const {
        return mNumElements;
      }

      size_type capacity() const {
        return Capacity;
      }

      iterator begin() {
        return mDataStorage.data();
      }

      const_iterator cbegin() const {
        return mDataStorage.data();
      }

      iterator end() {
        return mDataStorage.data() + mNumElements;
      }

      const_iterator cend() const {
        return mDataStorage.data() + mNumElements;
      }

      iterator erase(const_iterator position) {
        return erase(begin() + (position - cbegin()));
      }

      iterator erase(iterator position) {
        if (position + 1 != end()) {
          std::move(position + 1, end(), position);
        }
        --mNumElements;
        return position;
      }

      iterator erase(const_iterator first, const_iterator last) {
        return erase(begin() + (first - cbegin()), begin() + (last - cbegin()));
      }

      iterator erase(iterator first, iterator last) {
        if (first != last) {
          if (last != end()) {
            std::move(last, end(), first);
          }
          mNumElements -= last - first;
        }
        return first;
      }

    private:
      std::array<value_type, Capacity> mDataStorage;
      size_type mNumElements;
  };

  template<typename T, std::size_t U, std::size_t V>
  [[nodiscard]]
  bool operator==(const inplace_vector<T, U> &paFirst, const inplace_vector<T, V> &paSecond) {
    return paFirst.size() == paSecond.size() && std::equal(paFirst.cbegin(), paFirst.cend(), paSecond.cbegin());
  }

  template<typename T, std::size_t U, std::size_t V>
  [[nodiscard]]
  bool operator!=(const inplace_vector<T, U> &paFirst, const inplace_vector<T, V> &paSecond) {
    return !(paFirst == paSecond);
  }
} // namespace forte::core::util
