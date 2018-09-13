/*******************************************************************************
 * Copyright (c) 2018, TU Wien/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Melik-Merkumians - initial tests
 *******************************************************************************/

#ifndef TESTS_MODULES_IEC61131_3_F_DIV_TESTER_H_
#define TESTS_MODULES_IEC61131_3_F_DIV_TESTER_H_

#include "../../core/fbtests/fbtester.h"
#include "../../modules/IEC61131-3/Arithmetic/F_DIV.h"

class F_DIV_tester : public CFBTester{
    DECLARE_FB_TESTER(F_DIV_tester);
  public:

  private:
    explicit F_DIV_tester(CResource *mTestResource);
    virtual void executeAllTests();

    bool testCase_validDivision();
    bool testCase_divisionByZero();

    CIEC_INT mIn1_DIV; //DATA INPUT
    CIEC_INT mIn2_DIV; //DATA INPUT

    CIEC_INT mOut_DIV;
};


#endif /* TESTS_MODULES_IEC61131_3_F_DIV_TESTER_H_ */
