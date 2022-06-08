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
#include "resource.h"

CBasicFB::CBasicFB(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec, const CStringDictionary::TStringId pa_nInstanceNameId,
    const SInternalVarsInformation *pa_pstVarInternals, TForteByte *pa_acFBConnData, TForteByte *pa_acBasicFBVarsData) :
    CFunctionBlock(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData, pa_acBasicFBVarsData), m_nECCState(0),
        cm_pstVarInternals(pa_pstVarInternals), cm_amountOfInternalFBs(0) {

  m_aoInternals = nullptr;
  if((nullptr != cm_pstVarInternals) && (cm_pstVarInternals->m_nNumIntVars) && (nullptr != pa_acBasicFBVarsData)) {
    pa_acBasicFBVarsData += genFBVarsDataSize(m_pstInterfaceSpec->m_nNumDIs, m_pstInterfaceSpec->m_nNumDOs, m_pstInterfaceSpec->m_nNumAdapters);

    m_aoInternals = reinterpret_cast<CIEC_ANY*>(pa_acBasicFBVarsData);

    const CStringDictionary::TStringId *pnDataIds = cm_pstVarInternals->m_aunIntVarsDataTypeNames;
    for(int i = 0; i < cm_pstVarInternals->m_nNumIntVars; ++i) {
      createDataPoint(&pnDataIds, pa_acBasicFBVarsData);
      pa_acBasicFBVarsData += sizeof(CIEC_ANY);
    }
  }
}

CBasicFB::CBasicFB(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec, const CStringDictionary::TStringId pa_nInstanceNameId,
    const SInternalVarsInformation *pa_pstVarInternals, TForteByte *pa_acFBConnData, TForteByte *pa_acBasicFBVarsData,
    const SCFB_FBInstanceData *const pa_internalFBs, const size_t pa_numberOfInternalFbs) :
    CFunctionBlock(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData, pa_acBasicFBVarsData), m_nECCState(0),
        cm_pstVarInternals(pa_pstVarInternals), cm_amountOfInternalFBs(pa_numberOfInternalFbs) {

  createInternalFBs(pa_internalFBs);

  m_aoInternals = nullptr;
  if((nullptr != cm_pstVarInternals) && (cm_pstVarInternals->m_nNumIntVars) && (nullptr != pa_acBasicFBVarsData)) {
    pa_acBasicFBVarsData += genFBVarsDataSize(m_pstInterfaceSpec->m_nNumDIs, m_pstInterfaceSpec->m_nNumDOs, m_pstInterfaceSpec->m_nNumAdapters);

    m_aoInternals = reinterpret_cast<CIEC_ANY*>(pa_acBasicFBVarsData);

    const CStringDictionary::TStringId *pnDataIds = cm_pstVarInternals->m_aunIntVarsDataTypeNames;
    for(int i = 0; i < cm_pstVarInternals->m_nNumIntVars; ++i) {
      createDataPoint(&pnDataIds, pa_acBasicFBVarsData);
      pa_acBasicFBVarsData += sizeof(CIEC_ANY);
    }
  }
}

CBasicFB::~CBasicFB() {
  if(nullptr != m_aoInternals) {
    for(int i = 0; i < cm_pstVarInternals->m_nNumIntVars; ++i) {
      getVarInternal(i)->~CIEC_ANY();
    }
  }
}

CIEC_ANY* CBasicFB::getVar(CStringDictionary::TStringId *paNameList, unsigned int paNameListSize) {
  CIEC_ANY *poRetVal = CFunctionBlock::getVar(paNameList, paNameListSize);
  if((nullptr == poRetVal) && (1 == paNameListSize)) {
    poRetVal = getInternalVar(*paNameList);
    if(nullptr == poRetVal && !strcmp("!ECC", CStringDictionary::getInstance().get(*paNameList))) { //TODO consider if this can also be an string ID in a different way
      poRetVal = &m_nECCState;
    }
  }
  return poRetVal;
}

EMGMResponse CBasicFB::changeFBExecutionState(EMGMCommandType pa_unCommand) {
  EMGMResponse nRetVal = CFunctionBlock::changeFBExecutionState(pa_unCommand);
  for(size_t i = 0; ((i < cm_amountOfInternalFBs) && (e_RDY == nRetVal)); ++i) {
    if(mInternalFBs[i]) {
      nRetVal = mInternalFBs[i]->changeFBExecutionState(pa_unCommand);
    }
  }
  return nRetVal;
}

CIEC_ANY* CBasicFB::getInternalVar(CStringDictionary::TStringId pa_nInternalName) {
  CIEC_ANY *retVal = nullptr;
  if(nullptr != cm_pstVarInternals) {
    TPortId unVarId = getPortId(pa_nInternalName, cm_pstVarInternals->m_nNumIntVars, cm_pstVarInternals->m_aunIntVarsNames);
    if(cg_unInvalidPortId != unVarId) {
      retVal = getVarInternal(unVarId);
    }
  }
  return retVal;
}

void CBasicFB::createInternalFBs(const SCFB_FBInstanceData *const pa_InternalFBData) {
  if(cm_amountOfInternalFBs) {
    mInternalFBs = new TFunctionBlockPtr[cm_amountOfInternalFBs];
    for(size_t i = 0; i < cm_amountOfInternalFBs; ++i) {
      mInternalFBs[i] = CTypeLib::createFB(pa_InternalFBData[i].m_nFBInstanceNameId, pa_InternalFBData[i].m_nFBTypeNameId, getResourcePtr());
    }
  }
}

#ifdef FORTE_TRACE_CTF
void CBasicFB::traceInstanceData() {
  std::vector<std::string> inputs(m_pstInterfaceSpec->m_nNumDIs);
  std::vector<std::string> outputs(m_pstInterfaceSpec->m_nNumDOs);
  std::vector<std::string> internals(cm_pstVarInternals ? cm_pstVarInternals->m_nNumIntVars : 0);
  std::vector<const char *> inputs_c_str(inputs.size());
  std::vector<const char *> outputs_c_str(outputs.size());
  std::vector<const char *> internals_c_str(internals.size());

  for(TForteUInt8 i = 0; i < inputs.size(); ++i) {
    CIEC_ANY *value = getDI(i);
    std::string &valueString = inputs[i];
    valueString.reserve(value->getToStringBufferSize());
    value->toString(valueString.data(), valueString.capacity());
    inputs_c_str[i] = valueString.c_str();
  }

  for(TForteUInt8 i = 0; i < outputs.size(); ++i) {
    CIEC_ANY *value = getDO(i);
    std::string &valueString = outputs[i];
    valueString.reserve(value->getToStringBufferSize());
    value->toString(valueString.data(), valueString.capacity());
    outputs_c_str[i] = valueString.c_str();
  }

  for(TForteUInt8 i = 0; i < internals.size(); ++i) {
    CIEC_ANY *value = getVarInternal(i);
    std::string &valueString = internals[i];
    valueString.reserve(value->getToStringBufferSize());
    value->toString(valueString.data(), valueString.capacity());
    internals_c_str[i] = valueString.c_str();
  }

  barectf_default_trace_instanceData(getResource().getTracePlatformContext().getContext(),
                                     getInstanceName() ?: "null",
                                     static_cast<uint32_t >(inputs.size()), inputs_c_str.data(),
                                     static_cast<uint32_t >(outputs.size()), outputs_c_str.data(),
                                     static_cast<uint32_t >(internals.size()), internals_c_str.data());
}
#endif

