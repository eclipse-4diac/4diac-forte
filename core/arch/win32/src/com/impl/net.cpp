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
#include <utility>

#include <winsock2.h>
#include <ws2tcpip.h>

namespace forte::com::impl::net {
  namespace {
    Socket open(const ADDRINFOA *rp, const bool paPassive) {
      Socket sock{WSASocketA(rp->ai_family, rp->ai_socktype, rp->ai_protocol, nullptr, 0, WSA_FLAG_OVERLAPPED)};
      if (!sock) {
        return {};
      }

      if (paPassive) {
        if (bind(sock.get(), rp->ai_addr, static_cast<int>(rp->ai_addrlen))) {
          return {};
        }
      } else if (WSAConnect(sock.get(), rp->ai_addr, static_cast<int>(rp->ai_addrlen), nullptr, nullptr, nullptr,
                            nullptr)) {
        return {};
      }

      return sock;
    }
  } // namespace

  ADDRINFOA *resolve(const std::string_view paConfigString, const ADDRINFOA &paHints) {
    const size_t colonPos = paConfigString.rfind(':');
    if (colonPos == std::string_view::npos) {
      return nullptr;
    }

    const auto host = std::string{paConfigString.substr(0, colonPos)};
    const auto portStr = std::string{paConfigString.substr(colonPos + 1)};

    ADDRINFOA *result = nullptr;
    if (GetAddrInfoA(host.c_str(), portStr.c_str(), &paHints, &result)) {
      return nullptr;
    }
    return result;
  }

  Socket open(const std::string_view paConfigString, const ADDRINFOA &paHints) {
    ADDRINFOA *result = resolve(paConfigString, paHints);
    if (!result) {
      return {};
    }

    Socket sock;
    for (const ADDRINFOA *rp = result; rp != nullptr && !sock; rp = rp->ai_next) {
      sock = open(rp, paHints.ai_flags & AI_PASSIVE);
    }

    FreeAddrInfo(result);
    return sock;
  }
} // namespace forte::com::impl::net
