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
#include <utility>
#include <vector>

#include "forte/stringid.h"
#include "forte_boost_output_support.h"
#include "com/impl/tcp.h"
#include "socket_test_helpers.h"

using namespace forte::literals;

namespace forte::com::test {
  namespace {
    class ScriptedSocketChannel final : public impl::TCPChannel {
      public:
        explicit ScriptedSocketChannel(ComObserver<ComBuffer> &paObserver) : TCPChannel(paObserver) {
        }

        [[nodiscard]] bool supports(const std::span<const ComChannelDescriptor> paDescriptors) const override {
          return supportsChannel("scripted-socket"_STRID, paDescriptors);
        }

        void addResult(const ComResult paResult) {
          mResults.push_back(paResult);
        }

        std::vector<std::string> mConfigs;

        ComResult attach(Socket paSocket) {
          return setSocket(std::move(paSocket));
        }

        bool waitForReceiveData(const std::size_t paExpectedSize) const {
          return waitForSocketData(getSocket(), paExpectedSize);
        }

      protected:
        ComResultValue<Socket> socket(const std::string_view paConfigString) override {
          mConfigs.emplace_back(paConfigString);
          const ComResult result = mNextResult < mResults.size() ? mResults[mNextResult++] : ComResult::Unknown;
          if (result != ComResult::Ok) {
            return {.mResult = result};
          }

          Socket socket{::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)};
          if (!socket) {
            return {.mResult = ComResult::Unknown};
          }
          return {.mValue = std::move(socket)};
        }

      private:
        std::vector<ComResult> mResults;
        std::size_t mNextResult{0};
    };
  } // namespace

  BOOST_AUTO_TEST_SUITE(SOCKET_CHANNEL)

  BOOST_AUTO_TEST_CASE(OPEN_INVALID) {
    BufferObserver observer;
    ScriptedSocketChannel channel{observer};
    channel.addResult(ComResult::Ok);

    BOOST_TEST(channel.open("first", std::array{
                                         ComChannelDescriptor{"scripted-socket"_STRID, "first"},
                                     }) == ComResult::InvalidId);
    BOOST_TEST(channel.mConfigs.empty());
  }

  BOOST_AUTO_TEST_CASE(REPEATED_OPEN) {
    BufferObserver observer;
    ScriptedSocketChannel channel{observer};
    channel.addResult(ComResult::Ok);

    BOOST_TEST_REQUIRE(channel.open("first", {}) == ComResult::Ok);
    BOOST_TEST(channel.isOpen());
    BOOST_TEST(channel.mConfigs.size() == 1);
    BOOST_TEST(channel.mConfigs.back() == "first");

    BOOST_TEST(channel.open("changed", {}) == ComResult::Ok);
    BOOST_TEST(channel.isOpen());
    BOOST_TEST(channel.mConfigs.size() == 1);
    BOOST_TEST(channel.mConfigs.back() == "first");

    BOOST_TEST(channel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(OPEN_FAILURE) {
    BufferObserver observer;
    ScriptedSocketChannel channel{observer};
    channel.addResult(ComResult::InvalidId);
    channel.addResult(ComResult::Ok);

    BOOST_TEST(channel.open("first", {}) == ComResult::InvalidId);
    BOOST_TEST_REQUIRE(channel.mConfigs.size() == 1);
    BOOST_TEST(channel.mConfigs.back() == "first");

    BOOST_TEST_REQUIRE(channel.open("fresh", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(channel.mConfigs.size() == 2);
    BOOST_TEST(channel.mConfigs.back() == "fresh");
    BOOST_TEST(channel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(CLOSE_REOPEN) {
    BufferObserver observer;
    ScriptedSocketChannel channel{observer};
    channel.addResult(ComResult::Ok);
    channel.addResult(ComResult::Ok);

    BOOST_TEST(channel.open("first", {}) == ComResult::Ok);
    BOOST_TEST(channel.close() == ComResult::Ok);

    BOOST_TEST(channel.open("fresh", {}) == ComResult::Ok);
    BOOST_TEST_REQUIRE(channel.mConfigs.size() == 2);
    BOOST_TEST(channel.mConfigs.back() == "fresh");

    BOOST_TEST(channel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(POLL_ON_CLOSED_CHANNEL) {
    BufferObserver observer;
    ScriptedSocketChannel channel{observer};

    BOOST_TEST(channel.poll() == ComResult::NoSocket);
    BOOST_TEST_REQUIRE(channel.setMaxReceiveSize(8) == ComResult::Ok);
    BOOST_TEST(channel.poll() == ComResult::NoSocket);
  }

  BOOST_AUTO_TEST_CASE(FRAGMENTED_DATA) {
    BufferObserver observer;
    ScriptedSocketChannel channel{observer};
    auto [socket, peer] = socketpair();
    BOOST_TEST_REQUIRE(static_cast<bool>(socket));
    BOOST_TEST_REQUIRE(static_cast<bool>(peer));
    BOOST_TEST_REQUIRE(channel.setMaxReceiveSize(8) == ComResult::Ok);
    BOOST_TEST_REQUIRE(channel.setMinReceiveSize(2) == ComResult::Ok);
    BOOST_TEST_REQUIRE(channel.attach(std::move(socket)) == ComResult::Ok);

    BOOST_TEST_REQUIRE(sendBytes(peer, "a"));
    BOOST_TEST_REQUIRE(channel.waitForReceiveData(1));
    BOOST_TEST(channel.poll() == ComResult::More);
    BOOST_TEST(observer.mReceived.empty());

    observer.mReceiveResult = ComResult::More;
    BOOST_TEST_REQUIRE(sendBytes(peer, "b"));
    BOOST_TEST_REQUIRE(channel.waitForReceiveData(1));
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(observer.mReceived == "ab");

    observer.mReceiveResult = ComResult::Ok;
    BOOST_TEST_REQUIRE(sendBytes(peer, "c"));
    BOOST_TEST_REQUIRE(channel.waitForReceiveData(1));
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(observer.mReceived == "abc");

    BOOST_TEST_REQUIRE(channel.setMinReceiveSize(1) == ComResult::Ok);
    BOOST_TEST_REQUIRE(sendBytes(peer, "d"));
    BOOST_TEST_REQUIRE(channel.waitForReceiveData(1));
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(observer.mReceived == "d");
    BOOST_TEST(channel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(RECEIVE_LIMITS) {
    BufferObserver observer;
    ScriptedSocketChannel channel{observer};
    auto [socket, peer] = socketpair();
    BOOST_TEST_REQUIRE(static_cast<bool>(socket));
    BOOST_TEST_REQUIRE(static_cast<bool>(peer));
    BOOST_TEST_REQUIRE(channel.setMaxReceiveSize(0) == ComResult::Ok);
    BOOST_TEST_REQUIRE(channel.attach(std::move(socket)) == ComResult::Ok);

    BOOST_TEST(channel.poll() == ComResult::WouldBlock);

    BOOST_TEST_REQUIRE(channel.setMaxReceiveSize(3) == ComResult::Ok);
    BOOST_TEST_REQUIRE(sendBytes(peer, "abcd"));
    BOOST_TEST_REQUIRE(channel.waitForReceiveData(4));

    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(observer.mReceived == "abc");

    BOOST_TEST_REQUIRE(channel.waitForReceiveData(1));
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(observer.mReceived == "d");

    BOOST_TEST(channel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(RECEIVE_LIMITS_INVERTED) {
    BufferObserver observer;
    ScriptedSocketChannel channel{observer};
    auto [socket, peer] = socketpair();
    BOOST_TEST_REQUIRE(static_cast<bool>(socket));
    BOOST_TEST_REQUIRE(static_cast<bool>(peer));
    BOOST_TEST_REQUIRE(channel.setMaxReceiveSize(2) == ComResult::Ok);
    BOOST_TEST_REQUIRE(channel.setMinReceiveSize(3) == ComResult::Ok);
    BOOST_TEST_REQUIRE(channel.attach(std::move(socket)) == ComResult::Ok);

    BOOST_TEST_REQUIRE(sendBytes(peer, "ab"));
    BOOST_TEST_REQUIRE(channel.waitForReceiveData(2));
    BOOST_TEST(channel.poll() == ComResult::More);
    BOOST_TEST(observer.mReceived.empty());

    BOOST_TEST(channel.poll() == ComResult::WouldBlock);

    BOOST_TEST_REQUIRE(channel.setMaxReceiveSize(1) == ComResult::Ok);
    BOOST_TEST(channel.poll() == ComResult::WouldBlock);

    BOOST_TEST_REQUIRE(channel.setMaxReceiveSize(3) == ComResult::Ok);
    BOOST_TEST_REQUIRE(sendBytes(peer, "c"));
    BOOST_TEST_REQUIRE(channel.waitForReceiveData(1));
    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(observer.mReceived == "abc");

    BOOST_TEST(channel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(FULL_RECEIVE_BUFFER) {
    BufferObserver observer;
    observer.mReceiveResult = ComResult::More;
    ScriptedSocketChannel channel{observer};
    auto [socket, peer] = socketpair();
    BOOST_TEST_REQUIRE(static_cast<bool>(socket));
    BOOST_TEST_REQUIRE(static_cast<bool>(peer));
    BOOST_TEST_REQUIRE(channel.setMaxReceiveSize(3) == ComResult::Ok);
    BOOST_TEST_REQUIRE(channel.attach(std::move(socket)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(sendBytes(peer, "abc"));
    BOOST_TEST_REQUIRE(channel.waitForReceiveData(3));

    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(observer.mReceived == "abc");

    BOOST_TEST(channel.poll() == ComResult::WouldBlock);
    BOOST_TEST_REQUIRE(channel.setMaxReceiveSize(4) == ComResult::Ok);

    observer.mReceiveResult = ComResult::Ok;
    BOOST_TEST_REQUIRE(sendBytes(peer, "d"));
    BOOST_TEST_REQUIRE(channel.waitForReceiveData(1));

    BOOST_TEST(channel.poll() == ComResult::Async);
    BOOST_TEST(observer.mReceived == "abcd");

    BOOST_TEST(channel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
