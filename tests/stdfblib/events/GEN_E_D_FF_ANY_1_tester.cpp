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

using namespace forte::literals;

namespace forte::iec61499::events::test {
  struct GEN_E_D_FF_ANY_1_TestFixture : public forte::test::CFBTestFixtureBase {
      CIEC_ANY_VARIANT mD1;
      CIEC_ANY_VARIANT mQ1;

      GEN_E_D_FF_ANY_1_TestFixture() : CFBTestFixtureBase("iec61499::events::E_D_FF_ANY_1"_STRID) {
        setInputData({&mD1});
        setOutputData({&mQ1});
        setup();
      }

      static constexpr TEventID CLK = 0;
      static constexpr TEventID EO = 0;
  };

  BOOST_FIXTURE_TEST_SUITE(GenEDFfAny1Tests, GEN_E_D_FF_ANY_1_TestFixture)

  BOOST_AUTO_TEST_CASE(NoChangeDoesNotFire) {
    triggerEvent(CLK); // D1 still at its reset default, same as latched Q1
    BOOST_CHECK(eventChainEmpty());
  }

  BOOST_AUTO_TEST_CASE(InitialClockLatches) {
    mD1 = true_BOOL;
    triggerEvent(CLK);
    BOOST_CHECK(checkForSingleOutputEventOccurence(EO));
    BOOST_CHECK(mQ1.equals(true_BOOL));
  }

  BOOST_AUTO_TEST_CASE(UnchangedClockDoesNotFireAgain) {
    mD1 = true_BOOL;
    triggerEvent(CLK);
    BOOST_CHECK(checkForSingleOutputEventOccurence(EO));

    triggerEvent(CLK); // D1 unchanged
    BOOST_CHECK(eventChainEmpty());
  }

  BOOST_AUTO_TEST_CASE(ChangeFiresAgain) {
    mD1 = true_BOOL;
    triggerEvent(CLK);
    BOOST_CHECK(checkForSingleOutputEventOccurence(EO));

    mD1 = false_BOOL;
    triggerEvent(CLK);
    BOOST_CHECK(checkForSingleOutputEventOccurence(EO));
    BOOST_CHECK(mQ1.equals(false_BOOL));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61499::events::test
