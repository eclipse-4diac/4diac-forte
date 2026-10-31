/*******************************************************************************
 * Copyright (c) 2024 HR Agrartechnik GmbH
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
 *   Coding-Assistent
 *    - added detailed English comments for consistency
 *******************************************************************************/

#pragma once

#include <vector>
#include "forte/genfb.h"
#include "forte/stringid.h"

namespace forte::iec61499::events {
  /**
   * @brief A generic function block to split a single event input into multiple event outputs.
   *
   * The number of event outputs is determined by the instance name, e.g., an instance
   * named "E_SPLIT_5" will have 5 event outputs (EO1, EO2, ..., EO5).
   */
  class GEN_E_SPLIT final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_E_SPLIT)

    private:
      /** The ID of the single event input EI. */
      static const TEventID scmEventEIID = 0;

      /**
       * @brief Executes when an event is received on the EI port.
       *
       * It forwards the event to all configured EO ports.
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
       * It parses the number from the end of the config string (e.g., "5" from "E_SPLIT_5")
       * to determine the number of event outputs.
       * @param paConfigString The configuration string (typically the FB instance name).
       * @param paInterfaceSpec The interface specification structure to be filled.
       * @return true on success, false otherwise.
       */
      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      /** A vector to hold the dynamically generated StringIds for the event output names. */
      std::vector<StringId> mEventOutputNames;

    public:
      GEN_E_SPLIT(StringId paInstanceNameId, CFBContainer &paContainer);

      /** Functions to get data connections, returning nullptr as there are none. */
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
  };
} // namespace forte::iec61499::events
