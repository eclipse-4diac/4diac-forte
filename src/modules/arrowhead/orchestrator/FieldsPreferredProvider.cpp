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

#include "FieldsPreferredProvider.h"

USE_STRING_ID(ArrowheadCloud);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(FieldsPreferredProvider);
USE_STRING_ID(preferredProvider);
USE_STRING_ID(PreferredProvider);
USE_STRING_ID(providerCloud);
USE_STRING_ID(providerSystem);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_FieldsPreferredProvider, STRID(FieldsPreferredProvider))

namespace {
  const auto cDataInputNames = std::array{STRID(providerSystem),
                                                                                           STRID(providerCloud)};
  
                                                                                             STRID(ArrowheadCloud)};
  
  const auto cDataOutputNames = std::array{STRID(preferredProvider)};
  
  
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


void FORTE_FieldsPreferredProvider::alg_REQ() {
  preferredProvider().providerSystem() = providerSystem();
  preferredProvider().providerCloud() = providerCloud();
}

void FORTE_FieldsPreferredProvider::enterStateSTART(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateSTART;
}

void FORTE_FieldsPreferredProvider::enterStateREQ(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FieldsPreferredProvider::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
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
