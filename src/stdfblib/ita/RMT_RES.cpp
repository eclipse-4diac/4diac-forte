/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Rene Smodic, Thomas Strasser,
 *   Martin Melik Merkumians, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "RMT_RES.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RMT_RES_gen.cpp"
#endif

#include "../../core/ecet.h"

DEFINE_FIRMWARE_FB(RMT_RES, g_nStringIdRMT_RES);

const CStringDictionary::TStringId RMT_RES::scm_aunVarInputNameIds[] = {g_nStringIdMGR_ID};
const CStringDictionary::TStringId RMT_RES::scm_aunDIDataTypeIds[] = {g_nStringIdWSTRING};


const SFBInterfaceSpec RMT_RES::scm_stFBInterfaceSpec = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  scm_aunVarInputNameIds,
  scm_aunDIDataTypeIds,
  0,
  0,
  0,
  0,
  0
  };


RMT_RES::RMT_RES(CStringDictionary::TStringId pa_nInstanceNameId, CResource* pa_poDevice):
       CResource(pa_poDevice, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData){
  addFB(CTypeLib::createFB(g_nStringIdSTART, g_nStringIdE_RESTART, this));
  addFB(CTypeLib::createFB(g_nStringIdMGR_FF, g_nStringIdE_SR, this));
  addFB(CTypeLib::createFB(g_nStringIdMGR, g_nStringIdDEV_MGR, this));

  forte::core::SManagementCMD command;

  command.mFirstParam.pushBack(g_nStringIdSTART);
  command.mFirstParam.pushBack(g_nStringIdCOLD);
  command.mSecondParam.pushBack(g_nStringIdMGR_FF);
  command.mSecondParam.pushBack(g_nStringIdS);
  createConnection(command);

  command.mFirstParam.clear();
  command.mFirstParam.pushBack(g_nStringIdSTART);
  command.mFirstParam.pushBack(g_nStringIdwARM);
  command.mSecondParam.clear();
  command.mSecondParam.pushBack(g_nStringIdMGR_FF);
  command.mSecondParam.pushBack(g_nStringIdS);
  createConnection(command);

  command.mFirstParam.clear();
  command.mFirstParam.pushBack(g_nStringIdSTART);
  command.mFirstParam.pushBack(g_nStringIdSTOP);
  command.mSecondParam.clear();
  command.mSecondParam.pushBack(g_nStringIdMGR_FF);
  command.mSecondParam.pushBack(g_nStringIdR);
  createConnection(command);

  command.mFirstParam.clear();
  command.mFirstParam.pushBack(g_nStringIdMGR_FF);
  command.mFirstParam.pushBack(g_nStringIdEO);
  command.mSecondParam.clear();
  command.mSecondParam.pushBack(g_nStringIdMGR);
  command.mSecondParam.pushBack(g_nStringIdINIT);
  createConnection(command);

  command.mFirstParam.clear();
  command.mFirstParam.pushBack(g_nStringIdMGR_FF);
  command.mFirstParam.pushBack(g_nStringIdQ);
  command.mSecondParam.clear();
  command.mSecondParam.pushBack(g_nStringIdMGR);
  command.mSecondParam.pushBack(g_nStringIdQI);
  createConnection(command);

  command.mFirstParam.clear();
  command.mFirstParam.pushBack(g_nStringIdMGR_ID);
  command.mSecondParam.clear();
  command.mSecondParam.pushBack(g_nStringIdMGR);
  command.mSecondParam.pushBack(g_nStringIdID);
  createConnection(command);
  
  //Perform reset command normally done by the typelib during the creation process
  changeFBExecutionState(cg_nMGM_CMD_Reset);
}

RMT_RES::~RMT_RES(){
}

void RMT_RES::joinResourceThread() const {
  getResourceEventExecution()->joinEventChainExecutionThread();
}
