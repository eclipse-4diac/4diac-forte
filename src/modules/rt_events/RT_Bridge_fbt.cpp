/*******************************************************************************
 * Copyright (c) 2023 Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "RT_Bridge_fbt.h"
#include <memory>
#include "criticalregion.h"
#include "string_utils.h"

USE_STRING_ID(ANY);
USE_STRING_ID(Event);
USE_STRING_ID(GEN_RT_Bridge);
USE_STRING_ID(RD);
USE_STRING_ID(RDO);
USE_STRING_ID(WR);

DEFINE_GENERIC_FIRMWARE_FB(FORTE_GEN_RT_Bridge, STRID(GEN_RT_Bridge))

const CStringDictionary::TStringId FORTE_GEN_RT_Bridge::scmEventInputNames[] = {STRID(RD), STRID(WR)};
const CStringDictionary::TStringId FORTE_GEN_RT_Bridge::scmEventOutputNames[] = {STRID(RDO)};

FORTE_GEN_RT_Bridge::FORTE_GEN_RT_Bridge(const CStringDictionary::TStringId paInstanceNameId,
                                         forte::core::CFBContainer &paContainer) :
    CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
    conn_RDO(*this, 0) {};

void FORTE_GEN_RT_Bridge::setInitialValues() {
}

void FORTE_GEN_RT_Bridge::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  CCriticalRegion criticalRegion(mSyncObject); // protect the processing of the data independent of the calling event
  switch (paEIID) {
    case scmEventRDID: sendOutputEvent(scmEventRDOID, paECET); break;
    case scmEventWRID:
      for (size_t i = 0; i < getFBInterfaceSpec().mNumDIs; i++) {
        getDO(i)->setValue(getDI(i)->unwrap());
      }
      break;
  }
}

void FORTE_GEN_RT_Bridge::readInputData(const TEventID paEIID) {
  if (paEIID == scmEventWRID) {
    for (size_t i = 0; i < getFBInterfaceSpec().mNumDIs; i++) {
      readData(i, mGenDIs[i], mGenDIConns[i]);
    }
  }
}

void FORTE_GEN_RT_Bridge::writeOutputData(const TEventID paEIID) {
  if (paEIID == scmEventRDOID) {
    for (size_t i = 0; i < getFBInterfaceSpec().mNumDOs; i++) {
      writeData(getFBInterfaceSpec().mNumDIs + i, mGenDOs[i], mGenDOConns[i]);
    }
  }
}

bool FORTE_GEN_RT_Bridge::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  size_t numPorts = 0;

  const char *pos = strrchr(paConfigString, '_');
  if (nullptr != pos) {
    pos++; // move after underscore
    numPorts = static_cast<size_t>(forte::core::util::strtoul(pos, nullptr, 10));
  } else {
    return false;
  }

  if (numPorts < 1) {
    return false;
  }

  paInterfaceSpec.mEINames = scmEventInputNames;
  paInterfaceSpec.mEONames = scmEventOutputNames;

  mDINames = std::make_unique<CStringDictionary::TStringId[]>(numPorts);
  generateGenericInterfacePointNameArray("SD_", mDINames.get(), numPorts);

  mDONames = std::make_unique<CStringDictionary::TStringId[]>(numPorts);
  generateGenericInterfacePointNameArray("RD_", mDONames.get(), numPorts);

  paInterfaceSpec.mDINames = mDINames.get();
  paInterfaceSpec.mDONames = mDONames.get();

  return true;
}

CIEC_ANY *FORTE_GEN_RT_Bridge::getDI(size_t paIndex) {
  return &mGenDIs[paIndex];
}

CIEC_ANY *FORTE_GEN_RT_Bridge::getDO(size_t paIndex) {
  return &mGenDOs[paIndex];
}

CEventConnection *FORTE_GEN_RT_Bridge::getEOConUnchecked(TPortId paIndex) {
  return (paIndex == 0) ? &conn_RDO : nullptr;
}

void FORTE_GEN_RT_Bridge::createGenInputData() {
  mGenDIs = std::make_unique<CIEC_ANY_VARIANT[]>(getFBInterfaceSpec().mNumDIs);
}

void FORTE_GEN_RT_Bridge::createGenOutputData() {
  mGenDOs = std::make_unique<CIEC_ANY_VARIANT[]>(getFBInterfaceSpec().mNumDOs);
}
