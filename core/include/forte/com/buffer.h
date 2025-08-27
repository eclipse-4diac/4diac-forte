/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include <string>
#include <cstddef>

#include "forte/datatype.h"
#include "forte/util/devlog.h"

namespace forte::com {

  class ComBuffer final {
    public:
      ComBuffer() : mData(nullptr), mCapacity(0), mSize(0) {
      }

      ComBuffer(TForteByte *const paData, const std::size_t paCapacity) :
          mData(paData),
          mCapacity(paCapacity),
          mSize(0) {
      }

      ComBuffer(TForteByte *const paData, const std::size_t paCapacity, const std::size_t paSize) :
          mData(paData),
          mCapacity(paCapacity),
          mSize(paSize) {
      }

      explicit ComBuffer(std::string &paString) :
          mData(reinterpret_cast<TForteByte *>(paString.data())),
          mCapacity(paString.capacity()),
          mSize(paString.size()) {
      }

      ~ComBuffer() = default;

      [[nodiscard]] TForteByte *data() {
        return mData;
      }

      [[nodiscard]] const TForteByte *data() const {
        return mData;
      }

      [[nodiscard]] std::size_t capacity() const {
        return mCapacity;
      }

      [[nodiscard]] std::size_t size() const {
        return mSize;
      }

      void resize(const std::size_t paSize) {
        if (paSize > mCapacity) {
          DEVLOG_ERROR("Requested size exceeds capacity\n");
        }
        mSize = paSize;
      }

    private:
      TForteByte *mData;
      std::size_t mCapacity;
      std::size_t mSize;
  };

} // namespace forte::com
