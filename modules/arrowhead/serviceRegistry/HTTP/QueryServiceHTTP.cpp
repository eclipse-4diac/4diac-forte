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

#include "QueryServiceHTTP.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_QueryServiceHTTP, "QueryServiceHTTP"_STRID)

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

const SCFB_FBInstanceData FORTE_QueryServiceHTTP::scmInternalFBs[] = {
    {"APPEND_STRING_3"_STRID, "APPEND_STRING_3"_STRID},
    {"Query"_STRID, "CLIENT_1_2"_STRID},
    {"F_STRING_TO_WSTRING"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"InitFlag"_STRID, "E_SR"_STRID},
    {"E_PERMIT"_STRID, "E_PERMIT"_STRID},
    {"ANYToJSON"_STRID, "ANYToJSON"_STRID},
    {"GetHTTPResult"_STRID, "GetArrayResponseFromJSON"_STRID},
};

const SCFB_FBParameter FORTE_QueryServiceHTTP::scmParamters[] = {
    {0, "IN_1"_STRID, "STRING#http["},
    {0, "IN_3"_STRID, "STRING#/query; PUT; application/json]"},
};

const SCFB_FBConnectionData FORTE_QueryServiceHTTP::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {"APPEND_STRING_3"_STRID, "CNF"_STRID), 0,
     "F_STRING_TO_WSTRING"_STRID, "REQ"_STRID), 2},
    {"F_STRING_TO_WSTRING"_STRID, "CNF"_STRID), 2,
     "InitFlag"_STRID, "S"_STRID), 3},
    {"InitFlag"_STRID, "EO"_STRID), 3,
     "Query"_STRID, "INIT"_STRID), 1},
    {"Query"_STRID, "INITO"_STRID), 1,
     "E_PERMIT"_STRID, "EI"_STRID), 4},
    {"E_PERMIT"_STRID, "EO"_STRID), 4,
     "Query"_STRID, "REQ"_STRID), 1},
    {"Query"_STRID, "CNF"_STRID), 1,
     "InitFlag"_STRID, "R"_STRID), 3},
    {"ANYToJSON"_STRID, "CNF"_STRID), 5,
     "APPEND_STRING_3"_STRID, "REQ"_STRID), 0},
    {"queryServices"_STRID, "query"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "ANYToJSON"_STRID, "REQ"_STRID), 5},
    {"GetHTTPResult"_STRID, "CNF"_STRID), 6,
     "queryServices"_STRID, "queried"_STRID), CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBFannedOutConnectionData FORTE_QueryServiceHTTP::scmFannedOutEventConnections[] = {
    {5, "GetHTTPResult"_STRID, "REQ"_STRID), 6},
};

const SCFB_FBConnectionData FORTE_QueryServiceHTTPauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {"APPEND_STRING_3"_STRID, "OUT"_STRID), 0,
     "F_STRING_TO_WSTRING"_STRID, "IN"_STRID), 2},
    {"F_STRING_TO_WSTRING"_STRID, "OUT"_STRID), 2,
     "Query"_STRID, "ID"_STRID), 1},
    {"InitFlag"_STRID, "Q"_STRID), 3,
     "E_PERMIT"_STRID, "PERMIT"_STRID), 4},
    {"ANYToJSON"_STRID, "output"_STRID), 5,
     "Query"_STRID, "SD_1"_STRID), 1},
    {"queryServices"_STRID, "serviceQueryForm"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "ANYToJSON"_STRID, "input"_STRID), 5},
    {"queryServices"_STRID, "endpoint"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "APPEND_STRING_3"_STRID, "IN_2"_STRID), 0},
    {"Query"_STRID, "RD_2"_STRID), 1,
     "GetHTTPResult"_STRID, "response"_STRID), 6},
    {"GetHTTPResult"_STRID, "output"_STRID), 6,
     "queryServices"_STRID, "serviceEntries"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBFannedOutConnectionData FORTE_QueryServiceHTTP::scmFannedOutDataConnections[] = {
    {2, "Query"_STRID, "QI"_STRID), 1},
};

const SCFB_FBNData FORTE_QueryServiceHTTP::scmFBNData = {
    7, scmInternalFBs,     9, scmEventConnections,         1, scmFannedOutEventConnections,
    8, scmDataConnections, 1, scmFannedOutDataConnections, 0, nullptr,
    2, scmParamters};
