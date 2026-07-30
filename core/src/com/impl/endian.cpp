/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
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

#include "endian.h"

#include "forte/com/factory.h"
#include "forte/iec61131_functions/endianess.h"

#include <charconv>

using namespace forte::literals;

namespace forte::com::impl {
  namespace {
    [[maybe_unused]] ComChannelFactory<std::span<CIEC_ANY_VARIANT>>::EntryImpl<EndianChannel> entry("endian"_STRID);
  } // namespace

  ComResult EndianChannel::open(const std::string_view paConfigString,
                                const std::span<ComChannelDescriptor> paDescriptors) {
    if (paConfigString.empty() || paConfigString == "native") {
      mEndian = std::endian::native;
    } else if (paConfigString == "big") {
      mEndian = std::endian::big;
    } else if (paConfigString == "little") {
      mEndian = std::endian::little;
    } else {
      return ComResult::InvalidId;
    }

    if (paDescriptors.empty()) {
      return ComResult::InvalidId;
    }

    if (!getReceiveBuffer().has_value() || !getSendBuffer().has_value()) {
      return ComResult::DataTypeError;
    }

    mChannel = ComChannelFactory<std::span<CIEC_ANY_VARIANT>>::create(paDescriptors.front().mChannel, *this);
    if (!mChannel) {
      return ComResult::InvalidId;
    }

    return mChannel->open(paDescriptors.front().mConfigString, paDescriptors.subspan(1));
  }

  ComResult EndianChannel::send(const std::span<CIEC_ANY_VARIANT> paData) {
    return mChannel ? mChannel->send(convert(paData)) : ComResult::NoSocket;
  }

  ComResult EndianChannel::receive(const std::span<CIEC_ANY_VARIANT> paData, ComChannel &) {
    return getObserver().receive(convert(paData), *this);
  }

  std::span<CIEC_ANY_VARIANT> EndianChannel::convert(const std::span<CIEC_ANY_VARIANT> paData) const {
    if (mEndian == std::endian::native) {
      return paData;
    }
    for (auto &value : paData) {
      std::visit(
          []<typename T>(T &&paOUT) {
            if constexpr (std::is_same_v<std::remove_reference_t<T>, CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>> ||
                          std::is_same_v<std::remove_reference_t<T>, CIEC_ANY_UNIQUE_PTR<CIEC_STRUCT>>) {
              swapEndianessInplace(*paOUT);
            } else {
              paOUT = swapEndianess(paOUT);
            }
          },
          static_cast<CIEC_ANY_VARIANT::variant &>(value));
    }
    return paData;
  }

  void EndianChannel::sent(const ComResult paResult, ComChannel &) {
    getObserver().sent(paResult, *this);
  }

  ComResult EndianChannel::poll() {
    return mChannel ? mChannel->poll() : ComResult::NoSocket;
  }

  ComResult EndianChannel::close() {
    return mChannel ? mChannel->close() : ComResult::NoSocket;
  }

  ComResult EndianChannel::setMinReceiveSize(const std::size_t paSize) {
    if (const ComResult result = ComChannel::setMinReceiveSize(paSize); result != ComResult::Ok) {
      return result;
    }
    return mChannel ? mChannel->setMinReceiveSize(paSize) : ComResult::Ok;
  }

  ComResult EndianChannel::setMaxReceiveSize(const std::size_t paSize) {
    if (const ComResult result = ComChannel::setMaxReceiveSize(paSize); result != ComResult::Ok) {
      return result;
    }
    return mChannel ? mChannel->setMaxReceiveSize(paSize) : ComResult::Ok;
  }
} // namespace forte::com::impl
