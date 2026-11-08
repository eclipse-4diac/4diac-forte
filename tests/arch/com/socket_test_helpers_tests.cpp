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
#include <utility>

#include "forte_boost_output_support.h"
#include "com/impl/tcp.h"
#include "socket_test_helpers.h"

namespace forte::com::test {
  namespace {
    class TestTCPChannel final : public impl::TCPChannel {
      public:
        using impl::TCPChannel::TCPChannel;

        ComResult attach(Socket paSocket) {
          return setSocket(std::move(paSocket));
        }
    };
  } // namespace

  BOOST_AUTO_TEST_SUITE(SOCKET_TEST_HELPERS)

  BOOST_AUTO_TEST_CASE(RECEIVE_BYTES_PRESERVES_BUFFERED_DATA_AFTER_CLOSE) {
    auto [socket, peer] = socketpair();
    BOOST_TEST_REQUIRE(static_cast<bool>(socket));
    BOOST_TEST_REQUIRE(static_cast<bool>(peer));
    BOOST_TEST_REQUIRE(sendBytes(peer, "reply"));
    peer.reset();

    std::string received;
    BOOST_TEST(receiveBytes(socket, std::string_view{"reply"}.size(), received));
    BOOST_TEST(received == "reply");
  }

  BOOST_AUTO_TEST_CASE(RECEIVE_BYTES_RETURNS_PROMPTLY_ON_INCOMPLETE_EOF) {
    auto [socket, peer] = socketpair();
    BOOST_TEST_REQUIRE(static_cast<bool>(socket));
    BOOST_TEST_REQUIRE(static_cast<bool>(peer));
    BOOST_TEST_REQUIRE(sendBytes(peer, "part"));
    BOOST_TEST(::shutdown(peer.get(), SocketShutdownWrite) == 0);

    std::string received;
    const auto start = Clock::now();
    BOOST_TEST(!receiveBytes(socket, 8, received));
    BOOST_TEST(received == "part");
    const bool returnedPromptly = Clock::now() - start < gcReadinessTimeout / 2;
    BOOST_TEST(returnedPromptly);
  }

  BOOST_AUTO_TEST_CASE(RECEIVE_HELPER_EOF) {
    BufferObserver observer;
    TestTCPChannel channel{observer};
    auto [socket, peer] = socketpair();
    BOOST_TEST_REQUIRE(static_cast<bool>(socket));
    BOOST_TEST_REQUIRE(static_cast<bool>(peer));
    BOOST_TEST_REQUIRE(channel.setMaxReceiveSize(8) == ComResult::Ok);
    BOOST_TEST_REQUIRE(channel.attach(std::move(socket)) == ComResult::Ok);
    peer.reset();

    const auto start = Clock::now();
    BOOST_TEST(!waitForChannelData(channel));
    BOOST_TEST((Clock::now() - start) < gcReadinessTimeout);
    BOOST_TEST(channel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(SEND_HELPER_CLOSED_PEER) {
    auto [sender, peer] = socketpair();
    BOOST_TEST_REQUIRE(static_cast<bool>(sender));
    BOOST_TEST_REQUIRE(static_cast<bool>(peer));
    BOOST_TEST_REQUIRE(::shutdown(sender.get(), SocketShutdownWrite) == 0);
    peer.reset();

    BOOST_TEST(!sendBytes(sender, "closed"));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
