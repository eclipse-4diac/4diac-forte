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

#include "FieldsToEventFilter.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_FieldsToEventFilter, "FieldsToEventFilter"_STRID)

namespace {
  const auto cDataInputNames =
      std::array{"eventType"_STRID, "consumer"_STRID,       "sources"_STRID,   "startDate"_STRID,
                 "endDate"_STRID,   "filterMetadata"_STRID, "notifyUri"_STRID, "matchMetadata"_STRID};

  "ArrowheadSystem"_STRID, "ARRAY"_STRID, 10, "ArrowheadSystem"_STRID, "DATE_AND_TIME"_STRID, "DATE_AND_TIME"_STRID,
      "ARRAY"_STRID, 10, "WSTRING"_STRID, "WSTRING"_STRID, "BOOL"_STRID
};

const auto cDataOutputNames = std::array{"eventFilter"_STRID};

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

void FORTE_FieldsToEventFilter::alg_REQ() {
  CIEC_INT i;
  ;

  i = 0;
  eventFilter().eventType() = eventType();
  eventFilter().consumer() = consumer();
  eventFilter().startDate() = startDate();
  eventFilter().endDate() = endDate();
  eventFilter().notifyUri() = notifyUri();
  eventFilter().matchMetadata() = matchMetadata();

  while ((i < 10)) {
    // looks like while is faster than FOR when exporting to cpp

    eventFilter().sources()[i] = sources()[i];
    eventFilter().filterMetadata()[i] = filterMetadata()[i];
    i = i + 1;
  }
}

void FORTE_FieldsToEventFilter::enterStateSTART(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateSTART;
}

void FORTE_FieldsToEventFilter::enterStateREQ(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FieldsToEventFilter::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
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
