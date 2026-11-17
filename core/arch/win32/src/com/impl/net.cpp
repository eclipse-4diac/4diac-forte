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
    ComResultValue<Socket> open(const ADDRINFOA *rp, const bool paPassive, const bool paNonBlocking) {
      Socket sock{WSASocketA(rp->ai_family, rp->ai_socktype, rp->ai_protocol, nullptr, 0, WSA_FLAG_OVERLAPPED)};
      if (!sock) {
        return {.mResult = mapSocketError(WSAGetLastError(), ComResult::Unknown)};
      }

      if (paNonBlocking && sock.setNonBlocking()) {
        return {.mResult = mapSocketError(WSAGetLastError(), ComResult::Unknown)};
      }

      if (paPassive) {
        if (bind(sock.get(), rp->ai_addr, static_cast<int>(rp->ai_addrlen))) {
          return {.mResult = mapSocketError(WSAGetLastError(), ComResult::Unknown)};
        }
      } else if (WSAConnect(sock.get(), rp->ai_addr, static_cast<int>(rp->ai_addrlen), nullptr, nullptr, nullptr,
                            nullptr)) {
        if (const int error = WSAGetLastError(); error != WSAEWOULDBLOCK) {
          return {.mResult = mapSocketError(error, ComResult::Unknown)};
        }
        return {.mResult = ComResult::InProgress, .mValue = std::move(sock)};
      }

      return {.mValue = std::move(sock)};
    }
  } // namespace

  ComResult mapSocketError(const int paError, const ComResult paFallback) noexcept {
    if (paError == 0)
      return ComResult::Ok;
#ifdef WSAEADDRINUSE
    if (paError == WSAEADDRINUSE)
      return ComResult::AddressInUse;
#endif
#ifdef WSAEADDRNOTAVAIL
    if (paError == WSAEADDRNOTAVAIL)
      return ComResult::AddressNotAvailable;
#endif
#ifdef WSAECONNREFUSED
    if (paError == WSAECONNREFUSED)
      return ComResult::ConnectionRefused;
#endif
#ifdef WSAECONNRESET
    if (paError == WSAECONNRESET)
      return ComResult::ConnectionReset;
#endif
#ifdef WSAECONNABORTED
    if (paError == WSAECONNABORTED)
      return ComResult::ConnectionAborted;
#endif
#ifdef WSAESHUTDOWN
    if (paError == WSAESHUTDOWN)
      return ComResult::ConnectionClosed;
#endif
#ifdef WSAENETUNREACH
    if (paError == WSAENETUNREACH)
      return ComResult::NetworkUnreachable;
#endif
#ifdef WSAEHOSTUNREACH
    if (paError == WSAEHOSTUNREACH)
      return ComResult::HostUnreachable;
#endif
#ifdef WSAEHOSTDOWN
    if (paError == WSAEHOSTDOWN)
      return ComResult::HostUnreachable;
#endif
#ifdef WSAENETDOWN
    if (paError == WSAENETDOWN)
      return ComResult::NetworkDown;
#endif
#ifdef WSAENETRESET
    if (paError == WSAENETRESET)
      return ComResult::NetworkDown;
#endif
#ifdef WSAETIMEDOUT
    if (paError == WSAETIMEDOUT)
      return ComResult::TimedOut;
#endif
#ifdef WSAEWOULDBLOCK
    if (paError == WSAEWOULDBLOCK)
      return ComResult::WouldBlock;
#endif
#ifdef WSAEINTR
    if (paError == WSAEINTR)
      return ComResult::Interrupted;
#endif
#ifdef WSAEACCES
    if (paError == WSAEACCES)
      return ComResult::PermissionDenied;
#endif
#ifdef WSAENOTCONN
    if (paError == WSAENOTCONN)
      return ComResult::NotConnected;
#endif
#ifdef WSAEMSGSIZE
    if (paError == WSAEMSGSIZE)
      return ComResult::MessageTooLarge;
#endif
#ifdef WSAEBADF
    if (paError == WSAEBADF)
      return ComResult::NoSocket;
#endif
#ifdef WSAENOTSOCK
    if (paError == WSAENOTSOCK)
      return ComResult::NoSocket;
#endif
#ifdef WSAEINPROGRESS
    if (paError == WSAEINPROGRESS)
      return ComResult::InProgress;
#endif
    return paFallback;
  }

  ComResult mapResolverError(const int paResolverError, const ComResult paFallback) noexcept {
#ifdef WSAHOST_NOT_FOUND
    if (paResolverError == WSAHOST_NOT_FOUND)
      return ComResult::NameNotResolved;
#endif
#ifdef WSATRY_AGAIN
    if (paResolverError == WSATRY_AGAIN)
      return ComResult::NameResolutionTemporaryFailure;
#endif
    return paFallback;
  }

  ComResultValue<ADDRINFOA *> resolve(const std::string_view paConfigString, const ADDRINFOA &paHints) {
    const size_t colonPos = paConfigString.rfind(':');
    if (colonPos == std::string_view::npos) {
      return {.mResult = ComResult::InvalidId};
    }

    const auto host = std::string{paConfigString.substr(0, colonPos)};
    const auto portStr = std::string{paConfigString.substr(colonPos + 1)};

    ADDRINFOA *result = nullptr;
    if (const int error = GetAddrInfoA(host.c_str(), portStr.c_str(), &paHints, &result)) {
      return {.mResult = mapResolverError(error, ComResult::InvalidId)};
    }
    if (!result) {
      return {.mResult = ComResult::InvalidId};
    }
    return {.mValue = result};
  }

  ComResultValue<Socket> open(const ADDRINFOA *paAddresses, const bool paPassive) {
    ComResultValue<Socket> sock{.mResult = ComResult::InvalidId};
    for (const ADDRINFOA *rp = paAddresses; rp != nullptr && !sock; rp = rp->ai_next) {
      sock = open(rp, paPassive, false);
    }
    return sock;
  }

  ComResultValue<Socket> open(const std::string_view paConfigString, const ADDRINFOA &paHints) {
    auto [result, addresses] = resolve(paConfigString, paHints);
    if (result != ComResult::Ok) {
      return {.mResult = result};
    }

    auto sock = open(addresses, (paHints.ai_flags & AI_PASSIVE) != 0);
    FreeAddrInfoA(addresses);
    return sock;
  }

  ParallelConnectionState<Socket> openParallel(const ADDRINFOA *paAddresses, const bool paPassive) {
    ParallelConnectionState<Socket> state;
    for (const ADDRINFOA *rp = paAddresses; rp != nullptr; rp = rp->ai_next) {
      state.addCandidate(open(rp, paPassive, true));
    }
    return state;
  }

  ComResultValue<ParallelConnectionState<Socket>> openParallel(const std::string_view paConfigString,
                                                               const ADDRINFOA &paHints) {
    auto [result, addresses] = resolve(paConfigString, paHints);
    if (result != ComResult::Ok) {
      return {.mResult = result};
    }

    auto state = openParallel(addresses, (paHints.ai_flags & AI_PASSIVE) != 0);
    FreeAddrInfoA(addresses);
    return {.mValue = std::move(state)};
  }

  ComResultValue<Socket> pollParallel(ParallelConnectionState<Socket> &paState) {
    if (paState.hasWinner()) {
      return {.mValue = paState.takeWinner()};
    }
    if (!paState.hasPendingCandidates()) {
      return {.mResult = paState.result()};
    }

    for (std::size_t candidateIndex = 0; candidateIndex < paState.candidateCount(); ++candidateIndex) {
      const Socket &candidate = paState.candidateAt(candidateIndex);
      if (!candidate) {
        continue;
      }

      fd_set writeSet;
      fd_set errorSet;
      FD_ZERO(&writeSet);
      FD_ZERO(&errorSet);
      FD_SET(candidate.get(), &writeSet);
      FD_SET(candidate.get(), &errorSet);
      timeval timeout{};
      const int result = ::select(0, nullptr, &writeSet, &errorSet, &timeout);
      if (result == SOCKET_ERROR) {
        return {.mResult = mapSocketError(WSAGetLastError(), ComResult::Unknown)};
      }
      if (result == 0) {
        continue;
      }

      int socketError = 0;
      int socketErrorSize = sizeof(socketError);
      if (getsockopt(candidate.get(), SOL_SOCKET, SO_ERROR, reinterpret_cast<char *>(&socketError), &socketErrorSize) ==
          SOCKET_ERROR) {
        paState.updateCandidate(candidateIndex, mapSocketError(WSAGetLastError(), ComResult::Unknown));
        continue;
      }
      paState.updateCandidate(candidateIndex, mapSocketError(socketError, ComResult::Unknown));
      if (paState.hasWinner()) {
        return {.mValue = paState.takeWinner()};
      }
    }

    return {.mResult = paState.result()};
  }
} // namespace forte::com::impl::net
