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

#include <sys/socket.h>
#include <netinet/in.h>

#include "core/stringdict.h"
#include "core/com/factory.h"

USE_STRING_ID(tcp_listen)

using namespace forte::com;
using namespace forte::com::impl;

namespace {
  [[maybe_unused]] ComChannelEntryImpl<ComBuffer, TCPListenChannel> entry(STRID(tcp_listen));
}

int TCPListenChannel::socket(const std::string_view paConfigString) {
  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;
  const int sock = net::open(paConfigString, hints);
  if (sock < 0) {
    return -1;
  }

  if (::listen(sock, 1)) {
    ::close(sock);
    return -1;
  }
  return sock;
}

ComResult TCPListenChannel::send(ComBuffer paData) {
  const ssize_t bytesSent = ::send(mConnectionSocket, paData.data(), paData.size(), MSG_NOSIGNAL);
  if (bytesSent < 0 || static_cast<std::size_t>(bytesSent) != paData.size()) {
    return ComResult::SendFailed;
  }
  return ComResult::Ok;
}

ssize_t TCPListenChannel::recv() {
  if (mConnectionSocket < 0) {
    mConnectionSocket = ::accept(getSocket(), nullptr, nullptr);
    if (mConnectionSocket < 0 || net::setNonBlocking(mConnectionSocket)) {
      return -1;
    }
  }
  return ::recv(mConnectionSocket, getBuffer().data() + getBuffer().size(), getMaxReceiveSize() - getBuffer().size(),
                0);
}
