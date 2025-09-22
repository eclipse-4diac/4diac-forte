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
                              const std::span<ComChannelDescriptor> paDescriptors) {
    return paConfigString.empty() && paDescriptors.empty() ? ComResult::Ok : ComResult::InvalidId;
  }

  ComResult NullChannel::send(ComBuffer) {
    return ComResult::Ok;
  }

  ComResult NullChannel::poll() {
    getObserver().receive({nullptr, 0}, *this);
    return ComResult::Async;
  }

  ComResult NullChannel::close() {
    return ComResult::Ok;
  }

  ComResult NullChannel::setMinReceiveSize(const std::size_t paSize) {
    return paSize ? ComResult::Overflow : ComChannel::setMinReceiveSize(paSize);
  }
} // namespace forte::com::impl
