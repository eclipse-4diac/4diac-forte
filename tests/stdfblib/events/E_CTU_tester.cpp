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
#include "E_CTU_tester_gen.cpp"
#endif

struct E_CTU_TestFixture : public CFBTestFixtureBase{

    E_CTU_TestFixture() : CFBTestFixtureBase(g_nStringIdE_CTU){
      SETUP_INPUTDATA(&mInPV);
      SETUP_OUTPUTDATA(&mOutQ, &mOutCV);
      CFBTestFixtureBase::setup();
    }

    CIEC_UINT mInPV; //Preset value
    CIEC_BOOL mOutQ; //CV >= PV
    CIEC_UINT mOutCV; //count value

    bool checkCU(TForteUInt16 paPrevCV){
       if(paPrevCV < CIEC_UINT::scm_nMaxVal){
         if(((paPrevCV + 1) != mOutCV)){
           return false;
         }
         else if(!checkForSingleOutputEventOccurence(0)){
           return false;
         }
       }else{
         if(CIEC_UINT::scm_nMaxVal != mOutCV && !eventChainEmpty()){
           return false;
         }
       }
       if(mOutQ != (mOutCV >= mInPV)){
         return false;
       }
       return true;
     }

     bool checkR(){
       if(0 != mOutCV || mOutQ){
         return false;
       }
       if(!checkForSingleOutputEventOccurence(1)){
         return false;
       }
       return true;
     }
};

BOOST_FIXTURE_TEST_SUITE( CTUTests, E_CTU_TestFixture)

  BOOST_AUTO_TEST_CASE(EventCU){
    TForteUInt16 prevCV = 0;
    TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535 };
    size_t numberOfValues = static_cast<size_t>(sizeof(valuesToTest) / sizeof(TForteUInt16));
    for(size_t j = 0; j < numberOfValues; j++){
      triggerEvent(1);
      BOOST_CHECK(checkR());
      mInPV = valuesToTest[j];
      for(unsigned int k = 0; k < static_cast<unsigned int>(mInPV + 3); k++){
        prevCV = mOutCV;
        //Send event
        triggerEvent(0);
        BOOST_CHECK(checkCU(prevCV));
      }
    }
  }

  BOOST_AUTO_TEST_CASE(EventR){
    size_t numberOfTries = 100;
    TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535 };
    size_t numberOfValues = static_cast<size_t>(sizeof(valuesToTest) / sizeof(TForteUInt16));
    for(size_t i = 0; i < numberOfTries; i++){
      for(size_t j = 0; j < numberOfValues; j++){
        mInPV = valuesToTest[j];
        triggerEvent(1);
        BOOST_CHECK(checkR());
      }
    }
  }

  BOOST_AUTO_TEST_CASE(Mix){
    mInPV = 0;
    triggerEvent(1);
    BOOST_CHECK(checkR());

    triggerEvent(0);
    BOOST_CHECK(checkCU(0));

    mInPV = 1;
    triggerEvent(0);
    BOOST_CHECK(checkCU(1));

    triggerEvent(1);
    BOOST_CHECK(checkR());

    triggerEvent(0);
    BOOST_CHECK(checkCU(0));
    triggerEvent(0);
    BOOST_CHECK(checkCU(1));

    triggerEvent(0);
    BOOST_CHECK(checkCU(2));

    triggerEvent(1);
    BOOST_CHECK(checkR());

    mInPV = 65533;
    for(TForteUInt16 i = 0; i < CIEC_UINT::scm_nMaxVal; i++){
      //Send event
      triggerEvent(0);
      BOOST_CHECK(checkCU(i));
    }

    triggerEvent(0);
    BOOST_CHECK(checkCU(CIEC_UINT::scm_nMaxVal));

    triggerEvent(0);
    BOOST_CHECK(checkCU(CIEC_UINT::scm_nMaxVal));

    triggerEvent(0);
    BOOST_CHECK(checkCU(CIEC_UINT::scm_nMaxVal));

    triggerEvent(1);
    BOOST_CHECK(checkR());
  }

BOOST_AUTO_TEST_SUITE_END()
