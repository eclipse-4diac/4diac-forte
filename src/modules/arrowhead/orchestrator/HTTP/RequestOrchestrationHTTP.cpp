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

#include "RequestOrchestrationHTTP.h"

USE_STRING_ID(ANYToJSON);
USE_STRING_ID(APPEND_STRING_3);
USE_STRING_ID(CLIENT_1_2);
USE_STRING_ID(CNF);
USE_STRING_ID(EI);
USE_STRING_ID(endpoint);
USE_STRING_ID(EO);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_SR);
USE_STRING_ID(F_STRING_TO_WSTRING);
USE_STRING_ID(GetArrayResponseFromJSON);
USE_STRING_ID(GetHTTPOrchestrationForm);
USE_STRING_ID(ID);
USE_STRING_ID(IN);
USE_STRING_ID(IN_1);
USE_STRING_ID(IN_2);
USE_STRING_ID(IN_3);
USE_STRING_ID(INIT);
USE_STRING_ID(InitFlag);
USE_STRING_ID(INITO);
USE_STRING_ID(input);
USE_STRING_ID(orchestrationResponse);
USE_STRING_ID(OrchestratorRequestAdp);
USE_STRING_ID(OUT);
USE_STRING_ID(output);
USE_STRING_ID(PERMIT);
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(R);
USE_STRING_ID(RD_2);
USE_STRING_ID(REQ);
USE_STRING_ID(Request);
USE_STRING_ID(requestOrchestator);
USE_STRING_ID(requestOrchestration);
USE_STRING_ID(RequestOrchestrationHTTP);
USE_STRING_ID(response);
USE_STRING_ID(responseReceived);
USE_STRING_ID(S);
USE_STRING_ID(SD_1);
USE_STRING_ID(serviceRequestForm);

DEFINE_FIRMWARE_FB(FORTE_RequestOrchestrationHTTP, STRID(RequestOrchestrationHTTP))

namespace {
  const auto cSocketNameIds = std::array{STRID(requestOrchestration)};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = {},
      .mEITypeNames = {},
      .mEONames = {},
      .mEOTypeNames = {},
      .mDINames = {},
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = cSocketNameIds,
      .mPlugNames = {},
  };
} // namespace

const SCFB_FBInstanceData FORTE_RequestOrchestrationHTTP::scmInternalFBs[] = {
    {STRID(APPEND_STRING_3), STRID(APPEND_STRING_3)},
    {STRID(Request), STRID(CLIENT_1_2)},
    {STRID(InitFlag), STRID(E_SR)},
    {STRID(E_PERMIT), STRID(E_PERMIT)},
    {STRID(F_STRING_TO_WSTRING), STRID(F_STRING_TO_WSTRING)},
    {STRID(ANYToJSON), STRID(ANYToJSON)},
    {STRID(GetHTTPOrchestrationForm), STRID(GetArrayResponseFromJSON)},
};

const SCFB_FBParameter FORTE_RequestOrchestrationHTTP::scmParamters[] = {
    {0, STRID(IN_1), "STRING#http["},
    {0, STRID(IN_3), "STRING#/orchestration; POST; application/json]"},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationHTTP::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(APPEND_STRING_3), STRID(CNF)), 0,
     STRID(F_STRING_TO_WSTRING), STRID(REQ)), 4},
    {STRID(F_STRING_TO_WSTRING), STRID(CNF)), 4,
     STRID(InitFlag), STRID(S)), 2},
    {STRID(InitFlag), STRID(EO)), 2,
     STRID(Request), STRID(INIT)), 1},
    {STRID(E_PERMIT), STRID(EO)), 3,
     STRID(Request), STRID(REQ)), 1},
    {STRID(ANYToJSON), STRID(CNF)), 5,
     STRID(APPEND_STRING_3), STRID(REQ)), 0},
    {STRID(Request), STRID(CNF)), 1,
     STRID(InitFlag), STRID(R)), 2},
    {STRID(Request), STRID(INITO)), 1,
     STRID(E_PERMIT), STRID(EI)), 3},
    {STRID(requestOrchestration), STRID(requestOrchestator)),
     CCompositeFB::scmAdapterMarker | 0, STRID(ANYToJSON), STRID(REQ)), 5},
    {STRID(GetHTTPOrchestrationForm), STRID(CNF)), 6,
     STRID(requestOrchestration), STRID(responseReceived)),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationHTTP::scmFannedOutEventConnections[] = {
    {5, STRID(GetHTTPOrchestrationForm), STRID(REQ)), 6},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationHTTPauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(APPEND_STRING_3), STRID(OUT)), 0,
     STRID(F_STRING_TO_WSTRING), STRID(IN)), 4},
    {STRID(F_STRING_TO_WSTRING), STRID(OUT)), 4,
     STRID(Request), STRID(ID)), 1},
    {STRID(InitFlag), STRID(Q)), 2,
     STRID(E_PERMIT), STRID(PERMIT)), 3},
    {STRID(ANYToJSON), STRID(output)), 5,
     STRID(Request), STRID(SD_1)), 1},
    {STRID(requestOrchestration), STRID(serviceRequestForm)),
     CCompositeFB::scmAdapterMarker | 0, STRID(ANYToJSON), STRID(input)), 5},
    {STRID(requestOrchestration), STRID(endpoint)),
     CCompositeFB::scmAdapterMarker | 0, STRID(APPEND_STRING_3), STRID(IN_2)), 0},
    {STRID(Request), STRID(RD_2)), 1,
     STRID(GetHTTPOrchestrationForm), STRID(response)), 6},
    {STRID(GetHTTPOrchestrationForm), STRID(output)), 6,
     STRID(requestOrchestration), STRID(orchestrationResponse)),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationHTTP::scmFannedOutDataConnections[] = {
    {2, STRID(Request), STRID(QI)), 1},
};

const SCFB_FBNData FORTE_RequestOrchestrationHTTP::scmFBNData = {
    7, scmInternalFBs,     9, scmEventConnections,         1, scmFannedOutEventConnections,
    8, scmDataConnections, 1, scmFannedOutDataConnections, 0, nullptr,
    2, scmParamters};
