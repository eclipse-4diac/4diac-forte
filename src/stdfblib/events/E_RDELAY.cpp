/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_RDELAY.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_RDELAY_gen.cpp"
#endif
#include "../../arch/timerha.h"

DEFINE_FIRMWARE_FB(E_RDELAY, g_nStringIdE_RDELAY)

E_RDELAY::E_RDELAY(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes):
         CTimedFB( pa_nInstanceNameId, pa_poSrcRes){
  m_stTimeListEntry.m_eType = e_SingleShot;
}

void E_RDELAY::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case cg_nExternalEventID:
      sendOutputEvent(csm_nEOID);
      m_bActive = false;
      break;
    case csm_nEventSTARTID:
      if(m_bActive){
        //remove from the list as we want to be added with a new delay
        CTimerHandler::sm_poFORTETimer->unregisterTimedFB(this);
      }
      setEventChainExecutor(m_poInvokingExecEnv);  // E_RDELAY will execute in the same thread on as from where it has been triggered.
      CTimerHandler::sm_poFORTETimer->registerTimedFB( &m_stTimeListEntry, DT());
      m_bActive = true;
      break;
    default:
      CTimedFB::executeEvent(pa_nEIID);
      break;
  }
}


