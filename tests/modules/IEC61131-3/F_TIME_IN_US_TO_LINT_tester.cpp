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
#include "forte/datatypes/forte_lint.h"

using namespace forte::literals;

namespace forte::iec61131::arithmetic {
  struct F_TIME_IN_US_TO_LINT_TestFixture : public test::CFBTestFixtureBase {

      F_TIME_IN_US_TO_LINT_TestFixture() : CFBTestFixtureBase("iec61131::conversion::F_TIME_IN_US_TO_LINT"_STRID) {
        setInputData({&mIn_F_TIME_IN_US_TO_LINT});
        setOutputData({&mOut_F_TIME_IN_US_TO_LINT});
        setup();
      }

      CIEC_TIME mIn_F_TIME_IN_US_TO_LINT; // DATA INPUT
      CIEC_LINT mOut_F_TIME_IN_US_TO_LINT; // DATA OUTPUT
  };

  BOOST_FIXTURE_TEST_SUITE(F_TIME_IN_US_TO_LINT_Tests, F_TIME_IN_US_TO_LINT_TestFixture)

  BOOST_AUTO_TEST_CASE(timeConversion) {
    mIn_F_TIME_IN_US_TO_LINT.fromString("T#2d5h43m12s44ms27us");
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(INT64_C(172800000000) + INT64_C(18000000000) + INT64_C(2580000000) + INT64_C(12000000) +
                          INT64_C(44000) + INT64_C(27),
                      static_cast<CIEC_LINT::TValueType>(mOut_F_TIME_IN_US_TO_LINT));
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::iec61131::arithmetic
