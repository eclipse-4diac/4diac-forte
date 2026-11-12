/*******************************************************************************
 * Copyright (c) 2026 HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "../../core/fbtests/fbtestfixture.h"
#include <cmath>

using namespace forte::literals;

namespace forte::iec61131::arithmetic {
  struct GEN_MUL_3_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      GEN_MUL_3_TestFixture() : CFBTestFixtureBase("iec61131::arithmetic::MUL_3"_STRID) {
        setInputData({&mIn1, &mIn2, &mIn3});
        setOutputData({&mOut});
        setup();
      }

      CIEC_DINT mIn1;
      CIEC_DINT mIn2;
      CIEC_DINT mIn3;

      CIEC_DINT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(GenMul3Tests, GEN_MUL_3_TestFixture)

  BOOST_AUTO_TEST_CASE(validMultiplication) {
    mIn1 = 2_DINT;
    mIn2 = 3_DINT;
    mIn3 = 4_DINT;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(24, static_cast<CIEC_DINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct GEN_MUL_3_RealTestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      GEN_MUL_3_RealTestFixture() : CFBTestFixtureBase("iec61131::arithmetic::MUL_3"_STRID) {
        setInputData({&mIn1, &mIn2, &mIn3});
        setOutputData({&mOut});
        setup();
      }

      CIEC_REAL mIn1;
      CIEC_REAL mIn2;
      CIEC_REAL mIn3;

      CIEC_REAL mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(GenMul3RealTests, GEN_MUL_3_RealTestFixture)

  // IEC 61131-3 specifies MUL_n as the equivalent of IN1 * IN2 * ... * INn, evaluated as a left
  // fold: ((IN1 * IN2) * ...) * INn. For REAL/LREAL, an intermediate step overflowing to
  // IEEE-754 +/-INF stays that way for the rest of the fold, even when the true mathematical
  // result of the whole expression is a perfectly valid, finite, in-range value --
  // floating-point arithmetic is simply not associative. This is expected,
  // standard-conformant behavior, not a defect (see the analogous discussion on GEN_ADD in
  // issue #1029); this test locks it in as a regression test.
  //
  // IN1 * IN2 * IN3 == 1e30, a normal finite REAL value, but the left fold evaluates
  // (IN1 * IN2) first, which overflows to +INF, and +INF * IN3 stays +INF per IEEE 754.
  BOOST_AUTO_TEST_CASE(leftFoldIntermediateOverflowToInfIsExpected) {
    mIn1 = 1.0e30_REAL;
    mIn2 = 1.0e30_REAL;
    mIn3 = 1.0e-30_REAL;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    const CIEC_REAL::TValueType value = static_cast<CIEC_REAL::TValueType>(mOut);
    BOOST_CHECK(std::isinf(value) && value > 0);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61131::arithmetic
