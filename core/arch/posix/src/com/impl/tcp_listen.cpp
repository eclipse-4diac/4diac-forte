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

  Socket TCPListenChannel::socket(const std::string_view paConfigString) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    Socket sock = net::open(paConfigString, hints);
    if (!sock) {
      return {};
    }

    if (::listen(sock.get(), 1)) {
      return {};
    }
    return sock;
  }

  ComResult TCPListenChannel::open(const std::string_view paConfigString,
                                   const std::span<const ComChannelDescriptor> paDescriptors) {
    mConnectionSocket.reset();
    return SocketChannel::open(paConfigString, paDescriptors);
  }

  ComResult TCPListenChannel::send(ComBuffer paData) {
    const ssize_t bytesSent = ::send(mConnectionSocket.get(), paData.data(), paData.size(), MSG_NOSIGNAL);
    if (bytesSent < 0 || static_cast<std::size_t>(bytesSent) != paData.size()) {
      return ComResult::SendFailed;
    }
    return ComResult::Ok;
  }

  ComResult TCPListenChannel::close() {
    auto connectionResult = ComResult::Ok;
    if (mConnectionSocket) {
      if (mConnectionSocket.close()) {
        switch (errno) {
          case EBADF: connectionResult = ComResult::NoSocket; break;
          case EIO: connectionResult = ComResult::SendFailed; break;
          default: connectionResult = ComResult::Unknown; break;
        }
      }
    }

    if (const ComResult result = SocketChannel::close(); result != ComResult::Ok) {
      return result;
    }
    return connectionResult;
  }

  ssize_t TCPListenChannel::recv() {
    if (!mConnectionSocket) {
      Socket connectionSocket{accept(getSocket().get(), nullptr, nullptr)};
      if (!connectionSocket) {
        return -1;
      }
      if (connectionSocket.setNonBlocking()) {
        return -1;
      }
      mConnectionSocket = std::move(connectionSocket);
    }
    return ::recv(mConnectionSocket.get(), getBuffer().data() + getBuffer().size(),
                  getMaxReceiveSize() - getBuffer().size(), 0);
  }
} // namespace forte::com::impl
