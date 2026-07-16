/*******************************************************************************
 * Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Zijun Tang - initial API and implementation
 *******************************************************************************/

#include "bus_device_handler.h"
#include "ec_device.h"
#include <cstdint>

namespace forte::eclipse4diac::io::ethercat {

  class ECModuleHandler : public ECBusDeviceHandler {
    
    public:
      struct Config : ECBusDeviceHandler::Config {
        uint32_t mModuleIdent;
        uint16_t mSlot;
      };

      void setConfig(struct ECBusDeviceHandler::Config *paConfig) override;

      ECModuleHandler(ECBusHandler *paBus, size_t paDeviceIndex);

      uint32_t moduleIdent() const {
        return mConfig.mModuleIdent;
      }

      uint16_t slot() const {
        return mConfig.mSlot;
      }

    protected:
      ECModuleHandler *mDeviceHandler;
      struct Config mConfig;
  };
}