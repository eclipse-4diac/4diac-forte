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

USE_STRING_ID(E_R_TRIG);

struct E_R_TRIG_TestFixture : public CFBTestFixtureBase {

    E_R_TRIG_TestFixture() : CFBTestFixtureBase(STRID(E_R_TRIG)) {
      setInputData({&mInQI});
      CFBTestFixtureBase::setup();
    }

    CIEC_BOOL mInQI; // DATA INPUT
};

BOOST_FIXTURE_TEST_SUITE(RTrigTests, E_R_TRIG_TestFixture)

BOOST_AUTO_TEST_CASE(RaisingEdge) {
  mInQI = true_BOOL;
  triggerEvent(0);
  BOOST_CHECK(checkForSingleOutputEventOccurence(0));
}

BOOST_AUTO_TEST_CASE(FallingEdge) {
  mInQI = true_BOOL;
  triggerEvent(0);
  clearEventChain();
  mInQI = false_BOOL;
  triggerEvent(0);
  BOOST_CHECK(eventChainEmpty());
}

BOOST_AUTO_TEST_CASE(StableHigh) {
  mInQI = true_BOOL;
  triggerEvent(0);
  pullFirstChainEventID();
  for (unsigned int i = 0; i < 1000; i++) {
    triggerEvent(0);
    BOOST_CHECK(eventChainEmpty());
  }
}

BOOST_AUTO_TEST_CASE(StableLow) {
  mInQI = false_BOOL;
  for (unsigned int i = 0; i < 1000; i++) {
    triggerEvent(0);
    BOOST_CHECK(eventChainEmpty());
  }
}

BOOST_AUTO_TEST_SUITE_END()
