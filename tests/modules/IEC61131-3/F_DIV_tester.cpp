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
#include "F_DIV_tester_gen.cpp"
#endif

struct F_DIV_TestFixture : public CFBTestFixtureBase{

  F_DIV_TestFixture() : CFBTestFixtureBase(g_nStringIdF_DIV) {
    setInputData({&mIn1_DIV, &mIn2_DIV});
    setOutputData({&mOut_DIV});
    CFBTestFixtureBase::setup();
  }

    CIEC_INT mIn1_DIV; //DATA INPUT
    CIEC_INT mIn2_DIV; //DATA INPUT

    CIEC_INT mOut_DIV;
};

BOOST_FIXTURE_TEST_SUITE( F_DIV_Tests, F_DIV_TestFixture)

  BOOST_AUTO_TEST_CASE(validDivision) {
    mIn1_DIV = CIEC_INT(30);
    mIn2_DIV = CIEC_INT(5);
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(6, static_cast<CIEC_INT::TValueType>(mOut_DIV));
  }

  BOOST_AUTO_TEST_CASE(divisionByZero) {
    mIn1_DIV = CIEC_INT(30);
    mIn2_DIV = CIEC_INT(0);
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
  }

BOOST_AUTO_TEST_SUITE_END()
