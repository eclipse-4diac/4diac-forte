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

USE_STRING_ID(ANYToJSON);
USE_STRING_ID(APPEND_STRING_3);
USE_STRING_ID(CLIENT_1_2);
USE_STRING_ID(CNF);
USE_STRING_ID(CreateRegisterID);
USE_STRING_ID(CreateRegisterID_1);
USE_STRING_ID(EI);
USE_STRING_ID(endpoint);
USE_STRING_ID(EO);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_PERMIT_1);
USE_STRING_ID(E_SR);
USE_STRING_ID(eventFilter);
USE_STRING_ID(F_STRING_TO_WSTRING);
USE_STRING_ID(F_STRING_TO_WSTRING_2);
USE_STRING_ID(ID);
USE_STRING_ID(IN);
USE_STRING_ID(IN_1);
USE_STRING_ID(IN_2);
USE_STRING_ID(IN_3);
USE_STRING_ID(INIT);
USE_STRING_ID(InitFlagReg);
USE_STRING_ID(InitFlagUnReg);
USE_STRING_ID(INITO);
USE_STRING_ID(input);
USE_STRING_ID(OUT);
USE_STRING_ID(output);
USE_STRING_ID(PERMIT);
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(R);
USE_STRING_ID(Register);
USE_STRING_ID(REQ);
USE_STRING_ID(S);
USE_STRING_ID(SD_1);
USE_STRING_ID(Subscribe);
USE_STRING_ID(Subscribed);
USE_STRING_ID(SubscribeEventAdp);
USE_STRING_ID(SubscribeEventHTTP);
USE_STRING_ID(Unregister);
USE_STRING_ID(Unsubscribe);
USE_STRING_ID(Unsubscribed);

DEFINE_FIRMWARE_FB(FORTE_SubscribeEventHTTP, STRID(SubscribeEventHTTP))

namespace {
  const auto cSocketNameIds = std::array{STRID(SubscribeEventAdp)};

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
    {STRID(InitFlagReg), STRID(E_SR)},
    {STRID(E_PERMIT), STRID(E_PERMIT)},
    {STRID(InitFlagUnReg), STRID(E_SR)},
    {STRID(F_STRING_TO_WSTRING), STRID(F_STRING_TO_WSTRING)},
    {STRID(CreateRegisterID), STRID(APPEND_STRING_3)},
    {STRID(ANYToJSON), STRID(ANYToJSON)},
    {STRID(E_PERMIT_1), STRID(E_PERMIT)},
    {STRID(Register), STRID(CLIENT_1_2)},
    {STRID(Unregister), STRID(CLIENT_1_2)},
    {STRID(CreateRegisterID_1), STRID(APPEND_STRING_3)},
    {STRID(F_STRING_TO_WSTRING_2), STRID(F_STRING_TO_WSTRING)},
};

const SCFB_FBParameter FORTE_SubscribeEventHTTP::scmParamters[] = {
    {4, STRID(IN_1), "STRING#http["},
    {4, STRID(IN_3), "STRING#/subscription; POST; application/json]"},
    {9, STRID(IN_1), "STRING#http["},
    {9, STRID(IN_3), "STRING#/subscription; PUT; application/json]"},
};

const SCFB_FBConnectionData FORTE_SubscribeEventHTTP::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(Register), STRID(INITO)), 7,
     STRID(E_PERMIT), STRID(EI)), 1},
    {STRID(Register), STRID(CNF)), 7,
     STRID(InitFlagReg), STRID(R)), 0},
    {STRID(InitFlagReg), STRID(EO)), 0,
     STRID(Register), STRID(INIT)), 7},
    {STRID(CreateRegisterID), STRID(CNF)), 4,
     STRID(F_STRING_TO_WSTRING), STRID(REQ)), 3},
    {STRID(F_STRING_TO_WSTRING), STRID(CNF)), 3,
     STRID(ANYToJSON), STRID(REQ)), 5},
    {STRID(E_PERMIT), STRID(EO)), 1,
     STRID(Register), STRID(REQ)), 7},
    {STRID(SubscribeEventAdp), STRID(Subscribe)), CCompositeFB::scmAdapterMarker | 0,
     STRID(CreateRegisterID), STRID(REQ)), 4},
    {STRID(InitFlagUnReg), STRID(EO)), 2,
     STRID(Unregister), STRID(INIT)), 8},
    {STRID(E_PERMIT_1), STRID(EO)), 6,
     STRID(Unregister), STRID(REQ)), 8},
    {STRID(Unregister), STRID(INITO)), 8,
     STRID(E_PERMIT_1), STRID(EI)), 6},
    {STRID(Unregister), STRID(CNF)), 8,
     STRID(SubscribeEventAdp), STRID(Unsubscribed)),
     CCompositeFB::scmAdapterMarker | 0},
    {STRID(SubscribeEventAdp), STRID(Unsubscribe)),
     CCompositeFB::scmAdapterMarker | 0, STRID(CreateRegisterID_1), STRID(REQ)), 9},
    {STRID(CreateRegisterID_1), STRID(CNF)), 9,
     STRID(F_STRING_TO_WSTRING_2), STRID(REQ)), 10},
    {STRID(F_STRING_TO_WSTRING_2), STRID(CNF)), 10,
     STRID(ANYToJSON), STRID(REQ)), 5},
};

const SCFB_FBFannedOutConnectionData FORTE_SubscribeEventHTTP::scmFannedOutEventConnections[] = {
    {1, STRID(SubscribeEventAdp), STRID(Subscribed)),
     CCompositeFB::scmAdapterMarker | 0},
    {4, STRID(InitFlagReg), STRID(S)), 0},
    {10, STRID(InitFlagUnReg), STRID(R)), 2},
    {13, STRID(InitFlagUnReg), STRID(S)), 2},
};

const SCFB_FBConnectionData FORTE_SubscribeEventHTTPauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(InitFlagUnReg), STRID(Q)), 2,
     STRID(E_PERMIT_1), STRID(PERMIT)), 6},
    {STRID(InitFlagReg), STRID(Q)), 0,
     STRID(Register), STRID(QI)), 7},
    {STRID(ANYToJSON), STRID(output)), 5,
     STRID(Register), STRID(SD_1)), 7},
    {STRID(CreateRegisterID), STRID(OUT)), 4,
     STRID(F_STRING_TO_WSTRING), STRID(IN)), 3},
    {STRID(F_STRING_TO_WSTRING), STRID(OUT)), 3,
     STRID(Register), STRID(ID)), 7},
    {STRID(SubscribeEventAdp), STRID(endpoint)), CCompositeFB::scmAdapterMarker | 0,
     STRID(CreateRegisterID), STRID(IN_2)), 4},
    {STRID(SubscribeEventAdp), STRID(eventFilter)),
     CCompositeFB::scmAdapterMarker | 0, STRID(ANYToJSON), STRID(input)), 5},
    {STRID(CreateRegisterID_1), STRID(OUT)), 9,
     STRID(F_STRING_TO_WSTRING_2), STRID(IN)), 10},
    {STRID(F_STRING_TO_WSTRING_2), STRID(OUT)), 10,
     STRID(Unregister), STRID(ID)), 8},
};

const SCFB_FBFannedOutConnectionData FORTE_SubscribeEventHTTP::scmFannedOutDataConnections[] = {
    {0, STRID(Unregister), STRID(QI)), 8},
    {1, STRID(E_PERMIT), STRID(PERMIT)), 1},
    {2, STRID(Unregister), STRID(SD_1)), 8},
    {5, STRID(CreateRegisterID_1), STRID(IN_2)), 9},
};

const SCFB_FBNData FORTE_SubscribeEventHTTP::scmFBNData = {
    11, scmInternalFBs,     14, scmEventConnections,         4, scmFannedOutEventConnections,
    9,  scmDataConnections, 4,  scmFannedOutDataConnections, 0, nullptr,
    4,  scmParamters};
