/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef TESTS_CORE_FBTESTS_FBTESTERGLOBALFIXTURE_H_
#define TESTS_CORE_FBTESTS_FBTESTERGLOBALFIXTURE_H_

#include <device.h>

/**Global fixture for provding the resource and device needed for fb testing
 *
 */
class CFBTestDataGlobalFixture{

  public:
    CFBTestDataGlobalFixture();
    ~CFBTestDataGlobalFixture();

    static CResource *getResource(){
      return smTestRes;
    }

  private:
    static CDevice *smTestDev;
    static CResource *smTestRes;
};



#endif
