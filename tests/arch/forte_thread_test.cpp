/*******************************************************************************
 * Copyright (c) 2026 HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#include "forte/arch/forte_thread.h"

#include <atomic>

namespace forte::arch::test {
  namespace {
    class CRestartTestThread final : public CThread {
      public:
        std::atomic<int> mRuns{0};
        std::atomic<bool> mRunFinished{false};
        std::atomic<bool> mWaitForStop{false};

      private:
        void run() override {
          ++mRuns;
          while (mWaitForStop && isAlive()) {
            sleepThread(1);
          }
          sleepThread(100); // still busy after it was told to stop
          mRunFinished = true;
        }
    };
  } // namespace

  BOOST_AUTO_TEST_SUITE(forte_thread_test)

  BOOST_AUTO_TEST_CASE(endWaitsForASecondRun) {
    CRestartTestThread thread;

    // first run: ends by itself, the join leaves its completion posted
    thread.start();
    thread.join();
    BOOST_TEST(thread.mRunFinished);

    // second run: end() has to wait for this run, not take the completion of the first one
    thread.mRunFinished = false;
    thread.mWaitForStop = true;
    thread.start();
    for (int i = 0; i < 1000 && thread.mRuns < 2; ++i) {
      CThread::sleepThread(1);
    }
    BOOST_TEST(thread.mRuns == 2);
    thread.end();
    BOOST_TEST(thread.mRunFinished);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::arch::test
