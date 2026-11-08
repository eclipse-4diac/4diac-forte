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

#include "udp_listen.h"
#include "net.h"

#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>

#include "forte/stringid.h"
#include "forte/com/factory.h"

using namespace forte::literals;

namespace forte::com::impl {
  namespace {
    [[maybe_unused]] ComChannelFactory<ComBuffer>::EntryImpl<UDPListenChannel> entry("udp_listen"_STRID);
  }

  ComResultValue<Socket> UDPListenChannel::socket(const std::string_view paConfigString) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE;
    return net::open(paConfigString, hints);
  }

  ComResult UDPListenChannel::open(const std::string_view paConfigString,
                                   const std::span<const ComChannelDescriptor> paDescriptors) {
    const ComResult result = SocketChannel::open(paConfigString, paDescriptors);
    if (result == ComResult::Ok) {
      mConnected = false;
    }
    return result;
  }

  ComResult UDPListenChannel::close() {
    const ComResult result = SocketChannel::close();
    mConnected = false;
    return result;
  }

  ComResultValue<std::size_t> UDPListenChannel::recv() {
    sockaddr_storage remoteAddr{};
    socklen_t remoteAddrLen = sizeof(remoteAddr);
    const ssize_t bytesReceived =
        recvfrom(getSocket().get(), getBuffer().data() + getBuffer().size(), getMaxReceiveSize() - getBuffer().size(),
                 0, reinterpret_cast<sockaddr *>(&remoteAddr), &remoteAddrLen);
    if (bytesReceived < 0) {
      return {.mResult = net::mapSocketError(errno, ComResult::RecvFailed)};
    }
    if (!mConnected) {
      if (connect(getSocket().get(), reinterpret_cast<sockaddr *>(&remoteAddr), remoteAddrLen)) {
        return {.mResult = net::mapSocketError(errno, ComResult::RecvFailed)};
      }
      mConnected = true;
    }
    return {.mValue = static_cast<std::size_t>(bytesReceived)};
  }
} // namespace forte::com::impl
