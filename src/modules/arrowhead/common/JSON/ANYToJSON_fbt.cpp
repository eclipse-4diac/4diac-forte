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

#include "ANYToJSON_fbt.h"

USE_STRING_ID(ANY);
USE_STRING_ID(ANYToJSON);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(input);
USE_STRING_ID(output);
USE_STRING_ID(REQ);
USE_STRING_ID(STRING);

#include "ArrowheadJSONHelper.h"

DEFINE_FIRMWARE_FB(FORTE_ANYToJSON, STRID(ANYToJSON))

const CStringDictionary::TStringId FORTE_ANYToJSON::scmDataInputNames[] = {STRID(input)};

const CStringDictionary::TStringId FORTE_ANYToJSON::scmDataInputTypeIds[] = {STRID(ANY)};

const CStringDictionary::TStringId FORTE_ANYToJSON::scmDataOutputNames[] = {STRID(output)};

const CStringDictionary::TStringId FORTE_ANYToJSON::scmDataOutputTypeIds[] = {STRID(STRING)};

const TForteInt16 FORTE_ANYToJSON::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_ANYToJSON::scmEIWith[] = {0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_ANYToJSON::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_ANYToJSON::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_ANYToJSON::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_ANYToJSON::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_ANYToJSON::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_ANYToJSON::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_ANYToJSON::scmFBInterfaceSpec = {1,
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

void FORTE_ANYToJSON::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if (scmEventREQID == paEIID) {
    output() = "";
    ArrowheadJSONHelper::transformANYToJSON(input(), output());
    sendOutputEvent(scmEventCNFID, paECET);
  }
}
