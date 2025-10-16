/*******************************************************************************
 * Copyright (c) 2025 HR Agartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Coding-Assistent Gemini and Franz Höpfinger
 *    - implement Generic GEN_E_MERGE_fbt based on GEN_E_SPLIT
 *******************************************************************************/

#pragma once

#include <vector>
#include "forte/genfb.h"
#include "forte/stringid.h"

namespace forte::iec61499::events {
  /**
   * @brief A generic function block to merge multiple event inputs into one event output.
   *
   * The number of event inputs is determined by the instance name, e.g., an instance
   * named "E_MERGE_5" will have 5 event inputs (EI1, EI2, ..., EI5).
   */
  class GEN_E_MERGE final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_E_MERGE)

    private:
      /** The ID of the single event output EO. */
      static const TEventID scmEventEOID = 0;

      /**
       * @brief Executes when an event is received on any of the EI ports.
       *
       * It forwards the event to the single EO port.
       * @param paEIID The ID of the input event that triggered the execution.
       * @param paECET The event chain execution thread.
       */
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      /** These functions are not needed as this FB has no data inputs/outputs. */
      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;

      /**
       * @brief Creates the interface specification based on the configuration string.
       *
       * It parses the number from the end of the config string (e.g., "5" from "E_MERGE_5")
       * to determine the number of event inputs.
       * @param paConfigString The configuration string (typically the FB instance name).
       * @param paInterfaceSpec The interface specification structure to be filled.
       * @return true on success, false otherwise.
       */
      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      /** A vector to hold the dynamically generated StringIds for the event input names. */
      std::vector<StringId> mEventInputNames;

    public:
      GEN_E_MERGE(StringId paInstanceNameId, CFBContainer &paContainer);

      /** Functions to get data connections, returning nullptr as there are none. */
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
