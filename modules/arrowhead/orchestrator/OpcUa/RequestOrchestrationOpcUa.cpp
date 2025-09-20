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

#include "RequestOrchestrationOpcUa.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_RequestOrchestrationOpcUa, "RequestOrchestrationOpcUa"_STRID)

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

const SCFB_FBInstanceData FORTE_RequestOrchestrationOpcUa::scmInternalFBs[] = {
    {"APPEND_STRING_3"_STRID, "APPEND_STRING_3"_STRID},
    {"InitFlag"_STRID, "E_SR"_STRID},
    {"E_PERMIT"_STRID, "E_PERMIT"_STRID},
    {"F_STRING_TO_WSTRING"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"ANYToJSON"_STRID, "ANYToJSON"_STRID},
    {"GetHTTPOrchestrationForm"_STRID, "GetArrayResponseFromJSON"_STRID},
    {"Request"_STRID, "CLIENT_1"_STRID},
    {"E_SWITCH"_STRID, "E_SWITCH"_STRID},
    {"E_SWITCH_1"_STRID, "E_SWITCH"_STRID},
    {"E_CTU"_STRID, "E_CTU"_STRID},
    {"E_DELAY"_STRID, "E_DELAY"_STRID},
};

const SCFB_FBParameter FORTE_RequestOrchestrationOpcUa::scmParamters[] = {
    {0, "IN_1"_STRID, "STRING#opc_ua[CALL_METHOD; "},
    {0, "IN_3"_STRID, "STRING##; /Objects/2:Orchestrator/2:orchestration]"},
    {9, "PV"_STRID, "UINT#3"},
    {10, "DT"_STRID, "T#250ms"},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationOpcUa::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {"APPEND_STRING_3"_STRID, "CNF"_STRID), 0,
     "F_STRING_TO_WSTRING"_STRID, "REQ"_STRID), 3},
    {"F_STRING_TO_WSTRING"_STRID, "CNF"_STRID), 3,
     "InitFlag"_STRID, "S"_STRID), 1},
    {"ANYToJSON"_STRID, "CNF"_STRID), 4,
     "APPEND_STRING_3"_STRID, "REQ"_STRID), 0},
    {"requestOrchestration"_STRID, "requestOrchestator"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "ANYToJSON"_STRID, "REQ"_STRID), 4},
    {"GetHTTPOrchestrationForm"_STRID, "CNF"_STRID), 5,
     "requestOrchestration"_STRID, "responseReceived"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {"InitFlag"_STRID, "EO"_STRID), 1,
     "Request"_STRID, "INIT"_STRID), 6},
    {"E_PERMIT"_STRID, "EO"_STRID), 2,
     "Request"_STRID, "REQ"_STRID), 6},
    {"Request"_STRID, "INITO"_STRID), 6,
     "E_PERMIT"_STRID, "EI"_STRID), 2},
    {"E_SWITCH"_STRID, "EO1"_STRID), 7,
     "GetHTTPOrchestrationForm"_STRID, "REQ"_STRID), 5},
    {"Request"_STRID, "CNF"_STRID), 6,
     "E_SWITCH"_STRID, "EI"_STRID), 7},
    {"E_SWITCH"_STRID, "EO0"_STRID), 7,
     "E_CTU"_STRID, "CU"_STRID), 9},
    {"E_DELAY"_STRID, "EO"_STRID), 10,
     "Request"_STRID, "REQ"_STRID), 6},
    {"E_CTU"_STRID, "CUO"_STRID), 9,
     "E_SWITCH_1"_STRID, "EI"_STRID), 8},
    {"E_SWITCH_1"_STRID, "EO0"_STRID), 8,
     "E_DELAY"_STRID, "START"_STRID), 10},
    {"E_SWITCH_1"_STRID, "EO1"_STRID), 8,
     "InitFlag"_STRID, "R"_STRID), 1},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationOpcUa::scmFannedOutEventConnections[] = {
    {8, "InitFlag"_STRID, "R"_STRID), 1},
    {8, "E_CTU"_STRID, "R"_STRID), 9},
    {14, "GetHTTPOrchestrationForm"_STRID, "REQ"_STRID), 5},
    {14, "E_CTU"_STRID, "R"_STRID), 9},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationOpcUaauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {"APPEND_STRING_3"_STRID, "OUT"_STRID), 0,
     "F_STRING_TO_WSTRING"_STRID, "IN"_STRID), 3},
    {"InitFlag"_STRID, "Q"_STRID), 1,
     "E_PERMIT"_STRID, "PERMIT"_STRID), 2},
    {"requestOrchestration"_STRID, "serviceRequestForm"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "ANYToJSON"_STRID, "input"_STRID), 4},
    {"requestOrchestration"_STRID, "endpoint"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "APPEND_STRING_3"_STRID, "IN_2"_STRID), 0},
    {"GetHTTPOrchestrationForm"_STRID, "output"_STRID), 5,
     "requestOrchestration"_STRID, "orchestrationResponse"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {"F_STRING_TO_WSTRING"_STRID, "OUT"_STRID), 3,
     "Request"_STRID, "ID"_STRID), 6},
    {"ANYToJSON"_STRID, "output"_STRID), 4,
     "Request"_STRID, "SD_1"_STRID), 6},
    {"Request"_STRID, "RD_1"_STRID), 6,
     "GetHTTPOrchestrationForm"_STRID, "response"_STRID), 5},
    {"Request"_STRID, "QO"_STRID), 6,
     "E_SWITCH"_STRID, "G"_STRID), 7},
    {"E_CTU"_STRID, "Q"_STRID), 9,
     "E_SWITCH_1"_STRID, "G"_STRID), 8},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationOpcUa::scmFannedOutDataConnections[] = {
    {1, "Request"_STRID, "QI"_STRID), 6},
};

const SCFB_FBNData FORTE_RequestOrchestrationOpcUa::scmFBNData = {
    11, scmInternalFBs,     15, scmEventConnections,         4, scmFannedOutEventConnections,
    10, scmDataConnections, 1,  scmFannedOutDataConnections, 0, nullptr,
    4,  scmParamters};
