/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Kirill Dorofeev
 *      - initial implementation
 *******************************************************************************/
#ifndef _E_DEMUX_TESTER_H_
#define _E_DEMUX_TESTER_H_

#include "../../core/fbtests/fbtester.h"
#include <forte_bool.h>
#include <E_DEMUX.h>



class E_DEMUX_tester : public CFBTester{
    DECLARE_FB_TESTER(E_DEMUX_tester);
  public:

  private:
    explicit E_DEMUX_tester(CResource *m_poTestResource);
    virtual void executeAllTests();

    bool testCase_K0();
    bool testCase_K1();
    bool testCase_K2();
    bool testCase_K3();
    bool testCase_K_GT_3();

    CIEC_UINT m_oIn_K; //DATA INPUT
};

/***********************************************************************************/

#endif /* _E_DEMUX_TESTER_H_ */
