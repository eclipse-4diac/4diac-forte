/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
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
 *    Martin Jobst - account for data type size in FB initialization
 *******************************************************************************/
#include <string.h>
#include "basicfb.h"
#include "resource.h"

CBasicFB::CBasicFB(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec,
                   const CStringDictionary::TStringId pa_nInstanceNameId,
                   const SInternalVarsInformation *pa_pstVarInternals) :
        CFunctionBlock(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId), m_nECCState(0),
        cm_pstVarInternals(pa_pstVarInternals), mBasicFBVarsData(nullptr), m_aoInternals(nullptr) {
}

bool CBasicFB::initialize() {
  if(!CFunctionBlock::initialize()) {
    return false;
  }
  if((nullptr != cm_pstVarInternals) && (cm_pstVarInternals->m_nNumIntVars)) {
    size_t basicVarsDataSize = calculateBasicFBVarsDataSize(*cm_pstVarInternals);
    mBasicFBVarsData = basicVarsDataSize ? operator new(basicVarsDataSize) : nullptr;

    auto *basicVarsData = reinterpret_cast<TForteByte *>(mBasicFBVarsData);
    m_aoInternals = reinterpret_cast<CIEC_ANY**>(basicVarsData);
    basicVarsData += cm_pstVarInternals->m_nNumIntVars * sizeof(CIEC_ANY *);
    const CStringDictionary::TStringId *pnDataIds = cm_pstVarInternals->m_aunIntVarsDataTypeNames;
    for(TPortId i = 0; i < cm_pstVarInternals->m_nNumIntVars; ++i) {
      m_aoInternals[i] = createDataPoint(pnDataIds, basicVarsData);
    }
  }
  return true;
}

CBasicFB::~CBasicFB() {
  if(nullptr != m_aoInternals) {
    for(TPortId i = 0; i < cm_pstVarInternals->m_nNumIntVars; ++i) {
      if(CIEC_ANY* value = m_aoInternals[i]; nullptr != value) {
        std::destroy_at(value);
      }
    }
  }
  operator delete(mBasicFBVarsData);
  mBasicFBVarsData = nullptr;
}

void CBasicFB::setInitialValues() {
  CFunctionBlock::setInitialValues();
  if(cm_pstVarInternals) {
    const CStringDictionary::TStringId *pnDataIds = cm_pstVarInternals->m_aunIntVarsDataTypeNames;
    for (TPortId i = 0; i < cm_pstVarInternals->m_nNumIntVars; ++i) {
      TForteByte *varsData = nullptr;
      CIEC_ANY *value = createDataPoint(pnDataIds, varsData);
      if (value) { getDI(i)->setValue(*value); }
      delete value;
    }
  }
}

size_t CBasicFB::calculateBasicFBVarsDataSize(const SInternalVarsInformation &paVarInternals) {
  size_t result = 0;
  const CStringDictionary::TStringId *pnDataIds;

  result += paVarInternals.m_nNumIntVars * sizeof(CIEC_ANY *);
  pnDataIds = paVarInternals.m_aunIntVarsDataTypeNames;
  for (TPortId i = 0; i < paVarInternals.m_nNumIntVars; ++i) {
    result += getDataPointSize(pnDataIds);
  }

  return result;
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

CIEC_ANY* CBasicFB::getInternalVar(CStringDictionary::TStringId pa_nInternalName) {
  CIEC_ANY *retVal = nullptr;
  if(nullptr != cm_pstVarInternals) {
    TPortId unVarId = getPortId(pa_nInternalName, cm_pstVarInternals->m_nNumIntVars, cm_pstVarInternals->m_aunIntVarsNames);
    if(cgInvalidPortId != unVarId) {
      retVal = getVarInternal(unVarId);
    }
  }
  return retVal;
}

TFunctionBlockPtr *CBasicFB::createInternalFBs(const size_t paAmountOfInternalFBs, const SCFB_FBInstanceData *const paInternalFBData, CResource* const paResource) {
  TFunctionBlockPtr *internalFBs = nullptr;
  if (paAmountOfInternalFBs) {
    internalFBs = new TFunctionBlockPtr[paAmountOfInternalFBs];
    for(size_t i = 0; i < paAmountOfInternalFBs; ++i) {
      internalFBs[i] = CTypeLib::createFB(paInternalFBData[i].m_nFBInstanceNameId, paInternalFBData[i].m_nFBTypeNameId, paResource);
    }
  }
  return internalFBs;
}

void CBasicFB::deleteInternalFBs(const size_t paAmountOfInternalFBs, TFunctionBlockPtr *const paInternalFBs) {
  for (size_t i = 0; i < paAmountOfInternalFBs; ++i) {
    delete paInternalFBs[i];
  }
  delete[] paInternalFBs;
};

#ifdef FORTE_TRACE_CTF
void CBasicFB::traceInstanceData() {
  std::vector<std::string> inputs(mInterfaceSpec->m_nNumDIs);
  std::vector<std::string> outputs(mInterfaceSpec->m_nNumDOs);
  std::vector<std::string> internals(cm_pstVarInternals ? cm_pstVarInternals->m_nNumIntVars : 0);
  std::vector<const char *> inputs_c_str(inputs.size());
  std::vector<const char *> outputs_c_str(outputs.size());
  std::vector<const char *> internals_c_str(internals.size());

  for(TPortId i = 0; i < inputs.size(); ++i) {
    CIEC_ANY *value = getDI(i);
    std::string &valueString = inputs[i];
    valueString.reserve(value->getToStringBufferSize());
    value->toString(valueString.data(), valueString.capacity());
    inputs_c_str[i] = valueString.c_str();
  }

  for(TPortId i = 0; i < outputs.size(); ++i) {
    CIEC_ANY *value = getDO(i);
    std::string &valueString = outputs[i];
    valueString.reserve(value->getToStringBufferSize());
    value->toString(valueString.data(), valueString.capacity());
    outputs_c_str[i] = valueString.c_str();
  }

  for(TPortId i = 0; i < internals.size(); ++i) {
    CIEC_ANY *value = getVarInternal(i);
    std::string &valueString = internals[i];
    valueString.reserve(value->getToStringBufferSize());
    value->toString(valueString.data(), valueString.capacity());
    internals_c_str[i] = valueString.c_str();
  }

  barectf_default_trace_instanceData(getResource().getTracePlatformContext().getContext(),
                                     getFBTypeName() ?: "null",
                                     getInstanceName() ?: "null",
                                     static_cast<uint32_t >(inputs.size()), inputs_c_str.data(),
                                     static_cast<uint32_t >(outputs.size()), outputs_c_str.data(),
                                     static_cast<uint32_t >(internals.size()), internals_c_str.data());
}
#endif

