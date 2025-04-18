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

#include "OrchestrationForm2OrchestrationForm_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(OrchestrationForm);
USE_STRING_ID(OrchestrationForm2OrchestrationForm);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_OrchestrationForm2OrchestrationForm, STRID(OrchestrationForm2OrchestrationForm))

const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scmDataInputTypeIds[] = {
    STRID(OrchestrationForm)};

const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scmDataOutputTypeIds[] = {
    STRID(OrchestrationForm)};

const TForteInt16 FORTE_OrchestrationForm2OrchestrationForm::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_OrchestrationForm2OrchestrationForm::scmEIWith[] = {0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_OrchestrationForm2OrchestrationForm::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_OrchestrationForm2OrchestrationForm::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_OrchestrationForm2OrchestrationForm::scmFBInterfaceSpec = {1,
                                                                                        scmEventInputNames,
                                                                                        scmEventInputTypeIds,
                                                                                        scmEIWith,
                                                                                        scmEIWithIndexes,
                                                                                        1,
                                                                                        scmEventOutputNames,
                                                                                        scmEventOutputTypeIds,
                                                                                        scmEOWith,
                                                                                        scmEOWithIndexes,
                                                                                        1,
                                                                                        scmDataInputNames,
                                                                                        scmDataInputTypeIds,
                                                                                        1,
                                                                                        scmDataOutputNames,
                                                                                        scmDataOutputTypeIds,
                                                                                        0,
                                                                                        0};

void FORTE_OrchestrationForm2OrchestrationForm::executeEvent(TEventID paEIID,
                                                             CEventChainExecutionThread *const paECET) {
  if (scmEventREQID == paEIID) {
    st_OUT() = st_IN();
    sendOutputEvent(scmEventCNFID, paECET);
  }
}
