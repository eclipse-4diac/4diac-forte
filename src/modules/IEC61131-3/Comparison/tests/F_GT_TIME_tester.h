/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _E_SR_TESTER_H_
#define _E_SR_TESTER_H_

#include "fbtester.h"
#include "../F_GT.h"

/***********************************************************************************/
/***********************************************************************************/

class F_GT_TIME_tester  : public CFBTester{
    DECLARE_FB_TESTER(F_GT_TIME_tester);

  public:
    F_GT_TIME_tester(CResource *mTestResource);

  private:
    virtual void executeAllTests();

    bool testCase_GTual();
    bool testCase_Larger();
    bool testCase_Smaller();

    FORTE_F_GT mTestee;

    CFBTestEOConn mOut_EO_Conn;

    CIEC_TIME mIN1;
    CFBTestDIConn mIn_IN1_Conn;
    CIEC_TIME mIN2;
    CFBTestDIConn mIn_IN2_Conn;
    CIEC_BOOL mOut_OUT; //DATA OUTPUT
    CFBTestDOConn mOut_OUT_Conn; //DATA OUTPUT

};

/***********************************************************************************/
/***********************************************************************************/

#endif /* _E_SR_TESTER_H_ */ 
