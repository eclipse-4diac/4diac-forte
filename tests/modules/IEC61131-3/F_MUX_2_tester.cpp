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
#include "forte/datatypes/forte_bool.h"

using namespace forte::literals;

namespace forte::iec61131::arithmetic {
  struct F_MUX_2_TestFixture : public test::CFBTestFixtureBase {

      F_MUX_2_TestFixture() : CFBTestFixtureBase("iec61131::selection::F_MUX_2"_STRID) {
        setInputData({&mK_MUX_2, &mIn0_MUX_2, &mIn1_MUX_2});
        setOutputData({&mOut_MUX_2});
        setup();
      }

      CIEC_UINT mK_MUX_2;
      CIEC_BOOL mIn0_MUX_2 = false_BOOL;
      CIEC_BOOL mIn1_MUX_2 = true_BOOL;

      CIEC_BOOL mOut_MUX_2;
  };

  BOOST_FIXTURE_TEST_SUITE(F_MUX_2_Tests, F_MUX_2_TestFixture)

  BOOST_AUTO_TEST_CASE(CheckKEqualZero) {
    mK_MUX_2 = CIEC_UINT(0U);
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(false_BOOL, mOut_MUX_2);
  }
  BOOST_AUTO_TEST_CASE(CheckKEqualOne) {
    mK_MUX_2 = CIEC_UINT(1U);
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(true_BOOL, mOut_MUX_2);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61131::arithmetic
