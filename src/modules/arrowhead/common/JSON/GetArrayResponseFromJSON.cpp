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

#include "GetArrayResponseFromJSON.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GetArrayResponseFromJSON_gen.cpp"
#endif

#include "ArrowheadJSONHelper.h"

DEFINE_FIRMWARE_FB(FORTE_GetArrayResponseFromJSON, g_nStringIdGetArrayResponseFromJSON)

const CStringDictionary::TStringId FORTE_GetArrayResponseFromJSON::scm_anDataInputNames[] = {g_nStringIdresponse};

const CStringDictionary::TStringId FORTE_GetArrayResponseFromJSON::scm_anDataInputTypeIds[] = {g_nStringIdSTRING};

const CStringDictionary::TStringId FORTE_GetArrayResponseFromJSON::scm_anDataOutputNames[] = {g_nStringIdoutput};

const CStringDictionary::TStringId FORTE_GetArrayResponseFromJSON::scm_anDataOutputTypeIds[] = {g_nStringIdANY};

const TForteInt16 FORTE_GetArrayResponseFromJSON::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_GetArrayResponseFromJSON::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_GetArrayResponseFromJSON::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_GetArrayResponseFromJSON::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_GetArrayResponseFromJSON::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_GetArrayResponseFromJSON::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_GetArrayResponseFromJSON::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


bool FORTE_GetArrayResponseFromJSON::isResponseEmpty(char* paText) {
  while(']' != *paText) {
    if(' ' != *paText) {
      return false;
    }
    paText++;
  }
  return true;
}

void FORTE_GetArrayResponseFromJSON::executeEvent(int paEIID) {
  if(scm_nEventREQID == paEIID && CIEC_ANY::e_ARRAY == output().getDataTypeID() && CIEC_ANY::e_STRUCT == output_Array()[0]->getDataTypeID()) {
    //clean the output first
    output_Array().setup(output_Array().size(), static_cast<CIEC_STRUCT*>(output_Array()[0])->getStructTypeNameID());

    DEVLOG_DEBUG("[Arrowhead GetArrayResponseFromJSON]: Response received: %s\n", response().getValue());
    char* helper = strrchr(response().getValue(), ']');
    if(0 != helper) {

      *(helper + 1) = '\0';

      helper = strchr(response().getValue(), '[');
      if(0 != helper) {
        if(!isResponseEmpty(helper + 1)) {
          ArrowheadJSONHelper::transformJSONToStruct(helper);
          output().fromString(helper);
        } //if response is empty, don't do anything.
      } else {
        DEVLOG_ERROR("[Arrowhead GetArrayResponseFromJSON]: Invalid response, ] was found but not [: \n", response().getValue());
      }
    } else {
      DEVLOG_ERROR("[Arrowhead GetArrayResponseFromJSON]: Invalid response, no ] was found: %s\n", response().getValue());
    }
    sendOutputEvent(scm_nEventCNFID);
  }
}




