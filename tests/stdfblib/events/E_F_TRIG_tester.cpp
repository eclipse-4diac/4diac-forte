/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 *               2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"
#include "forte/datatypes/forte_bool.h"

using namespace forte::literals;

namespace forte::iec61499::events::test {
  struct E_F_TRIG_TestFixture : public forte::test::CFBTestFixtureBase {

      E_F_TRIG_TestFixture() : CFBTestFixtureBase("iec61499::events::E_F_TRIG"_STRID) {
        setInputData({&mInQI});
        setup();
      }

      CIEC_BOOL mInQI; // DATA INPUT
  };

  BOOST_FIXTURE_TEST_SUITE(FTrigTests, E_F_TRIG_TestFixture)

  BOOST_AUTO_TEST_CASE(RaisingEdge) {
    mInQI = true_BOOL;
    triggerEvent(0);
    BOOST_CHECK(eventChainEmpty());
  }

  BOOST_AUTO_TEST_CASE(FallingEdge) {
    mInQI = true_BOOL;
    triggerEvent(0);
    mInQI = false_BOOL;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
  }

  BOOST_AUTO_TEST_CASE(StableHigh) {
    mInQI = true_BOOL;
    triggerEvent(0);
    for (unsigned int i = 0; i < 1000; i++) {
      triggerEvent(0);
      BOOST_CHECK(eventChainEmpty());
    }
  }

  BOOST_AUTO_TEST_CASE(StableLow) {
    mInQI = false_BOOL;
    triggerEvent(0); // Just in case that the QI has been true first handle a potential falling edge
    clearEventChain();
    for (unsigned int i = 0; i < 1000; i++) {
      triggerEvent(0);
      BOOST_CHECK(eventChainEmpty());
    }
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61499::events::test
