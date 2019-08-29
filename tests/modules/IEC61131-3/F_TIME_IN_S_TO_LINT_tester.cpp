/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
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
#include "F_TIME_IN_S_TO_LINT_tester_gen.cpp"
#endif

struct F_TIME_IN_S_TO_LINT_TestFixture : public CFBTestFixtureBase {

    F_TIME_IN_S_TO_LINT_TestFixture() :
        CFBTestFixtureBase(g_nStringIdF_TIME_IN_S_TO_LINT) {
      SETUP_INPUTDATA(&mIn_F_TIME_IN_S_TO_LINT);
      SETUP_OUTPUTDATA(&mOut_F_TIME_IN_S_TO_LINT);
      CFBTestFixtureBase::setup();
    }

    CIEC_TIME mIn_F_TIME_IN_S_TO_LINT; //DATA INPUT
    CIEC_LINT mOut_F_TIME_IN_S_TO_LINT; //DATA OUTPUT
};

BOOST_FIXTURE_TEST_SUITE( F_TIME_IN_S_TO_LINT_Tests, F_TIME_IN_S_TO_LINT_TestFixture)

  BOOST_AUTO_TEST_CASE(timeConversion) {
    mIn_F_TIME_IN_S_TO_LINT.fromString("T#2d5h43m12s");
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(INT64_C(172800) + INT64_C(18000) + INT64_C(2580) + INT64_C(12), mOut_F_TIME_IN_S_TO_LINT);
  }

  BOOST_AUTO_TEST_SUITE_END()
