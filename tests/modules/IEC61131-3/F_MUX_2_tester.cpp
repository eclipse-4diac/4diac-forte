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
#include "F_MUX_2_tester_gen.cpp"
#endif

struct F_MUX_2_TestFixture : public CFBTestFixtureBase{

  F_MUX_2_TestFixture() : CFBTestFixtureBase(g_nStringIdF_MUX_2) {
    setInputData({&mK_MUX_2, &mIn0_MUX_2, &mIn1_MUX_2});
    setOutputData({&mOut_MUX_2});
    CFBTestFixtureBase::setup();
  }

  CIEC_UINT mK_MUX_2;
  CIEC_BOOL mIn0_MUX_2 = CIEC_BOOL(false);
  CIEC_BOOL mIn1_MUX_2 = CIEC_BOOL(true);

  CIEC_BOOL mOut_MUX_2;
};

BOOST_FIXTURE_TEST_SUITE( F_MUX_2_Tests, F_MUX_2_TestFixture)

  BOOST_AUTO_TEST_CASE(CheckKEqualZero) {
    mK_MUX_2 = CIEC_UINT(0U);
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(CIEC_BOOL(false), mOut_MUX_2);
  }
  BOOST_AUTO_TEST_CASE(CheckKEqualOne){
    mK_MUX_2 = CIEC_UINT(1U);
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(CIEC_BOOL(true), mOut_MUX_2);
  }

BOOST_AUTO_TEST_SUITE_END()
