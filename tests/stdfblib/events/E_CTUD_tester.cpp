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
#include "E_CTUD_tester.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CTUD_tester_gen.cpp"
#endif

#include <iostream>

DEFINE_FB_TESTER(E_CTUD_tester, g_nStringIdE_CTUD)

E_CTUD_tester::E_CTUD_tester(CResource *m_poTestResource) :
    CFBTester(m_poTestResource){
  SETUP_INPUTDATA(&m_oIn_PV);
  SETUP_OUTPUTDATA(&m_oOut_QU, &m_oOut_QD, &m_oOut_CV);
}

void E_CTUD_tester::executeAllTests(){
  evaluateTestResult(testCase_EventCU(), "EventCU");
  evaluateTestResult(testCase_EventCD(), "EventCD");
  evaluateTestResult(testCase_EventR(), "EventR");
  evaluateTestResult(testCase_EventLD(), "EventLD");
  evaluateTestResult(testCase_Mix(), "Alternating Events");
}

bool E_CTUD_tester::testCase_EventCU(){
  TForteUInt16 prevCV = 0;

  TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535 };
  unsigned int numberOfValues = static_cast<unsigned int>(sizeof(valuesToTest) / sizeof(TForteUInt16));

  for(unsigned int j = 0; j < numberOfValues; j++){
    triggerEvent(2);

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

bool E_CTUD_tester::testCase_EventR(){
  unsigned int numberOfTries = 100;

  TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535};
  unsigned int numberOfValues = static_cast<unsigned int>(sizeof(valuesToTest) / sizeof(TForteUInt16));

  for(unsigned int i = 0; i < numberOfTries; i++){
    for(unsigned int j = 0; j < numberOfValues; j++){
      m_oIn_PV = valuesToTest[j];

      triggerEvent(3); //loads the value to input of the FB, because the Rese event doesn't scan the PV input.
      checkForSingleOutputEventOccurence(1);


      triggerEvent(2);

      if(!checkR()){
        return false;
      }
    }
  }

  return true;
}

bool E_CTUD_tester::testCase_EventCD(){
  unsigned int numberOfTries = 100;

  TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535};
  unsigned int numberOfValues = static_cast<unsigned int>(sizeof(valuesToTest) / sizeof(TForteUInt16));

  for(unsigned int i = 0; i < numberOfTries; i++){
    for(unsigned int j = 0; j < numberOfValues; j++){
      m_oIn_PV = valuesToTest[j];
      triggerEvent(3);

      checkForSingleOutputEventOccurence(1);

      //Send event
      triggerEvent(1);

      if(!checkCD(valuesToTest[j])){
        return false;
      }
    }
  }
  return true;
}

bool E_CTUD_tester::testCase_EventLD(){
  unsigned int numberOfTries = 100;

  TForteUInt16 PVToTest[] = { 10, 1, 0, 65534, 65535 };
  unsigned int numberOftest = static_cast<unsigned int>(sizeof(PVToTest) / sizeof(TForteUInt16));

  for(unsigned int i = 0; i < numberOfTries; i++){
    for(unsigned int j = 0; j < numberOftest; j++){
      m_oIn_PV = PVToTest[j];

      triggerEvent(3);

      if(!checkLD(PVToTest[j])){
        return false;
      }
    }
  }

  return true;
}

bool E_CTUD_tester::testCase_Mix(){
  unsigned int numberOfTries = 10;

  for(unsigned int i = 0; i < numberOfTries; i++){
    m_oIn_PV = 0;
    triggerEvent(3);
    if(!checkLD(0)){
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

		triggerEvent(1);
    if(!checkCD(2)){
      return false;
    }

    triggerEvent(3);
    if(!checkLD(0)){
      return false;
    }

    triggerEvent(2);
    if(!checkR()){
      return false;
    }

    m_oIn_PV = 1;
    triggerEvent(0);
    if(!checkCU(0)){
      return false;
    }

    triggerEvent(3);
    if(!checkLD(1)){
      return false;
    }

    triggerEvent(3);
    if(!checkLD(1)){
      return false;
    }

    m_oIn_PV = 65533;
    triggerEvent(3);
    if(!checkLD(65533)){
      return false;
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

    triggerEvent(1);
    if(!checkCD(65535)){
      return false;
    }

    triggerEvent(1);
    if(!checkCD(65534)){
      return false;
    }

    triggerEvent(2);
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
    if(!checkCD(65535)){
      return false;
    }

    triggerEvent(0);
    if(!checkCU(65534)){
      return false;
    }

    triggerEvent(2);
    if(!checkR()){
      return false;
    }
  }

  return true;
}

bool E_CTUD_tester::checkCU(TForteUInt16 pa_prevCV){
  if(pa_prevCV < 65535){
    if(((pa_prevCV + 1) != m_oOut_CV)){
      std::cout << "Error 1\n";
      return false;
    }
    else if(!checkForSingleOutputEventOccurence(0)){
      std::cout << "Error 2\n";
      return false;
    }
  }
  else{
    if(65535 != m_oOut_CV && !eventChainEmpty()){
      std::cout << "Error 3\n";
      return false;
    }
  }

  if (!checkBooleans()){
	  return false;
  }
  return true;
}

bool E_CTUD_tester::checkCD(TForteUInt16 pa_prevCV){
  if(pa_prevCV < 1){ //no algorithm should have been executed
    if(m_oOut_CV != pa_prevCV || !m_oOut_QD || !eventChainEmpty()){
      std::cout << "Error 4\n";
      return false;
    }
  }
  else{
    if(((pa_prevCV - 1) != m_oOut_CV)){
      std::cout << "Error 5\n";
      return false;
    }
    else if(m_oOut_QD != (m_oOut_CV < 1)){
      std::cout << "Error 6\n";
      return false;
    }
    else if(!checkForSingleOutputEventOccurence(0)){
      std::cout << "Error 7\n";
      return false;
    }
  }

  if(!checkBooleans()){
    return false;
  }
  return true;
}

bool E_CTUD_tester::checkR(){
  if(0 != m_oOut_CV){
    std::cout << "Error 8\n";
    return false;
  }
  if(!checkForSingleOutputEventOccurence(1)){
    std::cout << "Error 9\n";
    return false;
  }
  if(!checkBooleans()){
    std::cout << "Error 10\n";
    return false;
  }
  return true;
}

bool E_CTUD_tester::checkLD(TForteUInt16 pa_usedPV){
  if(pa_usedPV != m_oIn_PV || m_oIn_PV != m_oOut_CV || ((pa_usedPV < 1) != (true == m_oOut_QD))){
    std::cout << "Error 11\n";
    return false;
  }
  if(!checkForSingleOutputEventOccurence(2)){
    std::cout << "Error 12\n";
    return false;
  }

  if(!checkBooleans()){
    std::cout << "Error 13\n";
    return false;
  }
  return true;
}

bool E_CTUD_tester::checkBooleans(){
	if(m_oOut_QU != (m_oOut_CV >= m_oIn_PV) || (m_oOut_QD != (m_oOut_CV < 1))){
	  std::cout << "m_oIn_PV\tm_oOut_CV\tm_oOut_QU\tm_oOut_QD\n";
	  std::cout << m_oIn_PV << "\t" << m_oOut_CV << "\t" << m_oOut_QU << "\t" << m_oOut_QD << "\n";
	  std::cout << "Boolean error\n";
		return false;
	}
	return true;
}
