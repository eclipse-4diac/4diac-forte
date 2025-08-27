/*******************************************************************************
 * Copyright (c) 2005, 2023 ACIN, Profactor GmbH, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
 *   Alois Zoitl  - Reworked to new timer handler interface
 *******************************************************************************/
#include "E_DELAY_fbt.h"

using namespace forte::core::literals;

#include "core/timerha.h"

DEFINE_FIRMWARE_FB(FORTE_E_DELAY, "E_DELAY"_STRID)

FORTE_E_DELAY::FORTE_E_DELAY(const forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CTimedFB(paInstanceNameId, paContainer) {
}

void FORTE_E_DELAY::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case cgExternalEventID:
      sendOutputEvent(csmEOID, paECET);
      mActive = false;
      break;
    case csmEventSTARTID:
      if (!mActive) {
        setEventChainExecutor(
            paECET); // E_DELAY will execute in the same thread on as from where it has been triggered.
        getTimer().registerOneShotTimedFB(this, var_DT);
        mActive = true;
      }
      break;
    default: CTimedFB::executeEvent(paEIID, paECET); break;
  }
}
