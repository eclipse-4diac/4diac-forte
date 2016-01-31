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
#include "CFB_TEST_tester.h"
#include <boost/test/unit_test.hpp>

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "CFB_TEST_tester_gen.cpp"
#endif

DEFINE_FB_TESTER(CFB_TEST_tester, g_nStringIdCFB_TEST)

CFB_TEST_tester::CFB_TEST_tester(CResource *m_poTestResource) :
    CFBTester(m_poTestResource){

  SETUP_INPUTDATA(&m_oIn_QI);
  SETUP_OUTPUTDATA(&m_oOut_SR);
}

void CFB_TEST_tester::executeAllTests(){
  evaluateTestResult(inhibitTest(), "Inhibit Test");
  evaluateTestResult(setTest(), "set Test");
  evaluateTestResult(resetTest(), "reset Test");
  evaluateTestResult(toggleTest(), "toggle Test");
}

bool CFB_TEST_tester::inhibitTest(){
  bool bResult = true;

  m_oIn_QI = false;

  for(unsigned int i = 0; i < 100; ++i){
    triggerEvent(0);

    if(!eventChainEmpty()){
      bResult = false;
    }

    if(false != m_oOut_SR){
      bResult = false;
    }

    triggerEvent(1);

    if(!eventChainEmpty()){
      bResult = false;
    }

    if(false != m_oOut_SR){
      bResult = false;
    }
  }

  return bResult;
}

bool CFB_TEST_tester::setTest(){
  bool bResult = true;

  m_oIn_QI = true;

  triggerEvent(0);

  if(!checkBothOutputEvents()){
    return false;
  }

  if(false == m_oOut_SR){
    return false;
  }

  for(unsigned int i = 0; i < 100; ++i){
    triggerEvent(0);
    if(!checkForSingleOutputEventOccurence(0)){
      bResult = false;
      break;
    }

    if(false == m_oOut_SR){
      bResult = false;
      break;
    }
  }

  return bResult;
}

bool CFB_TEST_tester::resetTest(){
  bool bResult = true;

  m_oIn_QI = true;

  triggerEvent(0);
  clearEventChain();

  triggerEvent(1);
  if(!checkBothOutputEvents()){
    return false;
  }

  if(true == m_oOut_SR){
    return false;
  }

  for(unsigned int i = 0; i < 100; ++i){
    triggerEvent(1);
    if(!checkForSingleOutputEventOccurence(0)){
      bResult = false;
      break;
    }

    if(true == m_oOut_SR){
      bResult = false;
      break;
    }
  }

  return bResult;
}

bool CFB_TEST_tester::toggleTest(){
  bool bResult = true;

  m_oIn_QI = true;

  for(int i = 0; i < 1000; ++i){
    triggerEvent(0);
    if(!checkBothOutputEvents()){
      bResult = false;
      break;
    }

    if(true != m_oOut_SR){
      bResult = false;
      break;
    }

    triggerEvent(1);
    if(!checkBothOutputEvents()){
      bResult = false;
      break;
    }

    if(false != m_oOut_SR){
      bResult = false;
      break;
    }
  }

  return bResult;
}

bool CFB_TEST_tester::checkBothOutputEvents(){
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
