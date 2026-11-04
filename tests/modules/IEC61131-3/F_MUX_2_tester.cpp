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
#include "../../core/fbtests/fbtesterglobalfixture.h"
#include "../../../core/src/typelib_internal.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_any_int_variant.h"
#include "forte/datatypes/forte_any_struct.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

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
    mK_MUX_2 = 0_UINT;
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut_MUX_2.equals(false_BOOL));
  }
  BOOST_AUTO_TEST_CASE(CheckKEqualOne) {
    mK_MUX_2 = 1_UINT;
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut_MUX_2.equals(true_BOOL));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct F_MUX_3_TestFixture : public test::CFBTestFixtureBase {
      F_MUX_3_TestFixture() : CFBTestFixtureBase("iec61131::selection::F_MUX_3"_STRID) {
        setInputData({&mK, &mIn0, &mIn1, &mIn2});
        setOutputData({&mOut});
        setup();
      }

      CIEC_ANY_INT_VARIANT mK;
      CIEC_ANY_VARIANT mIn0 = 10_SINT;
      CIEC_ANY_VARIANT mIn1 = 20_UINT;
      CIEC_ANY_VARIANT mIn2 = 30_DINT;
      CIEC_ANY_VARIANT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_MUX_3_Tests, F_MUX_3_TestFixture)

  BOOST_AUTO_TEST_CASE(CheckSelectionAndPromotion) {
    BOOST_CHECK(eventChainEmpty());
    mK = 0_INT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(10_DINT));

    mK = 1_UINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(20_DINT));

    mK = 2_ULINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(30_DINT));
    mIn0 = 30_DINT;
    mIn2 = 10_SINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(10_DINT));
  }

  BOOST_AUTO_TEST_CASE(CheckAllSelectorTypes) {
    const std::array<CIEC_ANY_INT_VARIANT, 8> selectors{1_SINT,  1_INT,  1_DINT,  1_LINT,
                                                        1_USINT, 1_UINT, 1_UDINT, 1_ULINT};
    for (const auto &selector : selectors) {
      mK = selector;
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      BOOST_TEST(mOut.equals(20_DINT));
    }
  }

  BOOST_AUTO_TEST_CASE(CheckCommonTypeNotPresentInInputs) {
    mIn2 = 30_SINT;
    mK = 0_INT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(10_DINT));
    mK = 1_INT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(20_DINT));
    mIn0 = 1_DINT;
    mIn1 = 2.0_REAL;
    mIn2 = 3_SINT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(2.0_LREAL));
  }

  BOOST_AUTO_TEST_CASE(CheckIncompatibleUnselectedInput) {
    mK = 0_INT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(10_DINT));
    mIn2 = "incompatible"_STRING;
    for (const auto &selector : std::array<CIEC_ANY_INT_VARIANT, 3>{0_INT, 2_INT, -1_INT}) {
      mK = selector;
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      BOOST_TEST(mOut.equals(0_SINT));
    }
  }

  BOOST_AUTO_TEST_CASE(CheckInvalidSelectorsConfirmWithDefault) {
    mK = 2_INT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(30_DINT));

    const std::array<CIEC_ANY_INT_VARIANT, 4> selectors{3_UINT, -1_INT,
                                                        CIEC_LINT(std::numeric_limits<CIEC_LINT::TValueType>::min()),
                                                        CIEC_ULINT(std::numeric_limits<CIEC_ULINT::TValueType>::max())};
    for (const auto &selector : selectors) {
      mK = selector;
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      BOOST_TEST(mOut.equals(0_DINT));
    }
  }

  BOOST_AUTO_TEST_CASE(CheckArrayIdentityAndCloning) {
    const CIEC_ARRAY_FIXED<CIEC_INT, 0, 1> first{10_INT, 20_INT};
    const CIEC_ARRAY_FIXED<CIEC_INT, 0, 1> second{30_INT, 40_INT};
    mIn0 = first;
    mIn1 = second;
    mIn2 = first;
    mK = 1_INT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(second));
    mIn1.unwrap().reset();
    BOOST_TEST(mOut.equals(second));
    mK = -1_INT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(CIEC_ARRAY_FIXED<CIEC_INT, 0, 1>{}));
    mIn2 = CIEC_ARRAY_FIXED<CIEC_INT, 1, 2>{10_INT, 20_INT};
    mK = 0_INT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(0_SINT));
    mIn2 = CIEC_ARRAY_FIXED<CIEC_SINT, 0, 1>{10_SINT, 20_SINT};
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(0_SINT));
    mIn2 = CIEC_ARRAY_VARIABLE<CIEC_INT>{};
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(0_SINT));
    mIn0 = CIEC_ARRAY_VARIABLE<CIEC_INT>{};
    mIn1 = CIEC_ARRAY_VARIABLE<CIEC_INT>{};
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(0_SINT));
  }

  BOOST_AUTO_TEST_SUITE_END()

  class CIEC_MuxTestStruct : public CIEC_ANY_STRUCT {
    public:
      explicit CIEC_MuxTestStruct(StringId paTypeName) : mTypeName(paTypeName) {
      }

      CIEC_ANY *clone(TForteByte *paDataBuf) const override {
        return paDataBuf ? new (paDataBuf) CIEC_MuxTestStruct(*this) : new CIEC_MuxTestStruct(*this);
      }

      StringId getStructTypeNameID() const override {
        return mTypeName;
      }

    private:
      StringId mTypeName;
  };

  BOOST_FIXTURE_TEST_SUITE(F_MUX_Derived_Tests, F_MUX_3_TestFixture)

  BOOST_AUTO_TEST_CASE(CheckStructIdentity) {
    const CIEC_MuxTestStruct first("MuxStructA"_STRID);
    const CIEC_MuxTestStruct second("MuxStructB"_STRID);
    mIn0 = first;
    mIn1 = first;
    mIn2 = first;
    mK = 0_INT;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(first));
    mIn2 = second;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_TEST(mOut.equals(0_SINT));
  }

  BOOST_AUTO_TEST_SUITE_END()

  BOOST_AUTO_TEST_CASE(F_MUX_ZeroBasedInputNames) {
    const std::unique_ptr<CFunctionBlock, decltype(&deleteFB)> block(
        createFB("MuxNames"_STRID, "iec61131::selection::F_MUX_3"_STRID, test::CFBTestDataGlobalFixture::getResource()),
        &deleteFB);
    BOOST_REQUIRE(block);
    BOOST_TEST(block->getDataInput("K"_STRID) == block->getDI(0));
    BOOST_TEST(block->getDataInput("IN0"_STRID) == block->getDI(1));
    BOOST_TEST(block->getDataInput("IN1"_STRID) == block->getDI(2));
    BOOST_TEST(block->getDataInput("IN2"_STRID) == block->getDI(3));
    BOOST_TEST(block->getDataInput("IN3"_STRID) == nullptr);
  }
} // namespace forte::iec61131::arithmetic
