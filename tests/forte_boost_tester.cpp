/*******************************************************************************
 * Copyright (c) 2011, 2018 ACIN, fortiss GmbH,
 *               2018 Johannes Kepler University
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Ingo Hegny - initial API and implementation and/or initial documentation
 *   Jose Cabral - Add dummy function for throw_exception
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
 *******************************************************************************/
#define BOOST_TEST_MODULE FORTE_TESTS
#include <boost/test/included/unit_test.hpp>
#include "core/fbtests/fbtesterglobalfixture.h"


void boost::throw_exception(std::exception const&) {
  //dummy
}

//BOOST_TEST_GLOBAL_FIXTURE(MyGlobalFixture)();
//fix for a problem in BOOST_TEST_GLOBAL_FIXTURE, revert when fixed, issue seams to be in BOOST_JOIN
static boost::unit_test::ut_detail::global_fixture_impl<CFBTestDataGlobalFixture> gfCFBTestDataGlobalFixture;
