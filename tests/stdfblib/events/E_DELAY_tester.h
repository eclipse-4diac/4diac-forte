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
#ifndef _E_DELAY_TESTER_H_
#define _E_DELAY_TESTER_H_

#include "../../core/fbtests/fbtester.h"
#include <forte_time.h>
#include <E_DELAY.h>

/***********************************************************************************/
/***********************************************************************************/

class E_DELAY_tester  : public CFBTester{
    DECLARE_FB_TESTER(E_DELAY_tester);

  public:
    explicit E_DELAY_tester(CResource *m_poTestResource);

  private:
    virtual void executeAllTests();

    bool testCase_NormalDelay();
    bool testCase_AbortedDelay();
    bool testCase_MultipleStarts();


    CIEC_TIME mDT; //DT data input
};

/***********************************************************************************/
/***********************************************************************************/

#endif /* _E_DELAY_TESTER_H_ */
