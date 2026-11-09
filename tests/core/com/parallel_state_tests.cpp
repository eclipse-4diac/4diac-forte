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
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <memory>
#include <utility>

#include "forte_boost_output_support.h"
#include "../../../core/arch/common/src/com/impl/parallel_state.h"

namespace forte::com::test {
  namespace {
    struct MockCandidateState {
        bool mActive{true};
        std::size_t mResetCount{};
        std::size_t mReleaseCount{};
    };

    struct MockCandidate {
        std::shared_ptr<MockCandidateState> mState{std::make_shared<MockCandidateState>()};

        MockCandidate() = default;

        explicit MockCandidate(std::shared_ptr<MockCandidateState> paState) : mState(std::move(paState)) {
        }

        MockCandidate(const MockCandidate &) = delete;
        MockCandidate &operator=(const MockCandidate &) = delete;
        MockCandidate(MockCandidate &&paOther) noexcept = default;
        MockCandidate &operator=(MockCandidate &&paOther) noexcept = default;

        ~MockCandidate() {
          reset();
        }

        [[nodiscard]] explicit operator bool() const {
          return mState && mState->mActive;
        }

        void reset() {
          if (mState && mState->mActive) {
            mState->mActive = false;
            ++mState->mResetCount;
          }
        }

        [[nodiscard]] int release() {
          if (!mState || !mState->mActive) {
            return -1;
          }
          mState->mActive = false;
          ++mState->mReleaseCount;
          return 1;
        }
    };

    using MockParallelConnectionState = impl::ParallelConnectionState<MockCandidate>;
  } // namespace

  BOOST_AUTO_TEST_SUITE(PARALLEL_STATE)

  BOOST_AUTO_TEST_CASE(EMPTY_AND_PENDING) {
    MockParallelConnectionState state;

    BOOST_TEST(state.result() == ComResult::InvalidId);
    BOOST_TEST(!state.hasPendingCandidates());
    BOOST_TEST(!state.hasWinner());

    const std::size_t first = state.addCandidate({.mResult = ComResult::InProgress});
    const std::size_t second = state.addCandidate({.mResult = ComResult::InProgress});

    BOOST_TEST(second == first + 1);
    BOOST_TEST(state.result() == ComResult::InProgress);
    BOOST_TEST(state.hasPendingCandidates());
    BOOST_TEST(state.candidateCount() == 2);

    state.updateCandidate(first, ComResult::Unknown);
    BOOST_TEST(!state.candidateAt(first));
    BOOST_TEST(state.candidateAt(first).mState->mResetCount == 1);
    BOOST_TEST(state.result() == ComResult::InProgress);
    BOOST_TEST(state.candidateCount() == 2);

    state.updateCandidate(first, ComResult::ConnectionRefused);
    BOOST_TEST(state.candidateAt(first).mState->mResetCount == 1);
    state.updateCandidate(second, ComResult::HostUnreachable);
    BOOST_TEST(state.result() == ComResult::HostUnreachable);
  }

  BOOST_AUTO_TEST_CASE(FIRST_SUCCESS_TAKES_WINNER_AND_CLOSES_LOSERS) {
    MockParallelConnectionState state;
    const auto loserState = std::make_shared<MockCandidateState>();
    const auto winnerState = std::make_shared<MockCandidateState>();
    const auto first = state.addCandidate({.mResult = ComResult::InProgress, .mValue = MockCandidate{loserState}});
    const auto second = state.addCandidate({.mResult = ComResult::InProgress, .mValue = MockCandidate{winnerState}});

    state.updateCandidate(second, ComResult::Ok);
    state.updateCandidate(first, ComResult::Ok);
    state.updateCandidate(second, ComResult::ConnectionRefused);

    BOOST_TEST(state.result() == ComResult::Ok);
    BOOST_TEST(state.hasWinner());
    BOOST_TEST(state.hasPendingCandidates());

    const MockCandidate winner = state.takeWinner();
    BOOST_TEST(static_cast<bool>(winner));
    BOOST_TEST(state.candidateCount() == 0);
    BOOST_TEST(!state.hasWinner());
    BOOST_TEST(state.result() == ComResult::InvalidId);
    BOOST_TEST(loserState->mResetCount == 1);
    BOOST_TEST(winnerState->mResetCount == 0);
  }

  BOOST_AUTO_TEST_CASE(TAKEN_WINNER_CAN_BE_RELEASED) {
    MockParallelConnectionState state;
    const auto winnerState = std::make_shared<MockCandidateState>();
    const auto pendingState = std::make_shared<MockCandidateState>();
    const auto winner = state.addCandidate({.mResult = ComResult::InProgress, .mValue = MockCandidate{winnerState}});
    const auto pending = state.addCandidate({.mResult = ComResult::InProgress, .mValue = MockCandidate{pendingState}});

    BOOST_TEST(pending == winner + 1);
    state.updateCandidate(winner, ComResult::Ok);
    MockCandidate takenWinner = state.takeWinner();
    BOOST_TEST(takenWinner.release() == 1);
    BOOST_TEST(winnerState->mReleaseCount == 1);

    state.clear();
    BOOST_TEST(state.candidateCount() == 0);
    BOOST_TEST(state.result() == ComResult::InvalidId);
    BOOST_TEST(winnerState->mReleaseCount == 1);
    BOOST_TEST(winnerState->mResetCount == 0);
    BOOST_TEST(pendingState->mResetCount == 1);
  }

  BOOST_AUTO_TEST_CASE(CLEAR_RESETS_CANDIDATES) {
    MockParallelConnectionState state;
    const auto winnerState = std::make_shared<MockCandidateState>();
    const auto pendingState = std::make_shared<MockCandidateState>();
    const auto winner = state.addCandidate({.mResult = ComResult::InProgress, .mValue = MockCandidate{winnerState}});
    const auto pending = state.addCandidate({.mResult = ComResult::InProgress, .mValue = MockCandidate{pendingState}});

    BOOST_TEST(pending == winner + 1);
    state.updateCandidate(winner, ComResult::Ok);
    state.clear();

    BOOST_TEST(state.result() == ComResult::InvalidId);
    BOOST_TEST(state.candidateCount() == 0);
    BOOST_TEST(winnerState->mResetCount == 1);
    BOOST_TEST(pendingState->mResetCount == 1);
  }

  BOOST_AUTO_TEST_CASE(TOTAL_FAILURE) {
    MockParallelConnectionState state;
    const auto first = state.addCandidate({.mResult = ComResult::InProgress});
    const auto second = state.addCandidate({.mResult = ComResult::InProgress});

    state.updateCandidate(first, ComResult::ConnectionRefused);
    state.updateCandidate(second, ComResult::HostUnreachable);
    BOOST_TEST(state.result() == ComResult::ConnectionRefused);
    BOOST_TEST(!state.hasPendingCandidates());
    BOOST_TEST(state.candidateAt(first).mState->mResetCount == 1);
    BOOST_TEST(state.candidateAt(second).mState->mResetCount == 1);

    state.clear();
    BOOST_TEST(state.result() == ComResult::InvalidId);

    const auto reused = state.addCandidate({.mResult = ComResult::InProgress});
    state.updateCandidate(reused, ComResult::HostUnreachable);
    BOOST_TEST(state.result() == ComResult::HostUnreachable);
  }

  BOOST_AUTO_TEST_CASE(IMMEDIATE_OPEN_FAILURE) {
    MockParallelConnectionState state;
    const auto failedState = std::make_shared<MockCandidateState>(false);
    state.addCandidate({.mResult = ComResult::ConnectionRefused, .mValue = MockCandidate{failedState}});

    BOOST_TEST(!state.hasPendingCandidates());
    BOOST_TEST(state.result() == ComResult::ConnectionRefused);

    state.clear();
    BOOST_TEST(state.result() == ComResult::InvalidId);

    const auto reused = state.addCandidate({.mResult = ComResult::InProgress});
    state.updateCandidate(reused, ComResult::HostUnreachable);
    BOOST_TEST(state.result() == ComResult::HostUnreachable);
  }

  BOOST_AUTO_TEST_CASE(IMMEDIATE_OPEN_SUCCESS) {
    MockParallelConnectionState state;
    state.addCandidate({.mResult = ComResult::Ok});

    BOOST_TEST(state.result() == ComResult::Ok);
    BOOST_TEST(state.hasWinner());
    const MockCandidate winner = state.takeWinner();
    BOOST_TEST(static_cast<bool>(winner));
    BOOST_TEST(state.candidateCount() == 0);

    const auto reused = state.addCandidate({.mResult = ComResult::InProgress});
    state.updateCandidate(reused, ComResult::ConnectionRefused);
    BOOST_TEST(state.result() == ComResult::ConnectionRefused);
  }

  BOOST_AUTO_TEST_CASE(FIRST_IMMEDIATE_SUCCESS_WINS_AND_CLOSES_LOSERS) {
    MockParallelConnectionState state;
    const auto pendingState = std::make_shared<MockCandidateState>();
    const auto firstWinnerState = std::make_shared<MockCandidateState>();
    const auto laterWinnerState = std::make_shared<MockCandidateState>();
    const auto pending = state.addCandidate({.mResult = ComResult::InProgress, .mValue = MockCandidate{pendingState}});
    const auto firstWinner = state.addCandidate({.mResult = ComResult::Ok, .mValue = MockCandidate{firstWinnerState}});
    const auto laterWinner = state.addCandidate({.mResult = ComResult::Ok, .mValue = MockCandidate{laterWinnerState}});

    BOOST_TEST(firstWinner == pending + 1);
    BOOST_TEST(laterWinner == firstWinner + 1);
    BOOST_TEST(state.result() == ComResult::Ok);
    BOOST_TEST(state.hasWinner());

    const MockCandidate winner = state.takeWinner();
    BOOST_TEST(static_cast<bool>(winner));
    BOOST_TEST(winner.mState == firstWinnerState);
    BOOST_TEST(state.candidateCount() == 0);
    BOOST_TEST(!state.hasWinner());
    BOOST_TEST(state.result() == ComResult::InvalidId);
    BOOST_TEST(pendingState->mResetCount == 1);
    BOOST_TEST(firstWinnerState->mResetCount == 0);
    BOOST_TEST(laterWinnerState->mResetCount == 1);
  }

  BOOST_AUTO_TEST_CASE(IN_PROGRESS_DOES_NOT_REPLACE_RECORDED_ERROR) {
    MockParallelConnectionState state;
    const auto failedState = std::make_shared<MockCandidateState>(false);
    state.addCandidate({.mResult = ComResult::ConnectionRefused, .mValue = MockCandidate{failedState}});
    const auto pending = state.addCandidate({.mResult = ComResult::InProgress});

    BOOST_TEST(state.result() == ComResult::InProgress);
    state.updateCandidate(pending, ComResult::HostUnreachable);

    BOOST_TEST(state.result() == ComResult::ConnectionRefused);
    BOOST_TEST(!state.hasPendingCandidates());
  }

  BOOST_AUTO_TEST_CASE(NON_TERMINAL_ERRORS_RETAIN_CANDIDATES_AND_CAN_SUCCEED) {
    MockParallelConnectionState state;
    const auto wouldBlockState = std::make_shared<MockCandidateState>();
    const auto interruptedState = std::make_shared<MockCandidateState>();
    const auto asyncState = std::make_shared<MockCandidateState>();
    const auto moreState = std::make_shared<MockCandidateState>();
    const auto inProgressState = std::make_shared<MockCandidateState>();

    const auto wouldBlock = state.addCandidate({
        .mResult = ComResult::InProgress,
        .mValue = MockCandidate{wouldBlockState},
    });
    const auto interrupted = state.addCandidate({
        .mResult = ComResult::InProgress,
        .mValue = MockCandidate{interruptedState},
    });
    const auto async = state.addCandidate({
        .mResult = ComResult::InProgress,
        .mValue = MockCandidate{asyncState},
    });
    const auto more = state.addCandidate({
        .mResult = ComResult::InProgress,
        .mValue = MockCandidate{moreState},
    });
    const auto inProgress = state.addCandidate({
        .mResult = ComResult::InProgress,
        .mValue = MockCandidate{inProgressState},
    });

    state.updateCandidate(wouldBlock, ComResult::WouldBlock);
    state.updateCandidate(interrupted, ComResult::Interrupted);
    state.updateCandidate(async, ComResult::Async);
    state.updateCandidate(more, ComResult::More);
    state.updateCandidate(inProgress, ComResult::InProgress);

    BOOST_TEST(state.result() == ComResult::InProgress);
    BOOST_TEST(state.hasPendingCandidates());
    BOOST_TEST(state.candidateAt(wouldBlock).mState.get() == wouldBlockState.get());
    BOOST_TEST(state.candidateAt(interrupted).mState.get() == interruptedState.get());
    BOOST_TEST(state.candidateAt(async).mState.get() == asyncState.get());
    BOOST_TEST(state.candidateAt(more).mState.get() == moreState.get());
    BOOST_TEST(state.candidateAt(inProgress).mState.get() == inProgressState.get());
    BOOST_TEST(wouldBlockState->mResetCount == 0);
    BOOST_TEST(interruptedState->mResetCount == 0);
    BOOST_TEST(asyncState->mResetCount == 0);
    BOOST_TEST(moreState->mResetCount == 0);
    BOOST_TEST(inProgressState->mResetCount == 0);

    state.updateCandidate(wouldBlock, ComResult::Ok);
    BOOST_TEST(state.result() == ComResult::Ok);
    const MockCandidate winner = state.takeWinner();
    BOOST_TEST(winner.mState.get() == wouldBlockState.get());
  }

  BOOST_AUTO_TEST_CASE(NON_TERMINAL_RETRY_CAN_FAIL_TERMINALLY) {
    MockParallelConnectionState state;
    const auto candidateState = std::make_shared<MockCandidateState>();
    const auto candidate = state.addCandidate({
        .mResult = ComResult::InProgress,
        .mValue = MockCandidate{candidateState},
    });

    state.updateCandidate(candidate, ComResult::WouldBlock);
    BOOST_TEST(static_cast<bool>(state.candidateAt(candidate)));
    BOOST_TEST(candidateState->mResetCount == 0);
    BOOST_TEST(state.result() == ComResult::InProgress);

    state.updateCandidate(candidate, ComResult::ConnectionRefused);
    BOOST_TEST(!state.candidateAt(candidate));
    BOOST_TEST(candidateState->mResetCount == 1);
    BOOST_TEST(state.result() == ComResult::ConnectionRefused);
  }

  BOOST_AUTO_TEST_CASE(INVALID_ID_REPLACES_UNKNOWN_ERROR) {
    MockParallelConnectionState state;
    const auto first = state.addCandidate({.mResult = ComResult::InProgress});
    const auto second = state.addCandidate({.mResult = ComResult::InProgress});
    const auto third = state.addCandidate({.mResult = ComResult::InProgress});

    state.updateCandidate(state.candidateCount(), ComResult::ConnectionRefused);
    state.updateCandidate(first, ComResult::Unknown);
    state.updateCandidate(second, ComResult::InvalidId);
    state.updateCandidate(third, ComResult::Unknown);

    BOOST_TEST(state.result() == ComResult::InvalidId);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
