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
#include "E_CTUD_tester_gen.cpp"
#endif

struct E_CTUD_TestFixture : public CFBTestFixtureBase{

    E_CTUD_TestFixture() : CFBTestFixtureBase(g_nStringIdE_CTUD){
      SETUP_INPUTDATA(&mInPV);
      SETUP_OUTPUTDATA(&mOutQU, &mOutQD, &mOutCV);
      CFBTestFixtureBase::setup();
    }

    CIEC_UINT mInPV; //Preset value
    CIEC_BOOL mOutQU; //CV >= PV
    CIEC_BOOL mOutQD; //CV <= 0
    CIEC_UINT mOutCV; //count value

    bool checkCU(unsigned int paPrevCV){
      if(paPrevCV < 65535){
        if(((paPrevCV + 1) != mOutCV)){
          return false;
        }
        else if(!checkForSingleOutputEventOccurence(0)){
          return false;
        }
      }
      else{
        if(65535 != mOutCV && !eventChainEmpty()){
          return false;
        }
      }
      if(!checkBooleans()){
        return false;
      }
      return true;
    }

    bool checkCD(unsigned int paPrevCV){
      if(paPrevCV < 1){
        //no algorithm should have been executed
        if(mOutCV != paPrevCV || !mOutQD || !eventChainEmpty()){
          return false;
        }
      }
      else{
        if(((paPrevCV - 1) != mOutCV)){
          return false;
        }
        else if(mOutQD != (mOutCV < 1)){
          return false;
        }
        else if(!checkForSingleOutputEventOccurence(0)){
          return false;
        }
      }
      if(!checkBooleans()){
        return false;
      }
      return true;
    }

    bool checkR(){
      if(0 != mOutCV){
        return false;
      }
      if(!checkForSingleOutputEventOccurence(1)){
        return false;
      }
      if(!checkBooleans()){
        return false;
      }
      return true;
    }

    bool checkLD(unsigned int paUsedPV){
      if(paUsedPV != mInPV || mInPV != mOutCV || ((paUsedPV < 1) != (true == mOutQD))){
        return false;
      }
      if(!checkForSingleOutputEventOccurence(2)){
        return false;
      }
      if(!checkBooleans()){
        return false;
      }
      return true;
    }

    bool checkBooleans(){
      return !(mOutQU != (mOutCV >= mInPV) || (mOutQD != (mOutCV < 1)));
    }
};

BOOST_FIXTURE_TEST_SUITE( CTUDTests, E_CTUD_TestFixture)

  BOOST_AUTO_TEST_CASE(EventCU){
    TForteUInt16 prevCV = 0;
    TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535 };
    unsigned int numberOfValues = static_cast<unsigned int>(sizeof(valuesToTest) / sizeof(TForteUInt16));
    for(unsigned int j = 0; j < numberOfValues; j++){
      triggerEvent(2);
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

  BOOST_AUTO_TEST_CASE(EventCD){
    unsigned int numberOfTries = 100;
    TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535 };
    unsigned int numberOfValues = static_cast<unsigned int>(sizeof(valuesToTest) / sizeof(TForteUInt16));
    for(unsigned int i = 0; i < numberOfTries; i++){
      for(unsigned int j = 0; j < numberOfValues; j++){
        mInPV = valuesToTest[j];
        triggerEvent(3);
        checkForSingleOutputEventOccurence(1);
        //Send event
        triggerEvent(1);
        BOOST_CHECK(checkCD(valuesToTest[j]));
      }
    }
  }

  BOOST_AUTO_TEST_CASE(EventR){
    unsigned int numberOfTries = 100;
    TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535 };
    unsigned int numberOfValues = static_cast<unsigned int>(sizeof(valuesToTest) / sizeof(TForteUInt16));
    for(unsigned int i = 0; i < numberOfTries; i++){
      for(unsigned int j = 0; j < numberOfValues; j++){
        mInPV = valuesToTest[j];
        triggerEvent(3); //loads the value to input of the FB, because the Rese event doesn't scan the PV input.
        checkForSingleOutputEventOccurence(1);
        triggerEvent(2);
        BOOST_CHECK(checkR());
      }
    }
  }

  BOOST_AUTO_TEST_CASE(EventLD){
    unsigned int numberOfTries = 100;
    TForteUInt16 PVToTest[] = { 10, 1, 0, 65534, 65535 };
    unsigned int numberOftest = static_cast<unsigned int>(sizeof(PVToTest) / sizeof(TForteUInt16));
    for(unsigned int i = 0; i < numberOfTries; i++){
      for(unsigned int j = 0; j < numberOftest; j++){
        mInPV = PVToTest[j];
        triggerEvent(3);
        BOOST_CHECK(checkLD(PVToTest[j]));
      }
    }
  }

  BOOST_AUTO_TEST_CASE(Mix){
      unsigned int numberOfTries = 10;
      for(unsigned int i = 0; i < numberOfTries; i++){
        mInPV = 0;
        triggerEvent(3);
        BOOST_CHECK(checkLD(0));
        triggerEvent(0);
        BOOST_CHECK(checkCU(0));
        triggerEvent(0);
        BOOST_CHECK(checkCU(1));
        triggerEvent(1);
        BOOST_CHECK(checkCD(2));
        triggerEvent(3);
        BOOST_CHECK(checkLD(0));
        triggerEvent(2);
        BOOST_CHECK(checkR());

        mInPV = 1;
        triggerEvent(0);
        BOOST_CHECK(checkCU(0));
        triggerEvent(3);
        BOOST_CHECK(checkLD(1));
        triggerEvent(3);
        BOOST_CHECK(checkLD(1));

        mInPV = 65533;
        triggerEvent(3);
        BOOST_CHECK(checkLD(65533));
        triggerEvent(0);
        BOOST_CHECK(checkCU(65533));
        triggerEvent(0);
        BOOST_CHECK(checkCU(65534));
        triggerEvent(0);
        BOOST_CHECK(checkCU(65535));
        triggerEvent(0);
        BOOST_CHECK(checkCU(65535));
        triggerEvent(1);
        BOOST_CHECK(checkCD(65535));
        triggerEvent(1);
        BOOST_CHECK(checkCD(65534));
        triggerEvent(2);
        BOOST_CHECK(checkR());

        mInPV = 65533;
        for(unsigned int j = 0; j < 65533; j++){
          //Send event
          triggerEvent(0);
          BOOST_CHECK(checkCU(j));
        }
        triggerEvent(0);
        BOOST_CHECK(checkCU(65533));
        triggerEvent(0);
        BOOST_CHECK(checkCU(65534));
        triggerEvent(0);
        BOOST_CHECK(checkCU(65535));
        triggerEvent(0);
        BOOST_CHECK(checkCU(65535));
        triggerEvent(0);
        BOOST_CHECK(checkCU(65535));
        triggerEvent(1);
        BOOST_CHECK(checkCD(65535));
        triggerEvent(0);
        BOOST_CHECK(checkCU(65534));
        triggerEvent(2);
        BOOST_CHECK(checkR());
      }
    }

BOOST_AUTO_TEST_SUITE_END()
