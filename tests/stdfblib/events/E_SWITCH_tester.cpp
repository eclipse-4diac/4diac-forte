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
#include "E_SWITCH_tester.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SWITCH_tester_gen.cpp"
#endif

DEFINE_FB_TESTER(E_SWITCH_tester, g_nStringIdE_SWITCH)

E_SWITCH_tester::E_SWITCH_tester(CResource *m_poTestResource) :
    CFBTester(m_poTestResource){

  SETUP_INPUTDATA(&m_oIn_G);
}

void E_SWITCH_tester::executeAllTests(){
  evaluateTestResult(testCase_SingelE0(), "Single E0");
  evaluateTestResult(testCase_SingleE1(), "Single E1");
  evaluateTestResult(testCase_MultipleE0(), "Multiple E0");
  evaluateTestResult(testCase_MultipleE1(), "Multiple E1");
  evaluateTestResult(testCase_Alternate(), "Alternate");
}

/***********************************************************************************/
bool E_SWITCH_tester::testCase_SingelE0(){
  m_oIn_G = false;
  triggerEvent(0);
  return checkForSingleOutputEventOccurence(0);
}

bool E_SWITCH_tester::testCase_SingleE1(){
  m_oIn_G = true;
  triggerEvent(0);
  return checkForSingleOutputEventOccurence(1);
}

bool E_SWITCH_tester::testCase_MultipleE0(){
  bool bRetval = true;

  m_oIn_G = false;

  for(unsigned int i = 0; i < 1000; ++i){
    triggerEvent(0);
    if(!checkForSingleOutputEventOccurence(0)){
      bRetval = false;
      break;
    }
  }
  return bRetval;
}

bool E_SWITCH_tester::testCase_MultipleE1(){
  bool bRetval = true;

  m_oIn_G = true;

  for(unsigned int i = 0; i < 1000; ++i){
    triggerEvent(0);
    if(!checkForSingleOutputEventOccurence(1)){
      bRetval = false;
      break;
    }
  }
  return bRetval;
}

bool E_SWITCH_tester::testCase_Alternate(){
  bool bRetval = true;

  for(unsigned int i = 0; i < 1000; ++i){
    m_oIn_G = !m_oIn_G;
    triggerEvent(0);
    if(!checkForSingleOutputEventOccurence((m_oIn_G) ? 1 : 0)){
      bRetval = false;
      break;
    }
  }
  return bRetval;
}
