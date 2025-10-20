/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** Name: GEN_F_SEL_E
 *** Description: Generic event-based selector
 *** Version:
 *** 1.0: 2025-10-16/Coding-Assistent - Google - initial creation
 *************************************************************************/

#pragma once

#include "forte/genfb.h"
#include "forte/datatypes/forte_any_variant.h"

namespace forte::eclipse4diac::utils::selection {

  class GEN_F_SEL_E final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_F_SEL_E)

    protected:
      /**
       * @brief Creates and allocates the memory for the generic data inputs.
       */
      void createGenInputData() override;

    private:
      // The ID for the single output event 'CNF'.
      static const TEventID scmEventCNFID = 0;

      // The core logic of the FB is executed here.
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      // Methods for reading/writing the port data.
      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      /**
       * @brief Creates the FB's interface based on the configuration string.
       *
       * This function parses the required number of ports from the FB name (e.g., the '4' from 'GEN_F_SEL_E_4').
       */
      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      // Storage for the values of the generic data inputs (IN0, IN1, ...).
      std::unique_ptr<CIEC_ANY_VARIANT[]> mGenDIs;

      // Vectors to store the names of the generic inputs.
      std::vector<StringId> mDINames;
      std::vector<StringId> mEINames;

    public:
      GEN_F_SEL_E(StringId paInstanceNameId, CFBContainer &paContainer);

      // The data output port.
      CIEC_ANY_VARIANT var_OUT;

      // The event output object.
      CEventConnection conn_CNF;

      // The data output object.
      COutDataConnection<CIEC_ANY_VARIANT> conn_OUT;

      // Getter methods for accessing the ports, required by FORTE.
      CIEC_ANY *getDI(size_t paIndex) override;
      CIEC_ANY *getDO(size_t paIndex) override;
      CEventConnection *getEOConUnchecked(TPortId paIndex) override;
      CDataConnection *getDOConUnchecked(TPortId paIndex) override;
  };

} // namespace forte::eclipse4diac::utils::selection
