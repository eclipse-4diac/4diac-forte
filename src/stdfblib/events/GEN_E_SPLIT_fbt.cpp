/*******************************************************************************
 * Copyright (c) 2024 HR Agartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger
 *     - implement Generic GEN_E_SPLIT_fbt
 *******************************************************************************/

#include "GEN_E_SPLIT_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_E_SPLIT_fbt_gen.cpp"
#endif

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_E_SPLIT, g_nStringIdGEN_E_SPLIT)

const CStringDictionary::TStringId GEN_E_SPLIT::scmEventInputNames[] = {g_nStringIdEI};


GEN_E_SPLIT::GEN_E_SPLIT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId) {
};

void GEN_E_SPLIT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventEIID:
      for (TEventID i = 0; i < getFBInterfaceSpec().mNumEOs; ++i) {
        sendOutputEvent(i, paECET);
      }
      break;
  }
}

void GEN_E_SPLIT::readInputData(TEventID) {
  // nothing to do
}

void GEN_E_SPLIT::writeOutputData(TEventID) {
  // nothing to do
}

bool GEN_E_SPLIT::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec){
  const char *acPos = strrchr(paConfigString, '_');

  if(nullptr != acPos){
    ++acPos;  // move after underscore
    //we have an underscore
    paInterfaceSpec.mNumEOs = static_cast<TEventID>(forte::core::util::strtoul(acPos, nullptr, 10));

    if(paInterfaceSpec.mNumEOs < CFunctionBlock::scmMaxInterfaceEvents && paInterfaceSpec.mNumEOs >= 2){
      scmEventOutputNames = std::make_unique<CStringDictionary::TStringId[]>(paInterfaceSpec.mNumEOs);

      generateGenericInterfacePointNameArray("EO", scmEventOutputNames.get(), paInterfaceSpec.mNumEOs);

      paInterfaceSpec.mNumEIs = 1;
      paInterfaceSpec.mEINames = scmEventInputNames;
      paInterfaceSpec.mEONames = scmEventOutputNames.get();
      paInterfaceSpec.mEITypeNames = nullptr;
      paInterfaceSpec.mEOTypeNames = nullptr;
      paInterfaceSpec.mNumDIs = 0;
      paInterfaceSpec.mDINames = nullptr;
      paInterfaceSpec.mDIDataTypeNames = nullptr;
      paInterfaceSpec.mNumDOs = 0;
      paInterfaceSpec.mDONames = nullptr;
      paInterfaceSpec.mDODataTypeNames = nullptr;
      return true;
    }
    else{
      if(paInterfaceSpec.mNumEOs >= CFunctionBlock::scmMaxInterfaceEvents){
        DEVLOG_ERROR("Cannot configure FB-Instance E_SPLIT_%d. Number of event outputs exceeds maximum of %d.\n", paInterfaceSpec.mNumEOs, CFunctionBlock::scmMaxInterfaceEvents);
      }
      else{
        DEVLOG_ERROR("Cannot configure FB-Instance E_SPLIT_%d. Number of event outputs smaller than minimum of 2.\n", paInterfaceSpec.mNumEOs);
      }
    }
  }
  return false;
}

CIEC_ANY *GEN_E_SPLIT::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *GEN_E_SPLIT::getDO(size_t) {
  return nullptr;
}


CDataConnection **GEN_E_SPLIT::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *GEN_E_SPLIT::getDOConUnchecked(TPortId) {
  return nullptr;
}
