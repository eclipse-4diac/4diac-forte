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
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_bool.h"

using namespace forte::literals;

namespace forte::iec61499::events::test {
  struct E_SELECT_TestFixture : public forte::test::CFBTestFixtureBase {

      E_SELECT_TestFixture() : CFBTestFixtureBase("iec61499::events::E_SELECT"_STRID) {
        setInputData({&mInG});
        setup();
      }

      CIEC_BOOL mInG; // DATA INPUT
  };

  BOOST_FIXTURE_TEST_SUITE(SelectTests, E_SELECT_TestFixture)

  BOOST_AUTO_TEST_CASE(SelectEI0) {
    mInG = false_BOOL;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    triggerEvent(1);
    BOOST_CHECK(eventChainEmpty());
  }

  BOOST_AUTO_TEST_CASE(SelectEI1) {
    mInG = true_BOOL;
    triggerEvent(1);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    triggerEvent(0);
    BOOST_CHECK(eventChainEmpty());
  }

  BOOST_AUTO_TEST_CASE(MultipleSelectEI0) {
    mInG = false_BOOL;
    for (unsigned int i = 0; i < 1000; i++) {
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    }
    for (unsigned int i = 0; i < 1000; i++) {
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      triggerEvent(1);
      BOOST_CHECK(eventChainEmpty());
    }
  }

  BOOST_AUTO_TEST_CASE(MultipleSelectEI1) {
    mInG = true_BOOL;
    for (unsigned int i = 0; i < 1000; i++) {
      triggerEvent(1);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    }
    for (unsigned int i = 0; i < 1000; i++) {
      triggerEvent(1);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      triggerEvent(0);
      BOOST_CHECK(eventChainEmpty());
    }
  }

  BOOST_AUTO_TEST_CASE(Alternate) {
    for (unsigned int i = 0; i < 1000; ++i) {
      mInG = func_NOT(mInG);
      triggerEvent((mInG) ? 1 : 0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    }
    for (unsigned int i = 0; i < 1000; i++) {
      triggerEvent((mInG) ? 1 : 0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
      triggerEvent((mInG) ? 0 : 1);
      BOOST_CHECK(eventChainEmpty());
    }
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::iec61499::events::test
