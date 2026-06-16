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

#include "ECSlave.h"

namespace forte::eclipse4diac::io::ethercat {

  class FORTE_ECCoupler : public FORTE_ECSlave {
      DECLARE_FIRMWARE_FB(FORTE_ECCoupler)

    public:
      FORTE_ECCoupler(forte::StringId paInstanceNameId, CFBContainer &paContainer);
      ~FORTE_ECCoupler() override = default;

    protected:
      bool createSlaveHandler() override;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      forte::IPlugPin *getPlugPinUnchecked(size_t paIndex) override;
      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      void forwardInitConfirmation(CEventChainExecutionThread *const paECET);

      forte::CPlugPin<FORTE_ECBusAdapter_Plug> var_ModuleAdapterOut;

      FORTE_ECBusAdapter &ModuleAdapterOut() {
        return (*static_cast<FORTE_ECBusAdapter *>(getPlugPinUnchecked(1)->getAdapterBlock()));
      }
  };

} // namespace forte::eclipse4diac::io::ethercat

