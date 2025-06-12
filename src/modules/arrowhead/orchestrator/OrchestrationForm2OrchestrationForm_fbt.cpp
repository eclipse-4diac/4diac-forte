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

namespace {
  const auto cDataInputNames = std::array{STRID(IN)};
  
      STRID(OrchestrationForm)};
  
  const auto cDataOutputNames = std::array{STRID(OUT)};
  
      STRID(OrchestrationForm)};
  
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


void FORTE_OrchestrationForm2OrchestrationForm::executeEvent(TEventID paEIID,
                                                             CEventChainExecutionThread *const paECET) {
  if (scmEventREQID == paEIID) {
    st_OUT() = st_IN();
    sendOutputEvent(scmEventCNFID, paECET);
  }
}
