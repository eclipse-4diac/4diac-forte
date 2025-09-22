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
#include "net.h"

namespace forte::com::impl {
  SocketChannel::~SocketChannel() {
    if (mSocket != INVALID_SOCKET) {
      ::closesocket(mSocket);
    }
    std::free(mBuffer.data());
  }

  ComResult SocketChannel::open(const std::string_view paConfigString,
                                const std::span<ComChannelDescriptor> paDescriptors) {
    if (!paDescriptors.empty()) {
      return ComResult::InvalidId;
    }

    const SOCKET sock = socket(paConfigString);
    if (sock == INVALID_SOCKET) {
      return ComResult::InvalidId;
    }

    if (net::setNonBlocking(sock)) {
      return ComResult::Unknown;
    }

    if (mSocket != INVALID_SOCKET) {
      ::closesocket(mSocket);
    }
    mSocket = sock;

    return ComResult::Ok;
  }

  ComResult SocketChannel::send(ComBuffer paData) {
    DWORD bytesSent = 0;
    WSABUF buffer{.len = static_cast<ULONG>(paData.size()), .buf = reinterpret_cast<CHAR *>(paData.data())};
    if (WSASend(mSocket, &buffer, 1, &bytesSent, 0, nullptr, nullptr) ||
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
    if (WSARecv(mSocket, &buffer, 1, &bytesReceived, &flags, nullptr, nullptr)) {
      return static_cast<std::size_t>(-1);
    }
    return static_cast<std::size_t>(bytesReceived);
  }

  ComResult SocketChannel::poll() {
    const std::size_t bytesReceived = recv();
    if (bytesReceived == static_cast<std::size_t>(-1)) {
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
    if (::closesocket(mSocket)) {
      switch (WSAGetLastError()) {
        case WSAENOTSOCK: return ComResult::NoSocket;
        case WSAENETDOWN: return ComResult::SendFailed;
        default: return ComResult::Unknown;
      }
    }
    mSocket = INVALID_SOCKET;
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
