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

#include "RequestOrchestrationForm.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_RequestOrchestrationForm, "RequestOrchestrationForm"_STRID)

namespace {
  const auto cDataInputNames = std::array{"serviceRequestForm"_STRID, "endpoint"_STRID};
  const auto cDataOutputNames = std::array{"orchestrationResponse"_STRID};
  const auto cEventInputNames = std::array{"requestOrchestator"_STRID};
  const auto cEventInputTypeIds = std::array{"Event"_STRID};
  const auto cEventOutputNames = std::array{"responseReceived"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID};
  const auto cPlugNameIds = std::array{"requestOrchestration"_STRID};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = cPlugNameIds,
  };
} // namespace

const SCFB_FBConnectionData FORTE_RequestOrchestrationForm::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {{}, "requestOrchestator"_STRID, -1,
     "requestOrchestration"_STRID, "requestOrchestator"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {"requestOrchestration"_STRID, "responseReceived"_STRID),
     CCompositeFB::scmAdapterMarker | 0, {}, "responseReceived"_STRID, -1},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationFormauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {{}, "serviceRequestForm"_STRID, -1,
     "requestOrchestration"_STRID, "serviceRequestForm"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {"requestOrchestration"_STRID, "orchestrationResponse"_STRID),
     CCompositeFB::scmAdapterMarker | 0, {}, "orchestrationResponse"_STRID, -1},
    {{}, "endpoint"_STRID, -1,
     "requestOrchestration"_STRID, "endpoint"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBNData FORTE_RequestOrchestrationForm::scmFBNData = {
    0, 0, 2, scmEventConnections, 0, 0, 3, scmDataConnections, 0, 0, 0, nullptr, 0, 0};
