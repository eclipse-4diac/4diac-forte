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

namespace forte::test {
  struct CFB_TEST_TestFixture : public CFBTestFixtureBase {

      CFB_TEST_TestFixture() : CFBTestFixtureBase("CFB_TEST"_STRID) {
        setInputData({&mInQI});
        setOutputData({&mOutSR});
        setup();
      }

      CIEC_BOOL mInQI;
      CIEC_BOOL mOutSR;

      bool checkBothOutputEvents() {
        bool bResult = true;
        if (0 != pullFirstChainEventID()) {
          bResult = false;
        }
        if (1 != pullFirstChainEventID()) {
          bResult = false;
        }
        if (!eventChainEmpty()) {
          bResult = false;
        }
        return bResult;
      }
  };

  BOOST_FIXTURE_TEST_SUITE(PermitTests, CFB_TEST_TestFixture)

  BOOST_AUTO_TEST_CASE(inhibitTest) {
    mInQI = false_BOOL;
    for (unsigned int i = 0; i < 100; ++i) {
      triggerEvent(0);
      BOOST_CHECK(eventChainEmpty());
      BOOST_CHECK_EQUAL(false, mOutSR);
      triggerEvent(1);
      BOOST_CHECK(eventChainEmpty());
      BOOST_CHECK_EQUAL(false, mOutSR);
    }
  }

  BOOST_AUTO_TEST_CASE(setTest) {
    mInQI = true_BOOL;
    triggerEvent(0);
    BOOST_CHECK(checkBothOutputEvents());
    BOOST_CHECK_EQUAL(true, mOutSR);
    for (unsigned int i = 0; i < 100; ++i) {
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      BOOST_CHECK_EQUAL(true, mOutSR);
    }
  }

  BOOST_AUTO_TEST_CASE(resetTest) {
    mInQI = true_BOOL;
    triggerEvent(0);
    clearEventChain();
    triggerEvent(1);
    BOOST_CHECK(checkBothOutputEvents());
    BOOST_CHECK_EQUAL(false, mOutSR);
    for (unsigned int i = 0; i < 100; ++i) {
      triggerEvent(1);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      BOOST_CHECK_EQUAL(false, mOutSR);
    }
  }

  BOOST_AUTO_TEST_CASE(toggleTest) {
    mInQI = true_BOOL;
    for (int i = 0; i < 1000; ++i) {
      triggerEvent(0);
      BOOST_CHECK(checkBothOutputEvents());
      BOOST_CHECK_EQUAL(true, mOutSR);
      triggerEvent(1);
      BOOST_CHECK(checkBothOutputEvents());
      BOOST_CHECK_EQUAL(false, mOutSR);
    }
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
