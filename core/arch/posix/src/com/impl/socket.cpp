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

#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

namespace forte::com::impl {
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

  SocketChannel::~SocketChannel() {
    if (mSocket >= 0) {
      ::close(mSocket);
    }
    std::free(mBuffer.data());
  }

  ComResult SocketChannel::open(const std::string_view paConfigString,
                                const std::span<ComChannelDescriptor> paDescriptors) {
    if (!paDescriptors.empty()) {
      return ComResult::InvalidId;
    }

    const int sock = socket(paConfigString);
    if (sock < 0) {
      return ComResult::InvalidId;
    }

    if (net::setNonBlocking(sock)) {
      return ComResult::Unknown;
    }

    if (mSocket >= 0) {
      ::close(mSocket);
    }
    mSocket = sock;

    return ComResult::Ok;
  }

  ComResult SocketChannel::send(ComBuffer paData) {
    const ssize_t bytesSent = ::send(mSocket, paData.data(), paData.size(), MSG_NOSIGNAL);
    if (bytesSent < 0 || static_cast<std::size_t>(bytesSent) != paData.size()) {
      return ComResult::SendFailed;
    }
    return ComResult::Ok;
  }

  ssize_t SocketChannel::recv() {
    return ::recv(mSocket, mBuffer.data() + mBuffer.size(), getMaxReceiveSize() - mBuffer.size(), 0);
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
    if (::close(mSocket)) {
      switch (errno) {
        case EBADF: return ComResult::NoSocket;
        case EIO: return ComResult::SendFailed;
        default: return ComResult::Unknown;
      }
    };
    mSocket = -1;
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
