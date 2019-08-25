/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_TimeOut.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_TimeOut_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_E_TimeOut, g_nStringIdE_TimeOut)

const TForteInt16 FORTE_E_TimeOut::scm_anEOWithIndexes[] = { -1 };
const SAdapterInstanceDef FORTE_E_TimeOut::scm_astAdapterInstances[] = { { g_nStringIdATimeOut, g_nStringIdTimeOutSocket, false } };

const SFBInterfaceSpec FORTE_E_TimeOut::scm_stFBInterfaceSpec = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, scm_astAdapterInstances };

void FORTE_E_TimeOut::executeEvent(int pa_nEIID){
  if(cg_nExternalEventID == pa_nEIID){
    mActive = false;
    sendAdapterEvent(scm_nTimeOutSocketAdpNum, FORTE_ATimeOut::scm_nEventTimeOutID);
  }
  else if(TimeOutSocket().START() == pa_nEIID){
    if(!mActive){
      setEventChainExecutor(m_poInvokingExecEnv);  // delay notification should be execute in the same thread on as from where it has been triggered.
      getTimer().registerTimedFB(&mTimeListEntry, TimeOutSocket().DT());
      mActive = true;
    }
  }
  else if(TimeOutSocket().STOP() == pa_nEIID){
    if(mActive){
      getTimer().unregisterTimedFB(this);
      mActive = false;
    }
  }
}

EMGMResponse FORTE_E_TimeOut::changeFBExecutionState(EMGMCommandType pa_unCommand){
  EMGMResponse eRetVal = CFunctionBlock::changeFBExecutionState(pa_unCommand);
  if((e_RDY == eRetVal) && ((cg_nMGM_CMD_Stop == pa_unCommand) || (cg_nMGM_CMD_Kill == pa_unCommand))){
    if(mActive){
      getTimer().unregisterTimedFB(this);
      mActive = false;
    }
  }
  return eRetVal;
}
