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
#include "E_CYCLE_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CYCLE_fbt_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_E_CYCLE, g_nStringIdE_CYCLE)

void FORTE_E_CYCLE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if(paEIID == csmEventSTARTID) {
    if(!mActive){
      setEventChainExecutor(paECET);
      getTimer().registerPeriodicTimedFB(this, var_DT);
      mActive = true;
    }
  } else {
    CTimedFB::executeEvent(paEIID, paECET);
  }
}
