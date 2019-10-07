/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "../../core/fbtests/fbtester.h"
#include <forte_time.h>
#include <E_DELAY.h>
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_DELAY_tester_gen.cpp"
#endif

#ifdef WIN32
  #include <windows.h>
  #define usleep(x) Sleep((x)/1000)
#else
  #include <unistd.h>
#endif

/***********************************************************************************/
/***********************************************************************************/

class E_DELAY_tester  : public CFBTester{
    DECLARE_FB_TESTER(E_DELAY_tester);

  public:
    E_DELAY_tester(CResource* m_poTestResource) :
        CFBTester(m_poTestResource){
      SETUP_INPUTDATA(&mDT);
    }

    virtual ~E_DELAY_tester() {

    }

  private:
    virtual void executeAllTests(){
      evaluateTestResult(testCase_NormalDelay(), "Normal Delay");
      evaluateTestResult(testCase_AbortedDelay(), "Aborted Delay");
      evaluateTestResult(testCase_MultipleStarts(), "Multiple Starts");
    }

    bool testCase_NormalDelay(){
      mDT.setFromMilliSeconds(500);
      triggerEvent(0);
      usleep(500000);
      return checkForSingleOutputEventOccurence(0);
    }
    bool testCase_AbortedDelay(){
      bool retVal = true;
      mDT.setFromMilliSeconds(1000);
      triggerEvent(0);
      if(!eventChainEmpty()){
        retVal = false;
      }
      triggerEvent(1);
      if(!eventChainEmpty()){
        retVal = false;
      }
      usleep(1000000);
      if(!eventChainEmpty()){
        retVal = false;
      }
      return retVal;
    }
    bool testCase_MultipleStarts(){
      bool retVal = true;
      mDT.setFromMilliSeconds(200);
      triggerEvent(0);
      usleep(50000);
      if(!eventChainEmpty()){
        retVal = false;
      }
      triggerEvent(0);
      usleep(150000);
      if(!checkForSingleOutputEventOccurence(0)){
        retVal = false;
      }
      usleep(50000);
      if(!eventChainEmpty()){
        retVal = false;
      }
      return retVal;
    }


    CIEC_TIME mDT; //DT data input
};

/***********************************************************************************/
/***********************************************************************************/

DEFINE_FB_TESTER(E_DELAY_tester, g_nStringIdE_DELAY)
