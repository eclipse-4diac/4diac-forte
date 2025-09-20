/*******************************************************************************
 * Copyright (c) 2023, 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation of `CRingBuffer`
 *    Maximilian Scharf - Copied and adapted the `CRingBuffer` class
 *      to meet the event-trigger requirements in the new `EBuffer` class.
 *******************************************************************************/
#pragma once

#include <array>

namespace forte::io {

  template<typename T, std::size_t size>
  class CeBuffer {
    public:
      CeBuffer() = default;

      bool push(T &elem) {
        mData[mPushIndex++ & cmIndexMask] = elem;
        return true;
      }

      void clear() {
        for (std::size_t i = 0; i < size; ++i) {
          mData[i] = T();
        }
        mPushIndex = 0;
      }

      T getCurrentData() {
        return this->mData[(mPushIndex - 1) & cmIndexMask];
      }

      void makeSnapshot() {
        for (std::size_t i = 0; i < size; i++) {
          mSnapshot[i] = mData[(mPushIndex - 1 - i) & cmIndexMask];
        }
      }

      const std::array<T, size> &getSnapshot() const {
        return mSnapshot;
      }

      static_assert((size & (size - 1)) == 0, "size must be a power of 2");

    private:
      constexpr static std::size_t cmIndexMask = size - 1;

      std::size_t mPushIndex{0};
      std::array<T, size> mData;
      std::array<T, size> mSnapshot;
  };
} // namespace forte::io
