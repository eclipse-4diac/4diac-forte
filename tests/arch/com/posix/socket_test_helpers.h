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

#pragma once

#include <chrono>
#include <string>
#include <string_view>
#include <utility>

#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>

#include "../../../../core/arch/posix/src/com/impl/socket.h"
#include "../../../../core/arch/common/src/com/impl/parallel_state.h"

namespace forte::com::test {

  using impl::Socket;
  using LoopbackAddress = sockaddr_in;
  using AddressInfo = addrinfo;
  using NativeSocket = decltype(std::declval<const Socket &>().get());
  using Clock = std::chrono::steady_clock;
  inline constexpr auto gcReadinessTimeout = std::chrono::seconds(2);
  inline constexpr auto gcConnectionFailureTimeout = std::chrono::seconds(2);
  inline constexpr char gcLoopbackAddress[] = "127.0.0.1";
  inline const addrinfo gcTcpAddrInfoHints{
      .ai_family = AF_INET, .ai_socktype = SOCK_STREAM, .ai_protocol = IPPROTO_TCP};
  inline constexpr short SocketReadEvent = POLLIN;
  inline constexpr short SocketWriteEvent = POLLOUT;
  inline constexpr short SocketErrorEvent = POLLERR | POLLNVAL;
  inline constexpr short SocketConnectCompletionEvent = SocketWriteEvent | SocketErrorEvent | POLLHUP;
  inline constexpr int SocketShutdownWrite = SHUT_WR;

  class BufferObserver final : public ComObserver<ComBuffer> {
    public:
      ComResult receive(const ComBuffer paData, ComChannel<ComBuffer> &) override {
        if (paData.size() == 0) {
          mReceived.clear();
        } else {
          mReceived.assign(reinterpret_cast<const char *>(paData.data()), paData.size());
        }
        return mReceiveResult;
      }

      void sent(ComResult, ComChannel<ComBuffer> &) override {
      }

      std::string mReceived;
      ComResult mReceiveResult{ComResult::Ok};
  };

  Socket createLoopbackListener(sockaddr_in &paAddress);
  Socket createLoopbackBoundSocket(sockaddr_in &paAddress);
  Socket createLoopbackDatagramSocket(sockaddr_in &paAddress);

  std::pair<Socket, Socket> socketpair();

  bool getBoundLoopbackAddress(const Socket &paSocket, sockaddr_in &paAddress);

  Socket connectLoopback(const sockaddr_in &paAddress);
  Socket acceptLoopbackConnection(const Socket &paListener);
  ComResultValue<Socket> openPendingConnection(const LoopbackAddress &paAddress);
  ComResultValue<Socket> pollParallel(impl::ParallelConnectionState<Socket> &paState);

  bool sendDatagram(const Socket &paSocket, const sockaddr_in &paAddress, std::string_view paData);
  bool receiveDatagram(const Socket &paSocket, std::string_view paExpected);
  bool receiveDatagram(const Socket &paSocket, std::string_view paExpected, sockaddr_in &paAddress);
  bool sendBytes(const Socket &paSocket, std::string_view paData);
  bool receiveBytes(const Socket &paSocket, std::size_t paExpectedSize, std::string &paReceived);
  bool closeWithReset(Socket &paSocket);
  ComResult receiveSocketResult(const Socket &paSocket);

  bool waitForSocketEvent(NativeSocket paSocket, short paEvents, Clock::time_point paDeadline);
  bool waitForSocketEvent(const Socket &paSocket, short paEvents, Clock::time_point paDeadline);
  bool nativeSocketIsClosed(NativeSocket paSocket);
  bool waitForSocketData(const Socket &paSocket, std::size_t paExpectedSize);

  ComResult openUntilConnected(ComChannel<ComBuffer> &paChannel,
                               std::string_view paConfig,
                               std::chrono::milliseconds paTimeout = gcReadinessTimeout);
  ComResult pollChannelUntilReady(ComChannel<ComBuffer> &paChannel);
  bool waitForChannelData(ComChannel<ComBuffer> &paChannel);

  inline AddressInfo makeAddressInfo(LoopbackAddress &paAddress, const int paFlags = 0) {
    AddressInfo result = gcTcpAddrInfoHints;
    result.ai_flags = paFlags;
    result.ai_addr = reinterpret_cast<decltype(result.ai_addr)>(&paAddress);
    result.ai_addrlen = static_cast<decltype(result.ai_addrlen)>(sizeof(paAddress));
    return result;
  }
} // namespace forte::com::test
