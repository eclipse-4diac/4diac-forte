/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "EMB_RMT_DEV.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "EMB_RMT_DEV_gen.cpp"
#endif
#include <stringdict.h>

const SFBInterfaceSpec EMB_RMT_DEV::scm_stFBInterfaceSpec = { 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0,
    0};

EMB_RMT_DEV::EMB_RMT_DEV(const char * pa_acId) :
  CDevice(&scm_stFBInterfaceSpec, CStringDictionary::scm_nInvalidStringId, 0, 0),
      MGR(g_nStringIdMGR, this, pa_acId){
      
  //Perform reset command normally done by the typelib during the creation process
  changeFBExecutionState(cg_nMGM_CMD_Reset);
}

EMB_RMT_DEV::~EMB_RMT_DEV(){
}

int EMB_RMT_DEV::startDevice(void){
  CDevice::startDevice();
  MGR.changeFBExecutionState(cg_nMGM_CMD_Start);
  return 0;
}

EMGMResponse EMB_RMT_DEV::changeFBExecutionState(EMGMCommandType pa_unCommand){
  EMGMResponse eRetVal = CDevice::changeFBExecutionState(pa_unCommand);
  if((e_RDY == eRetVal) && (cg_nMGM_CMD_Kill == pa_unCommand)){
    MGR.changeFBExecutionState(cg_nMGM_CMD_Kill);
  }
  return eRetVal;
}
