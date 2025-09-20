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

#include "RegisterServiceOpcUa.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_RegisterServiceOpcUa, "RegisterServiceOpcUa"_STRID)

namespace {
  const auto cSocketNameIds = std::array{"registerService"_STRID};

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

const SCFB_FBInstanceData FORTE_RegisterServiceOpcUa::scmInternalFBs[] = {
    {"InitFlagReg"_STRID, "E_SR"_STRID},
    {"E_PERMIT"_STRID, "E_PERMIT"_STRID},
    {"CreateRegisterID"_STRID, "APPEND_STRING_3"_STRID},
    {"F_STRING_TO_WSTRING"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"E_PERMIT_1"_STRID, "E_PERMIT"_STRID},
    {"InitFlagUnReg"_STRID, "E_SR"_STRID},
    {"CreateUnRegisterID"_STRID, "APPEND_STRING_3"_STRID},
    {"F_STRING_TO_WSTRING_1"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"ANYToJSON"_STRID, "ANYToJSON"_STRID},
    {"Register"_STRID, "CLIENT_1_0"_STRID},
    {"Unregister"_STRID, "CLIENT_1_0"_STRID},
    {"E_SWITCH"_STRID, "E_SWITCH"_STRID},
    {"E_SWITCH_1"_STRID, "E_SWITCH"_STRID},
    {"E_DELAY"_STRID, "E_DELAY"_STRID},
    {"E_CTU"_STRID, "E_CTU"_STRID},
    {"E_SWITCH_1_1"_STRID, "E_SWITCH"_STRID},
    {"E_SWITCH_2"_STRID, "E_SWITCH"_STRID},
    {"E_DELAY_1"_STRID, "E_DELAY"_STRID},
    {"E_CTU_1"_STRID, "E_CTU"_STRID},
};

const SCFB_FBParameter FORTE_RegisterServiceOpcUa::scmParamters[] = {
    {2, "IN_1"_STRID, "STRING#opc_ua[CALL_METHOD; "},
    {2, "IN_3"_STRID, "STRING##; /Objects/2:ServiceRegistry/2:register]"},
    {6, "IN_1"_STRID, "STRING#opc_ua[CALL_METHOD; "},
    {6, "IN_3"_STRID, "STRING##; /Objects/2:ServiceRegistry/2:remove]"},
    {13, "DT"_STRID, "T#250ms"},
    {14, "PV"_STRID, "UINT#3"},
    {17, "DT"_STRID, "T#250ms"},
    {18, "PV"_STRID, "UINT#3"},
};

const SCFB_FBConnectionData FORTE_RegisterServiceOpcUa::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {"CreateRegisterID"_STRID, "CNF"_STRID), 2,
     "F_STRING_TO_WSTRING"_STRID, "REQ"_STRID), 3},
    {"F_STRING_TO_WSTRING"_STRID, "CNF"_STRID), 3,
     "InitFlagReg"_STRID, "S"_STRID), 0},
    {"CreateUnRegisterID"_STRID, "CNF"_STRID), 6,
     "F_STRING_TO_WSTRING_1"_STRID, "REQ"_STRID), 7},
    {"F_STRING_TO_WSTRING_1"_STRID, "CNF"_STRID), 7,
     "InitFlagUnReg"_STRID, "S"_STRID), 5},
    {"registerService"_STRID, "registerService"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "CreateRegisterID"_STRID, "REQ"_STRID), 2},
    {"registerService"_STRID, "unregisterService"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "CreateUnRegisterID"_STRID, "REQ"_STRID), 6},
    {"InitFlagReg"_STRID, "EO"_STRID), 0,
     "Register"_STRID, "INIT"_STRID), 9},
    {"E_PERMIT"_STRID, "EO"_STRID), 1,
     "Register"_STRID, "REQ"_STRID), 9},
    {"Register"_STRID, "INITO"_STRID), 9,
     "E_PERMIT"_STRID, "EI"_STRID), 1},
    {"InitFlagUnReg"_STRID, "EO"_STRID), 5,
     "Unregister"_STRID, "INIT"_STRID), 10},
    {"E_PERMIT_1"_STRID, "EO"_STRID), 4,
     "Unregister"_STRID, "REQ"_STRID), 10},
    {"Unregister"_STRID, "INITO"_STRID), 10,
     "E_PERMIT_1"_STRID, "EI"_STRID), 4},
    {"E_CTU"_STRID, "CUO"_STRID), 14,
     "E_SWITCH_1"_STRID, "EI"_STRID), 12},
    {"E_SWITCH_1"_STRID, "EO0"_STRID), 12,
     "E_DELAY"_STRID, "START"_STRID), 13},
    {"E_SWITCH"_STRID, "EO0"_STRID), 11,
     "E_CTU"_STRID, "CU"_STRID), 14},
    {"E_SWITCH"_STRID, "EO1"_STRID), 11,
     "InitFlagReg"_STRID, "R"_STRID), 0},
    {"Register"_STRID, "CNF"_STRID), 9,
     "E_SWITCH"_STRID, "EI"_STRID), 11},
    {"E_DELAY"_STRID, "EO"_STRID), 13,
     "Register"_STRID, "REQ"_STRID), 9},
    {"E_SWITCH_1_1"_STRID, "EO0"_STRID), 15,
     "E_DELAY_1"_STRID, "START"_STRID), 17},
    {"E_CTU_1"_STRID, "CUO"_STRID), 18,
     "E_SWITCH_1_1"_STRID, "EI"_STRID), 15},
    {"E_SWITCH_2"_STRID, "EO0"_STRID), 16,
     "E_CTU_1"_STRID, "CU"_STRID), 18},
    {"E_SWITCH_2"_STRID, "EO1"_STRID), 16,
     "InitFlagUnReg"_STRID, "R"_STRID), 5},
    {"Unregister"_STRID, "CNF"_STRID), 10,
     "E_SWITCH_2"_STRID, "EI"_STRID), 16},
    {"E_DELAY_1"_STRID, "EO"_STRID), 17,
     "Unregister"_STRID, "REQ"_STRID), 10},
    {"E_SWITCH_1_1"_STRID, "EO1"_STRID), 15,
     "InitFlagUnReg"_STRID, "R"_STRID), 5},
    {"E_SWITCH_1"_STRID, "EO1"_STRID), 12,
     "InitFlagReg"_STRID, "R"_STRID), 0},
};

const SCFB_FBFannedOutConnectionData FORTE_RegisterServiceOpcUa::scmFannedOutEventConnections[] = {
    {1, "ANYToJSON"_STRID, "REQ"_STRID), 8},
    {3, "ANYToJSON"_STRID, "REQ"_STRID), 8},
    {15, "registerService"_STRID, "doneRegister"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {15, "E_CTU"_STRID, "R"_STRID), 14},
    {21, "registerService"_STRID, "doneUnregister"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {21, "E_CTU_1"_STRID, "R"_STRID), 18},
    {24, "registerService"_STRID, "doneUnregister"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {24, "E_CTU_1"_STRID, "R"_STRID), 18},
    {25, "registerService"_STRID, "doneRegister"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {25, "E_CTU"_STRID, "R"_STRID), 14},
};

const SCFB_FBConnectionData FORTE_RegisterServiceOpcUaauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {"InitFlagReg"_STRID, "Q"_STRID), 0,
     "E_PERMIT"_STRID, "PERMIT"_STRID), 1},
    {"CreateRegisterID"_STRID, "OUT"_STRID), 2,
     "F_STRING_TO_WSTRING"_STRID, "IN"_STRID), 3},
    {"CreateUnRegisterID"_STRID, "OUT"_STRID), 6,
     "F_STRING_TO_WSTRING_1"_STRID, "IN"_STRID), 7},
    {"InitFlagUnReg"_STRID, "Q"_STRID), 5,
     "E_PERMIT_1"_STRID, "PERMIT"_STRID), 4},
    {"registerService"_STRID, "serviceRegistryEntry"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "ANYToJSON"_STRID, "input"_STRID), 8},
    {"registerService"_STRID, "endpoint"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "CreateRegisterID"_STRID, "IN_2"_STRID), 2},
    {"F_STRING_TO_WSTRING"_STRID, "OUT"_STRID), 3,
     "Register"_STRID, "ID"_STRID), 9},
    {"ANYToJSON"_STRID, "output"_STRID), 8,
     "Register"_STRID, "SD_1"_STRID), 9},
    {"F_STRING_TO_WSTRING_1"_STRID, "OUT"_STRID), 7,
     "Unregister"_STRID, "ID"_STRID), 10},
    {"E_CTU"_STRID, "Q"_STRID), 14,
     "E_SWITCH_1"_STRID, "G"_STRID), 12},
    {"Register"_STRID, "QO"_STRID), 9,
     "E_SWITCH"_STRID, "G"_STRID), 11},
    {"E_CTU_1"_STRID, "Q"_STRID), 18,
     "E_SWITCH_1_1"_STRID, "G"_STRID), 15},
    {"Unregister"_STRID, "QO"_STRID), 10,
     "E_SWITCH_2"_STRID, "G"_STRID), 16},
};

const SCFB_FBFannedOutConnectionData FORTE_RegisterServiceOpcUa::scmFannedOutDataConnections[] = {
    {0, "Register"_STRID, "QI"_STRID), 9},
    {3, "Unregister"_STRID, "QI"_STRID), 10},
    {5, "CreateUnRegisterID"_STRID, "IN_2"_STRID), 6},
    {7, "Unregister"_STRID, "SD_1"_STRID), 10},
};

const SCFB_FBNData FORTE_RegisterServiceOpcUa::scmFBNData = {
    19, scmInternalFBs,     26, scmEventConnections,         10, scmFannedOutEventConnections,
    13, scmDataConnections, 4,  scmFannedOutDataConnections, 0,  nullptr,
    8,  scmParamters};
