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

#include "GetArrayResponseFromJSON_fbt.h"

using namespace forte::literals;

#include "ArrowheadJSONHelper.h"

DEFINE_FIRMWARE_FB(FORTE_GetArrayResponseFromJSON, "GetArrayResponseFromJSON"_STRID)

namespace {
  const auto cDataInputNames = std::array{"response"_STRID};

  const auto cDataOutputNames = std::array{"output"_STRID};

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
} // namespace

bool FORTE_GetArrayResponseFromJSON::isResponseEmpty(char *paText) {
  while (']' != *paText) {
    if (' ' != *paText) {
      return false;
    }
    paText++;
  }
  return true;
}

void FORTE_GetArrayResponseFromJSON::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if (scmEventREQID == paEIID && CIEC_ANY::e_ARRAY == output().getDataTypeID() &&
      CIEC_ANY::e_STRUCT == output_Array()[0]->getDataTypeID()) {
    // clean the output first
    output_Array().setup(output_Array().size(), static_cast<CIEC_STRUCT *>(output_Array()[0])->getStructTypeNameID());

    DEVLOG_DEBUG("[Arrowhead GetArrayResponseFromJSON]: Response received: %s\n", response().getValue());
    char *helper = strrchr(response().getValue(), ']');
    if (0 != helper) {

      *(helper + 1) = '\0';

      helper = strchr(response().getValue(), '[');
      if (0 != helper) {
        if (!isResponseEmpty(helper + 1)) {
          ArrowheadJSONHelper::transformJSONToStruct(helper);
          output().fromString(helper);
        } // if response is empty, don't do anything.
      } else {
        DEVLOG_ERROR("[Arrowhead GetArrayResponseFromJSON]: Invalid response, ] was found but not [: \n",
                     response().getValue());
      }
    } else {
      DEVLOG_ERROR("[Arrowhead GetArrayResponseFromJSON]: Invalid response, no ] was found: %s\n",
                   response().getValue());
    }
    sendOutputEvent(scmEventCNFID, paECET);
  }
}
