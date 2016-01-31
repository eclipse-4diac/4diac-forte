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
#include "E_DELAY_tester.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_DELAY_tester_gen.cpp"
#endif

#ifdef WIN32
	#include <windows.h>
	#define usleep(x) Sleep((x)/1000)
#else
	#include <unistd.h>
#endif


DEFINE_FB_TESTER(E_DELAY_tester, g_nStringIdE_DELAY)

E_DELAY_tester::E_DELAY_tester(CResource *m_poTestResource) :
    CFBTester(m_poTestResource){

  SETUP_INPUTDATA(&mDT);
}

void E_DELAY_tester::executeAllTests(){
  evaluateTestResult(testCase_NormalDelay(), "Normal Delay");
  evaluateTestResult(testCase_AbortedDelay(), "Aborted Delay");
  evaluateTestResult(testCase_MultipleStarts(), "Multiple Starts");
}

bool E_DELAY_tester::testCase_NormalDelay(){
  mDT.setFromMiliSeconds(500);
  triggerEvent(0);
  usleep(500000);
  return checkForSingleOutputEventOccurence(0);
}

bool E_DELAY_tester::testCase_AbortedDelay(){
  bool retVal = true;
  mDT.setFromMiliSeconds(1000);
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

bool E_DELAY_tester::testCase_MultipleStarts(){
  bool retVal = true;
  mDT.setFromMiliSeconds(200);
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
