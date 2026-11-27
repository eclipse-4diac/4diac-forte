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

#include <boost/test/unit_test.hpp>

#include <array>
#include <string>
#include <string_view>
#include <thread>

#include "forte_boost_output_support.h"
#include "com/impl/tcp_listen.h"
#include "socket_test_helpers.h"

namespace forte::com::test {
  namespace {
    class TestTCPListenChannel : public impl::TCPListenChannel {
      public:
        using impl::SocketChannel::getSocket;
        using impl::TCPListenChannel::getConnectionSocket;
        using impl::TCPListenChannel::TCPListenChannel;
    };

    struct TCPListenFixture {
        TCPListenFixture() : mChannel(mObserver) {
        }

        BufferObserver mObserver;
        sockaddr_in mAddress{};
        TestTCPListenChannel mChannel;
    };
  } // namespace

  BOOST_FIXTURE_TEST_SUITE(TCP_LISTEN_CHANNEL, TCPListenFixture)

  BOOST_AUTO_TEST_CASE(OPEN_CLOSE) {
    BOOST_TEST(!mChannel.isOpen());
    BOOST_TEST(!static_cast<bool>(mChannel.getSocket()));

    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST(mChannel.isOpen());
    BOOST_TEST(static_cast<bool>(mChannel.getSocket()));
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));
    BOOST_TEST(ntohs(mAddress.sin_port) != 0);

    BOOST_TEST(mChannel.close() == ComResult::Ok);
    BOOST_TEST(!mChannel.isOpen());
    BOOST_TEST(!static_cast<bool>(mChannel.getSocket()));
  }

  BOOST_AUTO_TEST_CASE(RECEIVE_AND_REPLY) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"request"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket client = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(client));
    BOOST_TEST_REQUIRE(sendBytes(client, "request"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "request");

    std::string received;
    std::string response{"reply"};
    BOOST_TEST(mChannel.send(ComBuffer(response)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveBytes(client, response.size(), received));
    BOOST_TEST(received == response);

    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(DISABLED_RECEIVING_ACCEPTS_PENDING_CLIENT) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(0) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket client = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(client));
    BOOST_TEST_REQUIRE(sendBytes(client, "pending"));
    BOOST_TEST_REQUIRE(waitForSocketEvent(mChannel.getSocket(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    BOOST_TEST(mChannel.poll() == ComResult::WouldBlock);

    std::string response{"reply"};
    std::string received;
    BOOST_TEST_REQUIRE(mChannel.send(ComBuffer(response)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveBytes(client, response.size(), received));
    BOOST_TEST(received == response);
    BOOST_TEST(mObserver.mReceived.empty());

    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"pending"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "pending");
    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(DISABLED_RECEIVING_PRESERVES_FULL_BUFFER_CONNECTION) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(4) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(4) == ComResult::Ok);
    mObserver.mReceiveResult = ComResult::More;
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket firstClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "part"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "part");

    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(0) == ComResult::Ok);
    const Socket pendingClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(pendingClient));
    BOOST_TEST_REQUIRE(sendBytes(pendingClient, "ignored"));
    BOOST_TEST(mChannel.poll() == ComResult::WouldBlock);

    std::string response{"still connected"};
    std::string received;
    BOOST_TEST(mChannel.send(ComBuffer(response)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveBytes(firstClient, response.size(), received));
    BOOST_TEST(received == response);
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(16) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"partial"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "ial"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "partial");

    BOOST_TEST(::shutdown(firstClient.get(), SocketShutdownWrite) == 0);
    BOOST_TEST(pollChannelUntilReady(mChannel) == ComResult::RecvFailed);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"ignored"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "ignored");

    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(REPEATED_OPEN_PRESERVES_ACCEPTED_CONNECTION) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"request"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket firstClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "request"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "request");

    BOOST_TEST(mChannel.open("127.0.0.1:1", {}) == ComResult::Ok);

    std::string received;
    std::string preservedData{"kept"};
    BOOST_TEST(mChannel.send(ComBuffer(preservedData)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveBytes(firstClient, preservedData.size(), received));
    BOOST_TEST(received == preservedData);

    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(PEER_SHUTDOWN) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"request"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket client = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(client));
    BOOST_TEST_REQUIRE(sendBytes(client, "request"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "request");

    mObserver.mReceived = "unchanged";
    BOOST_TEST(::shutdown(client.get(), SocketShutdownWrite) == 0);
    BOOST_TEST(pollChannelUntilReady(mChannel) == ComResult::RecvFailed);
    BOOST_TEST(mObserver.mReceived == "unchanged");
    BOOST_TEST(mChannel.isOpen());

    const Socket nextClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(nextClient));
    BOOST_TEST_REQUIRE(sendBytes(nextClient, "reconnect"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "reconnect");

    std::string received;
    std::string response{"again"};
    BOOST_TEST(mChannel.send(ComBuffer(response)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveBytes(nextClient, response.size(), received));
    BOOST_TEST(received == response);

    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(PEER_SHUTDOWN_DISCARDS_PARTIAL_DATA) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(7) == ComResult::Ok);
    mObserver.mReceiveResult = ComResult::More;
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket firstClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "part"));
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"part"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "part");
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(7) == ComResult::Ok);
    mObserver.mReceiveResult = ComResult::Ok;

    BOOST_TEST(::shutdown(firstClient.get(), SocketShutdownWrite) == 0);
    BOOST_TEST(pollChannelUntilReady(mChannel) == ComResult::RecvFailed);

    const Socket nextClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(nextClient));
    BOOST_TEST_REQUIRE(sendBytes(nextClient, "request"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "request");

    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(PEER_SHUTDOWN_WITH_FULL_BUFFER) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(4) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(4) == ComResult::Ok);
    mObserver.mReceiveResult = ComResult::More;
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket firstClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "part"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(::shutdown(firstClient.get(), SocketShutdownWrite) == 0);
    BOOST_TEST(pollChannelUntilReady(mChannel) == ComResult::RecvFailed);

    mObserver.mReceiveResult = ComResult::Ok;
    const Socket nextClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(nextClient));
    BOOST_TEST_REQUIRE(sendBytes(nextClient, "next"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "next");
    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(FULL_BUFFER_RETAINS_CLIENT_UNTIL_LIMIT_INCREASED) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(4) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(4) == ComResult::Ok);
    mObserver.mReceiveResult = ComResult::More;
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket firstClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "part"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mChannel.poll() == ComResult::WouldBlock);
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "extra"));
    BOOST_TEST_REQUIRE(
        waitForSocketEvent(mChannel.getConnectionSocket(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    BOOST_TEST(mChannel.poll() == ComResult::WouldBlock);

    const Socket pendingClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(pendingClient));
    BOOST_TEST_REQUIRE(sendBytes(pendingClient, "ignored"));
    BOOST_TEST_REQUIRE(waitForSocketEvent(mChannel.getSocket(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    BOOST_TEST(mChannel.poll() == ComResult::WouldBlock);

    std::string response{"alive"};
    std::string received;
    BOOST_TEST(mChannel.send(ComBuffer(response)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveBytes(firstClient, response.size(), received));
    BOOST_TEST(received == response);

    mObserver.mReceiveResult = ComResult::Ok;
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(16) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"partextra"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "partextra");

    BOOST_TEST(::shutdown(firstClient.get(), SocketShutdownWrite) == 0);
    BOOST_TEST(pollChannelUntilReady(mChannel) == ComResult::RecvFailed);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"ignored"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "ignored");

    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(FULL_BUFFER_PEER_RESET_ALLOWS_RECONNECTION) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(4) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(4) == ComResult::Ok);
    mObserver.mReceiveResult = ComResult::More;
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    Socket firstClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "part"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST_REQUIRE(closeWithReset(firstClient));
    BOOST_TEST(pollChannelUntilReady(mChannel) == ComResult::ConnectionReset);
    std::string response{"reply"};
    BOOST_TEST(mChannel.send(ComBuffer(response)) == ComResult::NoSocket);
    BOOST_TEST(mChannel.isOpen());

    mObserver.mReceiveResult = ComResult::Ok;
    const Socket nextClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(nextClient));
    BOOST_TEST_REQUIRE(sendBytes(nextClient, "next"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "next");
    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(QUEUE_CLIENT_UNTIL_ACTIVE_CLIENT_CLOSES) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"request"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket firstClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "request"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "request");

    const Socket pendingClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(pendingClient));
    BOOST_TEST_REQUIRE(sendBytes(pendingClient, "reconnect"));

    BOOST_TEST_REQUIRE(sendBytes(firstClient, "finally"));
    BOOST_TEST(::shutdown(firstClient.get(), SocketShutdownWrite) == 0);
    BOOST_TEST(pollChannelUntilReady(mChannel) == ComResult::Async);
    BOOST_TEST(mObserver.mReceived == "finally");
    BOOST_TEST(pollChannelUntilReady(mChannel) == ComResult::RecvFailed);
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "reconnect");

    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(PEER_RESET_ALLOWS_RECONNECTION) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"request"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    Socket firstClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "request"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(closeWithReset(firstClient));
    const ComResult resetResult = pollChannelUntilReady(mChannel);
    BOOST_TEST(resetResult == ComResult::ConnectionReset);
    BOOST_TEST(mChannel.isOpen());

    const Socket nextClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(nextClient));
    BOOST_TEST_REQUIRE(sendBytes(nextClient, "reconnect"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "reconnect");

    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(SEND_FAILURE_ALLOWS_RECONNECTION) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(7) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    Socket firstClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "request"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST_REQUIRE(closeWithReset(firstClient));

    std::string response{"reply"};
    ComResult result = ComResult::Ok;
    const auto deadline = Clock::now() + gcReadinessTimeout;
    while (Clock::now() < deadline &&
           (result == ComResult::Ok || result == ComResult::WouldBlock || result == ComResult::Interrupted)) {
      result = mChannel.send(ComBuffer(response));
      std::this_thread::yield();
    }
    BOOST_TEST_REQUIRE((result != ComResult::Ok && result != ComResult::WouldBlock &&
                        result != ComResult::Interrupted && result != ComResult::NoSocket));
    BOOST_TEST(mChannel.send(ComBuffer(response)) == ComResult::NoSocket);
    BOOST_TEST(mChannel.isOpen());

    const Socket nextClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(nextClient));
    BOOST_TEST_REQUIRE(sendBytes(nextClient, "reconnect"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "reconnect");
    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(CLOSE_REOPEN) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"request"}.size()) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket firstClient = connectLoopback(mAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(sendBytes(firstClient, "request"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "request");

    BOOST_TEST(mChannel.close() == ComResult::Ok);
    BOOST_TEST(!static_cast<bool>(mChannel.getSocket()));

    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(static_cast<bool>(mChannel.getSocket()));
    sockaddr_in reopenedAddress{};
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), reopenedAddress));
    BOOST_TEST(ntohs(reopenedAddress.sin_port) != 0);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(std::string_view{"again"}.size()) == ComResult::Ok);

    std::string newData{"new"};
    BOOST_TEST(mChannel.send(ComBuffer(newData)) == ComResult::NoSocket);

    const Socket secondClient = connectLoopback(reopenedAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(secondClient));
    BOOST_TEST_REQUIRE(sendBytes(secondClient, "again"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "again");

    std::string received;
    BOOST_TEST(mChannel.send(ComBuffer(newData)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveBytes(secondClient, newData.size(), received));
    BOOST_TEST(received == newData);

    BOOST_TEST(mChannel.close() == ComResult::Ok);
    BOOST_TEST(!static_cast<bool>(mChannel.getSocket()));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
