/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Rene Smodic, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "RMT_DEV.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RMT_DEV_gen.cpp"
#endif
#include <stringdict.h>

const CStringDictionary::TStringId RMT_DEV::scm_aunDINameIds[] = { g_nStringIdMGR_ID };
const CStringDictionary::TStringId RMT_DEV::scm_aunDIDataTypeIds[] = {g_nStringIdWSTRING};

const SFBInterfaceSpec RMT_DEV::scm_stFBInterfaceSpec = { 0, 0, 0, 0, 0, 0, 0,
    0, 1, scm_aunDINameIds, scm_aunDIDataTypeIds, 0, 0, 0,
    0,
    0};

RMT_DEV::RMT_DEV() :
  CDevice(&scm_stFBInterfaceSpec, CStringDictionary::scm_nInvalidStringId, m_anFBConnData, m_anFBVarsData),
      MGR(g_nStringIdMGR, this){

  MGR_ID().fromString("localhost:61499");

  //we nee to manually crate this interface2internal connection as the MGR is not managed by device
  m_oDConnMGR_ID.setSource(this, 0);
  m_oDConnMGR_ID.connect(&MGR, g_nStringIdMGR_ID);
  
  //Perform reset command normally done by the typelib during the creation process
  changeFBExecutionState(cg_nMGM_CMD_Reset);
}

RMT_DEV::~RMT_DEV(){
}

int RMT_DEV::startDevice(void){
  CDevice::startDevice();
  MGR.changeFBExecutionState(cg_nMGM_CMD_Start);
  return 0;
}

EMGMResponse RMT_DEV::changeFBExecutionState(EMGMCommandType pa_unCommand){
  EMGMResponse eRetVal = CDevice::changeFBExecutionState(pa_unCommand);
  if((e_RDY == eRetVal) && (cg_nMGM_CMD_Kill == pa_unCommand)){
    MGR.changeFBExecutionState(cg_nMGM_CMD_Kill);
  }
  return eRetVal;
}

void RMT_DEV::setMGR_ID(const char * const pa_acConn){
  MGR_ID().fromString(pa_acConn);
}
