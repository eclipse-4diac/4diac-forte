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

#include "net.h"

#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>

SOCKET forte::com::impl::net::open(const std::string_view paConfigString, const ADDRINFOA &paHints) {
  const size_t colonPos = paConfigString.rfind(':');
  if (colonPos == std::string_view::npos) {
    return -1;
  }

  const auto host = std::string{paConfigString.substr(0, colonPos)};
  const auto portStr = std::string{paConfigString.substr(colonPos + 1)};

  ADDRINFOA *result;
  if (GetAddrInfoA(host.c_str(), portStr.c_str(), &paHints, &result)) {
    return -1;
  }

  for (const ADDRINFOA *rp = result; rp != nullptr; rp = rp->ai_next) {
    const SOCKET sock = WSASocketA(rp->ai_family, rp->ai_socktype, rp->ai_protocol, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (sock == INVALID_SOCKET) {
      continue;
    }

    if ((paHints.ai_flags & AI_PASSIVE ? ::bind(sock, rp->ai_addr, static_cast<int>(rp->ai_addrlen))
                                       : WSAConnect(sock, rp->ai_addr, static_cast<int>(rp->ai_addrlen), nullptr,
                                                    nullptr, nullptr, nullptr)) == 0) {
      FreeAddrInfo(result);
      return sock;
    }

    ::closesocket(sock);
  }

  FreeAddrInfo(result);
  return -1;
}

int forte::com::impl::net::setNonBlocking(const SOCKET paSocket) {
  u_long mode = 1; // 1 to enable non-blocking socket
  if (ioctlsocket(paSocket, FIONBIO, &mode) != 0) {
    return -1;
  }
  return 0;
}
