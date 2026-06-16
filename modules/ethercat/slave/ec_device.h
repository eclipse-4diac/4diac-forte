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

#pragma once

#include "slave.h"
#include "model/ec_model.h"

namespace forte::eclipse4diac::io::ethercat {

  class ECDeviceHandler : public ECSlaveHandler {
    
    public:
      struct Config : ECSlaveHandler::Config {
        uint16_t mAlias;
        uint16_t mPosition;
        uint32_t mVendorId;
        uint32_t mProductCode;
      };

      ECDeviceModel mECDeviceModel;

      void setConfig(struct ECSlaveHandler::Config *paConfig) override;

      ECDeviceHandler(ECBusHandler *paBus, ECSlaveHandler::SlaveType paSlaveType, size_t paSlaveIndex);

    protected:
      struct Config mConfig;
  };
}
