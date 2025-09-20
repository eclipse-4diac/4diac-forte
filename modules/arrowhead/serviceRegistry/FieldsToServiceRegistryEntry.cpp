/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "FieldsToServiceRegistryEntry.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_FieldsToServiceRegistryEntry, "FieldsToServiceRegistryEntry"_STRID)

namespace {
  const auto cDataInputNames = std::array{"providedService"_STRID, "provider"_STRID, "serviceURI"_STRID,
                                          "version"_STRID,         "udp"_STRID,      "ttl"_STRID,
                                          "metadata"_STRID};

  "ArrowheadService"_STRID, "ArrowheadSystem"_STRID, "WSTRING"_STRID, "DINT"_STRID, "BOOL"_STRID, "DINT"_STRID,
      "WSTRING"_STRID
};

const auto cDataOutputNames = std::array{"serviceRegistryEntry"_STRID};

"ServiceRegistryEntry"_STRID
}
;

const auto cEventInputNames = std::array{"REQ"_STRID};
const auto cEventInputTypeIds = std::array{"Event"_STRID};

const auto cEventOutputNames = std::array{"CNF"_STRID};
const auto cEventOutputTypeIds = std::array{"Event"_STRID};

const SFBInterfaceSpec cFBInterfaceSpec = {
    .mEINames = cEventInputNames,
    .mEITypeNames = cEventInputTypeIds,
    .mEONames = cEventOutputNames,
    .mEOTypeNames = cEventOutputTypeIds,
    .mDINames = cDataInputNames,
    .mDONames = cDataOutputNames,
    .mDIONames = {},
    .mSocketNames = {},
    .mPlugNames = {},
};
}

void FORTE_FieldsToServiceRegistryEntry::alg_REQ() {
  serviceRegistryEntry().providedService() = providedService();
  serviceRegistryEntry().provider() = provider();
  serviceRegistryEntry().serviceURI() = serviceURI();
  serviceRegistryEntry().version() = version();
  serviceRegistryEntry().udp() = udp();
  serviceRegistryEntry().ttl() = ttl();
  serviceRegistryEntry().metadata() = metadata();
}

void FORTE_FieldsToServiceRegistryEntry::enterStateSTART(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateSTART;
}

void FORTE_FieldsToServiceRegistryEntry::enterStateREQ(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FieldsToServiceRegistryEntry::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  bool bTransitionCleared;
  do {
    bTransitionCleared = true;
    switch (mECCState) {
      case scmStateSTART:
        if (scmEventREQID == paEIID)
          enterStateREQ(paECET);
        else
          bTransitionCleared = false; // no transition cleared
        break;
      case scmStateREQ:
        if (1)
          enterStateSTART(paECET);
        else
          bTransitionCleared = false; // no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 1.",
                     mECCState.operator TForteUInt16());
        mECCState = 0; // 0 is always the initial state
        break;
    }
    paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while (bTransitionCleared);
}
