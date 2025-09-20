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

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_OrchestrationForm2OrchestrationForm, "OrchestrationForm2OrchestrationForm"_STRID)

namespace {
  const auto cDataInputNames = std::array{"IN"_STRID};

  "OrchestrationForm"_STRID
};

const auto cDataOutputNames = std::array{"OUT"_STRID};

"OrchestrationForm"_STRID
}
;

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
}

void FORTE_OrchestrationForm2OrchestrationForm::executeEvent(TEventID paEIID,
                                                             CEventChainExecutionThread *const paECET) {
  if (scmEventREQID == paEIID) {
    st_OUT() = st_IN();
    sendOutputEvent(scmEventCNFID, paECET);
  }
}
