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
#include "E_RDELAY_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_RDELAY_fbt_gen.cpp"
#endif
#include "../../arch/timerha.h"

DEFINE_FIRMWARE_FB(FORTE_E_RDELAY, g_nStringIdE_RDELAY)

FORTE_E_RDELAY::FORTE_E_RDELAY(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer):
    CTimedFB(paInstanceNameId, paContainer){
}

void FORTE_E_RDELAY::executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET){
  switch(paEIID){
    case cgExternalEventID:
      sendOutputEvent(csmEOID, paECET);
      mActive = false;
      break;
    case csmEventSTARTID:
      if(mActive){
        //remove from the list as we want to be added with a new delay
        getTimer().unregisterTimedFB(this);
      }
      setEventChainExecutor(paECET);  // E_RDELAY will execute in the same thread on as from where it has been triggered.
      getTimer().registerOneShotTimedFB(this, var_DT);
      mActive = true;
      break;
    default:
      CTimedFB::executeEvent(paEIID, paECET);
      break;
  }
}


