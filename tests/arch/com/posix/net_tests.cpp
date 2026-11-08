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

#include <cerrno>
#include <netdb.h>

#include "forte_boost_output_support.h"
#include "../../../../core/arch/posix/src/com/impl/net.h"

namespace forte::com::test {

  BOOST_AUTO_TEST_SUITE(POSIX_SOCKET_ERROR_MAPPING)

  BOOST_AUTO_TEST_CASE(SOCKET_ERRORS_MAP_TO_COMMUNICATION_RESULTS) {
    BOOST_TEST(impl::net::mapSocketError(ECONNREFUSED, ComResult::Unknown) == ComResult::ConnectionRefused);
    BOOST_TEST(impl::net::mapSocketError(EAGAIN, ComResult::Unknown) == ComResult::WouldBlock);
    BOOST_TEST(impl::net::mapSocketError(EBADF, ComResult::Unknown) == ComResult::NoSocket);
    BOOST_TEST(impl::net::mapSocketError(ENOENT, ComResult::SendFailed) == ComResult::SendFailed);
  }

  BOOST_AUTO_TEST_CASE(RESOLVER_ERRORS_MAP_TO_COMMUNICATION_RESULTS) {
    BOOST_TEST(impl::net::mapResolverError(EAI_NONAME, 0, ComResult::InvalidId) == ComResult::NameNotResolved);
#ifdef EAI_AGAIN
    BOOST_TEST(impl::net::mapResolverError(EAI_AGAIN, 0, ComResult::InvalidId) ==
               ComResult::NameResolutionTemporaryFailure);
#endif
#ifdef EAI_SYSTEM
    BOOST_TEST(impl::net::mapResolverError(EAI_SYSTEM, EACCES, ComResult::InvalidId) == ComResult::PermissionDenied);
#endif
    BOOST_TEST(impl::net::mapResolverError(-1, 0, ComResult::InvalidId) == ComResult::InvalidId);
  }

  BOOST_AUTO_TEST_CASE(RESOLVE_REJECTS_MALFORMED_ADDRESS) {
    constexpr addrinfo hints{};
    BOOST_TEST(impl::net::resolve("missing-port", hints).mResult == ComResult::InvalidId);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::com::test
