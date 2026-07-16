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

#pragma once

#include "forte/util/string_utils.h"

#include <cstdint>
#include <cstring>

namespace forte::eclipse4diac::io::ethercat {

  inline uint32_t parseEcNumber(const char *paText) {
    if (nullptr == paText) {
      return 0;
    }
    if (strlen(paText) > 2 && paText[0] == '#' && (paText[1] == 'x' || paText[1] == 'X')) {
      return static_cast<uint32_t>(util::strtoul(paText + 2, nullptr, 16));
    }
    if (strlen(paText) > 2 && paText[0] == '0' && (paText[1] == 'x' || paText[1] == 'X')) {
      return static_cast<uint32_t>(util::strtoul(paText + 2, nullptr, 16));
    }
    return static_cast<uint32_t>(util::strtoul(paText, nullptr, 10));
  }

} // namespace forte::eclipse4diac::io::ethercat
