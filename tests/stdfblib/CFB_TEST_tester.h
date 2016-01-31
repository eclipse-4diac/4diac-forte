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
#ifndef _CFB_TEST_TESTER_H_
#define _CFB_TEST_TESTER_H_

#include "../core/fbtests/fbtester.h"
#include <forte_bool.h>
#include "CFB_TEST.h"



class CFB_TEST_tester : public CFBTester{
    DECLARE_FB_TESTER(CFB_TEST_tester);
  public:

  private:
    CFB_TEST_tester(CResource *m_poTestResource);
    virtual void executeAllTests();

    bool inhibitTest();
    bool setTest();
    bool resetTest();
    bool toggleTest();

    bool checkBothOutputEvents();

    CIEC_BOOL m_oIn_QI;
    CIEC_BOOL m_oOut_SR;
};

/***********************************************************************************/

#endif /* _CFB_TEST_TESTER_H_ */
