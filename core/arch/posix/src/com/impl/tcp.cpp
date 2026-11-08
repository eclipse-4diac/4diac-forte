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

#include "tcp.h"
#include "net.h"

#include <sys/socket.h>
#include <netinet/in.h>

#include "forte/stringid.h"
#include "forte/com/factory.h"

using namespace forte::literals;

namespace forte::com::impl {
  namespace {
    [[maybe_unused]] ComChannelFactory<ComBuffer>::EntryImpl<TCPChannel> entry("tcp"_STRID);
  }

  ComResultValue<Socket> TCPChannel::socket(const std::string_view paConfigString) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    return net::open(paConfigString, hints);
  }

  ComResultValue<std::size_t> TCPChannel::recv() {
    auto result = SocketChannel::recv();
    if (result.mResult == ComResult::Ok && result.mValue == 0) {
      result.mResult = ComResult::RecvFailed; // socket closed
    }
    return result;
  }
} // namespace forte::com::impl
