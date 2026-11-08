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
#include <utility>

namespace forte::com::impl::net {
  namespace {
    Socket open(const addrinfo *rp) {
      Socket sock{::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)};
      if (!sock) {
        return {};
      }

      if (rp->ai_flags & AI_PASSIVE) {
        if (bind(sock.get(), rp->ai_addr, rp->ai_addrlen)) {
          return {};
        }
      } else if (connect(sock.get(), rp->ai_addr, rp->ai_addrlen)) {
        return {};
      }

      return sock;
    }
  } // namespace

  addrinfo *resolve(const std::string_view paConfigString, const addrinfo &paHints) {
    const size_t colonPos = paConfigString.rfind(':');
    if (colonPos == std::string_view::npos) {
      return nullptr;
    }

    const auto host = std::string{paConfigString.substr(0, colonPos)};
    const auto portStr = std::string{paConfigString.substr(colonPos + 1)};

    addrinfo *result = nullptr;
    if (getaddrinfo(host.c_str(), portStr.c_str(), &paHints, &result)) {
      return nullptr;
    }
    return result;
  }

  Socket open(const std::string_view paConfigString, const addrinfo &paHints) {
    addrinfo *result = resolve(paConfigString, paHints);
    if (!result) {
      return {};
    }

    Socket sock;
    for (const addrinfo *rp = result; rp != nullptr && !sock; rp = rp->ai_next) {
      sock = open(rp);
    }

    freeaddrinfo(result);
    return sock;
  }
} // namespace forte::com::impl::net
