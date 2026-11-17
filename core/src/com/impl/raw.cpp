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

#include "raw.h"

#include "forte/com/factory.h"

#include <array>
#include <charconv>

using namespace forte::literals;

namespace forte::com::impl {
  namespace {
    [[maybe_unused]] ComChannelFactory<std::span<CIEC_ANY_VARIANT>>::EntryImpl<RawChannel> entry("raw"_STRID);

    std::size_t parseSize(const std::string_view paConfigString) {
      std::size_t result{};
      if (auto [ptr, ec] =
              std::from_chars(paConfigString.data(), paConfigString.data() + paConfigString.size(), result);
          ptr != paConfigString.data() + paConfigString.size() || ec != std::errc()) {
        return std::string_view::npos;
      }
      return result;
    }
  } // namespace

  ComResult RawChannel::open(const std::string_view paConfigString,
                             const std::span<const ComChannelDescriptor> paDescriptors) {
    if (isOpen()) {
      return ComResult::Ok;
    }

    std::size_t maxReceiveSize;
    std::size_t minReceiveSize;
    if (paConfigString.empty()) {
      maxReceiveSize = CIEC_ANY_STRING::scmMaxStringLen;
      minReceiveSize = 0;
    } else {
      const std::size_t size = parseSize(paConfigString);
      if (size == std::string_view::npos) {
        return ComResult::InvalidId;
      }
      maxReceiveSize = size;
      minReceiveSize = size;
    }
    if (paDescriptors.empty()) {
      return ComResult::InvalidId;
    }

    if (!mChannel || !mChannel->supports(paDescriptors)) {
      mChannel = ComChannelFactory<ComBuffer>::create(paDescriptors.front().mChannel, *this);
      if (!mChannel) {
        return ComResult::InvalidId;
      }
    }

    if (const ComResult result = mChannel->setMaxReceiveSize(maxReceiveSize); result != ComResult::Ok) {
      return result;
    }
    if (const ComResult result = mChannel->setMinReceiveSize(minReceiveSize); result != ComResult::Ok) {
      return result;
    }

    return mChannel->open(paDescriptors.front().mConfigString, paDescriptors.subspan(1));
  }

  ComResult RawChannel::send(std::span<CIEC_ANY_VARIANT> paData) {
    if (paData.size() != 1) {
      return ComResult::DataTypeError;
    }
    const CIEC_ANY &data = paData[0].unwrap();
    if (data.getDataTypeID() != CIEC_ANY::e_STRING) {
      return ComResult::DataTypeError;
    }
    if (!mChannel) {
      return ComResult::NoSocket;
    }
    if (!mSendBuffer.empty()) {
      return ComResult::SendFailed;
    }
    mSendBuffer = static_cast<const CIEC_STRING &>(data).getStorage();
    const ComResult result = mChannel->send(ComBuffer(mSendBuffer));
    if (result != ComResult::Async) {
      mSendBuffer.clear();
    }
    return result;
  }

  ComResult RawChannel::receive(ComBuffer paData, ComChannel<ComBuffer> &) {
    if (std::optional<std::span<CIEC_ANY_VARIANT>> receiveBuffer = getObserver().getReceiveBuffer();
        receiveBuffer.has_value() && receiveBuffer->size() == 1 &&
        receiveBuffer->data()[0].getDataTypeID() == CIEC_ANY::e_STRING) {
      static_cast<CIEC_STRING &>((*receiveBuffer)[0].unwrap())
          .assign(reinterpret_cast<char *>(paData.data()), paData.size());
      return getObserver().receive(*receiveBuffer, *this);
    }
    std::array<CIEC_ANY_VARIANT, 1> data = {CIEC_STRING(reinterpret_cast<char *>(paData.data()), paData.size())};
    return getObserver().receive(data, *this);
  }

  void RawChannel::sent(const ComResult paResult, ComChannel<ComBuffer> &) {
    mSendBuffer.clear();
    getObserver().sent(paResult, *this);
  }

  ComResult RawChannel::poll() {
    return mChannel ? mChannel->poll() : ComResult::NoSocket;
  }

  ComResult RawChannel::close() {
    mSendBuffer.clear();
    return mChannel ? mChannel->close() : ComResult::NoSocket;
  }

  bool RawChannel::supports(const std::span<const ComChannelDescriptor> paDescriptors) const {
    return supportsChannel("raw"_STRID, paDescriptors);
  }

  ComResult RawChannel::setMinReceiveSize(const std::size_t paSize) {
    return paSize > 1 ? ComResult::DataTypeError : ComChannel::setMinReceiveSize(paSize);
  }

  ComResult RawChannel::setMaxReceiveSize(const std::size_t paSize) {
    return paSize < 1 ? ComResult::DataTypeError : ComChannel::setMaxReceiveSize(paSize);
  }
} // namespace forte::com::impl
