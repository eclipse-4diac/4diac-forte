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

#include "udp_listen.h"
#include "net.h"

#include "forte/stringid.h"
#include "forte/com/factory.h"

#include <ws2tcpip.h>

using namespace forte::literals;

namespace forte::com::impl {
  namespace {
    [[maybe_unused]] ComChannelFactory<ComBuffer>::EntryImpl<UDPListenChannel> entry("udp_listen"_STRID);
  }

  SOCKET UDPListenChannel::socket(const std::string_view paConfigString) {
    ADDRINFOA hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE;
    return net::open(paConfigString, hints);
  }

  std::size_t UDPListenChannel::recv() {
    DWORD flags = 0;
    DWORD bytesReceived = 0;
    sockaddr_storage remoteAddr{};
    socklen_t remoteAddrLen = sizeof(remoteAddr);
    WSABUF buffer{.len = static_cast<ULONG>(getMaxReceiveSize() - getBuffer().size()),
                  .buf = reinterpret_cast<CHAR *>(getBuffer().data() + getBuffer().size())};
    if (WSARecvFrom(getSocket(), &buffer, 1, &bytesReceived, &flags, reinterpret_cast<sockaddr *>(&remoteAddr),
                    &remoteAddrLen, nullptr, nullptr)) {
      return static_cast<std::size_t>(-1);
    }
    if (bytesReceived > 0 && !mConnected) {
      if (WSAConnect(getSocket(), reinterpret_cast<sockaddr *>(&remoteAddr), remoteAddrLen, nullptr, nullptr, nullptr,
                     nullptr)) {
        return -1;
      };
      mConnected = true;
    }
    return static_cast<std::size_t>(bytesReceived);
  }
} // namespace forte::com::impl
