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

#include <fcntl.h>
#include <unistd.h>

#include <sys/socket.h>

namespace forte::com::impl {
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

  int Socket::close() noexcept {
    return ::close(release());
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
    const ssize_t bytesSent = ::send(mSocket.get(), paData.data(), paData.size(), MSG_NOSIGNAL);
    if (bytesSent < 0 || static_cast<std::size_t>(bytesSent) != paData.size()) {
      return ComResult::SendFailed;
    }
    return ComResult::Ok;
  }

  ssize_t SocketChannel::recv() {
    return ::recv(mSocket.get(), mBuffer.data() + mBuffer.size(), getMaxReceiveSize() - mBuffer.size(), 0);
  }

  ComResult SocketChannel::poll() {
    const ssize_t bytesReceived = recv();
    if (bytesReceived <= 0) {
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
      switch (errno) {
        case EBADF: return ComResult::NoSocket;
        case EIO: return ComResult::SendFailed;
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
