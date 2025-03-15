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

#include "OrchestratorRequestAdp_adp.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(endpoint);
USE_STRING_ID(OrchestrationForm);
USE_STRING_ID(orchestrationResponse);
USE_STRING_ID(OrchestratorRequestAdp);
USE_STRING_ID(requestOrchestator);
USE_STRING_ID(responseReceived);
USE_STRING_ID(serviceRequestForm);
USE_STRING_ID(ServiceRequestForm);
USE_STRING_ID(WSTRING);


DEFINE_ADAPTER_TYPE(FORTE_OrchestratorRequestAdp, STRID(OrchestratorRequestAdp))

const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmDataInputNames[] = {STRID(orchestrationResponse)};

const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmDataInputTypeIds[] = {STRID(ARRAY), 10, STRID(OrchestrationForm)};

const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmDataOutputNames[] = {STRID(serviceRequestForm), STRID(endpoint)};

const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmDataOutputTypeIds[] = {STRID(ServiceRequestForm), STRID(WSTRING)};

const TDataIOID FORTE_OrchestratorRequestAdp::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_OrchestratorRequestAdp::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmEventInputNames[] = {STRID(responseReceived)};

const TDataIOID FORTE_OrchestratorRequestAdp::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_OrchestratorRequestAdp::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmEventOutputNames[] = {STRID(requestOrchestator)};

const SFBInterfaceSpec FORTE_OrchestratorRequestAdp::scmFBInterfaceSpecSocket = {
  1,  scmEventInputNames,  scmEIWith,  scmEIWithIndexes,
  1,  scmEventOutputNames,  scmEOWith, scmEOWithIndexes,  1,  scmDataInputNames, scmDataInputTypeIds,
  2,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_OrchestratorRequestAdp::scmFBInterfaceSpecPlug = {
  1,  scmEventOutputNames,  scmEOWith,  scmEOWithIndexes,
  1,  scmEventInputNames,  scmEIWith, scmEIWithIndexes,  2,  scmDataOutputNames, scmDataOutputTypeIds,
  1,  scmDataInputNames, scmDataInputTypeIds,
  0, 0
};



