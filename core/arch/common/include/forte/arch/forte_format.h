/*******************************************************************************
 * Copyright (c) 2026 Insolsoft
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Anton Gusev  - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#ifdef FORTE_CUSTOM_FORMAT
#include "forte/arch/forte_architecture_format.h"
#else

#include <format>

namespace forte::arch {
  template<typename... Args>
  std::string format(const std::format_string<Args...> fmt, Args &&...args) {
    return std::format(fmt, std::forward<Args>(args)...);
  }

  template<typename It, typename... Args>
  void format_to(It it, const std::format_string<Args...> fmt, Args &&...args) {
    std::format_to(it, fmt, std::forward<Args>(args)...);
  }
} // namespace forte::arch

#endif
