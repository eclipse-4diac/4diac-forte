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

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_PERMIT_tester_gen.cpp"
#endif

struct E_PERMIT_TestFixture : public CFBTestFixtureBase{

    E_PERMIT_TestFixture() : CFBTestFixtureBase(g_nStringIdE_PERMIT){
      SETUP_INPUTDATA(&mInPERMIT);
      CFBTestFixtureBase::setup();
    }
 
    CIEC_BOOL mInPERMIT; //DATA INPUT
};


BOOST_FIXTURE_TEST_SUITE( PermitTests, E_PERMIT_TestFixture)

  BOOST_AUTO_TEST_CASE(permit){
    mInPERMIT = true;
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
  }

  BOOST_AUTO_TEST_CASE(dontPermit){
    mInPERMIT = false;
    triggerEvent(0);
    BOOST_CHECK(eventChainEmpty());
  }

BOOST_AUTO_TEST_SUITE_END()
