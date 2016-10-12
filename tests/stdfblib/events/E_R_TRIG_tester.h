/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_R_TRIG_TESTER_H_
#define _E_R_TRIG_TESTER_H_

#include "../../core/fbtests/fbtester.h"
#include <E_R_TRIG.h>

/***********************************************************************************/
/***********************************************************************************/

class E_R_TRIG_tester  : public CFBTester{
    DECLARE_FB_TESTER(E_R_TRIG_tester);

  public:
    explicit E_R_TRIG_tester(CResource *m_poTestResource);

  private:
    virtual void executeAllTests();

    bool testCase_RaisingEdge();
    bool testCase_FallingEdge();
    bool testCase_StableHigh();
    bool testCase_StableLow();

    CIEC_BOOL m_oIn_QI; //DATA INPUT
};

/***********************************************************************************/
/***********************************************************************************/

#endif /* _E_SR_TESTER_H_ */ 
