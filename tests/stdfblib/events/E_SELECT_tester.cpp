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
#include "E_SELECT_tester.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SELECT_tester_gen.cpp"
#endif

DEFINE_FB_TESTER(E_SELECT_tester, g_nStringIdE_SELECT)

E_SELECT_tester::E_SELECT_tester(CResource *m_poTestResource) :
    CFBTester(m_poTestResource){
  SETUP_INPUTDATA(&m_oIn_G);
}

void E_SELECT_tester::executeAllTests(){
  evaluateTestResult(testCase_SelectEI0(), "Single EI0");
  evaluateTestResult(testCase_SelectEI1(), "Single EI1");
  evaluateTestResult(testCase_MultipleSelectEI0(), "Multiple EI0");
  evaluateTestResult(testCase_MultipleSelectEI1(), "Multiple EI1");
  evaluateTestResult(testCase_Alternate(), "Alternate");
}

/***********************************************************************************/
bool E_SELECT_tester::testCase_SelectEI0(){
  bool bRetVal = true;
  m_oIn_G = false;
  triggerEvent(0);
  if(!checkForSingleOutputEventOccurence(0)){
    bRetVal = false;
  }
  triggerEvent(1);
  if(!eventChainEmpty())
    bRetVal = false;
  return bRetVal;
}

bool E_SELECT_tester::testCase_SelectEI1(){
  bool bRetVal = true;
  m_oIn_G = true;
  triggerEvent(1);
  if(!checkForSingleOutputEventOccurence(0)){
    bRetVal = false;
  }
  triggerEvent(0);
  if(!eventChainEmpty())
    bRetVal = false;
  return bRetVal;
}

bool E_SELECT_tester::testCase_MultipleSelectEI0(){
  bool bRetVal = true;
  m_oIn_G = false;

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

bool E_SELECT_tester::testCase_MultipleSelectEI1(){
  bool bRetVal = true;
  m_oIn_G = true;

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

bool E_SELECT_tester::testCase_Alternate(){
  bool bRetVal = true;

  for(unsigned int i = 0; i < 1000; ++i){
    m_oIn_G = !m_oIn_G;
    triggerEvent((m_oIn_G) ? 1 : 0);
    if(!checkForSingleOutputEventOccurence(0)){
      bRetVal = false;
      break;
    }
  }

  for(unsigned int i = 0; i < 1000; i++){
      triggerEvent((m_oIn_G) ? 1 : 0);
      if(!checkForSingleOutputEventOccurence(0)){
        bRetVal = false;
        break;
      }
      triggerEvent((m_oIn_G) ? 0 : 1);
      if(!eventChainEmpty()){
        bRetVal = false;
        break;
      }
    }

  return bRetVal;
}
