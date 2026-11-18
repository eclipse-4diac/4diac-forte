/*******************************************************************************
 * Copyright (c) 2026 Martin Melik Merkumians
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * https://www.eclipse.org/legal/epl-2.0.
 *
 * AI Disclosure:
 * This file was largely AI-generated. AI-generated portions are made
 * available under CC0-1.0 and are not subject to the project's license.
 * Human contributions are made available under EPL-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0 AND CC0-1.0
 * Assisted-by: GitHub Copilot
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial generic MUX common-type and selector regression tests
 *******************************************************************************/

#include "../../core/fbtests/fbtestfixture.h"
#include "forte/datatypes/forte_ulint.h"

using namespace forte::literals;

namespace forte::iec61131::selection {
  struct GenMux3CommonTypeFixture : public test::CFBTestFixtureBase {
      GenMux3CommonTypeFixture() : CFBTestFixtureBase("iec61131::selection::F_MUX_3"_STRID) {
        setInputData({&mK, &mIN0, &mIN1, &mIN2});
        setOutputData({&mOUT});
        setup();
      }

      CIEC_UINT mK;
      CIEC_SINT mIN0 = 1_SINT;
      CIEC_INT mIN1 = 2_INT;
      CIEC_DINT mIN2 = 3_DINT;
      CIEC_DINT mOUT;
  };

  BOOST_FIXTURE_TEST_SUITE(GenMux3CommonTypeTests, GenMux3CommonTypeFixture)

  BOOST_AUTO_TEST_CASE(firstAndLastIntegerInputsSelectAsDintAndEmitCnf) {
    mK = 0_UINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOUT.equals(1_DINT));

    mK = 2_UINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOUT.equals(3_DINT));
  }

  BOOST_AUTO_TEST_CASE(selectorEqualToInputCountProducesDefaultDintAndEmitsCnf) {
    mK = 3_UINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOUT.equals(0_DINT));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct GenMux3LaterCommonTypeFixture : public test::CFBTestFixtureBase {
      GenMux3LaterCommonTypeFixture() : CFBTestFixtureBase("iec61131::selection::F_MUX_3"_STRID) {
        setInputData({&mSelector, &mSignedInput, &mUnsignedInput, &mCommonInput});
        setOutputData({&mOutput});
        setup();
      }

      CIEC_ULINT mSelector;
      CIEC_SINT mSignedInput = -1_SINT;
      CIEC_USINT mUnsignedInput = 255_USINT;
      CIEC_INT mCommonInput = 1000_INT;
      CIEC_INT mOutput;
  };

  BOOST_FIXTURE_TEST_SUITE(GenMux3LaterCommonTypeTests, GenMux3LaterCommonTypeFixture)

  BOOST_AUTO_TEST_CASE(selectsSintInputAsIntUsingLaterIntInputAndEmitsCnf) {
    mSelector = 0_ULINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOutput.equals(-1_INT));
  }

  BOOST_AUTO_TEST_CASE(selectsUsintInputAsIntUsingLaterIntInputAndEmitsCnf) {
    mSelector = 1_ULINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOutput.equals(255_INT));
  }

  BOOST_AUTO_TEST_CASE(selectsLaterIntInputAndEmitsCnf) {
    mSelector = 2_ULINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOutput.equals(1000_INT));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61131::selection
