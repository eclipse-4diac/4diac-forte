/*******************************************************************************
 * Copyright (c) 2024 HR Agartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger
 *     - implement Generic GEN_E_SPLIT_fbt
 *******************************************************************************/

#pragma once

#include <vector>
#include "forte/genfb.h"
#include "forte/stringid.h"

namespace forte::iec61499::events {
  class GEN_E_SPLIT final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_E_SPLIT)

    private:
      static const TEventID scmEventEIID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      std::vector<StringId> mEventOutputNames;

    public:
      GEN_E_SPLIT(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
