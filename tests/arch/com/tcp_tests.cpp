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

#include "forte/com/factory.h"
#include "forte/stringid.h"
#include "forte_boost_output_support.h"
#include "socket_test_helpers.h"

using namespace forte::literals;

namespace forte::com::test {

  namespace {
    struct TCPClientFixture {
        TCPClientFixture() :
            mListener(createLoopbackListener(mAddress)),
            mConfig("127.0.0.1:" + std::to_string(ntohs(mAddress.sin_port))),
            mChannel(ComChannelFactory<ComBuffer>::create("tcp"_STRID, mObserver)) {
          BOOST_TEST_REQUIRE(static_cast<bool>(mListener));
          BOOST_TEST_REQUIRE(mChannel);
        }

        BufferObserver mObserver;
        sockaddr_in mAddress{};
        Socket mListener;
        std::string mConfig;
        std::unique_ptr<ComChannel<ComBuffer>> mChannel;
    };
  } // namespace

  BOOST_FIXTURE_TEST_SUITE(TCP_CLIENT_CHANNEL, TCPClientFixture)

  BOOST_AUTO_TEST_CASE(CONNECT) {
    BOOST_TEST_REQUIRE(mChannel->open(mConfig, {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForSocketEvent(mListener.get(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    const Socket accepted(::accept(mListener.get(), nullptr, nullptr));
    BOOST_TEST_REQUIRE(static_cast<bool>(accepted));

    BOOST_TEST(mChannel->close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(SEND_AND_RECEIVE_DATA) {
    BOOST_TEST_REQUIRE(mChannel->setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel->setMinReceiveSize(4) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel->open(mConfig, {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForSocketEvent(mListener.get(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    const Socket accepted(::accept(mListener.get(), nullptr, nullptr));
    BOOST_TEST_REQUIRE(static_cast<bool>(accepted));
    BOOST_TEST_REQUIRE(accepted.setNonBlocking() == 0);
#ifndef _WIN32
    BOOST_TEST_REQUIRE(accepted.setNoSigPipe() == 0);
#endif

    std::string received;
    std::string request{"ping"};
    BOOST_TEST(mChannel->send(ComBuffer(request)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveBytes(accepted, request.size(), received));
    BOOST_TEST(received == request);

    constexpr std::string_view response = "pong";
    BOOST_TEST_REQUIRE(sendBytes(accepted, response));
    BOOST_TEST_REQUIRE(waitForChannelData(*mChannel));
    BOOST_TEST(mObserver.mReceived == response);

    BOOST_TEST(mChannel->close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(ORDERLY_CLOSE) {
    BOOST_TEST_REQUIRE(mChannel->setMaxReceiveSize(16) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel->open(mConfig, {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForSocketEvent(mListener.get(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    const Socket accepted(::accept(mListener.get(), nullptr, nullptr));
    BOOST_TEST_REQUIRE(static_cast<bool>(accepted));

    BOOST_TEST_REQUIRE(::shutdown(accepted.get(), SocketShutdownWrite) == 0);
    BOOST_TEST(pollChannelUntilReady(*mChannel) == ComResult::RecvFailed);
    BOOST_TEST(mChannel->close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
