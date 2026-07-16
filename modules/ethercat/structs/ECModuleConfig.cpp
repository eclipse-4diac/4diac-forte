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

#include "ECModuleConfig.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::ethercat {
  StringId CIEC_ECModuleConfig::getStructTypeNameID() const {
    return "ECModuleConfig"_STRID;
  }

  const StringId CIEC_ECModuleConfig::scmElementNames[] = {"ModuleIdent"_STRID, "Slot"_STRID};

  DEFINE_FIRMWARE_DATATYPE(ECModuleConfig, "ECModuleConfig"_STRID);
} // namespace forte::eclipse4diac::io::ethercat

