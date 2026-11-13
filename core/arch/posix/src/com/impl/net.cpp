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

#include "../../../../common/src/com/impl/parallel_state.h"

#include <cerrno>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <utility>
#include <poll.h>

namespace forte::com::impl::net {
  namespace {
    ComResultValue<Socket> open(const addrinfo *rp, const bool paPassive, const bool paNonBlocking) {
      Socket sock{::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)};
      if (!sock) {
        return {.mResult = mapSocketError(errno, ComResult::Unknown)};
      }

      if (paNonBlocking && sock.setNonBlocking()) {
        return {.mResult = mapSocketError(errno, ComResult::Unknown)};
      }

      if (paPassive) {
        if (bind(sock.get(), rp->ai_addr, rp->ai_addrlen)) {
          return {.mResult = mapSocketError(errno, ComResult::Unknown)};
        }
      } else if (connect(sock.get(), rp->ai_addr, rp->ai_addrlen)) {
        if (const int error = errno; error != EINPROGRESS && !(paNonBlocking && error == EINTR)) {
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

  ComResultValue<Socket> open(const addrinfo *paAddresses, const bool paPassive) {
    ComResultValue<Socket> sock{.mResult = ComResult::InvalidId};
    for (const addrinfo *rp = paAddresses; rp != nullptr && !sock; rp = rp->ai_next) {
      sock = open(rp, paPassive, false);
    }
    return sock;
  }

  ComResultValue<Socket> open(const std::string_view paConfigString, const addrinfo &paHints) {
    auto [result, addresses] = resolve(paConfigString, paHints);
    if (result != ComResult::Ok) {
      return {.mResult = result};
    }

    auto sock = open(addresses, (paHints.ai_flags & AI_PASSIVE) != 0);
    freeaddrinfo(addresses);
    return sock;
  }

  ParallelConnectionState<Socket> openParallel(const addrinfo *paAddresses, const bool paPassive) {
    ParallelConnectionState<Socket> state;
    for (const addrinfo *rp = paAddresses; rp != nullptr; rp = rp->ai_next) {
      state.addCandidate(open(rp, paPassive, true));
    }
    return state;
  }

  ComResultValue<ParallelConnectionState<Socket>> openParallel(const std::string_view paConfigString,
                                                               const addrinfo &paHints) {
    auto [result, addresses] = resolve(paConfigString, paHints);
    if (result != ComResult::Ok) {
      return {.mResult = result};
    }

    auto state = openParallel(addresses, (paHints.ai_flags & AI_PASSIVE) != 0);
    freeaddrinfo(addresses);
    return {.mValue = std::move(state)};
  }

  ComResultValue<Socket> pollParallel(ParallelConnectionState<Socket> &paState) {
    if (paState.hasWinner()) {
      return {.mValue = paState.takeWinner()};
    }
    if (!paState.hasPendingCandidates()) {
      return {.mResult = paState.result()};
    }
    std::vector<pollfd> descriptors;
    std::vector<std::size_t> candidateIndices;
    descriptors.reserve(paState.candidateCount());
    candidateIndices.reserve(paState.candidateCount());
    for (std::size_t candidateIndex = 0; candidateIndex < paState.candidateCount(); ++candidateIndex) {
      const Socket &candidate = paState.candidateAt(candidateIndex);
      if (candidate) {
        descriptors.push_back({.fd = candidate.get(), .events = POLLIN | POLLOUT, .revents = 0});
        candidateIndices.push_back(candidateIndex);
      }
    }

    const int result = ::poll(descriptors.data(), static_cast<nfds_t>(descriptors.size()), 0);
    if (result < 0) {
      const auto error = mapSocketError(errno, ComResult::Unknown);
      return {.mResult = error == ComResult::WouldBlock ? ComResult::InProgress : error};
    }
    if (result == 0) {
      return {.mResult = paState.result()};
    }

    for (std::size_t i = 0; i < descriptors.size(); ++i) {
      const std::size_t candidateIndex = candidateIndices[i];
      const Socket &candidate = paState.candidateAt(candidateIndex);
      const short events = descriptors[i].revents;
      if (events & POLLNVAL) {
        paState.updateCandidate(candidateIndex, ComResult::NoSocket);
        continue;
      }
      if (!(events & (POLLIN | POLLOUT | POLLERR | POLLHUP))) {
        continue;
      }

      int socketError = 0;
      socklen_t socketErrorSize = sizeof(socketError);
      if (getsockopt(candidate.get(), SOL_SOCKET, SO_ERROR, &socketError, &socketErrorSize) != 0) {
        paState.updateCandidate(candidateIndex, mapSocketError(errno, ComResult::Unknown));
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
