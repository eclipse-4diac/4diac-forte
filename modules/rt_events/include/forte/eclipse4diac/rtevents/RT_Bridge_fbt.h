/*******************************************************************************
 * Copyright (c) 2023 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_variant.h"
#include "forte/genfb.h"
#include "forte/arch/forte_sync.h"

namespace forte::eclipse4diac::rtevents {
  class FORTE_GEN_RT_Bridge final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(FORTE_GEN_RT_Bridge)

    public:
      FORTE_GEN_RT_Bridge(StringId paInstanceNameId, CFBContainer &paContainer);
      ~FORTE_GEN_RT_Bridge() override = default;

      CEventConnection conn_RDO;

    protected:
      void createGenInputData() override;
      void createGenOutputData() override;

      size_t getGenEOOffset() override {
        return 1;
      }

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;

    private:
      static const TEventID scmEventRDID = 0;
      static const TEventID scmEventWRID = 1;

      static const TEventID scmEventRDOID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      arch::CSyncObject mSyncObject;

      std::unique_ptr<CIEC_ANY_VARIANT[]> mGenDIs;
      std::unique_ptr<CIEC_ANY_VARIANT[]> mGenDOs;

      std::vector<StringId> mDINames;
      std::vector<StringId> mDONames;
  };
} // namespace forte::eclipse4diac::rtevents
