/*******************************************************************************
 * Copyright (c) 2018, 2024 fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *   Martin Jobst - add smart pointer for internal FBs
 *******************************************************************************/
#ifndef TESTS_CORE_FBTESTS_FBTESTERGLOBALFIXTURE_H_
#define TESTS_CORE_FBTESTS_FBTESTERGLOBALFIXTURE_H_

#include "forte/device.h"
#include "../../stdfblib/hardware/src/EMB_RES.h"

#include <memory>

namespace forte::test {
  class CTesterDevice : public CDevice {
    public:
      CTesterDevice(const forte::StringId paInstanceNameId = {});

      void awaitShutdown() override {
        // nothing to be done to join
      }

      CIEC_ANY *getDI(size_t) override {
        return nullptr;
      }

      CDataConnection **getDIConUnchecked(TPortId) override {
        return nullptr;
      }

      CResource &getTestResource() {
        return *mResource;
      }

    private:
      forte::CInternalFB<forte::iec61499::hardware::EMB_RES> mResource;

      constexpr static SFBInterfaceSpec scTestDevSpec = {};
  };

  /**Global fixture for providing the resource and device needed for fb testing
   *
   */
  class CFBTestDataGlobalFixture {

    public:
      CFBTestDataGlobalFixture();
      ~CFBTestDataGlobalFixture();

      static CResource &getResource() {
        return smTestDev->getTestResource();
      }

    private:
      static std::unique_ptr<CTesterDevice> smTestDev;
  };
} // namespace forte::test
#endif
