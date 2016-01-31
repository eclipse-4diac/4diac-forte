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
#include "E_PERMIT_tester.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_PERMIT_tester_gen.cpp"
#endif

DEFINE_FB_TESTER(E_PERMIT_tester, g_nStringIdE_PERMIT)

E_PERMIT_tester::E_PERMIT_tester(CResource *m_poTestResource) :
    CFBTester(m_poTestResource){

  SETUP_INPUTDATA(&m_oIn_PERMIT);
}

void E_PERMIT_tester::executeAllTests(){
  evaluateTestResult(testCase_Permit(), "Permit");
  evaluateTestResult(testCase_DontPermit(), "DontPermit");
}

/***********************************************************************************/

bool E_PERMIT_tester::testCase_Permit(){
  /* prepare inputparameters */
  m_oIn_PERMIT = true;

  /* trigger the inputevent */
  triggerEvent(0);

  return checkForSingleOutputEventOccurence(0);
}

bool E_PERMIT_tester::testCase_DontPermit(){
  bool bTestResult = true;

  /* prepare inputparameters */
  m_oIn_PERMIT = false;

  /* trigger the inputevent */
  triggerEvent(0);

  /* verify if there is no output event (testspecification)*/
  if(!eventChainEmpty())
    bTestResult = false;

  return bTestResult;
}
