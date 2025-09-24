/*******************************************************************************
 * Copyright (c) 2011, 2018 ACIN, fortiss GmbH,
 *               2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Ingo Hegny - initial API and implementation and/or initial documentation
 *   Jose Cabral - Add dummy function for throw_exception
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
 *******************************************************************************/
#define BOOST_TEST_MODULE FORTE_TESTS
#include <boost/test/included/unit_test.hpp>
#include "core/fbtests/fbtesterglobalfixture.h"

#if defined(BOOST_NO_EXCEPTIONS) &&                                                                                    \
    BOOST_VERSION < 106500 // At least Boost v1.65 provides a simple NO_EXCEPTION version of throw_exception
void boost::throw_exception(std::exception const &) {
  // dummy
}
#endif

namespace forte::test {
  BOOST_TEST_GLOBAL_FIXTURE(CFBTestDataGlobalFixture);
} // namespace forte::test
