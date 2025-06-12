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

USE_STRING_ID(ArrowheadService);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DINT);
USE_STRING_ID(Event);
USE_STRING_ID(FieldsToServiceRegistryEntry);
USE_STRING_ID(metadata);
USE_STRING_ID(providedService);
USE_STRING_ID(provider);
USE_STRING_ID(REQ);
USE_STRING_ID(serviceRegistryEntry);
USE_STRING_ID(ServiceRegistryEntry);
USE_STRING_ID(serviceURI);
USE_STRING_ID(ttl);
USE_STRING_ID(udp);
USE_STRING_ID(version);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_FieldsToServiceRegistryEntry, STRID(FieldsToServiceRegistryEntry))

namespace {
  const auto cDataInputNames = std::array{
      STRID(providedService), STRID(provider), STRID(serviceURI), STRID(version), STRID(udp), STRID(ttl),
      STRID(metadata)};
  
      STRID(ArrowheadService), STRID(ArrowheadSystem), STRID(WSTRING), STRID(DINT), STRID(BOOL), STRID(DINT),
      STRID(WSTRING)};
  
  const auto cDataOutputNames = std::array{
      STRID(serviceRegistryEntry)};
  
      STRID(ServiceRegistryEntry)};
  
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventInputTypeIds = std::array{STRID(Event)};
  
  const auto cEventOutputNames = std::array{STRID(CNF)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};
  
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
