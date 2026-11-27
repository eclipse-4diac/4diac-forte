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

  bool TCPListenChannel::supports(const std::span<const ComChannelDescriptor> paDescriptors) const {
    return supportsChannel("tcp_listen"_STRID, paDescriptors);
  }

  ComResultValue<Socket> TCPListenChannel::socket(const std::string_view paConfigString) {
    ADDRINFOA hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    ComResultValue<Socket> sock = net::open(paConfigString, hints);
    if (!sock) {
      return sock;
    }

    if (::listen(sock.mValue.get(), 1)) {
      return {.mResult = net::mapSocketError(WSAGetLastError(), ComResult::InvalidId)};
    }
    return sock;
  }

  ComResult TCPListenChannel::send(ComBuffer paData) {
    if (!mConnectionSocket) {
      return ComResult::NoSocket;
    }
    DWORD bytesSent = 0;
    WSABUF buffer{.len = static_cast<ULONG>(paData.size()), .buf = reinterpret_cast<CHAR *>(paData.data())};
    if (WSASend(mConnectionSocket.get(), &buffer, 1, &bytesSent, 0, nullptr, nullptr)) {
      const auto result = net::mapSocketError(WSAGetLastError(), ComResult::SendFailed);
      if (result != ComResult::WouldBlock && result != ComResult::Interrupted) {
        resetConnection();
      }
      return result;
    }
    if (static_cast<std::size_t>(bytesSent) != paData.size()) {
      resetConnection();
      return ComResult::SendFailed;
    }
    return ComResult::Ok;
  }

  ComResult TCPListenChannel::close() {
    auto connectionResult = ComResult::Ok;
    if (mConnectionSocket) {
      if (mConnectionSocket.close()) {
        connectionResult = net::mapSocketError(WSAGetLastError(), ComResult::Unknown);
      }
    }

    if (const ComResult result = SocketChannel::close(); result != ComResult::Ok) {
      return result;
    }
    return connectionResult;
  }

  ComResultValue<std::size_t> TCPListenChannel::recv() {
    if (!mConnectionSocket) {
      Socket connectionSocket{WSAAccept(getSocket().get(), nullptr, nullptr, nullptr, 0)};
      if (!connectionSocket) {
        return {.mResult = net::mapSocketError(WSAGetLastError(), ComResult::RecvFailed)};
      }
      if (connectionSocket.setNonBlocking()) {
        return {.mResult = net::mapSocketError(WSAGetLastError(), ComResult::Unknown)};
      }
      mConnectionSocket = std::move(connectionSocket);
    }
    if (getBuffer().size() >= getMaxReceiveSize()) {
      const auto result = net::probeStreamReceive(mConnectionSocket);
      if (result != ComResult::WouldBlock && result != ComResult::Interrupted) {
        resetConnection();
      }
      return {.mResult = result};
    }
    DWORD flags = 0;
    DWORD bytesReceived = 0;
    WSABUF buffer{.len = static_cast<ULONG>(getMaxReceiveSize() - getBuffer().size()),
                  .buf = reinterpret_cast<CHAR *>(getBuffer().data() + getBuffer().size())};
    if (WSARecv(mConnectionSocket.get(), &buffer, 1, &bytesReceived, &flags, nullptr, nullptr)) {
      const auto result = net::mapSocketError(WSAGetLastError(), ComResult::RecvFailed);
      if (result != ComResult::WouldBlock && result != ComResult::Interrupted) {
        resetConnection();
      }
      return {.mResult = result};
    }
    if (bytesReceived == 0) {
      resetConnection();
      return {.mResult = ComResult::RecvFailed};
    }
    return {.mValue = static_cast<std::size_t>(bytesReceived)};
  }

  void TCPListenChannel::resetConnection() {
    mConnectionSocket.reset();
    getBuffer().resize(0);
  }
} // namespace forte::com::impl
