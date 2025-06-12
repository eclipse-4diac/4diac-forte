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

namespace {
  const auto cDataInputNames = std::array{STRID(input)};
  
  
  const auto cDataOutputNames = std::array{STRID(output)};
  
  
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


void FORTE_ANYToJSON::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if (scmEventREQID == paEIID) {
    output() = "";
    ArrowheadJSONHelper::transformANYToJSON(input(), output());
    sendOutputEvent(scmEventCNFID, paECET);
  }
}
