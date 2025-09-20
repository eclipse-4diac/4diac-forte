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

#include "PublishEventOpcUa.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_PublishEventOpcUa, "PublishEventOpcUa"_STRID)

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

const SCFB_FBInstanceData FORTE_PublishEventOpcUa::scmInternalFBs[] = {
    {"ANYToJSON"_STRID, "ANYToJSON"_STRID},
    {"InitFlag"_STRID, "E_SR"_STRID},
    {"APPEND_STRING_3"_STRID, "APPEND_STRING_3"_STRID},
    {"F_STRING_TO_WSTRING"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"E_PERMIT"_STRID, "E_PERMIT"_STRID},
    {"E_SWITCH_2"_STRID, "E_SWITCH"_STRID},
    {"E_SWITCH_1_1"_STRID, "E_SWITCH"_STRID},
    {"E_CTU_1"_STRID, "E_CTU"_STRID},
    {"E_DELAY_1"_STRID, "E_DELAY"_STRID},
    {"Query"_STRID, "CLIENT_1_0"_STRID},
};

const SCFB_FBParameter FORTE_PublishEventOpcUa::scmParamters[] = {
    {2, "IN_1"_STRID, "STRING#opc_ua[CALL_METHOD; "},
    {2, "IN_3"_STRID, "STRING##; /Objects/2:EventHandler/2:publish]"},
    {7, "PV"_STRID, "UINT#3"},
    {8, "DT"_STRID, "T#250ms"},
};

const SCFB_FBConnectionData FORTE_PublishEventOpcUa::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {"APPEND_STRING_3"_STRID, "CNF"_STRID), 2,
     "F_STRING_TO_WSTRING"_STRID, "REQ"_STRID), 3},
    {"ANYToJSON"_STRID, "CNF"_STRID), 0,
     "APPEND_STRING_3"_STRID, "REQ"_STRID), 2},
    {"F_STRING_TO_WSTRING"_STRID, "CNF"_STRID), 3,
     "InitFlag"_STRID, "S"_STRID), 1},
    {"publishEvent"_STRID, "publish"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "ANYToJSON"_STRID, "REQ"_STRID), 0},
    {"E_SWITCH_2"_STRID, "EO1"_STRID), 5,
     "E_CTU_1"_STRID, "R"_STRID), 7},
    {"E_SWITCH_1_1"_STRID, "EO1"_STRID), 6,
     "E_CTU_1"_STRID, "R"_STRID), 7},
    {"E_SWITCH_2"_STRID, "EO0"_STRID), 5,
     "E_CTU_1"_STRID, "CU"_STRID), 7},
    {"E_CTU_1"_STRID, "CUO"_STRID), 7,
     "E_SWITCH_1_1"_STRID, "EI"_STRID), 6},
    {"E_SWITCH_1_1"_STRID, "EO0"_STRID), 6,
     "E_DELAY_1"_STRID, "START"_STRID), 8},
    {"InitFlag"_STRID, "EO"_STRID), 1,
     "Query"_STRID, "INIT"_STRID), 9},
    {"E_PERMIT"_STRID, "EO"_STRID), 4,
     "Query"_STRID, "REQ"_STRID), 9},
    {"E_DELAY_1"_STRID, "EO"_STRID), 8,
     "Query"_STRID, "REQ"_STRID), 9},
    {"Query"_STRID, "INITO"_STRID), 9,
     "E_PERMIT"_STRID, "EI"_STRID), 4},
    {"Query"_STRID, "CNF"_STRID), 9,
     "E_SWITCH_2"_STRID, "EI"_STRID), 5},
};

const SCFB_FBFannedOutConnectionData FORTE_PublishEventOpcUa::scmFannedOutEventConnections[] = {
    {4, "publishEvent"_STRID, "published"_STRID), CCompositeFB::scmAdapterMarker | 0},
    {4, "InitFlag"_STRID, "R"_STRID), 1},
    {5, "InitFlag"_STRID, "R"_STRID), 1},
    {5, "publishEvent"_STRID, "published"_STRID), CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBConnectionData FORTE_PublishEventOpcUaauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {"APPEND_STRING_3"_STRID, "OUT"_STRID), 2,
     "F_STRING_TO_WSTRING"_STRID, "IN"_STRID), 3},
    {"InitFlag"_STRID, "Q"_STRID), 1,
     "E_PERMIT"_STRID, "PERMIT"_STRID), 4},
    {"publishEvent"_STRID, "publishEvent"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "ANYToJSON"_STRID, "input"_STRID), 0},
    {"publishEvent"_STRID, "endpoint"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "APPEND_STRING_3"_STRID, "IN_2"_STRID), 2},
    {"E_CTU_1"_STRID, "Q"_STRID), 7,
     "E_SWITCH_1_1"_STRID, "G"_STRID), 6},
    {"F_STRING_TO_WSTRING"_STRID, "OUT"_STRID), 3,
     "Query"_STRID, "ID"_STRID), 9},
    {"ANYToJSON"_STRID, "output"_STRID), 0,
     "Query"_STRID, "SD_1"_STRID), 9},
    {"Query"_STRID, "QO"_STRID), 9,
     "E_SWITCH_2"_STRID, "G"_STRID), 5},
};

const SCFB_FBFannedOutConnectionData FORTE_PublishEventOpcUa::scmFannedOutDataConnections[] = {
    {1, "Query"_STRID, "QI"_STRID), 9},
};

const SCFB_FBNData FORTE_PublishEventOpcUa::scmFBNData = {
    10, scmInternalFBs,     14, scmEventConnections,         4, scmFannedOutEventConnections,
    8,  scmDataConnections, 1,  scmFannedOutDataConnections, 0, nullptr,
    4,  scmParamters};
