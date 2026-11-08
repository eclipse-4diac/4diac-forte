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

#include <cerrno>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <utility>

namespace forte::com::impl::net {
  namespace {
    ComResultValue<Socket> open(const addrinfo *rp) {
      Socket sock{::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)};
      if (!sock) {
        return {.mResult = mapSocketError(errno, ComResult::Unknown)};
      }

      if (rp->ai_flags & AI_PASSIVE) {
        if (bind(sock.get(), rp->ai_addr, rp->ai_addrlen)) {
          return {.mResult = mapSocketError(errno, ComResult::Unknown)};
        }
      } else if (connect(sock.get(), rp->ai_addr, rp->ai_addrlen)) {
        return {.mResult = mapSocketError(errno, ComResult::Unknown)};
      }

      return {.mValue = std::move(sock)};
    }
  } // namespace

  ComResult mapSocketError(const int paError, const ComResult paFallback) noexcept {
#ifdef EADDRINUSE
    if (paError == EADDRINUSE)
      return ComResult::AddressInUse;
#endif
#ifdef EADDRNOTAVAIL
    if (paError == EADDRNOTAVAIL)
      return ComResult::AddressNotAvailable;
#endif
#ifdef ECONNREFUSED
    if (paError == ECONNREFUSED)
      return ComResult::ConnectionRefused;
#endif
#ifdef ECONNRESET
    if (paError == ECONNRESET)
      return ComResult::ConnectionReset;
#endif
#ifdef ECONNABORTED
    if (paError == ECONNABORTED)
      return ComResult::ConnectionAborted;
#endif
#ifdef EPIPE
    if (paError == EPIPE)
      return ComResult::ConnectionClosed;
#endif
#ifdef ESHUTDOWN
    if (paError == ESHUTDOWN)
      return ComResult::ConnectionClosed;
#endif
#ifdef ENETUNREACH
    if (paError == ENETUNREACH)
      return ComResult::NetworkUnreachable;
#endif
#ifdef EHOSTUNREACH
    if (paError == EHOSTUNREACH)
      return ComResult::HostUnreachable;
#endif
#ifdef EHOSTDOWN
    if (paError == EHOSTDOWN)
      return ComResult::HostUnreachable;
#endif
#ifdef ENETDOWN
    if (paError == ENETDOWN)
      return ComResult::NetworkDown;
#endif
#ifdef ENETRESET
    if (paError == ENETRESET)
      return ComResult::NetworkDown;
#endif
#ifdef ETIMEDOUT
    if (paError == ETIMEDOUT)
      return ComResult::TimedOut;
#endif
#ifdef EAGAIN
    if (paError == EAGAIN)
      return ComResult::WouldBlock;
#endif
#ifdef EWOULDBLOCK
    if (paError == EWOULDBLOCK)
      return ComResult::WouldBlock;
#endif
#ifdef EINTR
    if (paError == EINTR)
      return ComResult::Interrupted;
#endif
#ifdef EACCES
    if (paError == EACCES)
      return ComResult::PermissionDenied;
#endif
#ifdef EPERM
    if (paError == EPERM)
      return ComResult::PermissionDenied;
#endif
#ifdef ENOTCONN
    if (paError == ENOTCONN)
      return ComResult::NotConnected;
#endif
#ifdef EMSGSIZE
    if (paError == EMSGSIZE)
      return ComResult::MessageTooLarge;
#endif
#ifdef EBADF
    if (paError == EBADF)
      return ComResult::NoSocket;
#endif
#ifdef ENOTSOCK
    if (paError == ENOTSOCK)
      return ComResult::NoSocket;
#endif
#ifdef EINPROGRESS
    if (paError == EINPROGRESS)
      return ComResult::InProgress;
#endif
    return paFallback;
  }

  ComResult mapResolverError(const int paResolverError, const int paSystemError, const ComResult paFallback) noexcept {
#ifdef EAI_NONAME
    if (paResolverError == EAI_NONAME)
      return ComResult::NameNotResolved;
#endif
#ifdef EAI_AGAIN
    if (paResolverError == EAI_AGAIN)
      return ComResult::NameResolutionTemporaryFailure;
#endif
#ifdef EAI_SYSTEM
    if (paResolverError == EAI_SYSTEM)
      return mapSocketError(paSystemError, paFallback);
#endif
    return paFallback;
  }

  ComResultValue<addrinfo *> resolve(const std::string_view paConfigString, const addrinfo &paHints) {
    const size_t colonPos = paConfigString.rfind(':');
    if (colonPos == std::string_view::npos) {
      return {.mResult = ComResult::InvalidId};
    }

    const auto host = std::string{paConfigString.substr(0, colonPos)};
    const auto portStr = std::string{paConfigString.substr(colonPos + 1)};

    addrinfo *result = nullptr;
    if (const int error = getaddrinfo(host.c_str(), portStr.c_str(), &paHints, &result)) {
      return {.mResult = mapResolverError(error, errno, ComResult::InvalidId)};
    }
    if (!result) {
      return {.mResult = ComResult::InvalidId};
    }
    return {.mValue = result};
  }

  ComResultValue<Socket> open(const std::string_view paConfigString, const addrinfo &paHints) {
    auto [result, addresses] = resolve(paConfigString, paHints);
    if (result != ComResult::Ok) {
      return {.mResult = result};
    }

    ComResultValue<Socket> sock{.mResult = ComResult::InvalidId};
    for (const addrinfo *rp = addresses; rp != nullptr && !sock; rp = rp->ai_next) {
      sock = open(rp);
    }

    freeaddrinfo(addresses);
    return sock;
  }
} // namespace forte::com::impl::net
