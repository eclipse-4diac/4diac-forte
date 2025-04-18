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

const CStringDictionary::TStringId FORTE_FieldsToServiceQueryForm::scmDataInputNames[] = {
    STRID(service), STRID(metadataSearch), STRID(pingProviders), STRID(version)};

const CStringDictionary::TStringId FORTE_FieldsToServiceQueryForm::scmDataInputTypeIds[] = {
    STRID(ArrowheadService), STRID(BOOL), STRID(BOOL), STRID(DINT)};

const CStringDictionary::TStringId FORTE_FieldsToServiceQueryForm::scmDataOutputNames[] = {STRID(serviceQueryForm)};

const CStringDictionary::TStringId FORTE_FieldsToServiceQueryForm::scmDataOutputTypeIds[] = {STRID(ServiceQueryForm)};

const TForteInt16 FORTE_FieldsToServiceQueryForm::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToServiceQueryForm::scmEIWith[] = {0, 3, 2, 1, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_FieldsToServiceQueryForm::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_FieldsToServiceQueryForm::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_FieldsToServiceQueryForm::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_FieldsToServiceQueryForm::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToServiceQueryForm::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_FieldsToServiceQueryForm::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_FieldsToServiceQueryForm::scmFBInterfaceSpec = {1,
                                                                             scmEventInputNames,
                                                                             scmEventInputTypeIds,
                                                                             scmEIWith,
                                                                             scmEIWithIndexes,
                                                                             1,
                                                                             scmEventOutputNames,
                                                                             scmEventOutputTypeIds,
                                                                             scmEOWith,
                                                                             scmEOWithIndexes,
                                                                             4,
                                                                             scmDataInputNames,
                                                                             scmDataInputTypeIds,
                                                                             1,
                                                                             scmDataOutputNames,
                                                                             scmDataOutputTypeIds,
                                                                             0,
                                                                             0};

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
