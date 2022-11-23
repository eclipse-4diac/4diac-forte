/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *               
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians - initial tests
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_MULTIME_tester_gen.cpp"
#endif

struct F_MULTIME_TestFixture : public CFBTestFixtureBase{

  F_MULTIME_TestFixture() : CFBTestFixtureBase(g_nStringIdF_MULTIME)
  {
    setInputData({&mIn_TIME_MULTIME, &mIn_REAL_MULTIME});
    setOutputData({&mOut_TIME_MULTIME});
    CFBTestFixtureBase::setup();
  }

  CIEC_TIME mIn_TIME_MULTIME; // DATA INPUT
  CIEC_REAL mIn_REAL_MULTIME; // DATA INPUT

  CIEC_TIME mOut_TIME_MULTIME;
};

BOOST_FIXTURE_TEST_SUITE(F_MULTIME_Tests, F_MULTIME_TestFixture)

BOOST_AUTO_TEST_CASE(mulWithRealWithOne) {
  mIn_TIME_MULTIME = CIEC_TIME(30000000);
  mIn_REAL_MULTIME = CIEC_REAL(1.0);
  /* trigger the inputevent */
  triggerEvent(0);
  BOOST_CHECK(checkForSingleOutputEventOccurence(0));
  BOOST_CHECK_EQUAL(CIEC_TIME(30000000), mOut_TIME_MULTIME);
}

  BOOST_AUTO_TEST_CASE(mulWithRealWithDecimal) {
    mIn_TIME_MULTIME = CIEC_TIME(30000000);
    mIn_REAL_MULTIME = CIEC_REAL(0.5);
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(CIEC_TIME(15000000), mOut_TIME_MULTIME);
  }

  BOOST_AUTO_TEST_CASE(mulWithRealWitGreaterThanOne) {
    mIn_TIME_MULTIME = CIEC_TIME(30000000);
    mIn_REAL_MULTIME = CIEC_REAL(2.0);
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(CIEC_TIME(60000000), mOut_TIME_MULTIME);
  }

BOOST_AUTO_TEST_SUITE_END()
