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

#include <fcntl.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int forte::com::impl::net::open(const std::string_view paConfigString, const addrinfo &paHints) {
  const size_t colonPos = paConfigString.rfind(':');
  if (colonPos == std::string_view::npos) {
    return -1;
  }

  const auto host = std::string{paConfigString.substr(0, colonPos)};
  const auto portStr = std::string{paConfigString.substr(colonPos + 1)};

  addrinfo *result;
  if (getaddrinfo(host.c_str(), portStr.c_str(), &paHints, &result)) {
    return -1;
  }

  for (const addrinfo *rp = result; rp != nullptr; rp = rp->ai_next) {
    const int sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sock < 0) {
      continue;
    }

    if ((rp->ai_flags & AI_PASSIVE ? bind(sock, rp->ai_addr, rp->ai_addrlen)
                                   : connect(sock, rp->ai_addr, rp->ai_addrlen)) == 0) {
      freeaddrinfo(result);
      return sock;
    }

    close(sock);
  }

  freeaddrinfo(result);
  return -1;
}

int forte::com::impl::net::setNonBlocking(const int paSocket) {
  const int flags = fcntl(paSocket, F_GETFL, 0);
  if (flags == -1) {
    return -1;
  }
  if (fcntl(paSocket, F_SETFL, flags | O_NONBLOCK) == -1) {
    return -1;
  }
  return 0;
}
