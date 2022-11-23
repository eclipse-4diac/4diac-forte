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
#include "F_DIVTIME_tester_gen.cpp"
#endif

struct F_DIVTIME_TestFixture : public CFBTestFixtureBase{

  F_DIVTIME_TestFixture() : CFBTestFixtureBase(g_nStringIdF_DIVTIME) {
    setInputData({&mIn_TIME_DIVTIME, &mIn_REAL_DIVTIME});
    setOutputData({&mOut_TIME_DIVTIME});
    CFBTestFixtureBase::setup();
  }

  CIEC_TIME mIn_TIME_DIVTIME; // DATA INPUT
  CIEC_REAL mIn_REAL_DIVTIME; // DATA INPUT

  CIEC_TIME mOut_TIME_DIVTIME;
};

BOOST_FIXTURE_TEST_SUITE(F_DIVTIME_Tests, F_DIVTIME_TestFixture)

BOOST_AUTO_TEST_CASE(divWithRealWithOne) {
  mIn_TIME_DIVTIME = CIEC_TIME(30000000);
  mIn_REAL_DIVTIME = CIEC_REAL(1.0);
  /* trigger the inputevent */
  triggerEvent(0);
  BOOST_CHECK(checkForSingleOutputEventOccurence(0));
  BOOST_CHECK_EQUAL(CIEC_TIME(30000000), mOut_TIME_DIVTIME);
}

BOOST_AUTO_TEST_CASE(divWithRealWithDecimal) {
  mIn_TIME_DIVTIME = CIEC_TIME(30000000);
  mIn_REAL_DIVTIME = CIEC_REAL(0.5);
  /* trigger the inputevent */
  triggerEvent(0);
  BOOST_CHECK(checkForSingleOutputEventOccurence(0));
  BOOST_CHECK_EQUAL(CIEC_TIME(60000000), mOut_TIME_DIVTIME);
}

BOOST_AUTO_TEST_CASE(divWithRealWitGreaterThanOne) {
  mIn_TIME_DIVTIME = CIEC_TIME(30000000);
  mIn_REAL_DIVTIME = CIEC_REAL(2.0);
  /* trigger the inputevent */
  triggerEvent(0);
  BOOST_CHECK(checkForSingleOutputEventOccurence(0));
  BOOST_CHECK_EQUAL(CIEC_TIME(15000000), mOut_TIME_DIVTIME);
}

BOOST_AUTO_TEST_SUITE_END()
