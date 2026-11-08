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

    BOOST_TEST(::shutdown(client.get(), SocketShutdownWrite) == 0);
    BOOST_TEST(pollChannelUntilReady(mChannel) == ComResult::RecvFailed);

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
