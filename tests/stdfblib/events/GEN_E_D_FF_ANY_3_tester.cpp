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
#include "forte/datatypes/forte_any_variant.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"

using namespace forte::literals;

namespace forte::iec61499::events::test {
  struct GEN_E_D_FF_ANY_3_TestFixture : public forte::test::CFBTestFixtureBase {
      CIEC_ANY_VARIANT mD1;
      CIEC_ANY_VARIANT mD2;
      CIEC_ANY_VARIANT mD3;
      CIEC_ANY_VARIANT mQ1;
      CIEC_ANY_VARIANT mQ2;
      CIEC_ANY_VARIANT mQ3;

      GEN_E_D_FF_ANY_3_TestFixture() : CFBTestFixtureBase("iec61499::events::E_D_FF_ANY_3"_STRID) {
        setInputData({&mD1, &mD2, &mD3});
        setOutputData({&mQ1, &mQ2, &mQ3});
        setup();
      }

      static constexpr TEventID CLK = 0;
      static constexpr TEventID EO = 0;
  };

  BOOST_FIXTURE_TEST_SUITE(GenEDFfAny3Tests, GEN_E_D_FF_ANY_3_TestFixture)

  BOOST_AUTO_TEST_CASE(NoChangeDoesNotFire) {
    triggerEvent(CLK); // all Di still at their reset default, same as latched Qi
    BOOST_CHECK(eventChainEmpty());
  }

  BOOST_AUTO_TEST_CASE(HeterogeneousChannelsLatchIndependently) {
    // channels may carry different ANY-compatible types simultaneously
    mD1 = true_BOOL;
    mD2 = 0_UINT;
    mD3 = false_BOOL;
    triggerEvent(CLK);
    BOOST_CHECK(checkForSingleOutputEventOccurence(EO));
    BOOST_CHECK(mQ1.equals(true_BOOL));
    BOOST_CHECK(mQ2.equals(0_UINT));
    BOOST_CHECK(mQ3.equals(false_BOOL));

    triggerEvent(CLK); // no change
    BOOST_CHECK(eventChainEmpty());

    mD2 = 42_UINT; // only D2 changes now
    triggerEvent(CLK);
    BOOST_CHECK(checkForSingleOutputEventOccurence(EO));
    BOOST_CHECK(mQ1.equals(true_BOOL));
    BOOST_CHECK(mQ2.equals(42_UINT));
    BOOST_CHECK(mQ3.equals(false_BOOL));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61499::events::test
