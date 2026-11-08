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

#include "tcp_listen.h"
#include "net.h"

#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>

#include "forte/stringid.h"
#include "forte/com/factory.h"

using namespace forte::literals;

namespace forte::com::impl {
  namespace {
    [[maybe_unused]] ComChannelFactory<ComBuffer>::EntryImpl<TCPListenChannel> entry("tcp_listen"_STRID);
  }

  bool TCPListenChannel::supports(const std::span<const ComChannelDescriptor> paDescriptors) const {
    return supportsChannel("tcp_listen"_STRID, paDescriptors);
  }

  ComResultValue<Socket> TCPListenChannel::socket(const std::string_view paConfigString) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    ComResultValue<Socket> sock = net::open(paConfigString, hints);
    if (!sock) {
      return sock;
    }

    if (::listen(sock.mValue.get(), 1)) {
      return {.mResult = net::mapSocketError(errno, ComResult::InvalidId)};
    }
    return sock;
  }

  ComResult TCPListenChannel::send(ComBuffer paData) {
    if (!mConnectionSocket) {
      return ComResult::NoSocket;
    }
    const ssize_t bytesSent = ::send(mConnectionSocket.get(), paData.data(), paData.size(), MSG_NOSIGNAL);
    if (bytesSent < 0) {
      return net::mapSocketError(errno, ComResult::SendFailed);
    }
    if (static_cast<std::size_t>(bytesSent) != paData.size()) {
      return ComResult::SendFailed;
    }
    return ComResult::Ok;
  }

  ComResult TCPListenChannel::close() {
    auto connectionResult = ComResult::Ok;
    if (mConnectionSocket) {
      if (mConnectionSocket.close()) {
        connectionResult = net::mapSocketError(errno, ComResult::Unknown);
      }
    }

    if (const ComResult result = SocketChannel::close(); result != ComResult::Ok) {
      return result;
    }
    return connectionResult;
  }

  ComResultValue<std::size_t> TCPListenChannel::recv() {
    if (!mConnectionSocket) {
      Socket connectionSocket{accept(getSocket().get(), nullptr, nullptr)};
      if (!connectionSocket) {
        return {.mResult = net::mapSocketError(errno, ComResult::RecvFailed)};
      }
      if (connectionSocket.setNonBlocking()) {
        return {.mResult = net::mapSocketError(errno, ComResult::Unknown)};
      }
      mConnectionSocket = std::move(connectionSocket);
    }
    const ssize_t bytesReceived = ::recv(mConnectionSocket.get(), getBuffer().data() + getBuffer().size(),
                                         getMaxReceiveSize() - getBuffer().size(), 0);
    if (bytesReceived < 0) {
      return {.mResult = net::mapSocketError(errno, ComResult::RecvFailed)};
    }
    if (bytesReceived == 0) {
      return {.mResult = ComResult::RecvFailed};
    }
    return {.mValue = static_cast<std::size_t>(bytesReceived)};
  }
} // namespace forte::com::impl
