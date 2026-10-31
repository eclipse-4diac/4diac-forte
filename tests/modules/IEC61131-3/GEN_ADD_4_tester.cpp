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
  struct GEN_ADD_4_TestFixture : public test::CFBTestFixtureBase {
      static constexpr TEventID REQ = 0;
      static constexpr TEventID CNF = 0;

      GEN_ADD_4_TestFixture() : CFBTestFixtureBase("iec61131::arithmetic::ADD_4"_STRID) {
        setInputData({&mIn1, &mIn2, &mIn3, &mIn4});
        setOutputData({&mOut});
        setup();
      }

      CIEC_DINT mIn1;
      CIEC_DINT mIn2;
      CIEC_DINT mIn3;
      CIEC_DINT mIn4;

      CIEC_DINT mOut;
  };

  BOOST_FIXTURE_TEST_SUITE(GenAdd4Tests, GEN_ADD_4_TestFixture)

  BOOST_AUTO_TEST_CASE(validAddition) {
    mIn1 = 1_DINT;
    mIn2 = 2_DINT;
    mIn3 = 3_DINT;
    mIn4 = 4_DINT;
    triggerEvent(REQ);
    BOOST_CHECK(checkForSingleOutputEventOccurence(CNF));
    BOOST_CHECK_EQUAL(10, static_cast<CIEC_DINT::TValueType>(mOut));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61131::arithmetic
