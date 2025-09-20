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

#include "SubscribeEventHTTP.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_SubscribeEventHTTP, "SubscribeEventHTTP"_STRID)

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

const SCFB_FBInstanceData FORTE_SubscribeEventHTTP::scmInternalFBs[] = {
    {"InitFlagReg"_STRID, "E_SR"_STRID},
    {"E_PERMIT"_STRID, "E_PERMIT"_STRID},
    {"InitFlagUnReg"_STRID, "E_SR"_STRID},
    {"F_STRING_TO_WSTRING"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"CreateRegisterID"_STRID, "APPEND_STRING_3"_STRID},
    {"ANYToJSON"_STRID, "ANYToJSON"_STRID},
    {"E_PERMIT_1"_STRID, "E_PERMIT"_STRID},
    {"Register"_STRID, "CLIENT_1_2"_STRID},
    {"Unregister"_STRID, "CLIENT_1_2"_STRID},
    {"CreateRegisterID_1"_STRID, "APPEND_STRING_3"_STRID},
    {"F_STRING_TO_WSTRING_2"_STRID, "F_STRING_TO_WSTRING"_STRID},
};

const SCFB_FBParameter FORTE_SubscribeEventHTTP::scmParamters[] = {
    {4, "IN_1"_STRID, "STRING#http["},
    {4, "IN_3"_STRID, "STRING#/subscription; POST; application/json]"},
    {9, "IN_1"_STRID, "STRING#http["},
    {9, "IN_3"_STRID, "STRING#/subscription; PUT; application/json]"},
};

const SCFB_FBConnectionData FORTE_SubscribeEventHTTP::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {"Register"_STRID, "INITO"_STRID), 7,
     "E_PERMIT"_STRID, "EI"_STRID), 1},
    {"Register"_STRID, "CNF"_STRID), 7,
     "InitFlagReg"_STRID, "R"_STRID), 0},
    {"InitFlagReg"_STRID, "EO"_STRID), 0,
     "Register"_STRID, "INIT"_STRID), 7},
    {"CreateRegisterID"_STRID, "CNF"_STRID), 4,
     "F_STRING_TO_WSTRING"_STRID, "REQ"_STRID), 3},
    {"F_STRING_TO_WSTRING"_STRID, "CNF"_STRID), 3,
     "ANYToJSON"_STRID, "REQ"_STRID), 5},
    {"E_PERMIT"_STRID, "EO"_STRID), 1,
     "Register"_STRID, "REQ"_STRID), 7},
    {"SubscribeEventAdp"_STRID, "Subscribe"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "CreateRegisterID"_STRID, "REQ"_STRID), 4},
    {"InitFlagUnReg"_STRID, "EO"_STRID), 2,
     "Unregister"_STRID, "INIT"_STRID), 8},
    {"E_PERMIT_1"_STRID, "EO"_STRID), 6,
     "Unregister"_STRID, "REQ"_STRID), 8},
    {"Unregister"_STRID, "INITO"_STRID), 8,
     "E_PERMIT_1"_STRID, "EI"_STRID), 6},
    {"Unregister"_STRID, "CNF"_STRID), 8,
     "SubscribeEventAdp"_STRID, "Unsubscribed"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {"SubscribeEventAdp"_STRID, "Unsubscribe"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "CreateRegisterID_1"_STRID, "REQ"_STRID), 9},
    {"CreateRegisterID_1"_STRID, "CNF"_STRID), 9,
     "F_STRING_TO_WSTRING_2"_STRID, "REQ"_STRID), 10},
    {"F_STRING_TO_WSTRING_2"_STRID, "CNF"_STRID), 10,
     "ANYToJSON"_STRID, "REQ"_STRID), 5},
};

const SCFB_FBFannedOutConnectionData FORTE_SubscribeEventHTTP::scmFannedOutEventConnections[] = {
    {1, "SubscribeEventAdp"_STRID, "Subscribed"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {4, "InitFlagReg"_STRID, "S"_STRID), 0},
    {10, "InitFlagUnReg"_STRID, "R"_STRID), 2},
    {13, "InitFlagUnReg"_STRID, "S"_STRID), 2},
};

const SCFB_FBConnectionData FORTE_SubscribeEventHTTPauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {"InitFlagUnReg"_STRID, "Q"_STRID), 2,
     "E_PERMIT_1"_STRID, "PERMIT"_STRID), 6},
    {"InitFlagReg"_STRID, "Q"_STRID), 0,
     "Register"_STRID, "QI"_STRID), 7},
    {"ANYToJSON"_STRID, "output"_STRID), 5,
     "Register"_STRID, "SD_1"_STRID), 7},
    {"CreateRegisterID"_STRID, "OUT"_STRID), 4,
     "F_STRING_TO_WSTRING"_STRID, "IN"_STRID), 3},
    {"F_STRING_TO_WSTRING"_STRID, "OUT"_STRID), 3,
     "Register"_STRID, "ID"_STRID), 7},
    {"SubscribeEventAdp"_STRID, "endpoint"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "CreateRegisterID"_STRID, "IN_2"_STRID), 4},
    {"SubscribeEventAdp"_STRID, "eventFilter"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "ANYToJSON"_STRID, "input"_STRID), 5},
    {"CreateRegisterID_1"_STRID, "OUT"_STRID), 9,
     "F_STRING_TO_WSTRING_2"_STRID, "IN"_STRID), 10},
    {"F_STRING_TO_WSTRING_2"_STRID, "OUT"_STRID), 10,
     "Unregister"_STRID, "ID"_STRID), 8},
};

const SCFB_FBFannedOutConnectionData FORTE_SubscribeEventHTTP::scmFannedOutDataConnections[] = {
    {0, "Unregister"_STRID, "QI"_STRID), 8},
    {1, "E_PERMIT"_STRID, "PERMIT"_STRID), 1},
    {2, "Unregister"_STRID, "SD_1"_STRID), 8},
    {5, "CreateRegisterID_1"_STRID, "IN_2"_STRID), 9},
};

const SCFB_FBNData FORTE_SubscribeEventHTTP::scmFBNData = {
    11, scmInternalFBs,     14, scmEventConnections,         4, scmFannedOutEventConnections,
    9,  scmDataConnections, 4,  scmFannedOutDataConnections, 0, nullptr,
    4,  scmParamters};
