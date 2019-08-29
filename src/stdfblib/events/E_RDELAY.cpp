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
#include "E_RDELAY.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_RDELAY_gen.cpp"
#endif
#include "../../arch/timerha.h"

DEFINE_FIRMWARE_FB(E_RDELAY, g_nStringIdE_RDELAY)

E_RDELAY::E_RDELAY(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes):
    CTimedFB( paInstanceNameId, paSrcRes, e_SingleShot){
}

void E_RDELAY::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case cg_nExternalEventID:
      sendOutputEvent(csm_nEOID);
      mActive = false;
      break;
    case csm_nEventSTARTID:
      if(mActive){
        //remove from the list as we want to be added with a new delay
        getTimer().unregisterTimedFB(this);
      }
      setEventChainExecutor(m_poInvokingExecEnv);  // E_RDELAY will execute in the same thread on as from where it has been triggered.
      getTimer().registerTimedFB( &mTimeListEntry, DT());
      mActive = true;
      break;
    default:
      CTimedFB::executeEvent(pa_nEIID);
      break;
  }
}


