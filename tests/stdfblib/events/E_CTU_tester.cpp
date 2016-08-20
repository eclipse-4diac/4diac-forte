/*******************************************************************************
 * Copyright (c) 2016, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_CTU_tester.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CTU_tester_gen.cpp"
#endif

#include <iostream>

DEFINE_FB_TESTER(E_CTU_tester, g_nStringIdE_CTU)

E_CTU_tester::E_CTU_tester(CResource *m_poTestResource) :
    CFBTester(m_poTestResource){
  SETUP_INPUTDATA(&m_oIn_PV);
  SETUP_OUTPUTDATA(&m_oOut_Q, &m_oOut_CV);
}

void E_CTU_tester::executeAllTests(){
  evaluateTestResult(testCase_EventCU(), "EventCU");
  evaluateTestResult(testCase_EventR(), "EventR");
  evaluateTestResult(testCase_Mix(), "Alternating CU and R");
}

bool E_CTU_tester::testCase_EventCU(){
  TForteUInt16 prevCV = 0;

  TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535 };
  unsigned int numberOfValues = sizeof(valuesToTest) / sizeof(TForteUInt16);

  for(unsigned int j = 0; j < numberOfValues; j++){
    triggerEvent(1);

    if(!checkR()){
      return false;
    }

    m_oIn_PV = valuesToTest[j];
    for(unsigned int k = 0; k < static_cast<unsigned int>(m_oIn_PV + 3); k++){
      prevCV = m_oOut_CV;
      //Send event
      triggerEvent(0);
      if(!checkCU(prevCV)){
        return false;
      }
    }
  }
  return true;
}

bool E_CTU_tester::testCase_EventR(){
  unsigned int numberOfTries = 100;

  TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535};
  unsigned int numberOfValues = sizeof(valuesToTest) / sizeof(TForteUInt16);

  for(unsigned int i = 0; i < numberOfTries; i++){
    for(unsigned int j = 0; j < numberOfValues; j++){
      m_oIn_PV = valuesToTest[j];
      triggerEvent(1);

      if(!checkR()){
        return false;
      }
    }
  }

  return true;
}

bool E_CTU_tester::testCase_Mix(){

  m_oIn_PV = 0;
  triggerEvent(1);
  if(!checkR()){
    return false;
  }

  triggerEvent(0);
  if(!checkCU(0)){
    return false;
  }

  m_oIn_PV = 1;
  triggerEvent(0);
  if(!checkCU(1)){
    return false;
  }

  triggerEvent(1);
  if(!checkR()){
    return false;
  }

  triggerEvent(0);
  if(!checkCU(0)){
    return false;
  }

  triggerEvent(0);
  if(!checkCU(1)){
    return false;
  }

  triggerEvent(0);
  if(!checkCU(2)){
    return false;
  }

  triggerEvent(1);
  if(!checkR()){
    return false;
  }

  m_oIn_PV = 65533;
  for(TForteUInt16 i = 0; i < 65533; i++){
    //Send event
    triggerEvent(0);
    if(!checkCU(i)){
      return false;
    }
  }

  triggerEvent(0);
  if(!checkCU(65533)){
    return false;
  }

  triggerEvent(0);
  if(!checkCU(65534)){
    return false;
  }

  triggerEvent(0);
  if(!checkCU(65535)){
    return false;
  }

  triggerEvent(0);
  if(!checkCU(65535)){
    return false;
  }

  triggerEvent(0);
  if(!checkCU(65535)){
    return false;
  }

  triggerEvent(1);
  if(!checkR()){
    return false;
  }

  return true;
}

bool E_CTU_tester::checkCU(TForteUInt16 pa_prevCV){
  if(pa_prevCV < 65535){
    if(((pa_prevCV + 1) != m_oOut_CV)){
      return false;
    }
    else if(!checkForSingleOutputEventOccurence(0)){
      return false;
    }
  }
  else{
    if(65535 != m_oOut_CV && !eventChainEmpty()){
      return false;
    }
  }

  if(m_oOut_Q != (m_oOut_CV >= m_oIn_PV)){
    return false;
  }
  return true;
}

bool E_CTU_tester::checkR(){
  if(0 != m_oOut_CV || m_oOut_Q){
    return false;
  }
  if(!checkForSingleOutputEventOccurence(1)){
    return false;
  }
  return true;
}
