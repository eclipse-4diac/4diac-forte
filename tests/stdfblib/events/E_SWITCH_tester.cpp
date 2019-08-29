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
#include <forte_bool.h>

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SWITCH_tester_gen.cpp"
#endif


struct E_SWITCH_TestFixture : public CFBTestFixtureBase{

    E_SWITCH_TestFixture() : CFBTestFixtureBase(g_nStringIdE_SWITCH){
      SETUP_INPUTDATA(&mInG);
      CFBTestFixtureBase::setup();
    }

    CIEC_BOOL mInG; //DATA INPUT
};

BOOST_FIXTURE_TEST_SUITE( SwitchTests, E_SWITCH_TestFixture)

  BOOST_AUTO_TEST_CASE(singleE0){
    mInG = false;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
  }

  BOOST_AUTO_TEST_CASE(SingleE1){
    mInG = true;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(1));
  }

  BOOST_AUTO_TEST_CASE(MultipleE0){
    mInG = false;
    for(unsigned int i = 0; i < 1000; ++i){
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    }
  }

  BOOST_AUTO_TEST_CASE(MultipleE1){
    mInG = true;
    for(unsigned int i = 0; i < 1000; ++i){
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence(1));
    }
  }

  BOOST_AUTO_TEST_CASE(Alternate){
    for(unsigned int i = 0; i < 1000; ++i){
      mInG = !mInG;
      triggerEvent(0);
      BOOST_CHECK(checkForSingleOutputEventOccurence((mInG) ? 1 : 0));
    }
  }

BOOST_AUTO_TEST_SUITE_END()
