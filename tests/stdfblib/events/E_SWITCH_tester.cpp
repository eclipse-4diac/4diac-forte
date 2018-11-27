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
#include <E_SWITCH.h>
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SWITCH_tester_gen.cpp"
#endif


class E_SWITCH_tester : public CFBTester{
    DECLARE_FB_TESTER(E_SWITCH_tester);
  public:
    E_SWITCH_tester(CResource* m_poTestResource) :
        CFBTester(m_poTestResource){
      SETUP_INPUTDATA(&mIn_G);
    }

    virtual ~E_SWITCH_tester() {

    }

  private:
    virtual void executeAllTests(){
      evaluateTestResult(testCase_SingelE0(), "Single E0");
      evaluateTestResult(testCase_SingleE1(), "Single E1");
      evaluateTestResult(testCase_MultipleE0(), "Multiple E0");
      evaluateTestResult(testCase_MultipleE1(), "Multiple E1");
      evaluateTestResult(testCase_Alternate(), "Alternate");
    }

    /***********************************************************************************/
    bool testCase_SingelE0(){
      mIn_G = false;
      triggerEvent(0);
      return checkForSingleOutputEventOccurence(0);
    }
    bool testCase_SingleE1(){
      mIn_G = true;
      triggerEvent(0);
      return checkForSingleOutputEventOccurence(1);
    }
    bool testCase_MultipleE0(){
      bool bRetval = true;
      mIn_G = false;
      for(unsigned int i = 0; i < 1000; ++i){
        triggerEvent(0);
        if(!checkForSingleOutputEventOccurence(0)){
          bRetval = false;
          break;
        }
      }
      return bRetval;
    }
    bool testCase_MultipleE1(){
      bool bRetval = true;
      mIn_G = true;
      for(unsigned int i = 0; i < 1000; ++i){
        triggerEvent(0);
        if(!checkForSingleOutputEventOccurence(1)){
          bRetval = false;
          break;
        }
      }
      return bRetval;
    }
    bool testCase_Alternate(){
      bool bRetval = true;
      for(unsigned int i = 0; i < 1000; ++i){
        mIn_G = !mIn_G;
        triggerEvent(0);
        if(!checkForSingleOutputEventOccurence((mIn_G) ? 1 : 0)){
          bRetval = false;
          break;
        }
      }
      return bRetval;
    }

    CIEC_BOOL mIn_G; //DATA INPUT
};

/***********************************************************************************/

DEFINE_FB_TESTER(E_SWITCH_tester, g_nStringIdE_SWITCH);
