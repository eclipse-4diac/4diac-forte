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
#include <utility>

#include "forte/com/factory.h"
#include "forte/stringid.h"
#include "forte_boost_output_support.h"
#include "com/impl/tcp_parallel.h"
#include "socket_test_helpers.h"

using namespace forte::literals;

namespace forte::com::test {

  namespace {
    class TestTCPParallelChannel final : public impl::TCPParallelChannel {
      public:
        using impl::TCPParallelChannel::getConnectionState;

        explicit TestTCPParallelChannel(ComObserver<ComBuffer> &paObserver) : TCPParallelChannel(paObserver) {
        }
    };

    struct TCPParallelClientFixture {
        TCPParallelClientFixture() :
            mListener(createLoopbackListener(mAddress)),
            mConfig("127.0.0.1:" + std::to_string(ntohs(mAddress.sin_port))),
            mChannel(ComChannelFactory<ComBuffer>::create("tcp_parallel"_STRID, mObserver)) {
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

  BOOST_FIXTURE_TEST_SUITE(TCP_PARALLEL_CHANNEL, TCPParallelClientFixture)

  BOOST_AUTO_TEST_CASE(CONNECT) {
    BOOST_TEST_REQUIRE(openUntilConnected(*mChannel, mConfig) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForSocketEvent(mListener.get(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    const Socket accepted(::accept(mListener.get(), nullptr, nullptr));
    BOOST_TEST_REQUIRE(static_cast<bool>(accepted));

    BOOST_TEST(mChannel->close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(SEND_AND_RECEIVE_DATA) {
    BOOST_TEST_REQUIRE(mChannel->setMaxReceiveSize(32) == ComResult::Ok);
    BOOST_TEST_REQUIRE(mChannel->setMinReceiveSize(4) == ComResult::Ok);
    BOOST_TEST_REQUIRE(openUntilConnected(*mChannel, mConfig) == ComResult::Ok);
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

  BOOST_AUTO_TEST_CASE(CLOSE_REOPEN) {
    sockaddr_in firstAddress{};
    const Socket firstListener = createLoopbackListener(firstAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstListener));

    sockaddr_in secondAddress{};
    const Socket secondListener = createLoopbackListener(secondAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(secondListener));

    const std::string firstConfig = "127.0.0.1:" + std::to_string(ntohs(firstAddress.sin_port));
    BOOST_TEST(openUntilConnected(*mChannel, firstConfig) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForSocketEvent(firstListener.get(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    const Socket firstAccepted(::accept(firstListener.get(), nullptr, nullptr));
    BOOST_TEST_REQUIRE(static_cast<bool>(firstAccepted));

    BOOST_TEST(mChannel->close() == ComResult::Ok);

    const std::string secondConfig = "127.0.0.1:" + std::to_string(ntohs(secondAddress.sin_port));
    BOOST_TEST(openUntilConnected(*mChannel, secondConfig) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForSocketEvent(secondListener.get(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    const Socket secondAccepted(::accept(secondListener.get(), nullptr, nullptr));
    BOOST_TEST_REQUIRE(static_cast<bool>(secondAccepted));

    BOOST_TEST(mChannel->close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(CONNECTION_REFUSED) {
    sockaddr_in address{};
    const Socket reservation = createLoopbackBoundSocket(address);
    BOOST_TEST_REQUIRE(static_cast<bool>(reservation));

    const std::string config = "127.0.0.1:" + std::to_string(ntohs(address.sin_port));

    BOOST_TEST(openUntilConnected(*mChannel, config, gcConnectionFailureTimeout) == ComResult::ConnectionRefused);
    BOOST_TEST(!mChannel->isOpen());
  }

  BOOST_AUTO_TEST_CASE(CLOSE_CLEARS_PENDING_CANDIDATES) {
    TestTCPParallelChannel channel(mObserver);
    sockaddr_in pendingAddress{};
    Socket pendingCandidate = createLoopbackBoundSocket(pendingAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(pendingCandidate));
    const NativeSocket pendingHandle = pendingCandidate.get();
    channel.getConnectionState().addCandidate(
        {.mResult = ComResult::InProgress, .mValue = std::move(pendingCandidate)});

    auto &state = channel.getConnectionState();
    BOOST_TEST(state.result() == ComResult::InProgress);
    BOOST_TEST(state.candidateCount() == 1U);
    BOOST_TEST_REQUIRE(static_cast<bool>(state.candidateAt(0)));
    BOOST_TEST(!state.hasWinner());
    BOOST_TEST(!channel.isOpen());

    BOOST_TEST(channel.close() == ComResult::NoSocket);
    BOOST_TEST(state.candidateCount() == 0U);
    BOOST_TEST(!state.hasWinner());
    BOOST_TEST(!state.hasPendingCandidates());
    BOOST_TEST(state.result() == ComResult::InvalidId);
    BOOST_TEST(nativeSocketIsClosed(pendingHandle));

    BOOST_TEST_REQUIRE(openUntilConnected(channel, mConfig) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForSocketEvent(mListener.get(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    const Socket accepted(::accept(mListener.get(), nullptr, nullptr));
    BOOST_TEST_REQUIRE(static_cast<bool>(accepted));
    BOOST_TEST(channel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(REUSES_SEEDED_PENDING_CONNECTION) {
    sockaddr_in address{};
    const Socket listener = createLoopbackListener(address);
    BOOST_TEST_REQUIRE(static_cast<bool>(listener));

    Socket candidate = connectLoopback(address);
    BOOST_TEST_REQUIRE(static_cast<bool>(candidate));
    BOOST_TEST_REQUIRE(waitForSocketEvent(listener, SocketReadEvent, Clock::now() + gcReadinessTimeout));
    const Socket accepted = acceptLoopbackConnection(listener);
    BOOST_TEST_REQUIRE(static_cast<bool>(accepted));

    TestTCPParallelChannel channel(mObserver);
    auto &state = channel.getConnectionState();
    state.addCandidate({.mResult = ComResult::InProgress, .mValue = std::move(candidate)});
    const NativeSocket candidateHandle = state.candidateAt(0).get();

    BOOST_TEST(channel.open("invalid replacement configuration", std::span<ComChannelDescriptor>{}) == ComResult::Ok);
    BOOST_TEST(channel.isOpen());
    BOOST_TEST(!state.hasPendingCandidates());

    std::string request{"ping"};
    std::string received;
    BOOST_TEST_REQUIRE(channel.send(ComBuffer(request)) == ComResult::Ok);
    BOOST_TEST_REQUIRE(receiveBytes(accepted, request.size(), received));
    BOOST_TEST(received == request);
    BOOST_TEST(!nativeSocketIsClosed(candidateHandle));

    BOOST_TEST(channel.close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(REPEATED_OPEN) {
    sockaddr_in address{};
    const Socket listener = createLoopbackListener(address);
    BOOST_TEST_REQUIRE(static_cast<bool>(listener));

    sockaddr_in secondAddress{};
    const Socket secondListener = createLoopbackListener(secondAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(secondListener));

    const std::string config = "127.0.0.1:" + std::to_string(ntohs(address.sin_port));
    const std::string replacementConfig = "127.0.0.1:" + std::to_string(ntohs(secondAddress.sin_port));

    const ComResult firstResult = mChannel->open(config, std::span<ComChannelDescriptor>{});
    BOOST_TEST_REQUIRE((firstResult == ComResult::InProgress || firstResult == ComResult::Ok));
    BOOST_TEST_REQUIRE(openUntilConnected(*mChannel, replacementConfig) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForSocketEvent(listener.get(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    const Socket accepted(::accept(listener.get(), nullptr, nullptr));
    BOOST_TEST_REQUIRE(static_cast<bool>(accepted));

    BOOST_TEST(mChannel->close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_CASE(ORDERLY_CLOSE) {
    BOOST_TEST_REQUIRE(mChannel->setMaxReceiveSize(16) == ComResult::Ok);
    BOOST_TEST_REQUIRE(openUntilConnected(*mChannel, mConfig) == ComResult::Ok);
    BOOST_TEST_REQUIRE(waitForSocketEvent(mListener.get(), SocketReadEvent, Clock::now() + gcReadinessTimeout));
    const Socket accepted(::accept(mListener.get(), nullptr, nullptr));
    BOOST_TEST_REQUIRE(static_cast<bool>(accepted));

    BOOST_TEST_REQUIRE(::shutdown(accepted.get(), SocketShutdownWrite) == 0);
    BOOST_TEST(pollChannelUntilReady(*mChannel) == ComResult::RecvFailed);
    BOOST_TEST(mChannel->close() == ComResult::Ok);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
