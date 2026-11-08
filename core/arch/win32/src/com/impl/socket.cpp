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

#include "socket.h"

#include <cstdlib>

namespace forte::com::impl {
  int Socket::close() noexcept {
    return ::closesocket(release());
  }

  int Socket::setNonBlocking() const {
    u_long mode = 1;
    return ioctlsocket(mSocket, FIONBIO, &mode);
  }

  void Socket::reset(const SOCKET paSocket) noexcept {
    if (mSocket == paSocket) {
      return;
    }
    if (mSocket != INVALID_SOCKET) {
      ::closesocket(mSocket);
    }
    mSocket = paSocket;
  }

  SocketChannel::~SocketChannel() {
    std::free(mBuffer.data());
  }

  ComResult SocketChannel::open(const std::string_view paConfigString,
                                const std::span<const ComChannelDescriptor> paDescriptors) {
    if (!paDescriptors.empty()) {
      return ComResult::InvalidId;
    }

    Socket sock = socket(paConfigString);
    if (!sock) {
      return ComResult::InvalidId;
    }

    return setSocket(std::move(sock));
  }

  ComResult SocketChannel::setSocket(Socket paSocket) {
    if (paSocket.setNonBlocking()) {
      return ComResult::Unknown;
    }

    mSocket = std::move(paSocket);
    mBuffer.resize(0);

    return ComResult::Ok;
  }

  ComResult SocketChannel::send(ComBuffer paData) {
    DWORD bytesSent = 0;
    WSABUF buffer{.len = static_cast<ULONG>(paData.size()), .buf = reinterpret_cast<CHAR *>(paData.data())};
    if (WSASend(mSocket.get(), &buffer, 1, &bytesSent, 0, nullptr, nullptr) ||
        static_cast<std::size_t>(bytesSent) != paData.size()) {
      return ComResult::SendFailed;
    }
    return ComResult::Ok;
  }

  std::size_t SocketChannel::recv() {
    DWORD flags = 0;
    DWORD bytesReceived = 0;
    WSABUF buffer{.len = static_cast<ULONG>(getMaxReceiveSize() - mBuffer.size()),
                  .buf = reinterpret_cast<CHAR *>(mBuffer.data() + mBuffer.size())};
    if (WSARecv(mSocket.get(), &buffer, 1, &bytesReceived, &flags, nullptr, nullptr)) {
      return static_cast<std::size_t>(-1);
    }
    return static_cast<std::size_t>(bytesReceived);
  }

  ComResult SocketChannel::poll() {
    const std::size_t bytesReceived = recv();
    if (bytesReceived == 0 || bytesReceived == static_cast<std::size_t>(-1)) {
      return ComResult::RecvFailed;
    }
    mBuffer.resize(mBuffer.size() + bytesReceived);
    if (mBuffer.size() < getMinReceiveSize()) {
      return ComResult::More;
    }
    if (getObserver().receive(mBuffer, *this) != ComResult::More) {
      mBuffer.resize(0); // clear buffer after receive, unless more data requested
    }
    return ComResult::Async;
  }

  ComResult SocketChannel::close() {
    if (!mSocket) {
      return ComResult::NoSocket;
    }
    if (mSocket.close()) {
      switch (WSAGetLastError()) {
        case WSAENOTSOCK: return ComResult::NoSocket;
        case WSAENETDOWN: return ComResult::SendFailed;
        default: return ComResult::Unknown;
      }
    }
    return ComResult::Ok;
  }

  ComResult SocketChannel::setMaxReceiveSize(const std::size_t paSize) {
    if (paSize > mBuffer.capacity()) {
      auto *data = static_cast<TForteByte *>(std::realloc(mBuffer.data(), paSize));
      if (!data) {
        return ComResult::Overflow;
      }
      mBuffer = ComBuffer(data, paSize, mBuffer.size());
    }
    return ComChannel::setMaxReceiveSize(paSize);
  }
} // namespace forte::com::impl
