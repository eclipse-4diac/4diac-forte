/*******************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 *               2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians - initial tests
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_TRUNC_tester_gen.cpp"
#endif

struct F_TRUNC_TestFixture : public CFBTestFixtureBase {

  F_TRUNC_TestFixture() : CFBTestFixtureBase(g_nStringIdF_TRUNC) {
    setInputData({&mIn_TRUNC});
    setOutputData({&mOut_TRUNC});
    CFBTestFixtureBase::setup();
  }

  CIEC_REAL mIn_TRUNC; // DATA INPUT

  CIEC_INT mOut_TRUNC;
};

BOOST_FIXTURE_TEST_SUITE( F_TRUNC_Tests, F_TRUNC_TestFixture)

  BOOST_AUTO_TEST_CASE(validTruncationFromRealToInt){
    mIn_TRUNC = CIEC_REAL(30.45343f);
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(30, static_cast<CIEC_INT::TValueType>(mOut_TRUNC));
  }

BOOST_AUTO_TEST_SUITE_END()
