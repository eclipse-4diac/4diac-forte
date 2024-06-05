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

#include "device.h"

class CTesterDevice : public CDevice {
  public:
    CTesterDevice(const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
        CDevice(paInterfaceSpec, paInstanceNameId){
    }

    void awaitShutdown() override {
      // nothing to be done to join
    }

    CIEC_ANY *getDI(size_t) override {
      return nullptr;
    }

    CDataConnection **getDIConUnchecked(TPortId) override {
      return nullptr;
    }

};

const static SFBInterfaceSpec gscTestDevSpec = {
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

/**Global fixture for providing the resource and device needed for fb testing
 *
 */
class CFBTestDataGlobalFixture{

  public:
    CFBTestDataGlobalFixture();
    ~CFBTestDataGlobalFixture();

    static CResource &getResource(){
      return *smTestRes;
    }

  private:
    static CTesterDevice *smTestDev;
    static CResource *smTestRes;
};



#endif
