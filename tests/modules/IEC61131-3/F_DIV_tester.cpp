/*******************************************************************************
 * Copyright (c) 2018, TU Wien/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Melik Merkumians - initial tests
 *******************************************************************************/
#include "F_DIV_tester.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_DIV_tester_gen.cpp"
#endif

DEFINE_FB_TESTER(F_DIV_tester, g_nStringIdF_DIV)

F_DIV_tester::F_DIV_tester(CResource *mTestResource) :
    CFBTester(mTestResource){

  SETUP_INPUTDATA(&mIn1_DIV, &mIn2_DIV);
  SETUP_OUTPUTDATA(&mOut_DIV);
}

void F_DIV_tester::executeAllTests(){
  evaluateTestResult(testCase_validDivision(), "Valid division");
  evaluateTestResult(testCase_divisionByZero(), "Division by zero");
}

/***********************************************************************************/

bool F_DIV_tester::testCase_validDivision() {
  mIn1_DIV = 30;
  mIn2_DIV = 5;
  /* trigger the inputevent */
  triggerEvent(0);

  return checkForSingleOutputEventOccurence(0);

}

bool F_DIV_tester::testCase_divisionByZero() {
  mIn1_DIV = 30;
  mIn2_DIV = 0;
  /* trigger the inputevent */
  triggerEvent(0);

  return checkForSingleOutputEventOccurence(0);

}
