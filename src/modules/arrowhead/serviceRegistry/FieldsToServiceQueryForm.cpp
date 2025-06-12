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

#include "FieldsToServiceQueryForm.h"

USE_STRING_ID(ArrowheadService);
USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DINT);
USE_STRING_ID(Event);
USE_STRING_ID(FieldsToServiceQueryForm);
USE_STRING_ID(metadataSearch);
USE_STRING_ID(pingProviders);
USE_STRING_ID(REQ);
USE_STRING_ID(service);
USE_STRING_ID(serviceQueryForm);
USE_STRING_ID(ServiceQueryForm);
USE_STRING_ID(version);

DEFINE_FIRMWARE_FB(FORTE_FieldsToServiceQueryForm, STRID(FieldsToServiceQueryForm))

namespace {
  const auto cDataInputNames = std::array{
      STRID(service), STRID(metadataSearch), STRID(pingProviders), STRID(version)};
  
      STRID(ArrowheadService), STRID(BOOL), STRID(BOOL), STRID(DINT)};
  
  const auto cDataOutputNames = std::array{STRID(serviceQueryForm)};
  
  
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


void FORTE_FieldsToServiceQueryForm::alg_REQ() {
  serviceQueryForm().service() = service();
  serviceQueryForm().metadataSearch() = metadataSearch();
  serviceQueryForm().pingProviders() = pingProviders();
  serviceQueryForm().version() = version();
}

void FORTE_FieldsToServiceQueryForm::enterStateSTART(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateSTART;
}

void FORTE_FieldsToServiceQueryForm::enterStateREQ(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FieldsToServiceQueryForm::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
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
