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

#include "PublishEventHTTP.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_PublishEventHTTP, "PublishEventHTTP"_STRID)

namespace {
  const auto cSocketNameIds = std::array{"publishEvent"_STRID};

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

const SCFB_FBInstanceData FORTE_PublishEventHTTP::scmInternalFBs[] = {
    {"ANYToJSON"_STRID, "ANYToJSON"_STRID},
    {"InitFlag"_STRID, "E_SR"_STRID},
    {"APPEND_STRING_3"_STRID, "APPEND_STRING_3"_STRID},
    {"F_STRING_TO_WSTRING"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"Query"_STRID, "CLIENT_1_2"_STRID},
    {"E_PERMIT"_STRID, "E_PERMIT"_STRID},
};

const SCFB_FBParameter FORTE_PublishEventHTTP::scmParamters[] = {
    {2, "IN_1"_STRID, "STRING#http["},
    {2, "IN_3"_STRID, "STRING#/publish; POST; application/json]"},
};

const SCFB_FBConnectionData FORTE_PublishEventHTTP::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {"E_PERMIT"_STRID, "EO"_STRID), 5,
     "Query"_STRID, "REQ"_STRID), 4},
    {"APPEND_STRING_3"_STRID, "CNF"_STRID), 2,
     "F_STRING_TO_WSTRING"_STRID, "REQ"_STRID), 3},
    {"InitFlag"_STRID, "EO"_STRID), 1,
     "Query"_STRID, "INIT"_STRID), 4},
    {"ANYToJSON"_STRID, "CNF"_STRID), 0,
     "APPEND_STRING_3"_STRID, "REQ"_STRID), 2},
    {"F_STRING_TO_WSTRING"_STRID, "CNF"_STRID), 3,
     "InitFlag"_STRID, "S"_STRID), 1},
    {"Query"_STRID, "INITO"_STRID), 4,
     "E_PERMIT"_STRID, "EI"_STRID), 5},
    {"Query"_STRID, "CNF"_STRID), 4,
     "InitFlag"_STRID, "R"_STRID), 1},
    {"publishEvent"_STRID, "publish"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "ANYToJSON"_STRID, "REQ"_STRID), 0},
};

const SCFB_FBFannedOutConnectionData FORTE_PublishEventHTTP::scmFannedOutEventConnections[] = {
    {6, "publishEvent"_STRID, "published"_STRID), CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBConnectionData FORTE_PublishEventHTTPauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {"ANYToJSON"_STRID, "output"_STRID), 0,
     "Query"_STRID, "SD_1"_STRID), 4},
    {"InitFlag"_STRID, "Q"_STRID), 1,
     "Query"_STRID, "QI"_STRID), 4},
    {"APPEND_STRING_3"_STRID, "OUT"_STRID), 2,
     "F_STRING_TO_WSTRING"_STRID, "IN"_STRID), 3},
    {"F_STRING_TO_WSTRING"_STRID, "OUT"_STRID), 3,
     "Query"_STRID, "ID"_STRID), 4},
    {"publishEvent"_STRID, "publishEvent"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "ANYToJSON"_STRID, "input"_STRID), 0},
    {"publishEvent"_STRID, "endpoint"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "APPEND_STRING_3"_STRID, "IN_2"_STRID), 2},
};

const SCFB_FBFannedOutConnectionData FORTE_PublishEventHTTP::scmFannedOutDataConnections[] = {
    {1, "E_PERMIT"_STRID, "PERMIT"_STRID), 5},
};

const SCFB_FBNData FORTE_PublishEventHTTP::scmFBNData = {
    6, scmInternalFBs,     8, scmEventConnections,         1, scmFannedOutEventConnections,
    6, scmDataConnections, 1, scmFannedOutDataConnections, 0, nullptr,
    2, scmParamters};
