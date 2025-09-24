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
#include "forte/datatypes/forte_ulint.h"

using namespace forte::literals;

namespace forte::iec61131::arithmetic {
  struct F_TIME_IN_S_TO_ULINT_TestFixture : public test::CFBTestFixtureBase {

      F_TIME_IN_S_TO_ULINT_TestFixture() : CFBTestFixtureBase("iec61131::conversion::F_TIME_IN_S_TO_ULINT"_STRID) {
        setInputData({&mIn_F_TIME_IN_S_TO_ULINT});
        setOutputData({&mOut_F_TIME_IN_S_TO_ULINT});
        setup();
      }

      CIEC_TIME mIn_F_TIME_IN_S_TO_ULINT; // DATA INPUT
      CIEC_ULINT mOut_F_TIME_IN_S_TO_ULINT; // DATA OUTPUT
  };

  BOOST_FIXTURE_TEST_SUITE(F_TIME_IN_S_TO_ULINT_Tests, F_TIME_IN_S_TO_ULINT_TestFixture)

  BOOST_AUTO_TEST_CASE(timeConversion) {
    mIn_F_TIME_IN_S_TO_ULINT.fromString("T#2d5h43m12s");
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(UINT64_C(172800) + UINT64_C(18000) + UINT64_C(2580) + UINT64_C(12),
                      static_cast<CIEC_ULINT::TValueType>(mOut_F_TIME_IN_S_TO_ULINT));
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::iec61131::arithmetic
