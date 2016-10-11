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

#ifndef _E_CTD_TESTER_H_
#define _E_CTD_TESTER_H_

#include "../../core/fbtests/fbtester.h"
#include <forte_uint.h>
#include <forte_bool.h>
#include <E_CTD.h>

class E_CTD_tester : public CFBTester{
  DECLARE_FB_TESTER(E_CTD_tester);

  public:
  	explicit E_CTD_tester(CResource *m_poTestResource);
    virtual ~E_CTD_tester(){};
  private:
    virtual void executeAllTests();

    bool testCase_EventCD();
    bool testCase_EventLD();
    bool testCase_Mix();

    bool checkCD(TForteUInt16 pa_prevCV);
    bool checkLD(TForteUInt16 pa_usedPV);

    CIEC_UINT m_oIn_PV; //Preset value
    CIEC_BOOL m_oOut_Q; //CV < 0
    CIEC_UINT m_oOut_CV; //count value
};

#endif /* _E_CTD_TESTER_H_ */
