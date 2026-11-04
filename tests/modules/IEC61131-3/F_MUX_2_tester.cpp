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
#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_any_int_variant.h"
#include "forte/iec61131_functions/func_EQ.h"

#include <array>
#include <limits>

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

  struct F_MUX_3_TestFixture : public test::CFBTestFixtureBase {
      F_MUX_3_TestFixture() : CFBTestFixtureBase("iec61131::selection::F_MUX_3"_STRID) {
        setInputData({&mK, &mIn0, &mIn1, &mIn2});
        setOutputData({&mOut});
        setup();
      }

      CIEC_ANY_INT_VARIANT mK;
      CIEC_SINT mIn0 = 10_SINT;
      CIEC_DINT mIn1 = 20_DINT;
      CIEC_STRING mIn2 = "last"_STRING;
      CIEC_ANY_VARIANT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_MUX_3_Tests, F_MUX_3_TestFixture)

  BOOST_AUTO_TEST_CASE(CheckSelectionAndTypeSwitching) {
    BOOST_CHECK(eventChainEmpty());
    mK = 0_INT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_REQUIRE_EQUAL(mOut.unwrap().getDataTypeID(), CIEC_ANY::e_SINT);
    BOOST_TEST(func_EQ(std::get<CIEC_SINT>(mOut), 10_SINT));

    mK = 1_UINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_REQUIRE_EQUAL(mOut.unwrap().getDataTypeID(), CIEC_ANY::e_DINT);
    BOOST_TEST(func_EQ(std::get<CIEC_DINT>(mOut), 20_DINT));

    mK = 2_ULINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_REQUIRE_EQUAL(mOut.unwrap().getDataTypeID(), CIEC_ANY::e_STRING);
    BOOST_TEST(func_EQ(std::get<CIEC_STRING>(mOut), "last"_STRING));
  }

  BOOST_AUTO_TEST_CASE(CheckInvalidSelectorsConfirmWithDefault) {
    mK = 2_INT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_REQUIRE_EQUAL(mOut.unwrap().getDataTypeID(), CIEC_ANY::e_STRING);

    const std::array<CIEC_ANY_INT_VARIANT, 4> selectors{3_UINT, -1_INT,
                                                        CIEC_LINT(std::numeric_limits<CIEC_LINT::TValueType>::min()),
                                                        CIEC_ULINT(std::numeric_limits<CIEC_ULINT::TValueType>::max())};
    for (const auto &selector : selectors) {
      mK = selector;
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      BOOST_REQUIRE_EQUAL(mOut.unwrap().getDataTypeID(), CIEC_ANY_VARIANT().unwrap().getDataTypeID());
      BOOST_TEST(func_EQ(std::get<CIEC_SINT>(mOut), 0_SINT));
    }
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61131::arithmetic
