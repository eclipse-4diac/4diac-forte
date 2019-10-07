/*******************************************************************************
 * Copyright (c) 2011 - 2014, 2018 ACIN, fortiss GmbH
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
#include <forte_bool.h>

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SR_tester_gen.cpp"
#endif

struct E_SR_TestFixture : public CFBTestFixtureBase{

    E_SR_TestFixture() : CFBTestFixtureBase(g_nStringIdE_SR){
      SETUP_OUTPUTDATA(&mOutQ);
      CFBTestFixtureBase::setup();
    }

    CIEC_BOOL mOutQ; //DATA OUTPUT

    /*\brief Check if the E_SR changed to the given target state
     */
    bool checkStateChange(bool paTargetState){
      return checkForSingleOutputEventOccurence(0) && (paTargetState == mOutQ);
    }
};

BOOST_FIXTURE_TEST_SUITE( SRTests, E_SR_TestFixture)

  BOOST_AUTO_TEST_CASE(EventS){
    //Send event
    triggerEvent(0);
    BOOST_CHECK(checkStateChange(true));

    for(unsigned int i = 0; i < 100; ++i){
      triggerEvent(0);
      BOOST_CHECK(eventChainEmpty());
      BOOST_CHECK(mOutQ);
    }
  }

  BOOST_AUTO_TEST_CASE(EventR){
    triggerEvent(0); //initially SR is reset, requires a set before reset
    BOOST_CHECK(checkStateChange(true));
    triggerEvent(1);
    //Test correct order of outgoing events
    BOOST_CHECK(checkStateChange(false));

    for(unsigned int i = 0; i < 100; ++i){
      triggerEvent(1);
      BOOST_CHECK(eventChainEmpty());
      BOOST_CHECK_EQUAL(false, mOutQ);
    }
  }

  BOOST_AUTO_TEST_CASE(Toggle){
    for(unsigned int i = 0; i < 100; ++i){
      triggerEvent(0);
      BOOST_CHECK(checkStateChange(true));
      triggerEvent(1);
      BOOST_CHECK(checkStateChange(false));
    }
  }

BOOST_AUTO_TEST_SUITE_END()
