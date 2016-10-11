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

#ifndef _E_CTU_TESTER_H_
#define _E_CTU_TESTER_H_

#include "../../core/fbtests/fbtester.h"
#include <forte_uint.h>
#include <forte_bool.h>
#include <E_CTU.h>

class E_CTU_tester : public CFBTester{
  DECLARE_FB_TESTER(E_CTU_tester);

  public:
    explicit E_CTU_tester(CResource *m_poTestResource);
    virtual ~E_CTU_tester(){};
  private:
    virtual void executeAllTests();

    bool testCase_EventCU();
    bool testCase_EventR();
    bool testCase_Mix();

    bool checkCU(TForteUInt16 pa_prevCV);
    bool checkR();

    CIEC_UINT m_oIn_PV; //Preset value
    CIEC_BOOL m_oOut_Q; //CV >= PV
    CIEC_UINT m_oOut_CV; //count value
};

#endif /* _E_CTU_TESTER_H_ */
