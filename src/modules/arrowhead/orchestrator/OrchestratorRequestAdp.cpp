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

#include "OrchestratorRequestAdp.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "OrchestratorRequestAdp_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_OrchestratorRequestAdp, g_nStringIdOrchestratorRequestAdp)

const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmDataInputNames[] = {g_nStringIdorchestrationResponse};

const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmDataInputTypeIds[] = {g_nStringIdARRAY, 10, g_nStringIdOrchestrationForm};

const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmDataOutputNames[] = {g_nStringIdserviceRequestForm, g_nStringIdendpoint};

const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmDataOutputTypeIds[] = {g_nStringIdServiceRequestForm, g_nStringIdWSTRING};

const TDataIOID FORTE_OrchestratorRequestAdp::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_OrchestratorRequestAdp::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmEventInputNames[] = {g_nStringIdresponseReceived};

const TDataIOID FORTE_OrchestratorRequestAdp::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_OrchestratorRequestAdp::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_OrchestratorRequestAdp::scmEventOutputNames[] = {g_nStringIdrequestOrchestator};

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



