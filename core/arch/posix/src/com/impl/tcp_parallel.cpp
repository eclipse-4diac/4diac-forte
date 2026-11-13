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

#include "tcp_parallel.h"
#include "net.h"

#include <cerrno>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <utility>
#include <vector>

#include "forte/stringid.h"
#include "forte/com/factory.h"

using namespace forte::literals;

namespace forte::com::impl {
  namespace {
    [[maybe_unused]] ComChannelFactory<ComBuffer>::EntryImpl<TCPParallelChannel> entry("tcp_parallel"_STRID);
  } // namespace

  bool TCPParallelChannel::supports(const std::span<const ComChannelDescriptor> paDescriptors) const {
    return supportsChannel("tcp_parallel"_STRID, paDescriptors);
  }

  ComResultValue<Socket> TCPParallelChannel::socket(const std::string_view paConfigString) {
    if (!mConnectionState.hasPendingCandidates()) {
      addrinfo hints{};
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;
      auto [result, state] = net::openParallel(paConfigString, hints);
      if (result != ComResult::Ok) {
        return {.mResult = result};
      }
      mConnectionState = std::move(state);
    }

    auto result = net::pollParallel(mConnectionState);
    if (result.mResult != ComResult::Ok && result.mResult != ComResult::InProgress &&
        result.mResult != ComResult::Interrupted) {
      mConnectionState.clear();
    }
    return result;
  }

  ComResult TCPParallelChannel::close() {
    mConnectionState.clear();
    return SocketChannel::close();
  }
} // namespace forte::com::impl
