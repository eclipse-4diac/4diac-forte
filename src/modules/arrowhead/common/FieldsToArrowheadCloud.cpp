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

#include "FieldsToArrowheadCloud.h"

USE_STRING_ID(address);
USE_STRING_ID(arrowheadCloud);
USE_STRING_ID(ArrowheadCloud);
USE_STRING_ID(authenticationInfo);
USE_STRING_ID(BOOL);
USE_STRING_ID(cloudName);
USE_STRING_ID(CNF);
USE_STRING_ID(DINT);
USE_STRING_ID(Event);
USE_STRING_ID(FieldsToArrowheadCloud);
USE_STRING_ID(gatekeeperServiceURI);
USE_STRING_ID(Operator);
USE_STRING_ID(port);
USE_STRING_ID(REQ);
USE_STRING_ID(secure);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_FieldsToArrowheadCloud, STRID(FieldsToArrowheadCloud))

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmDataInputNames[] = {
    STRID(Operator),           STRID(cloudName), STRID(address), STRID(port), STRID(gatekeeperServiceURI),
    STRID(authenticationInfo), STRID(secure)};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmDataInputTypeIds[] = {
    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(DINT), STRID(WSTRING), STRID(WSTRING), STRID(BOOL)};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmDataOutputNames[] = {STRID(arrowheadCloud)};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmDataOutputTypeIds[] = {STRID(ArrowheadCloud)};

const TForteInt16 FORTE_FieldsToArrowheadCloud::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToArrowheadCloud::scmEIWith[] = {0, 1, 2, 6, 5, 4, 3, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_FieldsToArrowheadCloud::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_FieldsToArrowheadCloud::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_FieldsToArrowheadCloud::scmFBInterfaceSpec = {1,
                                                                           scmEventInputNames,
                                                                           scmEventInputTypeIds,
                                                                           scmEIWith,
                                                                           scmEIWithIndexes,
                                                                           1,
                                                                           scmEventOutputNames,
                                                                           scmEventOutputTypeIds,
                                                                           scmEOWith,
                                                                           scmEOWithIndexes,
                                                                           7,
                                                                           scmDataInputNames,
                                                                           scmDataInputTypeIds,
                                                                           1,
                                                                           scmDataOutputNames,
                                                                           scmDataOutputTypeIds,
                                                                           0,
                                                                           0};

void FORTE_FieldsToArrowheadCloud::alg_REQ() {
  arrowheadCloud().Operator() = Operator();
  arrowheadCloud().cloudName() = cloudName();
  arrowheadCloud().address() = address();
  arrowheadCloud().port() = port();
  arrowheadCloud().gatekeeperServiceURI() = gatekeeperServiceURI();
  arrowheadCloud().authenticationInfo() = authenticationInfo();
  arrowheadCloud().secure() = secure();
}

void FORTE_FieldsToArrowheadCloud::enterStateSTART(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateSTART;
}

void FORTE_FieldsToArrowheadCloud::enterStateREQ(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FieldsToArrowheadCloud::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
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
