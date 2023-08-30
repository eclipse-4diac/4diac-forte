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

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_E_TimeOut, g_nStringIdE_TimeOut)

const SAdapterInstanceDef FORTE_E_TimeOut::scmAdapterInstances[] = { { g_nStringIdATimeOut, g_nStringIdTimeOutSocket, false } };

const SFBInterfaceSpec FORTE_E_TimeOut::scmFBInterfaceSpec = {
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  1, scmAdapterInstances
};

void FORTE_E_TimeOut::executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET){
  if(cgExternalEventID == paEIID){
    mActive = false;
    sendAdapterEvent(scmTimeOutSocketAdpNum, FORTE_ATimeOut::scmEventTimeOutID, getEventChainExecutor());
  }
  else if(var_TimeOutSocket().evt_START() == paEIID){
    if(!mActive){
      setEventChainExecutor(paECET);  // delay notification should be execute in the same thread on as from where it has been triggered.
      getTimer().registerTimedFB(&mTimeListEntry, var_TimeOutSocket().var_DT());
      mActive = true;
    }
  }
  else if(var_TimeOutSocket().evt_STOP() == paEIID){
    if(mActive){
      getTimer().unregisterTimedFB(this);
      mActive = false;
    }
  }
}

void FORTE_E_TimeOut::readInputData(TEventID) {
}

void FORTE_E_TimeOut::writeOutputData(TEventID) {
}

EMGMResponse FORTE_E_TimeOut::changeFBExecutionState(EMGMCommandType paCommand){
  EMGMResponse eRetVal = CFunctionBlock::changeFBExecutionState(paCommand);
  if((EMGMResponse::Ready == eRetVal) && ((EMGMCommandType::Stop == paCommand) || (EMGMCommandType::Kill == paCommand))){
    if(mActive){
      getTimer().unregisterTimedFB(this);
      mActive = false;
    }
  }
  return eRetVal;
}
