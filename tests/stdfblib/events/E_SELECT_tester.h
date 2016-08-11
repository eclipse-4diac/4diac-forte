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
#ifndef _E_SELECT_TESTER_H_
#define _E_SELECT_TESTER_H_

#include "../../core/fbtests/fbtester.h"
#include <forte_bool.h>
#include <E_SELECT.h>



class E_SELECT_tester : public CFBTester{
    DECLARE_FB_TESTER(E_SELECT_tester);
  public:

  private:
    explicit E_SELECT_tester(CResource *m_poTestResource);
    virtual void executeAllTests();

    bool testCase_SelectEI0();
    bool testCase_SelectEI1();
    bool testCase_MultipleSelectEI0();
    bool testCase_MultipleSelectEI1();
    bool testCase_Alternate();

    CIEC_BOOL m_oIn_G; //DATA INPUT
};

/***********************************************************************************/

#endif /* _E_SELECT_TESTER_H_ */
