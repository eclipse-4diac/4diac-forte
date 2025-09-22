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

#include <sys/socket.h>
#include <netinet/in.h>

#include "forte/stringid.h"
#include "forte/com/factory.h"

using namespace forte::literals;

namespace forte::com::impl {
  namespace {
    [[maybe_unused]] ComChannelFactory<ComBuffer>::EntryImpl<UDPListenChannel> entry("udp_listen"_STRID);
  }

  int UDPListenChannel::socket(const std::string_view paConfigString) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE;
    return net::open(paConfigString, hints);
  }

  ssize_t UDPListenChannel::recv() {
    sockaddr_storage remoteAddr{};
    socklen_t remoteAddrLen = sizeof(remoteAddr);
    const ssize_t bytesReceived =
        recvfrom(getSocket(), getBuffer().data() + getBuffer().size(), getMaxReceiveSize() - getBuffer().size(), 0,
                 reinterpret_cast<sockaddr *>(&remoteAddr), &remoteAddrLen);
    if (bytesReceived > 0 && !mConnected) {
      if (connect(getSocket(), reinterpret_cast<sockaddr *>(&remoteAddr), remoteAddrLen)) {
        return -1;
      };
      mConnected = true;
    }
    return bytesReceived;
  }
} // namespace forte::com::impl
