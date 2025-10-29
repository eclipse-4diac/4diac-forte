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

#include "forte/stringid.h"
#include "forte/com/factory.h"

using namespace forte::literals;

namespace forte::com::impl {
  namespace {
    [[maybe_unused]] ComChannelFactory<ComBuffer>::EntryImpl<TCPListenChannel> entry("tcp_listen"_STRID);
  }

  SOCKET TCPListenChannel::socket(const std::string_view paConfigString) {
    ADDRINFOA hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    const SOCKET sock = net::open(paConfigString, hints);
    if (sock == INVALID_SOCKET) {
      return INVALID_SOCKET;
    }

    if (::listen(sock, 1)) {
      ::closesocket(sock);
      return INVALID_SOCKET;
    }
    return sock;
  }

  ComResult TCPListenChannel::send(ComBuffer paData) {
    DWORD bytesSent = 0;
    WSABUF buffer{.len = static_cast<ULONG>(paData.size()), .buf = reinterpret_cast<CHAR *>(paData.data())};
    if (WSASend(mConnectionSocket, &buffer, 1, &bytesSent, 0, nullptr, nullptr) ||
        static_cast<std::size_t>(bytesSent) != paData.size()) {
      return ComResult::SendFailed;
    }
    return ComResult::Ok;
  }

  std::size_t TCPListenChannel::recv() {
    if (mConnectionSocket == INVALID_SOCKET) {
      mConnectionSocket = WSAAccept(getSocket(), nullptr, nullptr, nullptr, 0);
      if (mConnectionSocket == INVALID_SOCKET || net::setNonBlocking(mConnectionSocket)) {
        return static_cast<std::size_t>(-1);
      }
    }
    DWORD flags = 0;
    DWORD bytesReceived = 0;
    WSABUF buffer{.len = static_cast<ULONG>(getMaxReceiveSize() - getBuffer().size()),
                  .buf = reinterpret_cast<CHAR *>(getBuffer().data() + getBuffer().size())};
    if (WSARecv(mConnectionSocket, &buffer, 1, &bytesReceived, &flags, nullptr, nullptr)) {
      return static_cast<std::size_t>(-1);
    }
    return static_cast<std::size_t>(bytesReceived);
  }
} // namespace forte::com::impl
