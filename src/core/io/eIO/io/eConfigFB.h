/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#pragma once

#include "core/funcbloc.h"
#include "CeConfig.h"
#include "CeSpec.h"

class CeConfigFB : public CFunctionBlock, public CeConfig {
  public:
    CeConfigFB(const CStringDictionary::TStringId paInstanceNameId,
               const SFBInterfaceSpec &paInterfaceSpec,
               forte::core::CFBContainer &paContainer) :
        CFunctionBlock(paContainer, paInterfaceSpec, paInstanceNameId) {
    }

    bool checkEventTriggersConf(CIEC_ANY *paValue) {
      bool result = false;

      // iterating through all event-triggered assigned from this FB
      for (CeSpecBase *eventSpec : mEventGenList) {
        // fill the eventSpec buffer with inputs
        eventSpec->readToBuffer(paValue);
        if (eventSpec->checkCondition()) {
          eventSpec->trigger();
          result = true;
        }
      }
      return result;
    }
};
