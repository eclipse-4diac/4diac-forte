/*******************************************************************************
 * Copyright (c) 2013 - 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <F_EQ_TIME_tester.h>
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_EQ_TIME_gen.cpp"
#endif

DEFINE_FB_TESTER(F_EQ_TIME_tester, g_nStringIdF_GT)

F_EQ_TIME_tester::F_EQ_TIME_tester(CResource *mTestResource) :
    CFBTester(mTestee, mTestResource),
        mTestee(0, mTestResource),
        mOut_EO_Conn(mTestee, g_nStringIdCNF),
        mIn_IN1_Conn(mTestee, g_nStringIdIN1, &mIN1),
        mIn_IN2_Conn(mTestee, g_nStringIdIN2, &mIN2),
        mOut_OUT_Conn(mTestee, g_nStringIdOUT, &mOut_OUT){
  mTestee.changeExecutionState(EMGMCommandType::Start);
}

void F_EQ_TIME_tester::executeAllTests(){
  evaluateTestResult(testCase_Equal(), "Equal");
  evaluateTestResult(testCase_Larger(), "Larger");
  evaluateTestResult(testCase_Smaller(), "Smaller");
}

bool F_EQ_TIME_tester::testCase_Equal(){
  bool bResult = true;

  //Send event
  triggerEvent(0);

  //Test correct order of outgoing events
  if(pullFirstChainEventID() != 0) {
    bResult = false;
  }
  //Test if event chain is empty
  if(!eventChainEmpty()) {
    bResult = false;
  }
  //verify data output as both are zero the value value should be true
  if(mOut_OUT != true) {
    bResult = false;
  }
  mIN1.fromString("T#500ms");
  mIN2.fromString("T#500ms");

  triggerEvent(0);

  if(mOut_OUT != true) {
    bResult = false;
  }
  return bResult;
}

bool F_EQ_TIME_tester::testCase_Larger(){
  bool bResult = true;

  mIN1.fromString("T#800ms");
  mIN2.fromString("T#300ms");

  triggerEvent(0);

  if(mOut_OUT != false) {
    bResult = false;
  }
  return bResult;
}

bool F_EQ_TIME_tester::testCase_Smaller(){
  bool bResult = true;

  mIN1.fromString("T#150ms");
  mIN2.fromString("T#1s");

  triggerEvent(0);

  if(mOut_OUT != false) {
    bResult = false;
  }
  return bResult;
}
