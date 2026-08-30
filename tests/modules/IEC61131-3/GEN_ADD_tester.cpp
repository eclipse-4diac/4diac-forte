/*******************************************************************************
 * Copyright (c) 2026 HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "../../core/fbtests/fbtestfixture.h"
#include <cmath>

using namespace forte::literals;

namespace forte::iec61131::arithmetic {
  // See https://github.com/eclipse-4diac/4diac-forte/issues/1029: IEC 61131-3 specifies ADD_n
  // as the equivalent of IN1 + IN2 + ... + INn, evaluated as a left fold, i.e.
  // ((IN1 + IN2) + ...) + INn. For REAL/LREAL this means an intermediate step that overflows
  // to IEEE-754 +/-INF stays that way for the rest of the fold, even when the true
  // mathematical result of the whole expression is a perfectly valid, finite, in-range value
  // -- floating-point arithmetic is simply not associative. This is expected,
  // standard-conformant behavior, not a defect (see the issue discussion); this test locks it
  // in as a regression test rather than tracking it as something to fix.
  struct GenAdd3RealOverflowFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      GenAdd3RealOverflowFixture() : CFBTestFixtureBase("iec61131::arithmetic::ADD_3"_STRID) {
        setInputData({&mIn1, &mIn2, &mIn3});
        setOutputData({&mOut});
        setup();
      }

      CIEC_REAL mIn1;
      CIEC_REAL mIn2;
      CIEC_REAL mIn3;
      CIEC_REAL mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(GenAdd3RealOverflowTests, GenAdd3RealOverflowFixture)

  // IN1 + IN2 + IN3 == 3.0e38, a normal finite REAL value (REAL max is ~3.4e38), but the left
  // fold evaluates (IN1 + IN2) first, which overflows to +INF, and +INF + IN3 stays +INF per
  // IEEE 754. This is the correct, specified left-fold result, not a defect: if this test ever
  // starts failing, the fold's evaluation order has changed, which would itself be a
  // specification-incompatible behavior change worth flagging.
  BOOST_AUTO_TEST_CASE(leftFoldIntermediateOverflowToInfIsExpected) {
    mIn1 = 3.0e38_REAL;
    mIn2 = 3.0e38_REAL;
    mIn3 = -3.0e38_REAL;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    const CIEC_REAL::TValueType value = static_cast<CIEC_REAL::TValueType>(mOut);
    BOOST_CHECK(std::isinf(value) && value > 0);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61131::arithmetic
