/*******************************************************************************
 * Copyright (c) 2011 - 2014 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "../../core/fbtests/fbtester.h"
#include <forte_bool.h>
#include <E_SR.h>
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SR_tester_gen.cpp"
#endif

/***********************************************************************************/
/***********************************************************************************/

class E_SR_tester  : public CFBTester{
    DECLARE_FB_TESTER(E_SR_tester);

  public:
    E_SR_tester(CResource* m_poTestResource) :
        CFBTester(m_poTestResource){
      SETUP_OUTPUTDATA(&mOut_Q);
    }

    virtual ~E_SR_tester() {

    }

  private:
    virtual void executeAllTests(){
      evaluateTestResult(testCase_EventS(), "EventS");
      evaluateTestResult(testCase_EventR(), "EventR");
      evaluateTestResult(testCase_Toggle(), "Toggle");
    }

    bool testCase_EventS(){
      bool bResult = true;
      //Send event
      triggerEvent(0);
      if(!checkStateChange(true))
        bResult = false;

      for(unsigned int i = 0; i < 100; ++i){
        triggerEvent(0);
        if(!eventChainEmpty()){
          bResult = false;
          break;
        }
        if(mOut_Q != true){
          bResult = false;
          break;
        }
      }
      return bResult;
    }
    bool testCase_EventR(){
      bool bResult = true;
      //Send event
      triggerEvent(1);
      //Test correct order of outgoing events
      if(!checkStateChange(false))
        bResult = false;

      for(unsigned int i = 0; i < 100; ++i){
        triggerEvent(1);
        if(!eventChainEmpty()){
          bResult = false;
          break;
        }
        if(mOut_Q != false){
          bResult = false;
          break;
        }
      }
      return bResult;
    }
    bool testCase_Toggle(){
      bool bResult = true;
      for(unsigned int i = 0; i < 100; ++i){
        triggerEvent(0);
        if(!checkStateChange(true)){
          bResult = false;
          break;
        }
        triggerEvent(1);
        checkStateChange(false);
      }
      return bResult;
    }

    /*\brief Check if the E_SR changed to the given target state
     *
     */
    bool checkStateChange(bool bTargetState){
      bool bResult = checkForSingleOutputEventOccurence(0);
      //verify data output
      if(mOut_Q != bTargetState)
        bResult = false;

      return bResult;
    }

    CIEC_BOOL mOut_Q; //DATA OUTPUT
};

/***********************************************************************************/
/***********************************************************************************/

DEFINE_FB_TESTER(E_SR_tester, g_nStringIdE_SR);
