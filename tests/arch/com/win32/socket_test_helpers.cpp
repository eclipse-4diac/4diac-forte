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
#include <limits>
#include <thread>

namespace forte::com::test {
  namespace {
    class WinSockInitializer final {
      public:
        WinSockInitializer() : mStarted(WSAStartup(MAKEWORD(2, 2), &mData) == 0) {
        }

        ~WinSockInitializer() {
          if (mStarted) {
            WSACleanup();
          }
        }

        [[nodiscard]] bool started() const {
          return mStarted;
        }

      private:
        WSADATA mData{};
        bool mStarted;
    };

    WinSockInitializer gWinSockInitializer;

    Socket createLoopbackSocket(const int paType, const int paProtocol, sockaddr_in &paAddress) {
      if (!gWinSockInitializer.started()) {
        return {};
      }

      Socket socket{::socket(AF_INET, paType, paProtocol)};
      if (!socket) {
        return {};
      }

      if (socket.setNonBlocking()) {
        return {};
      }

      paAddress = {};
      paAddress.sin_family = AF_INET;
      paAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
      if (::bind(socket.get(), reinterpret_cast<const sockaddr *>(&paAddress), static_cast<int>(sizeof(paAddress)))) {
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
    sockaddr_in address{};
    Socket listener = createLoopbackListener(address);
    if (!listener) {
      return {};
    }

    Socket client = connectLoopback(address);
    if (!client || !waitForSocketEvent(listener.get(), SocketReadEvent, Clock::now() + gcReadinessTimeout)) {
      return {};
    }

    Socket accepted{::accept(listener.get(), nullptr, nullptr)};
    if (!accepted || accepted.setNonBlocking()) {
      return {};
    }
    return {std::move(client), std::move(accepted)};
  }

  bool getBoundLoopbackAddress(const Socket &paSocket, sockaddr_in &paAddress) {
    paAddress = {};
    int addressLength = static_cast<int>(sizeof(paAddress));
    return getsockname(paSocket.get(), reinterpret_cast<sockaddr *>(&paAddress), &addressLength) == 0 &&
           addressLength >= static_cast<int>(sizeof(paAddress)) && paAddress.sin_family == AF_INET &&
           paAddress.sin_addr.s_addr == htonl(INADDR_LOOPBACK);
  }

  Socket connectLoopback(const sockaddr_in &paAddress) {
    if (!gWinSockInitializer.started()) {
      return {};
    }

    Socket socket{::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)};
    if (!socket) {
      return {};
    }

    if (socket.setNonBlocking()) {
      return {};
    }

    if (::connect(socket.get(), reinterpret_cast<const sockaddr *>(&paAddress), static_cast<int>(sizeof(paAddress)))) {
      const int socketError = WSAGetLastError();
      if ((socketError != WSAEWOULDBLOCK && socketError != WSAEINPROGRESS) ||
          !waitForSocketEvent(socket.get(), SocketWriteEvent, Clock::now() + gcReadinessTimeout)) {
        return {};
      }

      int connectError = 0;
      int errorSize = static_cast<int>(sizeof(connectError));
      if (getsockopt(socket.get(), SOL_SOCKET, SO_ERROR, reinterpret_cast<char *>(&connectError), &errorSize) ||
          connectError) {
        return {};
      }
    }
    return socket;
  }

  bool sendDatagram(const Socket &paSocket, const sockaddr_in &paAddress, const std::string_view paData) {
    if (paData.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
      return false;
    }
    return sendto(paSocket.get(), paData.data(), static_cast<int>(paData.size()), 0,
                  reinterpret_cast<const sockaddr *>(&paAddress),
                  static_cast<int>(sizeof(paAddress))) == static_cast<int>(paData.size());
  }

  bool receiveDatagram(const Socket &paSocket, const std::string_view paExpected) {
    sockaddr_in address{};
    return receiveDatagram(paSocket, paExpected, address);
  }

  bool receiveDatagram(const Socket &paSocket, const std::string_view paExpected, sockaddr_in &paAddress) {
    if (!waitForSocketEvent(paSocket.get(), SocketReadEvent, Clock::now() + gcReadinessTimeout)) {
      return false;
    }
    std::array<char, 32> data{};
    int addressLength = static_cast<int>(sizeof(paAddress));
    const int bytesReceived = recvfrom(paSocket.get(), data.data(), static_cast<int>(data.size()), 0,
                                       reinterpret_cast<sockaddr *>(&paAddress), &addressLength);
    return bytesReceived == static_cast<int>(paExpected.size()) &&
           addressLength >= static_cast<int>(sizeof(paAddress)) && paAddress.sin_family == AF_INET &&
           std::string_view(data.data(), static_cast<std::size_t>(bytesReceived)) == paExpected;
  }

  bool sendBytes(const Socket &paSocket, const std::string_view paData) {
    const auto deadline = Clock::now() + gcReadinessTimeout;
    std::size_t sent = 0;
    while (sent < paData.size()) {
      if (!waitForSocketEvent(paSocket.get(), SocketWriteEvent, deadline)) {
        return false;
      }
      const std::size_t remaining = paData.size() - sent;
      const int sendSize =
          static_cast<int>(std::min(remaining, static_cast<std::size_t>(std::numeric_limits<int>::max())));
      const int result = ::send(paSocket.get(), paData.data() + sent, sendSize, 0);
      if (result == SOCKET_ERROR) {
        const int socketError = WSAGetLastError();
        if (socketError == WSAEINTR || socketError == WSAEWOULDBLOCK) {
          continue;
        }
        return false;
      }
      if (result == 0) {
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
      if (!waitForSocketEvent(paSocket.get(), SocketReadEvent, deadline)) {
        return false;
      }
      const std::size_t remaining = paExpectedSize - paReceived.size();
      const int receiveSize = static_cast<int>(std::min(buffer.size(), remaining));
      const int result = ::recv(paSocket.get(), buffer.data(), receiveSize, 0);
      if (result == SOCKET_ERROR) {
        const int socketError = WSAGetLastError();
        if (socketError == WSAEINTR || socketError == WSAEWOULDBLOCK) {
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

  bool waitForSocketEvent(const SOCKET paSocket, const short paEvents, const Clock::time_point paDeadline) {
    if (paSocket == INVALID_SOCKET || (paEvents & static_cast<short>(~(SocketReadEvent | SocketWriteEvent)))) {
      return false;
    }

    while (Clock::now() < paDeadline) {
      fd_set readSet;
      fd_set writeSet;
      fd_set exceptSet;
      FD_ZERO(&readSet);
      FD_ZERO(&writeSet);
      FD_ZERO(&exceptSet);
      if (paEvents & SocketReadEvent) {
        FD_SET(paSocket, &readSet);
      }
      if (paEvents & SocketWriteEvent) {
        FD_SET(paSocket, &writeSet);
      }
      FD_SET(paSocket, &exceptSet);

      const auto remaining = std::chrono::duration_cast<std::chrono::microseconds>(paDeadline - Clock::now());
      timeval timeout{};
      timeout.tv_sec = static_cast<long>(remaining.count() / 1000000);
      timeout.tv_usec = static_cast<long>(remaining.count() % 1000000);
      const int result = ::select(0, &readSet, &writeSet, &exceptSet, &timeout);
      if (result > 0) {
        if (FD_ISSET(paSocket, &exceptSet)) {
          return false;
        }
        return ((paEvents & SocketReadEvent) && FD_ISSET(paSocket, &readSet)) ||
               ((paEvents & SocketWriteEvent) && FD_ISSET(paSocket, &writeSet));
      }
      if (result == 0) {
        return false;
      }
      if (WSAGetLastError() != WSAEINTR) {
        return false;
      }
    }
    return false;
  }

  bool waitForSocketData(const Socket &paSocket, const std::size_t paExpectedSize) {
    const auto deadline = Clock::now() + gcReadinessTimeout;
    while (Clock::now() < deadline) {
      u_long bytesAvailable = 0;
      if (ioctlsocket(paSocket.get(), FIONREAD, &bytesAvailable) == SOCKET_ERROR) {
        if (WSAGetLastError() != WSAEINTR) {
          return false;
        }
      } else if (bytesAvailable >= paExpectedSize) {
        return true;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return false;
  }

  ComResult openUntilConnected(ComChannel<ComBuffer> &paChannel, const std::string_view paConfig) {
    const auto deadline = Clock::now() + gcReadinessTimeout;
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
