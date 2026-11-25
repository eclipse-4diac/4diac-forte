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
#include "forte/datatypes/forte_any_magnitude_variant.h"

using namespace forte::literals;

namespace forte::iec61131::arithmetic {
  struct F_ADD_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_ADD_TestFixture() : CFBTestFixtureBase("iec61131::arithmetic::F_ADD"_STRID) {
        setInputData({&mIn1, &mIn2});
        setOutputData({&mOut});
        setup();
      }

      CIEC_DINT mIn1;
      CIEC_DINT mIn2;

      CIEC_DINT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_ADD_Tests, F_ADD_TestFixture)

  BOOST_AUTO_TEST_CASE(validAddition) {
    mIn1 = 3_DINT;
    mIn2 = 4_DINT;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(7, static_cast<CIEC_DINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()

  struct F_ADD_MixedTypeTestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      F_ADD_MixedTypeTestFixture() : CFBTestFixtureBase("iec61131::arithmetic::F_ADD"_STRID) {
        setInputData({&mIn1, &mIn2});
        setOutputData({&mOut});
        setup();
      }

      CIEC_DINT mIn1;
      CIEC_ANY_MAGNITUDE_VARIANT mIn2;

      CIEC_DINT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(F_ADD_IncompatibleTypeTests, F_ADD_MixedTypeTestFixture)

  BOOST_AUTO_TEST_CASE(incompatibleTypesResetOutput) {
    // first a valid request, so that OUT is not zero any more
    mIn1 = 5_DINT;
    mIn2 = 4_DINT;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(9, static_cast<CIEC_DINT::TValueType>(mOut));

    // DINT and REAL have no common type: OUT has to be reset, not left at the last result
    mIn2 = 2.5_REAL;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(0, static_cast<CIEC_DINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61131::arithmetic
