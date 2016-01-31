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
#include "E_SR_tester.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SR_tester_gen.cpp"
#endif

DEFINE_FB_TESTER(E_SR_tester, g_nStringIdE_SR)

E_SR_tester::E_SR_tester(CResource *m_poTestResource) :
    CFBTester(m_poTestResource){
  SETUP_OUTPUTDATA(&m_oOut_Q);
}

void E_SR_tester::executeAllTests(){
  evaluateTestResult(testCase_EventS(), "EventS");
  evaluateTestResult(testCase_EventR(), "EventR");
  evaluateTestResult(testCase_Toggle(), "Toggle");
}

bool E_SR_tester::testCase_EventS(){
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
    if(m_oOut_Q != true){
      bResult = false;
      break;
    }
  }

  return bResult;
}

bool E_SR_tester::testCase_EventR(){
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
    if(m_oOut_Q != false){
      bResult = false;
      break;
    }
  }

  return bResult;
}

bool E_SR_tester::testCase_Toggle(){
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

bool E_SR_tester::checkStateChange(bool bTargetState){
  bool bResult = checkForSingleOutputEventOccurence(0);

  //verify data output
  if(m_oOut_Q != bTargetState)
    bResult = false;

  return bResult;
}
