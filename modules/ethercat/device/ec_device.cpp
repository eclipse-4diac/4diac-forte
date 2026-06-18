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

#include "ec_device.h"

namespace forte::eclipse4diac::io::ethercat {

  ECDeviceHandler::ECDeviceHandler(ECBusHandler *paBus, 
                                   ECBusDeviceHandler::DeviceType paDeviceType, 
                                   size_t paDeviceIndex) : 
      ECBusDeviceHandler(paBus, paDeviceType, paDeviceIndex) {
  }

  void ECDeviceHandler::setConfig(struct ECBusDeviceHandler::Config *paConfig) {
    mConfig = *static_cast<Config *>(paConfig);

    mECDeviceModel.mAlias = mConfig.mAlias;
    mECDeviceModel.mPosition = mConfig.mPosition;
    mECDeviceModel.mVendorId = mConfig.mVendorId;
    mECDeviceModel.mProductCode = mConfig.mProductCode;
  }
}