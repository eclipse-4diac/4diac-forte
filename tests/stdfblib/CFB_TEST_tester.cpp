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
#include "../core/fbtests/fbtester.h"
#include <forte_bool.h>
#include "CFB_TEST.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "CFB_TEST_tester_gen.cpp"
#endif

class CFB_TEST_tester : public CFBTester{
    DECLARE_FB_TESTER(CFB_TEST_tester);
  public:
    CFB_TEST_tester(CResource* m_poTestResource) :
        CFBTester(m_poTestResource){
      SETUP_INPUTDATA(&mIn_QI);
      SETUP_OUTPUTDATA(&mOut_SR);
    }

    virtual ~CFB_TEST_tester() {

    }

  private:
    virtual void executeAllTests(){
      evaluateTestResult(inhibitTest(), "Inhibit Test");
      evaluateTestResult(setTest(), "set Test");
      evaluateTestResult(resetTest(), "reset Test");
      evaluateTestResult(toggleTest(), "toggle Test");
    }

    bool inhibitTest(){
      bool bResult = true;
      mIn_QI = false;
      for(unsigned int i = 0; i < 100; ++i){
        triggerEvent(0);
        if(!eventChainEmpty()){
          bResult = false;
        }
        if(false != mOut_SR){
          bResult = false;
        }
        triggerEvent(1);
        if(!eventChainEmpty()){
          bResult = false;
        }
        if(false != mOut_SR){
          bResult = false;
        }
      }
      return bResult;
    }
    bool setTest(){
      bool bResult = true;
      mIn_QI = true;
      triggerEvent(0);
      if(!checkBothOutputEvents()){
        return false;
      }
      if(false == mOut_SR){
        return false;
      }
      for(unsigned int i = 0; i < 100; ++i){
        triggerEvent(0);
        if(!checkForSingleOutputEventOccurence(0)){
          bResult = false;
          break;
        }
        if(false == mOut_SR){
          bResult = false;
          break;
        }
      }
      return bResult;
    }
    bool resetTest(){
      bool bResult = true;
      mIn_QI = true;
      triggerEvent(0);
      clearEventChain();
      triggerEvent(1);
      if(!checkBothOutputEvents()){
        return false;
      }
      if(true == mOut_SR){
        return false;
      }
      for(unsigned int i = 0; i < 100; ++i){
        triggerEvent(1);
        if(!checkForSingleOutputEventOccurence(0)){
          bResult = false;
          break;
        }
        if(true == mOut_SR){
          bResult = false;
          break;
        }
      }
      return bResult;
    }
    bool toggleTest(){
      bool bResult = true;
      mIn_QI = true;
      for(int i = 0; i < 1000; ++i){
        triggerEvent(0);
        if(!checkBothOutputEvents()){
          bResult = false;
          break;
        }
        if(true != mOut_SR){
          bResult = false;
          break;
        }
        triggerEvent(1);
        if(!checkBothOutputEvents()){
          bResult = false;
          break;
        }
        if(false != mOut_SR){
          bResult = false;
          break;
        }
      }
      return bResult;
    }

    bool checkBothOutputEvents(){
      bool bResult = true;
      if(0 != pullFirstChainEventID()){
        bResult = false;
      }
      if(1 != pullFirstChainEventID()){
        bResult = false;
      }
      if(!eventChainEmpty()){
        bResult = false;
      }
      return bResult;
    }

    CIEC_BOOL mIn_QI;
    CIEC_BOOL mOut_SR;
};

/***********************************************************************************/

DEFINE_FB_TESTER(CFB_TEST_tester, g_nStringIdCFB_TEST);
