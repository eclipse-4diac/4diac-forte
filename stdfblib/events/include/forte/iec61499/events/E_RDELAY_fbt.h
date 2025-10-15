/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
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

#include "forte/iec61499/events/timedfb.h"

/*! \brief Implementation of the E_RDELAY FB.
 */
namespace forte::iec61499::events {
  class FORTE_E_RDELAY : public CTimedFB {
      DECLARE_FIRMWARE_FB(FORTE_E_RDELAY)
    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    public:
      FORTE_E_RDELAY(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~FORTE_E_RDELAY() override = default;
  };

} // namespace forte::iec61499::events
