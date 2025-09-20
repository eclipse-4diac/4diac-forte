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

#include "forte/eclipse4diac/rtevents/RT_Bridge_fbt.h"
#include <memory>
#include "forte/util/criticalregion.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::eclipse4diac::rtevents {
  namespace {
    const auto cEventInputNames = std::array{"RD"_STRID, "WR"_STRID};
    const auto cEventOutputNames = std::array{"RDO"_STRID};
  } // namespace

  DEFINE_GENERIC_FIRMWARE_FB(FORTE_GEN_RT_Bridge, "eclipse4diac::rtevents::GEN_RT_Bridge"_STRID)

  FORTE_GEN_RT_Bridge::FORTE_GEN_RT_Bridge(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      conn_RDO(*this, 0) {};

  void FORTE_GEN_RT_Bridge::setInitialValues() {
  }

  void FORTE_GEN_RT_Bridge::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  util::CCriticalRegion criticalRegion(
      mSyncObject); // protect the processing of the data independent of the calling event
    switch (paEIID) {
      case scmEventRDID: sendOutputEvent(scmEventRDOID, paECET); break;
      case scmEventWRID:
        for (size_t i = 0; i < getFBInterfaceSpec().getNumDIs(); i++) {
          getDO(i)->setValue(getDI(i)->unwrap());
        }
        break;
    }
  }

  void FORTE_GEN_RT_Bridge::readInputData(const TEventID paEIID) {
    if (paEIID == scmEventWRID) {
      for (size_t i = 0; i < getFBInterfaceSpec().getNumDIs(); i++) {
        readData(i, mGenDIs[i], mGenDIConns[i]);
      }
    }
  }

  void FORTE_GEN_RT_Bridge::writeOutputData(const TEventID paEIID) {
    if (paEIID == scmEventRDOID) {
      for (size_t i = 0; i < getFBInterfaceSpec().getNumDOs(); i++) {
        writeData(getFBInterfaceSpec().getNumDIs() + i, mGenDOs[i], mGenDOConns[i]);
      }
    }
  }

  bool FORTE_GEN_RT_Bridge::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    size_t numPorts = 0;

    const char *pos = strrchr(paConfigString, '_');
    if (nullptr != pos) {
      pos++; // move after underscore
      numPorts = static_cast<size_t>(util::strtoul(pos, nullptr, 10));
    } else {
      return false;
    }

    if (numPorts < 1) {
      return false;
    }

    paInterfaceSpec.mEINames = cEventInputNames;
    paInterfaceSpec.mEONames = cEventOutputNames;

    generateGenericInterfacePointNameArray("SD_", mDINames, numPorts);
    generateGenericInterfacePointNameArray("RD_", mDONames, numPorts);

    paInterfaceSpec.mDINames = mDINames;
    paInterfaceSpec.mDONames = mDONames;

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
    mGenDIs = std::make_unique<CIEC_ANY_VARIANT[]>(getFBInterfaceSpec().getNumDIs());
  }

  void FORTE_GEN_RT_Bridge::createGenOutputData() {
    mGenDOs = std::make_unique<CIEC_ANY_VARIANT[]>(getFBInterfaceSpec().getNumDOs());
  }
} // namespace forte::eclipse4diac::rtevents
