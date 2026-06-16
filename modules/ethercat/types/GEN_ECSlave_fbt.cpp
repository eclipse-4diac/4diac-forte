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

#include "GEN_ECSlave_fbt.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::ethercat {

  DEFINE_GENERIC_FIRMWARE_FB(GEN_ECSlave, "eclipse4diac::io::ethercat::GEN_ECSlave"_STRID)

  GEN_ECSlave::GEN_ECSlave(const StringId paInstanceNameId, CFBContainer &paContainer) :
      FORTE_ECSlave(paInstanceNameId, paContainer) {
  }

} // namespace forte::eclipse4diac::io::ethercat

