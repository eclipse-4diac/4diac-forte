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
#ifndef _E_PERMIT_TESTER_H_
#define _E_PERMIT_TESTER_H_

#include "../../core/fbtests/fbtester.h"
#include <forte_bool.h>
#include <E_PERMIT.h>



class E_PERMIT_tester : public CFBTester{
    DECLARE_FB_TESTER(E_PERMIT_tester);
  public:

  private:
    E_PERMIT_tester(CResource *m_poTestResource);
    virtual void executeAllTests();

    bool testCase_Permit();
    bool testCase_DontPermit();

    CIEC_BOOL m_oIn_PERMIT; //DATA INPUT
};

/***********************************************************************************/

#endif /* _E_PERMIT_TESTER_H_ */ 
