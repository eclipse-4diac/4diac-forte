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

#include "forte_boost_output_support.h"
#include "socket_test_helpers.h"

namespace forte::com::test {

  BOOST_AUTO_TEST_SUITE(SOCKET_TEST_HELPERS)

  BOOST_AUTO_TEST_CASE(HANGUP_DOES_NOT_SATISFY_WRITE_READINESS) {
    auto [socket, peer] = socketpair();
    BOOST_TEST_REQUIRE(static_cast<bool>(socket));
    BOOST_TEST_REQUIRE(static_cast<bool>(peer));
    peer.reset();

    BOOST_TEST(waitForSocketEvent(socket.get(), SocketConnectCompletionEvent, Clock::now() + gcReadinessTimeout));
    BOOST_TEST(!waitForSocketEvent(socket.get(), SocketWriteEvent, Clock::now() + gcReadinessTimeout));
    BOOST_TEST(!sendBytes(socket, "message"));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
