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

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_RequestOrchestrationHTTP, "RequestOrchestrationHTTP"_STRID)

namespace {
  const auto cSocketNameIds = std::array{"requestOrchestration"_STRID};

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
    {"APPEND_STRING_3"_STRID, "APPEND_STRING_3"_STRID},
    {"Request"_STRID, "CLIENT_1_2"_STRID},
    {"InitFlag"_STRID, "E_SR"_STRID},
    {"E_PERMIT"_STRID, "E_PERMIT"_STRID},
    {"F_STRING_TO_WSTRING"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"ANYToJSON"_STRID, "ANYToJSON"_STRID},
    {"GetHTTPOrchestrationForm"_STRID, "GetArrayResponseFromJSON"_STRID},
};

const SCFB_FBParameter FORTE_RequestOrchestrationHTTP::scmParamters[] = {
    {0, "IN_1"_STRID, "STRING#http["},
    {0, "IN_3"_STRID, "STRING#/orchestration; POST; application/json]"},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationHTTP::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {"APPEND_STRING_3"_STRID, "CNF"_STRID), 0,
     "F_STRING_TO_WSTRING"_STRID, "REQ"_STRID), 4},
    {"F_STRING_TO_WSTRING"_STRID, "CNF"_STRID), 4,
     "InitFlag"_STRID, "S"_STRID), 2},
    {"InitFlag"_STRID, "EO"_STRID), 2,
     "Request"_STRID, "INIT"_STRID), 1},
    {"E_PERMIT"_STRID, "EO"_STRID), 3,
     "Request"_STRID, "REQ"_STRID), 1},
    {"ANYToJSON"_STRID, "CNF"_STRID), 5,
     "APPEND_STRING_3"_STRID, "REQ"_STRID), 0},
    {"Request"_STRID, "CNF"_STRID), 1,
     "InitFlag"_STRID, "R"_STRID), 2},
    {"Request"_STRID, "INITO"_STRID), 1,
     "E_PERMIT"_STRID, "EI"_STRID), 3},
    {"requestOrchestration"_STRID, "requestOrchestator"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "ANYToJSON"_STRID, "REQ"_STRID), 5},
    {"GetHTTPOrchestrationForm"_STRID, "CNF"_STRID), 6,
     "requestOrchestration"_STRID, "responseReceived"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationHTTP::scmFannedOutEventConnections[] = {
    {5, "GetHTTPOrchestrationForm"_STRID, "REQ"_STRID), 6},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationHTTPauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {"APPEND_STRING_3"_STRID, "OUT"_STRID), 0,
     "F_STRING_TO_WSTRING"_STRID, "IN"_STRID), 4},
    {"F_STRING_TO_WSTRING"_STRID, "OUT"_STRID), 4,
     "Request"_STRID, "ID"_STRID), 1},
    {"InitFlag"_STRID, "Q"_STRID), 2,
     "E_PERMIT"_STRID, "PERMIT"_STRID), 3},
    {"ANYToJSON"_STRID, "output"_STRID), 5,
     "Request"_STRID, "SD_1"_STRID), 1},
    {"requestOrchestration"_STRID, "serviceRequestForm"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "ANYToJSON"_STRID, "input"_STRID), 5},
    {"requestOrchestration"_STRID, "endpoint"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "APPEND_STRING_3"_STRID, "IN_2"_STRID), 0},
    {"Request"_STRID, "RD_2"_STRID), 1,
     "GetHTTPOrchestrationForm"_STRID, "response"_STRID), 6},
    {"GetHTTPOrchestrationForm"_STRID, "output"_STRID), 6,
     "requestOrchestration"_STRID, "orchestrationResponse"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationHTTP::scmFannedOutDataConnections[] = {
    {2, "Request"_STRID, "QI"_STRID), 1},
};

const SCFB_FBNData FORTE_RequestOrchestrationHTTP::scmFBNData = {
    7, scmInternalFBs,     9, scmEventConnections,         1, scmFannedOutEventConnections,
    8, scmDataConnections, 1, scmFannedOutDataConnections, 0, nullptr,
    2, scmParamters};
