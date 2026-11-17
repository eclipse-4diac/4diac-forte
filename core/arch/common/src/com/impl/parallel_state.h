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

#pragma once

#include <concepts>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include "forte/com/common.h"

namespace forte::com::impl {
  /**
   * \brief ParallelConnectionCandidate represents a candidate for a parallel connection.
   *
   * This concept defines the requirements for a type to be used as a parallel connection candidate.
   * It requires the type to be movable and provide certain operations.
   * Candidates own their resources and release them when destroyed.
   */
  template<typename Candidate>
  concept ParallelConnectionCandidate = std::movable<Candidate> && requires(Candidate &paCandidate) {
    { static_cast<bool>(paCandidate) } -> std::same_as<bool>;
    paCandidate.reset();
    paCandidate.release();
  };

  /**
   * \brief ParallelConnectionState represents the state of a parallel connection.
   *
   * This class manages a collection of parallel connection candidates and tracks the winner.
   * It provides methods to add candidates, check if a winner has been determined, and retrieve the winner.
   */
  template<ParallelConnectionCandidate Candidate>
  class ParallelConnectionState {
    public:
      ParallelConnectionState() = default;
      ParallelConnectionState(ParallelConnectionState &) = delete;
      ParallelConnectionState(ParallelConnectionState &&) = default;
      ParallelConnectionState &operator=(ParallelConnectionState &) = delete;
      ParallelConnectionState &operator=(ParallelConnectionState &&) = default;

      std::size_t addCandidate(ComResultValue<Candidate> paCandidate) {
        if (paCandidate.mResult == ComResult::Ok && !mWinner) {
          mWinner = mCandidates.size();
        }
        updateError(paCandidate.mResult);
        mCandidates.emplace_back(std::move(paCandidate.mValue));
        return mCandidates.size() - 1;
      }

      void updateCandidate(const std::size_t paIndex, const ComResult paResult) {
        if (paIndex >= mCandidates.size() || !mCandidates[paIndex] || mWinner == paIndex) {
          return;
        }
        if (paResult == ComResult::Ok && !mWinner) {
          mWinner = paIndex;
        }
        updateError(paResult);
        if (isTerminalError(paResult)) {
          mCandidates[paIndex].reset();
        }
      }

      [[nodiscard]] ComResult result() const {
        if (hasWinner()) {
          return ComResult::Ok;
        }
        if (!hasCandidates()) {
          return ComResult::InvalidId;
        }
        if (hasPendingCandidates()) {
          return ComResult::InProgress;
        }
        return mError;
      }

      [[nodiscard]] bool hasCandidates() const {
        return !mCandidates.empty();
      }

      [[nodiscard]] bool hasPendingCandidates() const {
        for (const Candidate &candidate : mCandidates) {
          if (candidate) {
            return true;
          }
        }
        return false;
      }

      [[nodiscard]] bool hasWinner() const {
        return mWinner.has_value();
      }

      [[nodiscard]] Candidate takeWinner() {
        const std::size_t winnerIndex = mWinner.value();
        Candidate winner = std::move(mCandidates[winnerIndex]);
        mError = ComResult::Unknown;
        mCandidates.clear();
        mWinner.reset();
        return winner;
      }

      [[nodiscard]] std::size_t candidateCount() const {
        return mCandidates.size();
      }

      [[nodiscard]] Candidate &candidateAt(const std::size_t paIndex) {
        return mCandidates[paIndex];
      }

      [[nodiscard]] const Candidate &candidateAt(const std::size_t paIndex) const {
        return mCandidates[paIndex];
      }

      void clear() {
        for (Candidate &candidate : mCandidates) {
          if (candidate) {
            candidate.reset();
          }
        }
        mError = ComResult::Unknown;
        mCandidates.clear();
        mWinner.reset();
      }

    private:
      void updateError(const ComResult paError) {
        if (paError != ComResult::Ok && (mError == ComResult::Unknown || !isTerminalError(mError))) {
          mError = paError;
        }
      }

      static bool isTerminalError(const ComResult paError) {
        switch (paError) {
          case ComResult::Ok:
          case ComResult::Async:
          case ComResult::More:
          case ComResult::WouldBlock:
          case ComResult::Interrupted:
          case ComResult::InProgress: return false;
          default: return true;
        }
      }

      ComResult mError{ComResult::Unknown};
      std::vector<Candidate> mCandidates;
      std::optional<std::size_t> mWinner;
  };
} // namespace forte::com::impl
