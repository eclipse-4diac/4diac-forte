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

#include "ECSlaveConfig.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::ethercat {
  StringId CIEC_ECSlaveConfig::getStructTypeNameID() const {
    return "ECSlaveConfig"_STRID;
  }

  const StringId CIEC_ECSlaveConfig::scmElementNames[] = {"Alias"_STRID, "Position"_STRID, "VendorId"_STRID, "ProductCode"_STRID};

  DEFINE_FIRMWARE_DATATYPE(ECSlaveConfig, "ECSlaveConfig"_STRID);

} // namespace forte::eclipse4diac::io::ethercat
