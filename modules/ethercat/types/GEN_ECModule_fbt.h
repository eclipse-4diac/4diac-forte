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

#include "ECModule.h"

namespace forte::eclipse4diac::io::ethercat {
  class GEN_ECModule : public FORTE_ECModule {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_ECModule)

    public:
      GEN_ECModule(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_ECModule() override = default;

      template<typename... Args>
      void evt_MAP(Args &&...paArgs) {
        writeInputArguments(std::forward<Args>(paArgs)...);
        receiveInputEvent(scmEventMAPID, nullptr);
        readOutputArguments(std::forward<Args>(paArgs)...);
      }

      template<typename ...Args>
        void operator()(Args &&...paArgs) {
        evt_INIT(std::forward<Args>(paArgs)...);
      }
  };
} // namespace forte::eclipse4diac::io::ethercat

