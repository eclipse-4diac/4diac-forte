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
#include "forte/iec61499/events/E_CYCLE_fbt.h"

using namespace forte::literals;

namespace forte::iec61499::events {
  DEFINE_FIRMWARE_FB(FORTE_E_CYCLE, "iec61499::events::E_CYCLE"_STRID)

  void FORTE_E_CYCLE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (paEIID == csmEventSTARTID) {
      if (!mActive) {
        setEventChainExecutor(paECET);
        getTimer().registerPeriodicTimedFB(this, var_DT);
        mActive = true;
      }
    } else {
      CTimedFB::executeEvent(paEIID, paECET);
    }
  }
} // namespace forte::iec61499::events
