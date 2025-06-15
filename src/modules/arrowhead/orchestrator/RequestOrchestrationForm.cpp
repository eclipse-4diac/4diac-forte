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

USE_STRING_ID(ARRAY);
USE_STRING_ID(endpoint);
USE_STRING_ID(Event);
USE_STRING_ID(OrchestrationForm);
USE_STRING_ID(orchestrationResponse);
USE_STRING_ID(OrchestratorRequestAdp);
USE_STRING_ID(requestOrchestator);
USE_STRING_ID(requestOrchestration);
USE_STRING_ID(RequestOrchestrationForm);
USE_STRING_ID(responseReceived);
USE_STRING_ID(serviceRequestForm);
USE_STRING_ID(ServiceRequestForm);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_RequestOrchestrationForm, STRID(RequestOrchestrationForm))

namespace {
  const auto cDataInputNames = std::array{STRID(serviceRequestForm), STRID(endpoint)};
  const auto cDataOutputNames = std::array{STRID(orchestrationResponse)};
  const auto cEventInputNames = std::array{STRID(requestOrchestator)};
  const auto cEventInputTypeIds = std::array{STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(responseReceived)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};
  const auto cPlugNameIds = std::array{STRID(requestOrchestration)};

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
    {CStringDictionary::scmInvalidStringId, STRID(requestOrchestator), -1,
     STRID(requestOrchestration), STRID(requestOrchestator)),
     CCompositeFB::scmAdapterMarker | 0},
    {STRID(requestOrchestration), STRID(responseReceived)),
     CCompositeFB::scmAdapterMarker | 0, CStringDictionary::scmInvalidStringId, STRID(responseReceived), -1},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationFormauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {CStringDictionary::scmInvalidStringId, STRID(serviceRequestForm), -1,
     STRID(requestOrchestration), STRID(serviceRequestForm)),
     CCompositeFB::scmAdapterMarker | 0},
    {STRID(requestOrchestration), STRID(orchestrationResponse)),
     CCompositeFB::scmAdapterMarker | 0, CStringDictionary::scmInvalidStringId, STRID(orchestrationResponse), -1},
    {CStringDictionary::scmInvalidStringId, STRID(endpoint), -1,
     STRID(requestOrchestration), STRID(endpoint)),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBNData FORTE_RequestOrchestrationForm::scmFBNData = {
    0, 0, 2, scmEventConnections, 0, 0, 3, scmDataConnections, 0, 0, 0, nullptr, 0, 0};
