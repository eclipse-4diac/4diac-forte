/*******************************************************************************
 * Copyright (c) 2026 HR Agartechnik GmbH
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
#include "forte/datatypes/forte_bool.h"

using namespace forte::literals;

namespace forte::iec61499::events::test {
  struct GEN_E_D_FF_3_TestFixture : public forte::test::CFBTestFixtureBase {
      CIEC_BOOL mD1{false_BOOL};
      CIEC_BOOL mD2{false_BOOL};
      CIEC_BOOL mD3{false_BOOL};
      CIEC_BOOL mQ1{false_BOOL};
      CIEC_BOOL mQ2{false_BOOL};
      CIEC_BOOL mQ3{false_BOOL};

      GEN_E_D_FF_3_TestFixture() : CFBTestFixtureBase("iec61499::events::E_D_FF_3"_STRID) {
        setInputData({&mD1, &mD2, &mD3});
        setOutputData({&mQ1, &mQ2, &mQ3});
        setup();
      }

      static constexpr TEventID CLK = 0;
      static constexpr TEventID EO = 0;
  };

  BOOST_FIXTURE_TEST_SUITE(GenEDFf3Tests, GEN_E_D_FF_3_TestFixture)

  BOOST_AUTO_TEST_CASE(AllEqualDoesNotFire) {
    triggerEvent(CLK); // all Di still false, same as latched Qi
    BOOST_CHECK(eventChainEmpty());
  }

  BOOST_AUTO_TEST_CASE(PartialChangeFiresAndLatchesAll) {
    mD1 = true_BOOL;
    mD2 = false_BOOL;
    mD3 = false_BOOL;
    triggerEvent(CLK);
    BOOST_CHECK(checkForSingleOutputEventOccurence(EO));
    BOOST_CHECK(static_cast<bool>(mQ1));
    BOOST_CHECK(!static_cast<bool>(mQ2));
    BOOST_CHECK(!static_cast<bool>(mQ3));

    triggerEvent(CLK); // no change
    BOOST_CHECK(eventChainEmpty());

    mD2 = true_BOOL; // only D2 changes now
    triggerEvent(CLK);
    BOOST_CHECK(checkForSingleOutputEventOccurence(EO));
    BOOST_CHECK(static_cast<bool>(mQ1));
    BOOST_CHECK(static_cast<bool>(mQ2));
    BOOST_CHECK(!static_cast<bool>(mQ3));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61499::events::test
