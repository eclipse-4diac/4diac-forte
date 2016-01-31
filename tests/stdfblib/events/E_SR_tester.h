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
#ifndef _E_SR_TESTER_H_
#define _E_SR_TESTER_H_

#include "../../core/fbtests/fbtester.h"
#include <forte_bool.h>
#include <E_SR.h>

/***********************************************************************************/
/***********************************************************************************/

class E_SR_tester  : public CFBTester{
    DECLARE_FB_TESTER(E_SR_tester);

  public:
    E_SR_tester(CResource *m_poTestResource);

  private:
    virtual void executeAllTests();

    bool testCase_EventS();
    bool testCase_EventR();
    bool testCase_Toggle();

    /*\brief Check if the E_SR changed to the given target state
     *
     */
    bool checkStateChange(bool bTargetState);

    CIEC_BOOL m_oOut_Q; //DATA OUTPUT
};

/***********************************************************************************/
/***********************************************************************************/

#endif /* _E_SR_TESTER_H_ */ 
