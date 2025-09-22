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
#include "forte/iec61499/events/E_RDELAY_fbt.h"

using namespace forte::literals;

#include "forte/timerha.h"

namespace forte::iec61499::events {
  DEFINE_FIRMWARE_FB(FORTE_E_RDELAY, "iec61499::events::E_RDELAY"_STRID)

  FORTE_E_RDELAY::FORTE_E_RDELAY(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CTimedFB(paInstanceNameId, paContainer) {
  }

  void FORTE_E_RDELAY::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case cgExternalEventID:
        sendOutputEvent(csmEOID, paECET);
        mActive = false;
        break;
      case csmEventSTARTID:
        if (mActive) {
          // remove from the list as we want to be added with a new delay
          getTimer().unregisterTimedFB(this);
        }
        setEventChainExecutor(
            paECET); // E_RDELAY will execute in the same thread on as from where it has been triggered.
        getTimer().registerOneShotTimedFB(this, var_DT);
        mActive = true;
        break;
      default: CTimedFB::executeEvent(paEIID, paECET); break;
    }
  }
} // namespace forte::iec61499::events
