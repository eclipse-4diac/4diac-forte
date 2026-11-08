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

#include <winsock2.h>

#include "../../../../core/arch/win32/src/com/impl/socket.h"

namespace forte::com::test {

  using impl::Socket;
  using Clock = std::chrono::steady_clock;
  inline constexpr auto gcReadinessTimeout = std::chrono::seconds(2);
  inline constexpr short SocketReadEvent = 0x01;
  inline constexpr short SocketWriteEvent = 0x02;
  inline constexpr int SocketShutdownWrite = SD_SEND;

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

  bool sendDatagram(const Socket &paSocket, const sockaddr_in &paAddress, std::string_view paData);
  bool receiveDatagram(const Socket &paSocket, std::string_view paExpected);
  bool receiveDatagram(const Socket &paSocket, std::string_view paExpected, sockaddr_in &paAddress);
  bool sendBytes(const Socket &paSocket, std::string_view paData);
  bool receiveBytes(const Socket &paSocket, std::size_t paExpectedSize, std::string &paReceived);

  bool waitForSocketEvent(SOCKET paSocket, short paEvents, Clock::time_point paDeadline);
  bool waitForSocketData(const Socket &paSocket, std::size_t paExpectedSize);

  ComResult openUntilConnected(ComChannel<ComBuffer> &paChannel, std::string_view paConfig);
  ComResult pollChannelUntilReady(ComChannel<ComBuffer> &paChannel);
  bool waitForChannelData(ComChannel<ComBuffer> &paChannel);

} // namespace forte::com::test
