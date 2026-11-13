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
#include <boost/test/data/test_case.hpp>

#include <array>
#include <string>
#include <string_view>

#include "forte_boost_output_support.h"
#include "socket_test_helpers.h"
#include "com/impl/net.h"

namespace forte::com::test {
  namespace {
    void verifyWinnerUsable(Socket &paWinner, const auto paExpectedHandle, const Socket &paListener) {
      BOOST_TEST(paWinner.get() == paExpectedHandle);
      BOOST_TEST_REQUIRE(waitForSocketEvent(paListener, SocketReadEvent, Clock::now() + gcReadinessTimeout));
      const Socket accepted = acceptLoopbackConnection(paListener);
      BOOST_TEST_REQUIRE(static_cast<bool>(accepted));

      constexpr std::string_view request = "ping";
      std::string received;
      BOOST_TEST_REQUIRE(sendBytes(paWinner, request));
      BOOST_TEST_REQUIRE(receiveBytes(accepted, request.size(), received));
      BOOST_TEST(received == request);
      received.clear();

      constexpr std::string_view response = "pong";
      BOOST_TEST_REQUIRE(sendBytes(accepted, response));
      BOOST_TEST_REQUIRE(receiveBytes(paWinner, response.size(), received));
      BOOST_TEST(received == response);
      received.clear();
    }

  } // namespace

  BOOST_AUTO_TEST_SUITE(PARALLEL_CONNECTION_POLLING)

  BOOST_DATA_TEST_CASE(FAILURE_AND_SUCCESS_ORDERING,
                       boost::unit_test::data::make(std::array{false, true}),
                       successFirst) {
    LoopbackAddress refusedAddress{};
    const Socket reservation = createLoopbackBoundSocket(refusedAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(reservation));

    LoopbackAddress listenerAddress{};
    const Socket listener = createLoopbackListener(listenerAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(listener));

    AddressInfo refusedInfo = makeAddressInfo(refusedAddress);
    AddressInfo successInfo = makeAddressInfo(listenerAddress);

    AddressInfo *paAddresses;
    if (successFirst) {
      successInfo.ai_next = &refusedInfo;
      paAddresses = &successInfo;
    } else {
      refusedInfo.ai_next = &successInfo;
      paAddresses = &refusedInfo;
    }

    auto state = impl::net::openParallel(paAddresses, false);
    BOOST_TEST_REQUIRE(state.candidateCount() == 2U);
    const Socket &refusedCandidate = state.candidateAt(successFirst ? 1U : 0U);
    const Socket &successCandidate = state.candidateAt(successFirst ? 0U : 1U);
    const bool hasRefusedSocket = static_cast<bool>(refusedCandidate);
    const NativeSocket refusedHandle = refusedCandidate.get();
    const NativeSocket successHandle = successCandidate.get();

    BOOST_TEST_REQUIRE(static_cast<bool>(successCandidate));
    BOOST_TEST((state.result() == ComResult::InProgress || state.result() == ComResult::Ok));

    BOOST_TEST_REQUIRE(
        waitForSocketEvent(successHandle, SocketConnectCompletionEvent, Clock::now() + gcReadinessTimeout));
    if (hasRefusedSocket) {
      BOOST_TEST_REQUIRE(
          waitForSocketEvent(refusedHandle, SocketConnectCompletionEvent, Clock::now() + gcConnectionFailureTimeout));
    }
    BOOST_TEST_REQUIRE(waitForSocketEvent(listener, SocketReadEvent, Clock::now() + gcReadinessTimeout));

    auto [result, winner] = pollParallel(state);
    BOOST_TEST(result == ComResult::Ok);
    BOOST_TEST(static_cast<bool>(winner));
    if (hasRefusedSocket) {
      BOOST_TEST(nativeSocketIsClosed(refusedHandle));
    }

    verifyWinnerUsable(winner, successHandle, listener);
  }

  BOOST_AUTO_TEST_CASE(ALL_PARALLEL_CONNECTIONS_FAIL) {
    LoopbackAddress firstAddress{};
    const Socket firstReservation = createLoopbackBoundSocket(firstAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(firstReservation));

    LoopbackAddress secondAddress{};
    const Socket secondReservation = createLoopbackBoundSocket(secondAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(secondReservation));

    AddressInfo firstInfo = makeAddressInfo(firstAddress);
    AddressInfo secondInfo = makeAddressInfo(secondAddress);
    firstInfo.ai_next = &secondInfo;

    auto state = impl::net::openParallel(&firstInfo, false);
    BOOST_TEST_REQUIRE(state.candidateCount() == 2U);

    std::array<NativeSocket, 2> liveCandidateHandles{};
    std::size_t liveCandidateCount = 0;
    for (std::size_t candidateIndex = 0; candidateIndex < state.candidateCount(); ++candidateIndex) {
      const Socket &candidate = state.candidateAt(candidateIndex);
      if (candidate) {
        liveCandidateHandles[liveCandidateCount++] = candidate.get();
      }
    }

    const auto deadline = Clock::now() + gcConnectionFailureTimeout;
    for (std::size_t candidateIndex = 0; candidateIndex < liveCandidateCount; ++candidateIndex) {
      BOOST_TEST_REQUIRE(
          waitForSocketEvent(liveCandidateHandles[candidateIndex], SocketConnectCompletionEvent, deadline));
    }

    auto [result, winner] = pollParallel(state);
    BOOST_TEST(result == ComResult::ConnectionRefused);
    BOOST_TEST(!winner);
    BOOST_TEST(!state.hasWinner());
    BOOST_TEST(!state.hasPendingCandidates());
    BOOST_TEST(state.result() == ComResult::ConnectionRefused);
    for (std::size_t candidateIndex = 0; candidateIndex < liveCandidateCount; ++candidateIndex) {
      BOOST_TEST(nativeSocketIsClosed(liveCandidateHandles[candidateIndex]));
    }

    auto [retryResult, retryWinner] = pollParallel(state);
    BOOST_TEST(retryResult == ComResult::ConnectionRefused);
    BOOST_TEST(!retryWinner);
  }

  BOOST_AUTO_TEST_CASE(SEQUENTIAL_OPEN_FALLS_BACK_TO_NEXT_ADDRESS) {
    LoopbackAddress refusedAddress{};
    const Socket reservation = createLoopbackBoundSocket(refusedAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(reservation));

    LoopbackAddress listenerAddress{};
    const Socket listener = createLoopbackListener(listenerAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(listener));

    AddressInfo refusedInfo = makeAddressInfo(refusedAddress);
    AddressInfo successInfo = makeAddressInfo(listenerAddress);
    refusedInfo.ai_next = &successInfo;

    auto [result, winner] = impl::net::open(&refusedInfo, false);
    BOOST_TEST(result == ComResult::Ok);
    BOOST_TEST_REQUIRE(static_cast<bool>(winner));
    BOOST_TEST_REQUIRE(waitForSocketEvent(listener, SocketReadEvent, Clock::now() + gcReadinessTimeout));
    verifyWinnerUsable(winner, winner.get(), listener);
  }

  BOOST_AUTO_TEST_CASE(EMPTY_ADDRESS_LISTS) {
    BOOST_TEST(impl::net::open(static_cast<const AddressInfo *>(nullptr), false).mResult == ComResult::InvalidId);

    const auto state = impl::net::openParallel(static_cast<const AddressInfo *>(nullptr), false);
    BOOST_TEST(state.candidateCount() == 0U);
    BOOST_TEST(state.result() == ComResult::InvalidId);
  }

  BOOST_AUTO_TEST_CASE(WINNER_CLOSES_OTHER_LIVE_CANDIDATE) {
    LoopbackAddress winnerAddress{};
    const Socket winnerListener = createLoopbackListener(winnerAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(winnerListener));

    LoopbackAddress otherAddress{};
    const Socket otherListener = createLoopbackListener(otherAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(otherListener));

    auto winnerCandidate = openPendingConnection(winnerAddress);
    auto otherCandidate = openPendingConnection(otherAddress);
    const NativeSocket winnerHandle = winnerCandidate.mValue.get();
    const NativeSocket otherHandle = otherCandidate.mValue.get();

    impl::ParallelConnectionState<Socket> state;
    state.addCandidate(std::move(winnerCandidate));
    state.addCandidate(std::move(otherCandidate));
    BOOST_TEST_REQUIRE(state.candidateCount() == 2U);
    BOOST_TEST_REQUIRE(static_cast<bool>(state.candidateAt(0)));
    BOOST_TEST_REQUIRE(static_cast<bool>(state.candidateAt(1)));
    BOOST_TEST(state.result() == ComResult::InProgress);
    BOOST_TEST_REQUIRE(
        waitForSocketEvent(winnerHandle, SocketConnectCompletionEvent, Clock::now() + gcReadinessTimeout));
    BOOST_TEST_REQUIRE(waitForSocketEvent(winnerListener, SocketReadEvent, Clock::now() + gcReadinessTimeout));
    BOOST_TEST_REQUIRE(static_cast<bool>(state.candidateAt(1)));

    auto [result, winner] = pollParallel(state);
    BOOST_TEST(result == ComResult::Ok);
    BOOST_TEST(static_cast<bool>(winner));
    BOOST_TEST(nativeSocketIsClosed(otherHandle));

    verifyWinnerUsable(winner, winnerHandle, winnerListener);
  }

  BOOST_AUTO_TEST_CASE(IMMEDIATE_WINNER_IS_HANDED_OFF_AND_CLOSES_OTHER_LIVE_CANDIDATE) {
    LoopbackAddress winnerAddress{};
    const Socket winnerListener = createLoopbackListener(winnerAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(winnerListener));

    LoopbackAddress otherAddress{};
    const Socket otherListener = createLoopbackListener(otherAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(otherListener));

    Socket winnerCandidate = connectLoopback(winnerAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(winnerCandidate));
    auto otherCandidate = openPendingConnection(otherAddress);
    BOOST_TEST_REQUIRE(otherCandidate.mResult == ComResult::InProgress);
    BOOST_TEST_REQUIRE(static_cast<bool>(otherCandidate.mValue));

    const NativeSocket winnerHandle = winnerCandidate.get();
    const NativeSocket otherHandle = otherCandidate.mValue.get();
    impl::ParallelConnectionState<Socket> state;
    state.addCandidate({.mResult = ComResult::Ok, .mValue = std::move(winnerCandidate)});
    state.addCandidate(std::move(otherCandidate));

    BOOST_TEST(state.result() == ComResult::Ok);
    BOOST_TEST_REQUIRE(state.candidateCount() == 2U);
    BOOST_TEST_REQUIRE(static_cast<bool>(state.candidateAt(0)));
    BOOST_TEST_REQUIRE(static_cast<bool>(state.candidateAt(1)));

    auto [result, winner] = pollParallel(state);
    BOOST_TEST(result == ComResult::Ok);
    BOOST_TEST(static_cast<bool>(winner));
    BOOST_TEST(winner.get() == winnerHandle);
    BOOST_TEST(state.candidateCount() == 0U);
    BOOST_TEST(!state.hasWinner());
    BOOST_TEST(state.result() == ComResult::InvalidId);
    BOOST_TEST(nativeSocketIsClosed(otherHandle));

    verifyWinnerUsable(winner, winnerHandle, winnerListener);
  }

  BOOST_AUTO_TEST_CASE(IMMEDIATE_WINNER_IS_HANDED_OFF_AFTER_PEER_RESET) {
    LoopbackAddress winnerAddress{};
    const Socket winnerListener = createLoopbackListener(winnerAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(winnerListener));

    LoopbackAddress otherAddress{};
    const Socket otherListener = createLoopbackListener(otherAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(otherListener));

    Socket winnerCandidate = connectLoopback(winnerAddress);
    BOOST_TEST_REQUIRE(static_cast<bool>(winnerCandidate));
    BOOST_TEST_REQUIRE(waitForSocketEvent(winnerListener, SocketReadEvent, Clock::now() + gcReadinessTimeout));
    Socket resetPeer = acceptLoopbackConnection(winnerListener);
    BOOST_TEST_REQUIRE(static_cast<bool>(resetPeer));
    auto otherCandidate = openPendingConnection(otherAddress);
    BOOST_TEST_REQUIRE(otherCandidate.mResult == ComResult::InProgress);
    BOOST_TEST_REQUIRE(static_cast<bool>(otherCandidate.mValue));

    const NativeSocket winnerHandle = winnerCandidate.get();
    const NativeSocket otherHandle = otherCandidate.mValue.get();
    impl::ParallelConnectionState<Socket> state;
    state.addCandidate({.mResult = ComResult::Ok, .mValue = std::move(winnerCandidate)});
    state.addCandidate(std::move(otherCandidate));

    BOOST_TEST(state.result() == ComResult::Ok);
    BOOST_TEST_REQUIRE(state.candidateCount() == 2U);
    BOOST_TEST_REQUIRE(static_cast<bool>(state.candidateAt(0)));
    BOOST_TEST_REQUIRE(static_cast<bool>(state.candidateAt(1)));
    BOOST_TEST_REQUIRE(closeWithReset(resetPeer));
    BOOST_TEST_REQUIRE(waitForSocketEvent(winnerHandle, SocketReadEvent, Clock::now() + gcReadinessTimeout));

    auto [result, winner] = pollParallel(state);
    BOOST_TEST(result == ComResult::Ok);
    BOOST_TEST(static_cast<bool>(winner));
    BOOST_TEST(winner.get() == winnerHandle);
    BOOST_TEST(state.candidateCount() == 0U);
    BOOST_TEST(!state.hasWinner());
    BOOST_TEST(state.result() == ComResult::InvalidId);
    BOOST_TEST(nativeSocketIsClosed(otherHandle));
    BOOST_TEST(receiveSocketResult(winner) == ComResult::ConnectionReset);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
