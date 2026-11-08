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

#include "null.h"

#include "forte/stringid.h"
#include "forte/com/factory.h"

using namespace forte::literals;

namespace forte::com::impl {
  namespace {
    [[maybe_unused]] ComChannelFactory<ComBuffer>::EntryImpl<NullChannel> entry("null"_STRID);
  }

  ComResult NullChannel::open(const std::string_view paConfigString,
                              const std::span<const ComChannelDescriptor> paDescriptors) {
    if (isOpen()) {
      return ComResult::Ok;
    }

    if (!paConfigString.empty() || !paDescriptors.empty()) {
      return ComResult::InvalidId;
    }

    mIsOpen = true;
    return ComResult::Ok;
  }

  bool NullChannel::supports(const std::span<const ComChannelDescriptor> paDescriptors) const {
    return supportsChannel("null"_STRID, paDescriptors);
  }

  ComResult NullChannel::send(ComBuffer) {
    return mIsOpen ? ComResult::Ok : ComResult::NoSocket;
  }

  ComResult NullChannel::poll() {
    if (!mIsOpen) {
      return ComResult::NoSocket;
    }
    getObserver().receive({nullptr, 0}, *this);
    return ComResult::Async;
  }

  ComResult NullChannel::close() {
    mIsOpen = false;
    return ComResult::Ok;
  }

  ComResult NullChannel::setMinReceiveSize(const std::size_t paSize) {
    return paSize ? ComResult::Overflow : ComChannel::setMinReceiveSize(paSize);
  }
} // namespace forte::com::impl
