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

#include "bus_device_handler.h"
#include "model/ec_model.h"

namespace forte::eclipse4diac::io::ethercat {

  class ECDeviceHandler : public ECBusDeviceHandler {
    
    public:
      struct Config : ECBusDeviceHandler::Config {
        uint16_t mAlias;
        uint16_t mPosition;
        uint32_t mVendorId;
        uint32_t mProductCode;
      };

      ECDeviceModel mECDeviceModel;

      void setConfig(struct ECBusDeviceHandler::Config *paConfig) override;

      ECDeviceHandler(ECBusHandler *paBus, ECBusDeviceHandler::DeviceType paDeviceType, size_t paDeviceIndex);

    protected:
      struct Config mConfig;
  };
}
