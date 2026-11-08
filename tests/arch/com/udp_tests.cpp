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

#include <string>
#include <string_view>

#include "forte/com/factory.h"
#include "forte/stringid.h"
#include "forte_boost_output_support.h"
#include "socket_test_helpers.h"

using namespace forte::literals;

namespace forte::com::test {

  namespace {
    struct UDPClientFixture {
        UDPClientFixture() :
            mServer(createLoopbackDatagramSocket(mAddress)),
            mConfig("127.0.0.1:" + std::to_string(ntohs(mAddress.sin_port))),
            mChannel(ComChannelFactory<ComBuffer>::create("udp"_STRID, mObserver)) {
          BOOST_TEST_REQUIRE(static_cast<bool>(mServer));
          BOOST_TEST_REQUIRE(mChannel);
        }

        BufferObserver mObserver;
        sockaddr_in mAddress{};
        Socket mServer;
        std::string mConfig;
        std::unique_ptr<ComChannel<ComBuffer>> mChannel;
    };
  } // namespace

  BOOST_FIXTURE_TEST_SUITE(UDP_CLIENT_CHANNEL, UDPClientFixture)

  BOOST_AUTO_TEST_CASE(SEND_EMPTY_DATAGRAM) {
    BOOST_TEST_REQUIRE(mChannel->open(mConfig, {}) == ComResult::Ok);

    std::string empty;
    BOOST_TEST(mChannel->send(ComBuffer(empty)) == ComResult::Ok);
    BOOST_TEST(receiveDatagram(mServer, {}));
    BOOST_TEST(mChannel->close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(RECEIVE_EMPTY_DATAGRAM) {
    mObserver.mReceived = "not empty";
    BOOST_TEST_REQUIRE(mChannel->setMaxReceiveSize(16) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel->open(mConfig, {}) == ComResult::Ok);

    std::string request{"request"};
    BOOST_TEST(mChannel->send(ComBuffer(request)) == ComResult::Ok);

    sockaddr_in peer{};
    BOOST_TEST_REQUIRE(receiveDatagram(mServer, request, peer));

    BOOST_TEST_REQUIRE(sendDatagram(mServer, peer, {}));
    BOOST_TEST_REQUIRE(waitForChannelData(*mChannel));
    BOOST_TEST(mObserver.mReceived.empty());
    BOOST_TEST(mChannel->close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(SEND_AND_RECEIVE_DATA) {
    BOOST_TEST_REQUIRE(mChannel->setMaxReceiveSize(16) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel->setMinReceiveSize(1) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel->open(mConfig, {}) == ComResult::Ok);

    std::string request{"ping"};
    BOOST_TEST(mChannel->send(ComBuffer(request)) == ComResult::Ok);

    sockaddr_in peer{};
    BOOST_TEST_REQUIRE(receiveDatagram(mServer, request, peer));

    constexpr std::string_view response = "pong";
    BOOST_TEST_REQUIRE(sendDatagram(mServer, peer, response));
    BOOST_TEST_REQUIRE(waitForChannelData(*mChannel));
    BOOST_TEST(mObserver.mReceived == response);
    BOOST_TEST(mChannel->close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
