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

#include "SubscribeEventOpcUa.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_SubscribeEventOpcUa, "SubscribeEventOpcUa"_STRID)

namespace {
  const auto cSocketNameIds = std::array{"SubscribeEventAdp"_STRID};

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

const SCFB_FBInstanceData FORTE_SubscribeEventOpcUa::scmInternalFBs[] = {
    {"InitFlagReg"_STRID, "E_SR"_STRID},
    {"E_PERMIT"_STRID, "E_PERMIT"_STRID},
    {"InitFlagUnReg"_STRID, "E_SR"_STRID},
    {"F_STRING_TO_WSTRING"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"CreateRegisterID"_STRID, "APPEND_STRING_3"_STRID},
    {"ANYToJSON"_STRID, "ANYToJSON"_STRID},
    {"E_PERMIT_1"_STRID, "E_PERMIT"_STRID},
    {"CreateRegisterID_1"_STRID, "APPEND_STRING_3"_STRID},
    {"F_STRING_TO_WSTRING_2"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"Register"_STRID, "CLIENT_1_0"_STRID},
    {"Unregister"_STRID, "CLIENT_1_0"_STRID},
    {"E_DELAY_1"_STRID, "E_DELAY"_STRID},
    {"E_SWITCH_1_1"_STRID, "E_SWITCH"_STRID},
    {"E_SWITCH_2"_STRID, "E_SWITCH"_STRID},
    {"E_CTU_1"_STRID, "E_CTU"_STRID},
    {"E_DELAY_1_1"_STRID, "E_DELAY"_STRID},
    {"E_SWITCH_1_1_1"_STRID, "E_SWITCH"_STRID},
    {"E_SWITCH_2_1"_STRID, "E_SWITCH"_STRID},
    {"E_CTU_1_1"_STRID, "E_CTU"_STRID},
};

const SCFB_FBParameter FORTE_SubscribeEventOpcUa::scmParamters[] = {
    {4, "IN_1"_STRID, "STRING#opc_ua[CALL_METHOD; "},
    {4, "IN_3"_STRID, "STRING##; /Objects/2:EventHandler/2:subscribe]"},
    {7, "IN_1"_STRID, "STRING#opc_ua[CALL_METHOD; "},
    {7, "IN_3"_STRID, "STRING##; /Objects/2:EventHandler/2:unsubscribe]"},
    {11, "DT"_STRID, "T#250ms"},
    {14, "PV"_STRID, "UINT#3"},
    {15, "DT"_STRID, "T#250ms"},
    {18, "PV"_STRID, "UINT#3"},
};

const SCFB_FBConnectionData FORTE_SubscribeEventOpcUa::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {"CreateRegisterID"_STRID, "CNF"_STRID), 4,
     "F_STRING_TO_WSTRING"_STRID, "REQ"_STRID), 3},
    {"F_STRING_TO_WSTRING"_STRID, "CNF"_STRID), 3,
     "ANYToJSON"_STRID, "REQ"_STRID), 5},
    {"SubscribeEventAdp"_STRID, "Subscribe"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "CreateRegisterID"_STRID, "REQ"_STRID), 4},
    {"SubscribeEventAdp"_STRID, "Unsubscribe"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "CreateRegisterID_1"_STRID, "REQ"_STRID), 7},
    {"CreateRegisterID_1"_STRID, "CNF"_STRID), 7,
     "F_STRING_TO_WSTRING_2"_STRID, "REQ"_STRID), 8},
    {"F_STRING_TO_WSTRING_2"_STRID, "CNF"_STRID), 8,
     "ANYToJSON"_STRID, "REQ"_STRID), 5},
    {"InitFlagReg"_STRID, "EO"_STRID), 0,
     "Register"_STRID, "INIT"_STRID), 9},
    {"E_PERMIT"_STRID, "EO"_STRID), 1,
     "Register"_STRID, "REQ"_STRID), 9},
    {"Register"_STRID, "INITO"_STRID), 9,
     "E_PERMIT"_STRID, "EI"_STRID), 1},
    {"InitFlagUnReg"_STRID, "EO"_STRID), 2,
     "Unregister"_STRID, "INIT"_STRID), 10},
    {"E_PERMIT_1"_STRID, "EO"_STRID), 6,
     "Unregister"_STRID, "REQ"_STRID), 10},
    {"Unregister"_STRID, "INITO"_STRID), 10,
     "E_PERMIT_1"_STRID, "EI"_STRID), 6},
    {"E_SWITCH_2"_STRID, "EO1"_STRID), 13,
     "E_CTU_1"_STRID, "R"_STRID), 14},
    {"E_SWITCH_1_1"_STRID, "EO1"_STRID), 12,
     "E_CTU_1"_STRID, "R"_STRID), 14},
    {"E_CTU_1"_STRID, "CUO"_STRID), 14,
     "E_SWITCH_1_1"_STRID, "EI"_STRID), 12},
    {"E_SWITCH_2"_STRID, "EO0"_STRID), 13,
     "E_CTU_1"_STRID, "CU"_STRID), 14},
    {"E_SWITCH_1_1"_STRID, "EO0"_STRID), 12,
     "E_DELAY_1"_STRID, "START"_STRID), 11},
    {"E_SWITCH_2_1"_STRID, "EO1"_STRID), 17,
     "E_CTU_1_1"_STRID, "R"_STRID), 18},
    {"E_SWITCH_1_1_1"_STRID, "EO1"_STRID), 16,
     "E_CTU_1_1"_STRID, "R"_STRID), 18},
    {"E_CTU_1_1"_STRID, "CUO"_STRID), 18,
     "E_SWITCH_1_1_1"_STRID, "EI"_STRID), 16},
    {"E_SWITCH_2_1"_STRID, "EO0"_STRID), 17,
     "E_CTU_1_1"_STRID, "CU"_STRID), 18},
    {"E_SWITCH_1_1_1"_STRID, "EO0"_STRID), 16,
     "E_DELAY_1_1"_STRID, "START"_STRID), 15},
    {"Unregister"_STRID, "CNF"_STRID), 10,
     "E_SWITCH_2"_STRID, "EI"_STRID), 13},
    {"E_DELAY_1"_STRID, "EO"_STRID), 11,
     "Unregister"_STRID, "REQ"_STRID), 10},
    {"Register"_STRID, "CNF"_STRID), 9,
     "E_SWITCH_2_1"_STRID, "EI"_STRID), 17},
    {"E_DELAY_1_1"_STRID, "EO"_STRID), 15,
     "Register"_STRID, "REQ"_STRID), 9},
};

const SCFB_FBFannedOutConnectionData FORTE_SubscribeEventOpcUa::scmFannedOutEventConnections[] = {
    {1, "InitFlagReg"_STRID, "S"_STRID), 0},
    {5, "InitFlagUnReg"_STRID, "S"_STRID), 2},
    {12, "SubscribeEventAdp"_STRID, "Unsubscribed"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {12, "InitFlagUnReg"_STRID, "R"_STRID), 2},
    {13, "InitFlagUnReg"_STRID, "R"_STRID), 2},
    {13, "SubscribeEventAdp"_STRID, "Unsubscribed"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {17, "SubscribeEventAdp"_STRID, "Subscribed"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {17, "InitFlagReg"_STRID, "R"_STRID), 0},
    {18, "InitFlagReg"_STRID, "R"_STRID), 0},
    {18, "SubscribeEventAdp"_STRID, "Subscribed"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBConnectionData FORTE_SubscribeEventOpcUaauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {"InitFlagUnReg"_STRID, "Q"_STRID), 2,
     "E_PERMIT_1"_STRID, "PERMIT"_STRID), 6},
    {"CreateRegisterID"_STRID, "OUT"_STRID), 4,
     "F_STRING_TO_WSTRING"_STRID, "IN"_STRID), 3},
    {"InitFlagReg"_STRID, "Q"_STRID), 0,
     "E_PERMIT"_STRID, "PERMIT"_STRID), 1},
    {"SubscribeEventAdp"_STRID, "endpoint"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "CreateRegisterID"_STRID, "IN_2"_STRID), 4},
    {"SubscribeEventAdp"_STRID, "eventFilter"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "ANYToJSON"_STRID, "input"_STRID), 5},
    {"CreateRegisterID_1"_STRID, "OUT"_STRID), 7,
     "F_STRING_TO_WSTRING_2"_STRID, "IN"_STRID), 8},
    {"F_STRING_TO_WSTRING"_STRID, "OUT"_STRID), 3,
     "Register"_STRID, "ID"_STRID), 9},
    {"ANYToJSON"_STRID, "output"_STRID), 5,
     "Register"_STRID, "SD_1"_STRID), 9},
    {"F_STRING_TO_WSTRING_2"_STRID, "OUT"_STRID), 8,
     "Unregister"_STRID, "ID"_STRID), 10},
    {"E_CTU_1"_STRID, "Q"_STRID), 14,
     "E_SWITCH_1_1"_STRID, "G"_STRID), 12},
    {"E_CTU_1_1"_STRID, "Q"_STRID), 18,
     "E_SWITCH_1_1_1"_STRID, "G"_STRID), 16},
    {"Unregister"_STRID, "QO"_STRID), 10,
     "E_SWITCH_2"_STRID, "G"_STRID), 13},
    {"Register"_STRID, "QO"_STRID), 9,
     "E_SWITCH_2_1"_STRID, "G"_STRID), 17},
};

const SCFB_FBFannedOutConnectionData FORTE_SubscribeEventOpcUa::scmFannedOutDataConnections[] = {
    {0, "Unregister"_STRID, "QI"_STRID), 10},
    {2, "Register"_STRID, "QI"_STRID), 9},
    {3, "CreateRegisterID_1"_STRID, "IN_2"_STRID), 7},
    {7, "Unregister"_STRID, "SD_1"_STRID), 10},
};

const SCFB_FBNData FORTE_SubscribeEventOpcUa::scmFBNData = {
    19, scmInternalFBs,     26, scmEventConnections,         10, scmFannedOutEventConnections,
    13, scmDataConnections, 4,  scmFannedOutDataConnections, 0,  nullptr,
    8,  scmParamters};
