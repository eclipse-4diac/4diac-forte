/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Kirill Dorofeev
 *      - initial implementation
 *******************************************************************************/

#include "../../core/fbtests/fbtester.h"
#include <forte_bool.h>
#include <E_DEMUX.h>
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_DEMUX_tester_gen.cpp"
#endif


class E_DEMUX_tester : public CFBTester{
    DECLARE_FB_TESTER(E_DEMUX_tester);
  public:

  private:
    E_DEMUX_tester(CResource* m_poTestResource) :
        CFBTester(m_poTestResource){
      SETUP_INPUTDATA(&m_oIn_K);
    }
    virtual ~E_DEMUX_tester() {

    }

    virtual void executeAllTests(){
      evaluateTestResult(testCase_K0(), "K0");
      evaluateTestResult(testCase_K1(), "K1");
      evaluateTestResult(testCase_K2(), "K2");
      evaluateTestResult(testCase_K3(), "K3");
      evaluateTestResult(testCase_K_GT_3(), "K_GT_3");
    }

    /***********************************************************************************/
    bool testCase_K0(){
      /* prepare inputparameters */
      m_oIn_K = 0;
      /* trigger the inputevent */
      triggerEvent(0);
      return checkForSingleOutputEventOccurence(0);
    }
    bool testCase_K1(){
      /* prepare inputparameters */
      m_oIn_K = 1;
      /* trigger the inputevent */
      triggerEvent(0);
      return checkForSingleOutputEventOccurence(1);
    }
    bool testCase_K2(){
      /* prepare inputparameters */
      m_oIn_K = 2;
      /* trigger the inputevent */
      triggerEvent(0);
      return checkForSingleOutputEventOccurence(2);
    }
    bool testCase_K3(){
      /* prepare inputparameters */
      m_oIn_K = 3;
      /* trigger the inputevent */
      triggerEvent(0);
      return checkForSingleOutputEventOccurence(3);
    }
    bool testCase_K_GT_3(){
      /* prepare inputparameters */
      m_oIn_K = 4;
      /* trigger the inputevent */
      triggerEvent(0);
      return eventChainEmpty();
    }

    CIEC_UINT m_oIn_K; //DATA INPUT
};

/***********************************************************************************/

DEFINE_FB_TESTER(E_DEMUX_tester, g_nStringIdE_DEMUX);
