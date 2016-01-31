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
#include "E_F_TRIG_tester.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_F_TRIG_tester_gen.cpp"
#endif

DEFINE_FB_TESTER(E_F_TRIG_tester, g_nStringIdE_F_TRIG)

E_F_TRIG_tester::E_F_TRIG_tester(CResource *m_poTestResource) :
    CFBTester(m_poTestResource){

  SETUP_INPUTDATA(&m_oIn_QI);
}

void E_F_TRIG_tester::executeAllTests(){
  evaluateTestResult(testCase_RaisingEdge(), "Raising Edge");
  evaluateTestResult(testCase_FallingEdge(), "Falling Edge");
  evaluateTestResult(testCase_StableHigh(), "Stable High");
  evaluateTestResult(testCase_StableLow(), "Stable Low");
}

bool E_F_TRIG_tester::testCase_RaisingEdge(){
  bool bResult = true;

  m_oIn_QI = true;
  triggerEvent(0);

  if(!eventChainEmpty()){
    bResult = false;
  }

  return bResult;
}

bool E_F_TRIG_tester::testCase_FallingEdge(){
  m_oIn_QI = true;
  triggerEvent(0);

  m_oIn_QI = false;
  triggerEvent(0);

  return checkForSingleOutputEventOccurence(0);
}

bool E_F_TRIG_tester::testCase_StableHigh(){
  bool bResult = true;

  m_oIn_QI = true;
  triggerEvent(0);

  for(unsigned int i = 0; i < 1000; i++){
    triggerEvent(0);
    if(!eventChainEmpty()){
      bResult = false;
      break;
    }
  }

  return bResult;
}

bool E_F_TRIG_tester::testCase_StableLow(){
  bool bResult = true;

  m_oIn_QI = false;
  triggerEvent(0);          //Just in case that the QI has been true first handle a potential falling edge
  pullFirstChainEventID();

  for(unsigned int i = 0; i < 1000; i++){
    triggerEvent(0);
    if(!eventChainEmpty()){
      bResult = false;
      break;
    }
  }

  return bResult;
}

