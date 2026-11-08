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

#include <cerrno>
#include <cstdlib>

#include <fcntl.h>
#include <unistd.h>

#include <sys/socket.h>

namespace forte::com::impl {
  int Socket::close() noexcept {
    return ::close(release());
  }

  int Socket::setNoSigPipe() const {
#if !defined(MSG_NOSIGNAL) && defined(SO_NOSIGPIPE)
    const int enabled = 1;
    return setsockopt(mSocket, SOL_SOCKET, SO_NOSIGPIPE, &enabled, sizeof(enabled));
#else
    return 0;
#endif
  }

  int Socket::setNonBlocking() const {
    const int flags = fcntl(mSocket, F_GETFL, 0);
    if (flags == -1) {
      return -1;
    }
    if (fcntl(mSocket, F_SETFL, flags | O_NONBLOCK) == -1) {
      return -1;
    }
    return 0;
  }

  void Socket::reset(const int paSocket) noexcept {
    if (mSocket == paSocket) {
      return;
    }
    if (mSocket >= 0) {
      ::close(mSocket);
    }
    mSocket = paSocket;
  }

  SocketChannel::~SocketChannel() {
    std::free(mBuffer.data());
  }

  ComResult SocketChannel::open(const std::string_view paConfigString,
                                const std::span<const ComChannelDescriptor> paDescriptors) {
    if (isOpen()) {
      return ComResult::Ok;
    }
    if (!paDescriptors.empty()) {
      return ComResult::InvalidId;
    }

    auto [result, sock] = socket(paConfigString);
    if (result != ComResult::Ok) {
      return result;
    }

    return setSocket(std::move(sock));
  }

  ComResult SocketChannel::setSocket(Socket paSocket) {
    if (paSocket.setNoSigPipe()) {
      return net::mapSocketError(errno, ComResult::Unknown);
    }
    if (paSocket.setNonBlocking()) {
      return net::mapSocketError(errno, ComResult::Unknown);
    }

    mSocket = std::move(paSocket);
    mBuffer.resize(0);

    return ComResult::Ok;
  }

  ComResult SocketChannel::send(ComBuffer paData) {
    if (!mSocket) {
      return ComResult::NoSocket;
    }
    const ssize_t bytesSent = ::send(mSocket.get(), paData.data(), paData.size(), SocketSendFlags);
    if (bytesSent < 0) {
      return net::mapSocketError(errno, ComResult::SendFailed);
    }
    if (static_cast<std::size_t>(bytesSent) != paData.size()) {
      return ComResult::SendFailed;
    }
    return ComResult::Ok;
  }

  ComResultValue<std::size_t> SocketChannel::recv() {
    const ssize_t bytesReceived =
        ::recv(mSocket.get(), mBuffer.data() + mBuffer.size(), getMaxReceiveSize() - mBuffer.size(), 0);
    if (bytesReceived < 0) {
      return {.mResult = net::mapSocketError(errno, ComResult::RecvFailed)};
    }
    return {.mValue = static_cast<std::size_t>(bytesReceived)};
  }

  ComResult SocketChannel::poll() {
    if (!mSocket) {
      return ComResult::NoSocket;
    }
    if (mBuffer.size() >= getMaxReceiveSize()) {
      return ComResult::WouldBlock;
    }
    const auto [result, bytesReceived] = recv();
    if (result != ComResult::Ok) {
      return result;
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
      return net::mapSocketError(errno, ComResult::Unknown);
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
