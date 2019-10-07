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

F_EQ_TIME_tester::F_EQ_TIME_tester(CResource *m_poTestResource) :
    CFBTester(m_oTestee, m_poTestResource),
        m_oTestee(0, m_poTestResource),
        m_poOut_EO_Conn(m_oTestee, g_nStringIdCNF),
        m_oIn_IN1_Conn(m_oTestee, g_nStringIdIN1, &m_oIN1),
        m_oIn_IN2_Conn(m_oTestee, g_nStringIdIN2, &m_oIN2),
        m_oOut_OUT_Conn(m_oTestee, g_nStringIdOUT, &m_oOut_OUT){
  m_oTestee.changeFBExecutionState(cg_nMGM_CMD_Start);
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
  if(m_oOut_OUT != true) {
    bResult = false;
  }
  m_oIN1.fromString("T#500ms");
  m_oIN2.fromString("T#500ms");

  triggerEvent(0);

  if(m_oOut_OUT != true) {
    bResult = false;
  }
  return bResult;
}

bool F_EQ_TIME_tester::testCase_Larger(){
  bool bResult = true;

  m_oIN1.fromString("T#800ms");
  m_oIN2.fromString("T#300ms");

  triggerEvent(0);

  if(m_oOut_OUT != false) {
    bResult = false;
  }
  return bResult;
}

bool F_EQ_TIME_tester::testCase_Smaller(){
  bool bResult = true;

  m_oIN1.fromString("T#150ms");
  m_oIN2.fromString("T#1s");

  triggerEvent(0);

  if(m_oOut_OUT != false) {
    bResult = false;
  }
  return bResult;
}
