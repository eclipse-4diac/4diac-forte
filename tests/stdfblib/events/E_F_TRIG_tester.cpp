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

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_F_TRIG_tester_gen.cpp"
#endif


struct E_F_TRIG_TestFixture : public CFBTestFixtureBase{

    E_F_TRIG_TestFixture() : CFBTestFixtureBase(g_nStringIdE_F_TRIG){
      SETUP_INPUTDATA(&mInQI);
      CFBTestFixtureBase::setup();
    }

    CIEC_BOOL mInQI; //DATA INPUT
};


BOOST_FIXTURE_TEST_SUITE( FTrigTests, E_F_TRIG_TestFixture)

  BOOST_AUTO_TEST_CASE(RaisingEdge){
    mInQI = true;
    triggerEvent(0);
    BOOST_CHECK(eventChainEmpty());
  }

  BOOST_AUTO_TEST_CASE(FallingEdge){
      mInQI = true;
      triggerEvent(0);
      mInQI = false;
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    }

  BOOST_AUTO_TEST_CASE(StableHigh){
      mInQI = true;
      triggerEvent(0);
      for(unsigned int i = 0; i < 1000; i++){
        triggerEvent(0);
        BOOST_CHECK(eventChainEmpty());
      }
    }

  BOOST_AUTO_TEST_CASE(StableLow){
      mInQI = false;
      triggerEvent(0); //Just in case that the QI has been true first handle a potential falling edge
      clearEventChain();
      for(unsigned int i = 0; i < 1000; i++){
        triggerEvent(0);
        BOOST_CHECK(eventChainEmpty());
      }
    }

BOOST_AUTO_TEST_SUITE_END()

