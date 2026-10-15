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

using namespace forte::literals;

namespace forte::iec61131::arithmetic {
  struct F_MUL_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_MUL_TestFixture() : CFBTestFixtureBase("iec61131::arithmetic::F_MUL"_STRID) {
        setInputData({&mIn1, &mIn2});
        setOutputData({&mOut});
        setup();
      }

      CIEC_DINT mIn1;
      CIEC_DINT mIn2;

      CIEC_DINT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_MUL_Tests, F_MUL_TestFixture)

  BOOST_AUTO_TEST_CASE(validMultiplication) {
    mIn1 = 3_DINT;
    mIn2 = 4_DINT;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(12, static_cast<CIEC_DINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct F_MUL_MixedTypeTestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_MUL_MixedTypeTestFixture() : CFBTestFixtureBase("iec61131::arithmetic::F_MUL"_STRID) {
        setInputData({&mIn1, &mIn2});
        setOutputData({&mOut});
        setup();
      }

      CIEC_DINT mIn1;
      CIEC_REAL mIn2;

      CIEC_DINT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_MUL_IncompatibleTypeTests, F_MUL_MixedTypeTestFixture)

  BOOST_AUTO_TEST_CASE(incompatibleTypesResetOutput) {
    mIn1 = 5_DINT;
    mIn2 = 2.5_REAL;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_DINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61131::arithmetic
