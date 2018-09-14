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

#include "../../core/fbtests/fbtester.h"
#include <forte_uint.h>
#include <forte_bool.h>
#include <E_CTD.h>

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CTD_tester_gen.cpp"
#endif

class E_CTD_tester : public CFBTester{
  DECLARE_FB_TESTER(E_CTD_tester);

  public:
    E_CTD_tester(CResource* m_poTestResource) :
        CFBTester(m_poTestResource){
      SETUP_INPUTDATA(&m_oIn_PV);
      SETUP_OUTPUTDATA(&m_oOut_Q, &m_oOut_CV);
    }
    virtual ~E_CTD_tester(){
    }
    ;
  private:
    virtual void executeAllTests(){
      evaluateTestResult(testCase_EventCD(), "EventCD");
      evaluateTestResult(testCase_EventLD(), "EventLD");
      evaluateTestResult(testCase_Mix(), "Alternating CD and LD");
    }

    bool testCase_EventCD(){
      unsigned int numberOfTries = 100;
      TForteUInt16 valuesToTest[] = { 10, 1, 0, 65534, 65535 };
      unsigned int numberOfValues = static_cast<unsigned int>(sizeof(valuesToTest) / sizeof(TForteUInt16));
      for(unsigned int i = 0; i < numberOfValues; i++){
        for(unsigned int j = 0; j < numberOfTries; j++){
          m_oIn_PV = valuesToTest[i];
          triggerEvent(1);
          checkForSingleOutputEventOccurence(1);
          //Send event
          triggerEvent(0);
          if(!checkCD(valuesToTest[i])){
            return false;
          }
        }
      }
      return true;
    }
    bool testCase_EventLD(){
      unsigned int numberOfTries = 100;
      TForteUInt16 PVToTest[] = { 10, 1, 0, 65534, 65535 };
      unsigned int numberOftest = static_cast<unsigned int>(sizeof(PVToTest) / sizeof(TForteUInt16));
      for(unsigned int i = 0; i < numberOftest; ++i){
        for(unsigned int j = 0; j < numberOfTries; j++){
          m_oIn_PV = PVToTest[i];
          triggerEvent(1);
          if(!checkLD(PVToTest[i])){
            return false;
          }
        }
      }
      return true;
    }
    bool testCase_Mix(){
      unsigned int numberOfTries = 100;
      for(unsigned int i = 0; i < numberOfTries; i++){
        m_oIn_PV = 0;
        triggerEvent(1);
        if(!checkLD(0)){
          return false;
        }
        triggerEvent(0);
        if(!checkCD(0)){
          return false;
        }
        m_oIn_PV = 1;
        triggerEvent(0);
        if(!checkCD(0)){
          return false;
        }
        triggerEvent(1);
        if(!checkLD(1)){
          return false;
        }
        triggerEvent(0);
        if(!checkCD(1)){
          return false;
        }
        triggerEvent(0);
        if(!checkCD(0)){
          return false;
        }
        triggerEvent(1);
        if(!checkLD(1)){
          return false;
        }
        triggerEvent(1);
        if(!checkLD(1)){
          return false;
        }
        triggerEvent(1);
        if(!checkLD(1)){
          return false;
        }
        triggerEvent(0);
        if(!checkCD(1)){
          return false;
        }
        triggerEvent(0);
        if(!checkCD(0)){
          return false;
        }
        m_oIn_PV = 65535;
        triggerEvent(1);
        if(!checkLD(65535)){
          return false;
        }
        triggerEvent(0);
        if(!checkCD(65535)){
          return false;
        }
        triggerEvent(0);
        if(!checkCD(65534)){
          return false;
        }
        triggerEvent(1);
        if(!checkLD(65535)){
          return false;
        }
        triggerEvent(1);
        if(!checkLD(65535)){
          return false;
        }
        triggerEvent(1);
        if(!checkLD(65535)){
          return false;
        }
        triggerEvent(0);
        if(!checkCD(65535)){
          return false;
        }
        triggerEvent(0);
        if(!checkCD(65534)){
          return false;
        }
      }
      return true;
    }

    bool checkCD(TForteUInt16 pa_prevCV){
      if(pa_prevCV < 1){
        //no algorithm should have been executed
        if(m_oOut_CV != pa_prevCV || !m_oOut_Q || !eventChainEmpty()){
          return false;
        }
      }
      else{
        if(((pa_prevCV - 1) != m_oOut_CV)){
          return false;
        }
        else if(m_oOut_Q != (m_oOut_CV < 1)){
          return false;
        }
        else if(!checkForSingleOutputEventOccurence(0)){
          return false;
        }
      }
      return true;
    }
    bool checkLD(TForteUInt16 pa_usedPV){
      if(pa_usedPV != m_oIn_PV || m_oIn_PV != m_oOut_CV || ((pa_usedPV < 1) != (true == m_oOut_Q))){
        return false;
      }
      if(!checkForSingleOutputEventOccurence(1)){
        return false;
      }
      return true;
    }

    CIEC_UINT m_oIn_PV; //Preset value
    CIEC_BOOL m_oOut_Q; //CV < 0
    CIEC_UINT m_oOut_CV; //count value
};

DEFINE_FB_TESTER(E_CTD_tester, g_nStringIdE_CTD);
