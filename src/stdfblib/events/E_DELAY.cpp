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
#include "E_DELAY.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_DELAY_gen.cpp"
#endif
#include "../../arch/timerha.h"

DEFINE_FIRMWARE_FB(E_DELAY, g_nStringIdE_DELAY)

E_DELAY::E_DELAY(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer):
         CTimedFB( paInstanceNameId, paContainer){
}

void E_DELAY::executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET){
  switch(paEIID){
    case cgExternalEventID:
      sendOutputEvent(csmEOID, paECET);
      mActive = false;
      break;
    case csmEventSTARTID:
      if(!mActive){
        setEventChainExecutor(paECET); // E_DELAY will execute in the same thread on as from where it has been triggered.
        getTimer().registerOneShotTimedFB(this, var_DT);
        mActive = true;
      }
      break;
    default:
      CTimedFB::executeEvent(paEIID, paECET);
      break;
  }
}


