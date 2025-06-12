/*******************************************************************************
 * Copyright (c) 2019, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "FieldsToPublishEvent.h"

USE_STRING_ID(ArrowheadEvent);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(CNF);
USE_STRING_ID(deliveryCompleteUri);
USE_STRING_ID(event);
USE_STRING_ID(Event);
USE_STRING_ID(FieldsToPublishEvent);
USE_STRING_ID(publishEvent);
USE_STRING_ID(PublishEvent);
USE_STRING_ID(REQ);
USE_STRING_ID(source);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_FieldsToPublishEvent, STRID(FieldsToPublishEvent))

namespace {
  const auto cDataInputNames = std::array{STRID(source), STRID(event),
                                                                                        STRID(deliveryCompleteUri)};
  
      STRID(ArrowheadSystem), STRID(ArrowheadEvent), STRID(WSTRING)};
  
  const auto cDataOutputNames = std::array{STRID(publishEvent)};
  
  
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


void FORTE_FieldsToPublishEvent::alg_REQ() {
  publishEvent().source() = source();
  publishEvent().event() = event();
  publishEvent().deliveryCompleteUri() = deliveryCompleteUri();
}

void FORTE_FieldsToPublishEvent::enterStateSTART(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateSTART;
}

void FORTE_FieldsToPublishEvent::enterStateREQ(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FieldsToPublishEvent::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
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
        if ((1))
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
