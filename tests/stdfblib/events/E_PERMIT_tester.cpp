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
#include "../../core/fbtests/fbtester.h"
#include <forte_bool.h>
#include <E_PERMIT.h>
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_PERMIT_tester_gen.cpp"
#endif


class E_PERMIT_tester : public CFBTester{
    DECLARE_FB_TESTER(E_PERMIT_tester);
  public:

    E_PERMIT_tester(CResource* m_poTestResource) :
        CFBTester(m_poTestResource){
      SETUP_INPUTDATA(&m_oIn_PERMIT);
    }
    virtual ~E_PERMIT_tester() {

    }
  private:
    virtual void executeAllTests(){
      evaluateTestResult(testCase_Permit(), "Permit");
      evaluateTestResult(testCase_DontPermit(), "DontPermit");
    }

    /***********************************************************************************/
    bool testCase_Permit(){
      /* prepare inputparameters */
      m_oIn_PERMIT = true;
      /* trigger the inputevent */
      triggerEvent(0);
      return checkForSingleOutputEventOccurence(0);
    }
    bool testCase_DontPermit(){
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

    CIEC_BOOL m_oIn_PERMIT; //DATA INPUT
};

/***********************************************************************************/

DEFINE_FB_TESTER(E_PERMIT_tester, g_nStringIdE_PERMIT)
