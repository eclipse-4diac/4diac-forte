/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 ******************************************************************************/

#include "socket_test_helpers.h"

#include <algorithm>
#include <array>
#include <cerrno>
#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <thread>

#include "../../../../core/arch/posix/src/com/impl/net.h"

namespace forte::com::test {
  namespace {
    Socket createLoopbackSocket(const int paType, const int paProtocol, sockaddr_in &paAddress) {
      Socket socket{::socket(AF_INET, paType, paProtocol)};
      if (!socket) {
        return {};
      }

      if (socket.setNonBlocking() || socket.setNoSigPipe()) {
        return {};
      }

      paAddress = {};
      paAddress.sin_family = AF_INET;
      paAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
      if (::bind(socket.get(), reinterpret_cast<sockaddr *>(&paAddress), sizeof(paAddress))) {
        return {};
      }

      if (!getBoundLoopbackAddress(socket, paAddress)) {
        return {};
      }
      return socket;
    }
  } // namespace

  Socket createLoopbackListener(sockaddr_in &paAddress) {
    Socket socket = createLoopbackSocket(SOCK_STREAM, IPPROTO_TCP, paAddress);
    if (!socket || ::listen(socket.get(), 1)) {
      return {};
    }
    return socket;
  }

  Socket createLoopbackBoundSocket(sockaddr_in &paAddress) {
    return createLoopbackSocket(SOCK_STREAM, IPPROTO_TCP, paAddress);
  }

  Socket createLoopbackDatagramSocket(sockaddr_in &paAddress) {
    return createLoopbackSocket(SOCK_DGRAM, IPPROTO_UDP, paAddress);
  }

  std::pair<Socket, Socket> socketpair() {
    int sockets[2]{};
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sockets)) {
      return {};
    }
    Socket first{sockets[0]};
    Socket second{sockets[1]};
    if (first.setNonBlocking() || second.setNonBlocking() || first.setNoSigPipe() || second.setNoSigPipe()) {
      return {};
    }
    return {std::move(first), std::move(second)};
  }

  bool getBoundLoopbackAddress(const Socket &paSocket, sockaddr_in &paAddress) {
    paAddress = {};
    socklen_t addressLength = sizeof(paAddress);
    return getsockname(paSocket.get(), reinterpret_cast<sockaddr *>(&paAddress), &addressLength) == 0 &&
           addressLength >= sizeof(paAddress) && paAddress.sin_family == AF_INET &&
           paAddress.sin_addr.s_addr == htonl(INADDR_LOOPBACK);
  }

  Socket connectLoopback(const sockaddr_in &paAddress) {
    Socket socket{::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)};
    if (!socket) {
      return {};
    }

    if (socket.setNonBlocking() || socket.setNoSigPipe()) {
      return {};
    }

    if (::connect(socket.get(), reinterpret_cast<const sockaddr *>(&paAddress), sizeof(paAddress))) {
      if (errno != EINPROGRESS || !waitForSocketEvent(socket.get(), POLLOUT, Clock::now() + gcReadinessTimeout)) {
        return {};
      }

      int socketError = 0;
      socklen_t errorSize = sizeof(socketError);
      if (getsockopt(socket.get(), SOL_SOCKET, SO_ERROR, &socketError, &errorSize) || socketError) {
        return {};
      }
    }
    return socket;
  }

  Socket acceptLoopbackConnection(const Socket &paListener) {
    return Socket{::accept(paListener.get(), nullptr, nullptr)};
  }

  ComResultValue<Socket> openPendingConnection(const LoopbackAddress &paAddress) {
    const std::string config = std::string(gcLoopbackAddress) + ":" + std::to_string(ntohs(paAddress.sin_port));
    auto [result, state] = impl::net::openParallel(config, gcTcpAddrInfoHints);
    if (result != ComResult::Ok) {
      return {.mResult = result};
    }
    if (state.candidateCount() != 1) {
      return {.mResult = ComResult::InvalidId};
    }
    if (state.result() == ComResult::Ok && state.hasWinner()) {
      return {.mResult = ComResult::InProgress, .mValue = state.takeWinner()};
    }
    if (state.result() != ComResult::InProgress || !state.candidateAt(0)) {
      return {.mResult = ComResult::InvalidId};
    }
    return {.mResult = ComResult::InProgress, .mValue = std::move(state.candidateAt(0))};
  }

  ComResultValue<Socket> pollParallel(impl::ParallelConnectionState<Socket> &paState) {
    return impl::net::pollParallel(paState);
  }

  bool sendDatagram(const Socket &paSocket, const sockaddr_in &paAddress, const std::string_view paData) {
    return sendto(paSocket.get(), paData.data(), paData.size(), 0, reinterpret_cast<const sockaddr *>(&paAddress),
                  sizeof(paAddress)) == static_cast<ssize_t>(paData.size());
  }

  bool receiveDatagram(const Socket &paSocket, const std::string_view paExpected) {
    sockaddr_in address{};
    return receiveDatagram(paSocket, paExpected, address);
  }

  bool receiveDatagram(const Socket &paSocket, const std::string_view paExpected, sockaddr_in &paAddress) {
    if (!waitForSocketEvent(paSocket.get(), POLLIN, Clock::now() + gcReadinessTimeout)) {
      return false;
    }
    std::array<char, 32> data{};
    socklen_t addressLength = sizeof(paAddress);
    const ssize_t bytesReceived =
        recvfrom(paSocket.get(), data.data(), data.size(), 0, reinterpret_cast<sockaddr *>(&paAddress), &addressLength);
    return bytesReceived == static_cast<ssize_t>(paExpected.size()) && addressLength >= sizeof(paAddress) &&
           paAddress.sin_family == AF_INET &&
           std::string_view(data.data(), static_cast<std::size_t>(bytesReceived)) == paExpected;
  }

  bool sendBytes(const Socket &paSocket, const std::string_view paData) {
    const auto deadline = Clock::now() + gcReadinessTimeout;
    std::size_t sent = 0;
    while (sent < paData.size()) {
      if (!waitForSocketEvent(paSocket.get(), POLLOUT, deadline)) {
        return false;
      }
      const ssize_t result = ::send(paSocket.get(), paData.data() + sent, paData.size() - sent, impl::SocketSendFlags);
      if (result < 0) {
        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
          continue;
        }
        return false;
      }
      sent += static_cast<std::size_t>(result);
    }
    return true;
  }

  bool receiveBytes(const Socket &paSocket, const std::size_t paExpectedSize, std::string &paReceived) {
    const auto deadline = Clock::now() + gcReadinessTimeout;
    std::array<char, 32> buffer{};
    while (paReceived.size() < paExpectedSize) {
      if (!waitForSocketEvent(paSocket.get(), POLLIN, deadline)) {
        return false;
      }
      const std::size_t remaining = paExpectedSize - paReceived.size();
      const ssize_t result = ::recv(paSocket.get(), buffer.data(), std::min(buffer.size(), remaining), 0);
      if (result < 0) {
        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
          continue;
        }
        return false;
      }
      if (result == 0) {
        return false;
      }
      paReceived.append(buffer.data(), static_cast<std::size_t>(result));
    }
    return true;
  }

  bool closeWithReset(Socket &paSocket) {
    const linger resetLinger{.l_onoff = 1, .l_linger = 0};
    return setsockopt(paSocket.get(), SOL_SOCKET, SO_LINGER, &resetLinger, sizeof(resetLinger)) == 0 &&
           paSocket.close() == 0;
  }

  ComResult receiveSocketResult(const Socket &paSocket) {
    const auto deadline = Clock::now() + gcReadinessTimeout;
    char buffer{};
    while (Clock::now() < deadline) {
      if (!waitForSocketEvent(paSocket, SocketReadEvent, deadline)) {
        return ComResult::WouldBlock;
      }
      const ssize_t result = ::recv(paSocket.get(), &buffer, sizeof(buffer), 0);
      if (result < 0) {
        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
          continue;
        }
        return impl::net::mapSocketError(errno, ComResult::RecvFailed);
      }
      return result == 0 ? ComResult::ConnectionClosed : ComResult::Ok;
    }
    return ComResult::WouldBlock;
  }

  bool waitForSocketEvent(const NativeSocket paSocket, const short paEvents, const Clock::time_point paDeadline) {
    if (paEvents & static_cast<short>(~(POLLIN | POLLOUT | POLLERR | POLLNVAL | POLLHUP))) {
      return false;
    }
    while (Clock::now() < paDeadline) {
      pollfd descriptor{.fd = paSocket, .events = paEvents, .revents = 0};
      const auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(paDeadline - Clock::now());
      const int timeout = std::max(1, static_cast<int>(remaining.count()));
      const int result = ::poll(&descriptor, 1, timeout);
      if (result > 0) {
        if (descriptor.revents & POLLHUP) {
          if (paEvents & POLLHUP) {
            return true;
          }
          if (!(paEvents & POLLIN && descriptor.revents & POLLIN)) {
            return false;
          }
        }
        return descriptor.revents & paEvents;
      }
      if (result == 0 || errno != EINTR) {
        return false;
      }
    }
    return false;
  }

  bool waitForSocketEvent(const Socket &paSocket, const short paEvents, const Clock::time_point paDeadline) {
    return waitForSocketEvent(paSocket.get(), paEvents, paDeadline);
  }

  bool nativeSocketIsClosed(const NativeSocket paSocket) {
    return fcntl(paSocket, F_GETFD) == -1 && errno == EBADF;
  }

  bool waitForSocketData(const Socket &paSocket, const std::size_t paExpectedSize) {
    const auto deadline = Clock::now() + gcReadinessTimeout;
    while (Clock::now() < deadline) {
      int bytesAvailable = 0;
      if (ioctl(paSocket.get(), FIONREAD, &bytesAvailable)) {
        if (errno != EINTR) {
          return false;
        }
      } else if (bytesAvailable >= 0 && static_cast<std::size_t>(bytesAvailable) >= paExpectedSize) {
        return true;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return false;
  }

  ComResult openUntilConnected(ComChannel<ComBuffer> &paChannel,
                               const std::string_view paConfig,
                               const std::chrono::milliseconds paTimeout) {
    const auto deadline = Clock::now() + paTimeout;
    auto result = ComResult::InProgress;
    while (result == ComResult::InProgress && Clock::now() < deadline) {
      result = paChannel.open(paConfig, std::span<ComChannelDescriptor>{});
      if (result == ComResult::InProgress) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
    }
    return result;
  }

  ComResult pollChannelUntilReady(ComChannel<ComBuffer> &paChannel) {
    const auto deadline = Clock::now() + gcReadinessTimeout;
    while (Clock::now() < deadline) {
      const ComResult result = paChannel.poll();
      if (result != ComResult::More && result != ComResult::WouldBlock && result != ComResult::InProgress) {
        return result;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return ComResult::WouldBlock;
  }

  bool waitForChannelData(ComChannel<ComBuffer> &paChannel) {
    return pollChannelUntilReady(paChannel) == ComResult::Async;
  }

} // namespace forte::com::test
