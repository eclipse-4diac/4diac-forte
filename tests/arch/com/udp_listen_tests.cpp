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

#include "forte_boost_output_support.h"
#include "com/impl/udp_listen.h"
#include "socket_test_helpers.h"

namespace forte::com::test {
  namespace {
    class TestUDPListenChannel : public impl::UDPListenChannel {
      public:
        using impl::SocketChannel::getSocket;
        using impl::UDPListenChannel::UDPListenChannel;
    };

    struct UDPListenFixture {
        UDPListenFixture() : mChannel(mObserver) {
        }

        BufferObserver mObserver;
        sockaddr_in mAddress{};
        TestUDPListenChannel mChannel;
    };
  } // namespace

  BOOST_FIXTURE_TEST_SUITE(UDP_LISTEN_CHANNEL, UDPListenFixture)

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

  BOOST_AUTO_TEST_CASE(EMPTY_DATAGRAM) {
    mObserver.mReceived = "not empty";
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(0) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket client{::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)};
    BOOST_TEST_REQUIRE(static_cast<bool>(client));
    BOOST_TEST_REQUIRE(client.setNonBlocking() == 0);

    BOOST_TEST_REQUIRE(sendDatagram(client, mAddress, {}));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived.empty());
    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(RECEIVE_AND_REPLY) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(1) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    Socket client{::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)};
    BOOST_TEST_REQUIRE(static_cast<bool>(client));
    BOOST_TEST_REQUIRE(client.setNonBlocking() == 0);

    BOOST_TEST_REQUIRE(sendDatagram(client, mAddress, "request"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "request");

    std::string response{"reply"};
    BOOST_TEST(mChannel.send(ComBuffer(response)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveDatagram(client, response));

    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(REPEATED_OPEN_PRESERVES_PEER) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(1) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket firstClient{::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)};
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(firstClient.setNonBlocking() == 0);

    BOOST_TEST_REQUIRE(sendDatagram(firstClient, mAddress, "first"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "first");

    BOOST_TEST(mChannel.open("127.0.0.1:1", {}) == ComResult::Ok);

    std::string response{"preserved"};
    BOOST_TEST(mChannel.send(ComBuffer(response)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveDatagram(firstClient, response));

    BOOST_TEST(mChannel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(CLOSE_REOPEN_ACCEPTS_NEW_PEER) {
    BOOST_TEST_REQUIRE(mChannel.setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.setMinReceiveSize(1) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), mAddress));

    const Socket firstClient{::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)};
    BOOST_TEST_REQUIRE(static_cast<bool>(firstClient));
    BOOST_TEST_REQUIRE(firstClient.setNonBlocking() == 0);
    BOOST_TEST_REQUIRE(sendDatagram(firstClient, mAddress, "first"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "first");

    BOOST_TEST(mChannel.close() == ComResult::Ok);
    BOOST_TEST(!static_cast<bool>(mChannel.getSocket()));

    BOOST_TEST_REQUIRE(mChannel.open("127.0.0.1:0", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(static_cast<bool>(mChannel.getSocket()));
    sockaddr_in reopenedAddress{};
    BOOST_TEST_REQUIRE(getBoundLoopbackAddress(mChannel.getSocket(), reopenedAddress));
    BOOST_TEST_REQUIRE(ntohs(reopenedAddress.sin_port) != 0);

    Socket secondClient{::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)};
    BOOST_TEST_REQUIRE(static_cast<bool>(secondClient));
    BOOST_TEST_REQUIRE(secondClient.setNonBlocking() == 0);
    BOOST_TEST_REQUIRE(sendDatagram(secondClient, reopenedAddress, "second"));
    BOOST_TEST_REQUIRE(waitForChannelData(mChannel));
    BOOST_TEST(mObserver.mReceived == "second");

    std::string response{"reopened"};
    BOOST_TEST(mChannel.send(ComBuffer(response)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveDatagram(secondClient, response));
    BOOST_TEST(mChannel.close() == ComResult::Ok);
    BOOST_TEST(!static_cast<bool>(mChannel.getSocket()));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
