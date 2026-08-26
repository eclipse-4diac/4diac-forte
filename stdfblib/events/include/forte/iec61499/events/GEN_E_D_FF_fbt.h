/*******************************************************************************
 * Copyright (c) 2026 HR Agartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger
 *     - implement Generic GEN_E_D_FF_fbt
 *******************************************************************************/

#pragma once

#include <memory>
#include <vector>
#include "forte/genfb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/stringid.h"

namespace forte::iec61499::events {
  /**
   * @brief A generic function block for a data (D) latch over multiple parallel BOOL channels.
   *
   * The number of channels is determined by the instance name, e.g., an instance
   * named "E_D_FF_3" will have 3 data inputs (D1, D2, D3) and 3 data outputs (Q1, Q2, Q3).
   * On CLK, all Qi are latched to the corresponding Di, and EO is fired only if at
   * least one Di differed from the previously latched Qi.
   */
  class GEN_E_D_FF final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_E_D_FF)

    protected:
      /** EO is the single fixed event output; it is not part of the generic set. */
      size_t getGenEOOffset() override {
        return 1;
      }

      CEventConnection *getEOConUnchecked(TPortId) override;

      void createGenInputData() override;
      void createGenOutputData() override;

      CIEC_ANY *getDI(size_t paIndex) override;
      CIEC_ANY *getDO(size_t paIndex) override;

    private:
      static const TEventID scmEventCLKID = 0;
      static const TEventID scmEventEOID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      /** Dynamically generated port names for D1..Dn / Q1..Qn. */
      std::vector<StringId> mDINames;
      std::vector<StringId> mDONames;

      /** Latched data values D1..Dn / Q1..Qn. */
      std::unique_ptr<CIEC_BOOL[]> mGenDs;
      std::unique_ptr<CIEC_BOOL[]> mGenQs;

    public:
      GEN_E_D_FF(StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_E_D_FF() override = default;

      CEventConnection conn_EO;
  };
} // namespace forte::iec61499::events
