/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_TimeOut.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_TimeOut_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(FORTE_E_TimeOut, g_nStringIdE_TimeOut)

const TForteInt16 FORTE_E_TimeOut::scm_anEOWithIndexes[] = { -1 };
const SAdapterInstanceDef FORTE_E_TimeOut::scm_astAdapterInstances[] = { { g_nStringIdATimeOut, g_nStringIdTimeOutSocket, false } };

const SFBInterfaceSpec FORTE_E_TimeOut::scm_stFBInterfaceSpec = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, scm_astAdapterInstances };

void FORTE_E_TimeOut::executeEvent(int pa_nEIID){
  if(cg_nExternalEventID == pa_nEIID){
    m_bActive = false;
    sendAdapterEvent(scm_nTimeOutSocketAdpNum, FORTE_ATimeOut::scm_nEventTimeOutID);
  }
  else if(TimeOutSocket().START() == pa_nEIID){
    if(!m_bActive){
      setEventChainExecutor(m_poInvokingExecEnv);  // delay notification should be execute in the same thread on as from where it has been triggered.
      getTimer().registerTimedFB(&m_stTimeListEntry, TimeOutSocket().DT());
      m_bActive = true;
    }
  }
  else if(TimeOutSocket().STOP() == pa_nEIID){
    if(m_bActive){
      getTimer().unregisterTimedFB(this);
      m_bActive = false;
    }
  }
}

EMGMResponse FORTE_E_TimeOut::changeFBExecutionState(EMGMCommandType pa_unCommand){
  EMGMResponse eRetVal = CFunctionBlock::changeFBExecutionState(pa_unCommand);
  if((e_RDY == eRetVal) && ((cg_nMGM_CMD_Stop == pa_unCommand) || (cg_nMGM_CMD_Kill == pa_unCommand))){
    if(m_bActive){
      getTimer().unregisterTimedFB(this);
      m_bActive = false;
    }
  }
  return eRetVal;
}

#else

DEFINE_FIRMWARE_FB(FORTE_E_TimeOut, g_nStringIdE_TimeOut)

const TForteInt16 FORTE_E_TimeOut::scm_anEOWithIndexes[] = {-1};
const SAdapterInstanceDef FORTE_E_TimeOut::scm_astAdapterInstances[] = {
{g_nStringIdATimeOut, g_nStringIdTimeOutSocket, false }};

const SFBInterfaceSpec FORTE_E_TimeOut::scm_stFBInterfaceSpec = {
  0,  0,  0,  0,
  0,  0,   0, 0,  0,  0, 0,
  0,  0, 0,
  1,scm_astAdapterInstances};


const SCFB_FBInstanceData FORTE_E_TimeOut::scm_astInternalFBs[] = {
  {g_nStringIdDLY, g_nStringIdE_DELAY},
};

const SCFB_FBConnectionData FORTE_E_TimeOut::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdTimeOutSocket, g_nStringIdSTART), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdSTART), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdTimeOutSocket, g_nStringIdSTOP), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdSTOP), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdEO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdTimeOutSocket, g_nStringIdTimeOut), CCompositeFB::scm_nAdapterMarker |0},
};

const SCFB_FBConnectionData FORTE_E_TimeOut::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdTimeOutSocket, g_nStringIdDT), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdDT), 0},
};

const SCFB_FBNData FORTE_E_TimeOut::scm_stFBNData = {
  1, scm_astInternalFBs,
  3, scm_astEventConnections,
  0, 0,
  1, scm_astDataConnections,
  0, 0,
  0, 0
};

#endif
