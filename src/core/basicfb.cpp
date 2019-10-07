/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include <string.h>
#include "basicfb.h"

CBasicFB::CBasicFB(CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    const SInternalVarsInformation *pa_pstVarInternals,
    TForteByte *pa_acFBConnData, TForteByte *pa_acBasicFBVarsData) :
        CFunctionBlock(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData, pa_acBasicFBVarsData),
        m_nECCState(0),
        cm_pstVarInternals(pa_pstVarInternals){

  m_aoInternals = 0;
  if((0 != cm_pstVarInternals) && (cm_pstVarInternals->m_nNumIntVars) && (0 != pa_acBasicFBVarsData)) {
    pa_acBasicFBVarsData += genFBVarsDataSize(m_pstInterfaceSpec->m_nNumDIs, m_pstInterfaceSpec->m_nNumDOs, m_pstInterfaceSpec->m_nNumAdapters);

    m_aoInternals = reinterpret_cast<CIEC_ANY *>(pa_acBasicFBVarsData);

    const CStringDictionary::TStringId *pnDataIds = cm_pstVarInternals->m_aunIntVarsDataTypeNames;
    for(int i = 0; i < cm_pstVarInternals->m_nNumIntVars; ++i) {
      createDataPoint(&pnDataIds, pa_acBasicFBVarsData);
      pa_acBasicFBVarsData += sizeof(CIEC_ANY);
    }
  }
}

CBasicFB::~CBasicFB(){
  if(0 != m_aoInternals){
    for(int i = 0; i < cm_pstVarInternals->m_nNumIntVars; ++i){
      getVarInternal(i)->~CIEC_ANY();
    }
  }
}

CIEC_ANY *CBasicFB::getVar(CStringDictionary::TStringId *paNameList,
    unsigned int paNameListSize){
  CIEC_ANY *poRetVal = CFunctionBlock::getVar(paNameList, paNameListSize);
  if((0 == poRetVal) && (1 == paNameListSize)){
    poRetVal = getInternalVar(*paNameList);
    if(0 == poRetVal && !strcmp("$ECC", CStringDictionary::getInstance().get(*paNameList))) { //TODO consider if this can also be an string ID in a differnt way
        poRetVal = &m_nECCState;
    }
  }
  return poRetVal;
}

EMGMResponse CBasicFB::changeFBExecutionState(EMGMCommandType pa_unCommand){
  EMGMResponse nRetVal = CFunctionBlock::changeFBExecutionState(pa_unCommand);
  if((e_RDY == nRetVal) && (cg_nMGM_CMD_Reset == pa_unCommand)){
    m_nECCState = 0;
  }
  return nRetVal;
}

CIEC_ANY *CBasicFB::getInternalVar(CStringDictionary::TStringId pa_nInternalName){
  CIEC_ANY *retVal = 0;
  if(0 != cm_pstVarInternals){
    TPortId unVarId =
        getPortId(pa_nInternalName, cm_pstVarInternals->m_nNumIntVars, cm_pstVarInternals->m_aunIntVarsNames);
    if(cg_unInvalidPortId != unVarId){
      retVal = getVarInternal(unVarId);
    }
  }
  return retVal;
}

