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
#include "F_TIME_IN_S_TO_LREAL_tester_gen.cpp"
#endif

struct F_TIME_IN_S_TO_LREAL_TestFixture : public CFBTestFixtureBase {

    F_TIME_IN_S_TO_LREAL_TestFixture() :
        CFBTestFixtureBase(g_nStringIdF_TIME_IN_S_TO_LREAL) {
      SETUP_INPUTDATA(&mIn_F_TIME_IN_S_TO_LREAL);
      SETUP_OUTPUTDATA(&mOut_F_TIME_IN_S_TO_LREAL);
      CFBTestFixtureBase::setup();
    }

    CIEC_TIME mIn_F_TIME_IN_S_TO_LREAL; //DATA INPUT
    CIEC_LREAL mOut_F_TIME_IN_S_TO_LREAL; //DATA OUTPUT
};

BOOST_FIXTURE_TEST_SUITE( F_TIME_IN_S_TO_LREAL_Tests, F_TIME_IN_S_TO_LREAL_TestFixture)

  BOOST_AUTO_TEST_CASE(timeConversion) {
    mIn_F_TIME_IN_S_TO_LREAL.fromString("T#2d5h43m12s44ms27us31ns");
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(static_cast<CIEC_LREAL>(INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)) / static_cast<CIEC_LREAL>(1E9), mOut_F_TIME_IN_S_TO_LREAL);
  }

  BOOST_AUTO_TEST_SUITE_END()
