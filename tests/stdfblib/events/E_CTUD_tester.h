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

#ifndef _E_CTUD_TESTER_H_
#define _E_CTUD_TESTER_H_

#include "../../core/fbtests/fbtester.h"
#include <forte_uint.h>
#include <forte_bool.h>
#include <E_CTUD.h>

class E_CTUD_tester : public CFBTester{
  DECLARE_FB_TESTER(E_CTUD_tester);

  public:
    explicit E_CTUD_tester(CResource *m_poTestResource);
    virtual ~E_CTUD_tester(){};
  private:
    virtual void executeAllTests();

    bool testCase_EventCU();
    bool testCase_EventCD();
    bool testCase_EventR();
    bool testCase_EventLD();
    bool testCase_Mix();

    bool checkCU(TForteUInt16 pa_prevCV);
    bool checkCD(TForteUInt16 pa_prevCV);
    bool checkR();
    bool checkLD(TForteUInt16 pa_usedPV);
    bool checkBooleans();


    CIEC_UINT m_oIn_PV; //Preset value
    CIEC_BOOL m_oOut_QU; //CV >= PV
    CIEC_BOOL m_oOut_QD; //CV <= 0
    CIEC_UINT m_oOut_CV; //count value
};

#endif /* _E_CTUD_TESTER_H_ */
