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

#include <winsock2.h>
#include <ws2tcpip.h>

#include "forte_boost_output_support.h"
#include "../../../../core/arch/win32/src/com/impl/net.h"

namespace forte::com::test {

  BOOST_AUTO_TEST_SUITE(WIN32_SOCKET_ERROR_MAPPING)

  BOOST_AUTO_TEST_CASE(SOCKET_ERRORS_MAP_TO_COMMUNICATION_RESULTS) {
    BOOST_TEST(impl::net::mapSocketError(0, ComResult::Unknown) == ComResult::Ok);
    BOOST_TEST(impl::net::mapSocketError(0, ComResult::SendFailed) == ComResult::Ok);
    BOOST_TEST(impl::net::mapSocketError(WSAECONNREFUSED, ComResult::Unknown) == ComResult::ConnectionRefused);
    BOOST_TEST(impl::net::mapSocketError(WSAEWOULDBLOCK, ComResult::Unknown) == ComResult::WouldBlock);
    BOOST_TEST(impl::net::mapSocketError(WSAENOTSOCK, ComResult::Unknown) == ComResult::NoSocket);
    BOOST_TEST(impl::net::mapSocketError(WSAEOPNOTSUPP, ComResult::SendFailed) == ComResult::SendFailed);
  }

  BOOST_AUTO_TEST_CASE(RESOLVER_ERRORS_MAP_TO_COMMUNICATION_RESULTS) {
    BOOST_TEST(impl::net::mapResolverError(WSAHOST_NOT_FOUND, ComResult::InvalidId) == ComResult::NameNotResolved);
#ifdef WSATRY_AGAIN
    BOOST_TEST(impl::net::mapResolverError(WSATRY_AGAIN, ComResult::InvalidId) ==
               ComResult::NameResolutionTemporaryFailure);
#endif
    BOOST_TEST(impl::net::mapResolverError(-1, ComResult::InvalidId) == ComResult::InvalidId);
  }

  BOOST_AUTO_TEST_CASE(RESOLVE_REJECTS_MALFORMED_ADDRESS) {
    constexpr ADDRINFOA hints{};
    BOOST_TEST(impl::net::resolve("missing-port", hints).mResult == ComResult::InvalidId);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
