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

#include "ANYToJSON.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ANYToJSON_gen.cpp"
#endif
#include "ArrowheadJSONHelper.h"

DEFINE_FIRMWARE_FB(FORTE_ANYToJSON, g_nStringIdANYToJSON)

const CStringDictionary::TStringId FORTE_ANYToJSON::scm_anDataInputNames[] = {g_nStringIdinput};

const CStringDictionary::TStringId FORTE_ANYToJSON::scm_anDataInputTypeIds[] = {g_nStringIdANY};

const CStringDictionary::TStringId FORTE_ANYToJSON::scm_anDataOutputNames[] = {g_nStringIdoutput};

const CStringDictionary::TStringId FORTE_ANYToJSON::scm_anDataOutputTypeIds[] = {g_nStringIdSTRING};

const TForteInt16 FORTE_ANYToJSON::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_ANYToJSON::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_ANYToJSON::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_ANYToJSON::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_ANYToJSON::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_ANYToJSON::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_ANYToJSON::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_ANYToJSON::executeEvent(int paEIID) {
  if(scm_nEventREQID == paEIID) {
    output() = "";
    ArrowheadJSONHelper::transformANYToJSON(input(), output());
    sendOutputEvent(scm_nEventCNFID);
  }
}



