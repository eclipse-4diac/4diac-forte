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
#include "QueryServiceOpcUa.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_QueryServiceOpcUa, "QueryServiceOpcUa"_STRID)

namespace {
  const auto cSocketNameIds = std::array{"queryServices"_STRID};

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

const SCFB_FBInstanceData FORTE_QueryServiceOpcUa::scmInternalFBs[] = {
    {"APPEND_STRING_3"_STRID, "APPEND_STRING_3"_STRID},
    {"F_STRING_TO_WSTRING"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"InitFlag"_STRID, "E_SR"_STRID},
    {"E_PERMIT"_STRID, "E_PERMIT"_STRID},
    {"ANYToJSON"_STRID, "ANYToJSON"_STRID},
    {"GetHTTPResult"_STRID, "GetArrayResponseFromJSON"_STRID},
    {"Query"_STRID, "CLIENT_1"_STRID},
    {"E_SWITCH_1"_STRID, "E_SWITCH"_STRID},
    {"E_SWITCH"_STRID, "E_SWITCH"_STRID},
    {"E_DELAY"_STRID, "E_DELAY"_STRID},
    {"E_CTU"_STRID, "E_CTU"_STRID},
};

const SCFB_FBParameter FORTE_QueryServiceOpcUa::scmParamters[] = {
    {0, "IN_1"_STRID, "STRING#opc_ua[CALL_METHOD; "},
    {0, "IN_3"_STRID, "STRING##; /Objects/2:ServiceRegistry/2:query]"},
    {9, "DT"_STRID, "T#250ms"},
    {10, "PV"_STRID, "UINT#3"},
};

const SCFB_FBConnectionData FORTE_QueryServiceOpcUa::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {"APPEND_STRING_3"_STRID, "CNF"_STRID), 0,
     "F_STRING_TO_WSTRING"_STRID, "REQ"_STRID), 1},
    {"F_STRING_TO_WSTRING"_STRID, "CNF"_STRID), 1,
     "InitFlag"_STRID, "S"_STRID), 2},
    {"ANYToJSON"_STRID, "CNF"_STRID), 4,
     "APPEND_STRING_3"_STRID, "REQ"_STRID), 0},
    {"queryServices"_STRID, "query"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "ANYToJSON"_STRID, "REQ"_STRID), 4},
    {"GetHTTPResult"_STRID, "CNF"_STRID), 5,
     "queryServices"_STRID, "queried"_STRID), CCompositeFB::scmAdapterMarker | 0},
    {"InitFlag"_STRID, "EO"_STRID), 2,
     "Query"_STRID, "INIT"_STRID), 6},
    {"E_PERMIT"_STRID, "EO"_STRID), 3,
     "Query"_STRID, "REQ"_STRID), 6},
    {"Query"_STRID, "INITO"_STRID), 6,
     "E_PERMIT"_STRID, "EI"_STRID), 3},
    {"E_SWITCH_1"_STRID, "EO0"_STRID), 7,
     "E_DELAY"_STRID, "START"_STRID), 9},
    {"E_CTU"_STRID, "CUO"_STRID), 10,
     "E_SWITCH_1"_STRID, "EI"_STRID), 7},
    {"E_SWITCH"_STRID, "EO0"_STRID), 8,
     "E_CTU"_STRID, "CU"_STRID), 10},
    {"E_SWITCH"_STRID, "EO1"_STRID), 8,
     "GetHTTPResult"_STRID, "REQ"_STRID), 5},
    {"Query"_STRID, "CNF"_STRID), 6,
     "E_SWITCH"_STRID, "EI"_STRID), 8},
    {"E_SWITCH_1"_STRID, "EO1"_STRID), 7,
     "InitFlag"_STRID, "R"_STRID), 2},
    {"E_DELAY"_STRID, "EO"_STRID), 9,
     "Query"_STRID, "REQ"_STRID), 6},
};

const SCFB_FBFannedOutConnectionData FORTE_QueryServiceOpcUa::scmFannedOutEventConnections[] = {
    {11, "InitFlag"_STRID, "R"_STRID), 2},
    {11, "E_CTU"_STRID, "R"_STRID), 10},
    {13, "GetHTTPResult"_STRID, "REQ"_STRID), 5},
    {13, "E_CTU"_STRID, "R"_STRID), 10},
};

const SCFB_FBConnectionData FORTE_QueryServiceOpcUaauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {"APPEND_STRING_3"_STRID, "OUT"_STRID), 0,
     "F_STRING_TO_WSTRING"_STRID, "IN"_STRID), 1},
    {"InitFlag"_STRID, "Q"_STRID), 2,
     "E_PERMIT"_STRID, "PERMIT"_STRID), 3},
    {"queryServices"_STRID, "serviceQueryForm"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "ANYToJSON"_STRID, "input"_STRID), 4},
    {"queryServices"_STRID, "endpoint"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "APPEND_STRING_3"_STRID, "IN_2"_STRID), 0},
    {"GetHTTPResult"_STRID, "output"_STRID), 5,
     "queryServices"_STRID, "serviceEntries"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {"F_STRING_TO_WSTRING"_STRID, "OUT"_STRID), 1,
     "Query"_STRID, "ID"_STRID), 6},
    {"ANYToJSON"_STRID, "output"_STRID), 4,
     "Query"_STRID, "SD_1"_STRID), 6},
    {"Query"_STRID, "RD_1"_STRID), 6,
     "GetHTTPResult"_STRID, "response"_STRID), 5},
    {"E_CTU"_STRID, "Q"_STRID), 10,
     "E_SWITCH_1"_STRID, "G"_STRID), 7},
    {"Query"_STRID, "QO"_STRID), 6,
     "E_SWITCH"_STRID, "G"_STRID), 8},
};

const SCFB_FBFannedOutConnectionData FORTE_QueryServiceOpcUa::scmFannedOutDataConnections[] = {
    {1, "Query"_STRID, "QI"_STRID), 6},
};

const SCFB_FBNData FORTE_QueryServiceOpcUa::scmFBNData = {
    11, scmInternalFBs,     15, scmEventConnections,         4, scmFannedOutEventConnections,
    10, scmDataConnections, 1,  scmFannedOutDataConnections, 0, nullptr,
    4,  scmParamters};
