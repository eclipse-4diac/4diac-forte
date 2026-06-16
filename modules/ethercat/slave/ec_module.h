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
 *   Sichuan Qunyuan Technology Co., Ltd. - initial API and implementation
 *******************************************************************************/

#include "slave.h"
#include "ec_device.h"
#include <cstdint>

namespace forte::eclipse4diac::io::ethercat {

  class ECModuleHandler : public ECSlaveHandler {
    
    public:
      struct Config : ECSlaveHandler::Config {
        uint32_t mModuleIdent;
        uint16_t mSlot;
      };

      void setConfig(struct ECSlaveHandler::Config *paConfig) override;

      ECModuleHandler(ECBusHandler *paBus, size_t paSlaveIndex);

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