/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH, 2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *   Alois Zoitl - migrated fb tests to boost test infrastructure
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"
#include <forte_uint.h>
#include <forte_bool.h>


#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CTD_tester_gen.cpp"
#endif

struct E_CTD_TestFixture : public CFBTestFixtureBase{

    E_CTD_TestFixture() : CFBTestFixtureBase(g_nStringIdE_CTD){
      SETUP_INPUTDATA(&mInPV);
      SETUP_OUTPUTDATA(&mOutQ, &mOutCV);
      CFBTestFixtureBase::setup();
    }

    CIEC_UINT mInPV; //Preset value
    CIEC_BOOL mOutQ; //CV < 0
    CIEC_UINT mOutCV; //count value

    bool checkCD(TForteUInt16 pa_prevCV){
      if(pa_prevCV < 1){
        //no algorithm should have been executed
        if(mOutCV != pa_prevCV || !mOutQ || !eventChainEmpty()){
          return false;
        }
      }
      else{
        if(((pa_prevCV - 1) != mOutCV)){
          return false;
        } else if(mOutQ != (mOutCV < 1)){
          return false;
        } else if(!checkForSingleOutputEventOccurence(0)){
          return false;
        }
      }
      return true;
    }

    bool checkLD(TForteUInt16 pa_usedPV){
      if(pa_usedPV != mInPV || mInPV != mOutCV || ((pa_usedPV < 1) != (true == mOutQ))){
        return false;
      }
      if(!checkForSingleOutputEventOccurence(1)){
        return false;
      }
      return true;
    }
};

BOOST_FIXTURE_TEST_SUITE( CTDTests, E_CTD_TestFixture)

  BOOST_AUTO_TEST_CASE(EventCD){
    unsigned int numberOfTries = 100;
    TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535 };
    unsigned int numberOfValues = static_cast<unsigned int>(sizeof(valuesToTest) / sizeof(TForteUInt16));
    for(unsigned int i = 0; i < numberOfValues; i++){
      for(unsigned int j = 0; j < numberOfTries; j++){
        mInPV = valuesToTest[i];
        triggerEvent(1);
        checkForSingleOutputEventOccurence(1);
        //Send event
        triggerEvent(0);
        BOOST_CHECK(checkCD(valuesToTest[i]));
      }
    }
  }

  BOOST_AUTO_TEST_CASE(EventLD){
    unsigned int numberOfTries = 100;
    TForteUInt16 PVToTest[] = { 10, 1, 0, 65534, 65535 };
    unsigned int numberOftest = static_cast<unsigned int>(sizeof(PVToTest) / sizeof(TForteUInt16));
    for(unsigned int i = 0; i < numberOftest; ++i){
      for(unsigned int j = 0; j < numberOfTries; j++){
        mInPV = PVToTest[i];
        triggerEvent(1);
        BOOST_CHECK(checkLD(PVToTest[i]));
      }
    }
  }

  BOOST_AUTO_TEST_CASE(Mix){
    unsigned int numberOfTries = 100;
    for(unsigned int i = 0; i < numberOfTries; i++){
      mInPV = 0;
      triggerEvent(1);
      BOOST_CHECK(checkLD(0));
      triggerEvent(0);
      BOOST_CHECK(checkCD(0));

      mInPV = 1;
      triggerEvent(0);
      BOOST_CHECK(checkCD(0));
      triggerEvent(1);
      BOOST_CHECK(checkLD(1));
      triggerEvent(0);
      BOOST_CHECK(checkCD(1));
      triggerEvent(0);
      BOOST_CHECK(checkCD(0));
      triggerEvent(1);
      BOOST_CHECK(checkLD(1));
      triggerEvent(1);
      BOOST_CHECK(checkLD(1));
      triggerEvent(1);
      BOOST_CHECK(checkLD(1));
      triggerEvent(0);
      BOOST_CHECK(checkCD(1));
      triggerEvent(0);
      BOOST_CHECK(checkCD(0));

      mInPV = 65535;
      triggerEvent(1);
      BOOST_CHECK(checkLD(65535));
      triggerEvent(0);
      BOOST_CHECK(checkCD(65535));
      triggerEvent(0);
      BOOST_CHECK(checkCD(65534));
      triggerEvent(1);
      BOOST_CHECK(checkLD(65535));
      triggerEvent(1);
      BOOST_CHECK(checkLD(65535));
      triggerEvent(1);
      BOOST_CHECK(checkLD(65535));
      triggerEvent(0);
      BOOST_CHECK(checkCD(65535));
      triggerEvent(0);
      BOOST_CHECK(checkCD(65534));
    }
  }

BOOST_AUTO_TEST_SUITE_END()
