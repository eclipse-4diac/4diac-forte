/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
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
#include <E_SELECT.h>
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SELECT_tester_gen.cpp"
#endif


class E_SELECT_tester : public CFBTester{
    DECLARE_FB_TESTER(E_SELECT_tester);
  public:
    E_SELECT_tester(CResource* m_poTestResource) :
        CFBTester(m_poTestResource){
      SETUP_INPUTDATA(&mIn_G);
    }

    virtual ~E_SELECT_tester() {

    }

  private:
    virtual void executeAllTests(){
      evaluateTestResult(testCase_SelectEI0(), "Single EI0");
      evaluateTestResult(testCase_SelectEI1(), "Single EI1");
      evaluateTestResult(testCase_MultipleSelectEI0(), "Multiple EI0");
      evaluateTestResult(testCase_MultipleSelectEI1(), "Multiple EI1");
      evaluateTestResult(testCase_Alternate(), "Alternate");
    }

    /***********************************************************************************/
    bool testCase_SelectEI0(){
      bool bRetVal = true;
      mIn_G = false;
      triggerEvent(0);
      if(!checkForSingleOutputEventOccurence(0)){
        bRetVal = false;
      }
      triggerEvent(1);
      if(!eventChainEmpty())
        bRetVal = false;

      return bRetVal;
    }
    bool testCase_SelectEI1(){
      bool bRetVal = true;
      mIn_G = true;
      triggerEvent(1);
      if(!checkForSingleOutputEventOccurence(0)){
        bRetVal = false;
      }
      triggerEvent(0);
      if(!eventChainEmpty())
        bRetVal = false;

      return bRetVal;
    }
    bool testCase_MultipleSelectEI0(){
      bool bRetVal = true;
      mIn_G = false;
      for(unsigned int i = 0; i < 1000; i++){
        triggerEvent(0);
        if(!checkForSingleOutputEventOccurence(0)){
          bRetVal = false;
          break;
        }
      }
      for(unsigned int i = 0; i < 1000; i++){
        triggerEvent(0);
        if(!checkForSingleOutputEventOccurence(0)){
          bRetVal = false;
          break;
        }
        triggerEvent(1);
        if(!eventChainEmpty()){
          bRetVal = false;
          break;
        }
      }
      return bRetVal;
    }
    bool testCase_MultipleSelectEI1(){
      bool bRetVal = true;
      mIn_G = true;
      for(unsigned int i = 0; i < 1000; i++){
        triggerEvent(1);
        if(!checkForSingleOutputEventOccurence(0)){
          bRetVal = false;
          break;
        }
      }
      for(unsigned int i = 0; i < 1000; i++){
        triggerEvent(1);
        if(!checkForSingleOutputEventOccurence(0)){
          bRetVal = false;
          break;
        }
        triggerEvent(0);
        if(!eventChainEmpty()){
          bRetVal = false;
          break;
        }
      }
      return bRetVal;
    }
    bool testCase_Alternate(){
      bool bRetVal = true;
      for(unsigned int i = 0; i < 1000; ++i){
        mIn_G = !mIn_G;
        triggerEvent((mIn_G) ? 1 : 0);
        if(!checkForSingleOutputEventOccurence(0)){
          bRetVal = false;
          break;
        }
      }
      for(unsigned int i = 0; i < 1000; i++){
        triggerEvent((mIn_G) ? 1 : 0);
        if(!checkForSingleOutputEventOccurence(0)){
          bRetVal = false;
          break;
        }
        triggerEvent((mIn_G) ? 0 : 1);
        if(!eventChainEmpty()){
          bRetVal = false;
          break;
        }
      }
      return bRetVal;
    }

    CIEC_BOOL mIn_G; //DATA INPUT
};

/***********************************************************************************/

DEFINE_FB_TESTER(E_SELECT_tester, g_nStringIdE_SELECT)
