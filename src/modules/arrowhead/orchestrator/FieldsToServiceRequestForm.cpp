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

#include "FieldsToServiceRequestForm.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(ArrowheadCloud);
USE_STRING_ID(ArrowheadService);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(FieldsToServiceRequestForm);
USE_STRING_ID(orchestrationFlags);
USE_STRING_ID(PreferredProvider);
USE_STRING_ID(preferredProviders);
USE_STRING_ID(REQ);
USE_STRING_ID(requestedQoS);
USE_STRING_ID(requestedService);
USE_STRING_ID(requesterCloud);
USE_STRING_ID(requesterSystem);
USE_STRING_ID(serviceRequestForm);
USE_STRING_ID(ServiceRequestForm);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_FieldsToServiceRequestForm, STRID(FieldsToServiceRequestForm))

namespace {
  const auto cDataInputNames = std::array{
      STRID(requesterSystem),    STRID(requesterCloud),     STRID(requestedService),
      STRID(orchestrationFlags), STRID(preferredProviders), STRID(requestedQoS)};
  
                                                                                                STRID(ArrowheadCloud),
                                                                                                STRID(ArrowheadService),
                                                                                                STRID(ARRAY),
                                                                                                10,
                                                                                                STRID(WSTRING),
                                                                                                STRID(ARRAY),
                                                                                                10,
                                                                                                STRID(PreferredProvider),
                                                                                                STRID(ARRAY),
                                                                                                10,
                                                                                                STRID(WSTRING)};
  
  const auto cDataOutputNames = std::array{STRID(serviceRequestForm)};
  
      STRID(ServiceRequestForm)};
  
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


void FORTE_FieldsToServiceRequestForm::alg_REQ() {
  CIEC_INT i;
  ;

  serviceRequestForm().requesterSystem() = requesterSystem();
  serviceRequestForm().requesterCloud() = requesterCloud();
  serviceRequestForm().requestedService() = requestedService();

  i = 0;
  while ((i < 10)) {
    // looks like while is faster than FOR when exporting to cpp

    serviceRequestForm().orchestrationFlags()[i] = orchestrationFlags()[i];
    serviceRequestForm().preferredProviders()[i] = preferredProviders()[i];
    serviceRequestForm().requestedQoS()[i] = requestedQoS()[i];
    i = i + 1;
  }
}

void FORTE_FieldsToServiceRequestForm::enterStateSTART(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateSTART;
}

void FORTE_FieldsToServiceRequestForm::enterStateREQ(, CEventChainExecutionThread *const paECET) {
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FieldsToServiceRequestForm::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
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
