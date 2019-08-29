/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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
