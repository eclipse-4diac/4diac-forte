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

#include "RegisterServiceHTTP.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_RegisterServiceHTTP, "RegisterServiceHTTP"_STRID)

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

const SCFB_FBInstanceData FORTE_RegisterServiceHTTP::scmInternalFBs[] = {
    {"Register"_STRID, "CLIENT_1_2"_STRID},
    {"Unregister"_STRID, "CLIENT_1_2"_STRID},
    {"InitFlagReg"_STRID, "E_SR"_STRID},
    {"E_PERMIT"_STRID, "E_PERMIT"_STRID},
    {"CreateRegisterID"_STRID, "APPEND_STRING_3"_STRID},
    {"F_STRING_TO_WSTRING"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"E_PERMIT_1"_STRID, "E_PERMIT"_STRID},
    {"InitFlagUnReg"_STRID, "E_SR"_STRID},
    {"CreateUnRegisterID"_STRID, "APPEND_STRING_3"_STRID},
    {"F_STRING_TO_WSTRING_1"_STRID, "F_STRING_TO_WSTRING"_STRID},
    {"ANYToJSON"_STRID, "ANYToJSON"_STRID},
};

const SCFB_FBParameter FORTE_RegisterServiceHTTP::scmParamters[] = {
    {4, "IN_1"_STRID, "STRING#http["},
    {4, "IN_3"_STRID, "STRING#/register; POST; application/json]"},
    {8, "IN_1"_STRID, "STRING#http["},
    {8, "IN_3"_STRID, "STRING#/remove; PUT; application/json]"},
};

const SCFB_FBConnectionData FORTE_RegisterServiceHTTP::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {"CreateRegisterID"_STRID, "CNF"_STRID), 4,
     "F_STRING_TO_WSTRING"_STRID, "REQ"_STRID), 5},
    {"F_STRING_TO_WSTRING"_STRID, "CNF"_STRID), 5,
     "InitFlagReg"_STRID, "S"_STRID), 2},
    {"CreateUnRegisterID"_STRID, "CNF"_STRID), 8,
     "F_STRING_TO_WSTRING_1"_STRID, "REQ"_STRID), 9},
    {"F_STRING_TO_WSTRING_1"_STRID, "CNF"_STRID), 9,
     "InitFlagUnReg"_STRID, "S"_STRID), 7},
    {"InitFlagUnReg"_STRID, "EO"_STRID), 7,
     "Unregister"_STRID, "INIT"_STRID), 1},
    {"Unregister"_STRID, "INITO"_STRID), 1,
     "E_PERMIT_1"_STRID, "EI"_STRID), 6},
    {"E_PERMIT_1"_STRID, "EO"_STRID), 6,
     "Unregister"_STRID, "REQ"_STRID), 1},
    {"Unregister"_STRID, "CNF"_STRID), 1,
     "InitFlagUnReg"_STRID, "R"_STRID), 7},
    {"InitFlagReg"_STRID, "EO"_STRID), 2,
     "Register"_STRID, "INIT"_STRID), 0},
    {"E_PERMIT"_STRID, "EO"_STRID), 3,
     "Register"_STRID, "REQ"_STRID), 0},
    {"Register"_STRID, "INITO"_STRID), 0,
     "E_PERMIT"_STRID, "EI"_STRID), 3},
    {"Register"_STRID, "CNF"_STRID), 0,
     "InitFlagReg"_STRID, "R"_STRID), 2},
    {"registerService"_STRID, "registerService"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "CreateRegisterID"_STRID, "REQ"_STRID), 4},
    {"registerService"_STRID, "unregisterService"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "CreateUnRegisterID"_STRID, "REQ"_STRID), 8},
};

const SCFB_FBFannedOutConnectionData FORTE_RegisterServiceHTTP::scmFannedOutEventConnections[] = {
    {1, "ANYToJSON"_STRID, "REQ"_STRID), 10},
    {3, "ANYToJSON"_STRID, "REQ"_STRID), 10},
    {7, "registerService"_STRID, "doneUnregister"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {11, "registerService"_STRID, "doneRegister"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBConnectionData FORTE_RegisterServiceHTTPauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {"InitFlagReg"_STRID, "Q"_STRID), 2,
     "E_PERMIT"_STRID, "PERMIT"_STRID), 3},
    {"CreateRegisterID"_STRID, "OUT"_STRID), 4,
     "F_STRING_TO_WSTRING"_STRID, "IN"_STRID), 5},
    {"CreateUnRegisterID"_STRID, "OUT"_STRID), 8,
     "F_STRING_TO_WSTRING_1"_STRID, "IN"_STRID), 9},
    {"F_STRING_TO_WSTRING_1"_STRID, "OUT"_STRID), 9,
     "Unregister"_STRID, "ID"_STRID), 1},
    {"InitFlagUnReg"_STRID, "Q"_STRID), 7,
     "Unregister"_STRID, "QI"_STRID), 1},
    {"F_STRING_TO_WSTRING"_STRID, "OUT"_STRID), 5,
     "Register"_STRID, "ID"_STRID), 0},
    {"ANYToJSON"_STRID, "output"_STRID), 10,
     "Register"_STRID, "SD_1"_STRID), 0},
    {"registerService"_STRID, "serviceRegistryEntry"_STRID),
     CCompositeFB::scmAdapterMarker | 0, "ANYToJSON"_STRID, "input"_STRID), 10},
    {"registerService"_STRID, "endpoint"_STRID), CCompositeFB::scmAdapterMarker | 0,
     "CreateRegisterID"_STRID, "IN_2"_STRID), 4},
};

const SCFB_FBFannedOutConnectionData FORTE_RegisterServiceHTTP::scmFannedOutDataConnections[] = {
    {0, "Register"_STRID, "QI"_STRID), 0},
    {4, "E_PERMIT_1"_STRID, "PERMIT"_STRID), 6},
    {6, "Unregister"_STRID, "SD_1"_STRID), 1},
    {8, "CreateUnRegisterID"_STRID, "IN_2"_STRID), 8},
};

const SCFB_FBNData FORTE_RegisterServiceHTTP::scmFBNData = {
    11, scmInternalFBs,     14, scmEventConnections,         4, scmFannedOutEventConnections,
    9,  scmDataConnections, 4,  scmFannedOutDataConnections, 0, nullptr,
    4,  scmParamters};
