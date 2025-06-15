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

USE_STRING_ID(ANYToJSON);
USE_STRING_ID(APPEND_STRING_3);
USE_STRING_ID(CLIENT_1_0);
USE_STRING_ID(CNF);
USE_STRING_ID(CreateRegisterID);
USE_STRING_ID(CreateRegisterID_1);
USE_STRING_ID(CU);
USE_STRING_ID(CUO);
USE_STRING_ID(DT);
USE_STRING_ID(E_CTU);
USE_STRING_ID(E_CTU_1);
USE_STRING_ID(E_CTU_1_1);
USE_STRING_ID(E_DELAY);
USE_STRING_ID(E_DELAY_1);
USE_STRING_ID(E_DELAY_1_1);
USE_STRING_ID(EI);
USE_STRING_ID(endpoint);
USE_STRING_ID(EO);
USE_STRING_ID(EO0);
USE_STRING_ID(EO1);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_PERMIT_1);
USE_STRING_ID(E_SR);
USE_STRING_ID(E_SWITCH);
USE_STRING_ID(E_SWITCH_1_1);
USE_STRING_ID(E_SWITCH_1_1_1);
USE_STRING_ID(E_SWITCH_2);
USE_STRING_ID(E_SWITCH_2_1);
USE_STRING_ID(eventFilter);
USE_STRING_ID(F_STRING_TO_WSTRING);
USE_STRING_ID(F_STRING_TO_WSTRING_2);
USE_STRING_ID(G);
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
USE_STRING_ID(PV);
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(R);
USE_STRING_ID(Register);
USE_STRING_ID(REQ);
USE_STRING_ID(S);
USE_STRING_ID(SD_1);
USE_STRING_ID(START);
USE_STRING_ID(Subscribe);
USE_STRING_ID(Subscribed);
USE_STRING_ID(SubscribeEventAdp);
USE_STRING_ID(SubscribeEventOpcUa);
USE_STRING_ID(Unregister);
USE_STRING_ID(Unsubscribe);
USE_STRING_ID(Unsubscribed);

DEFINE_FIRMWARE_FB(FORTE_SubscribeEventOpcUa, STRID(SubscribeEventOpcUa))

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

const SCFB_FBInstanceData FORTE_SubscribeEventOpcUa::scmInternalFBs[] = {
    {STRID(InitFlagReg), STRID(E_SR)},
    {STRID(E_PERMIT), STRID(E_PERMIT)},
    {STRID(InitFlagUnReg), STRID(E_SR)},
    {STRID(F_STRING_TO_WSTRING), STRID(F_STRING_TO_WSTRING)},
    {STRID(CreateRegisterID), STRID(APPEND_STRING_3)},
    {STRID(ANYToJSON), STRID(ANYToJSON)},
    {STRID(E_PERMIT_1), STRID(E_PERMIT)},
    {STRID(CreateRegisterID_1), STRID(APPEND_STRING_3)},
    {STRID(F_STRING_TO_WSTRING_2), STRID(F_STRING_TO_WSTRING)},
    {STRID(Register), STRID(CLIENT_1_0)},
    {STRID(Unregister), STRID(CLIENT_1_0)},
    {STRID(E_DELAY_1), STRID(E_DELAY)},
    {STRID(E_SWITCH_1_1), STRID(E_SWITCH)},
    {STRID(E_SWITCH_2), STRID(E_SWITCH)},
    {STRID(E_CTU_1), STRID(E_CTU)},
    {STRID(E_DELAY_1_1), STRID(E_DELAY)},
    {STRID(E_SWITCH_1_1_1), STRID(E_SWITCH)},
    {STRID(E_SWITCH_2_1), STRID(E_SWITCH)},
    {STRID(E_CTU_1_1), STRID(E_CTU)},
};

const SCFB_FBParameter FORTE_SubscribeEventOpcUa::scmParamters[] = {
    {4, STRID(IN_1), "STRING#opc_ua[CALL_METHOD; "},
    {4, STRID(IN_3), "STRING##; /Objects/2:EventHandler/2:subscribe]"},
    {7, STRID(IN_1), "STRING#opc_ua[CALL_METHOD; "},
    {7, STRID(IN_3), "STRING##; /Objects/2:EventHandler/2:unsubscribe]"},
    {11, STRID(DT), "T#250ms"},
    {14, STRID(PV), "UINT#3"},
    {15, STRID(DT), "T#250ms"},
    {18, STRID(PV), "UINT#3"},
};

const SCFB_FBConnectionData FORTE_SubscribeEventOpcUa::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(CreateRegisterID), STRID(CNF)), 4,
     STRID(F_STRING_TO_WSTRING), STRID(REQ)), 3},
    {STRID(F_STRING_TO_WSTRING), STRID(CNF)), 3,
     STRID(ANYToJSON), STRID(REQ)), 5},
    {STRID(SubscribeEventAdp), STRID(Subscribe)), CCompositeFB::scmAdapterMarker | 0,
     STRID(CreateRegisterID), STRID(REQ)), 4},
    {STRID(SubscribeEventAdp), STRID(Unsubscribe)),
     CCompositeFB::scmAdapterMarker | 0, STRID(CreateRegisterID_1), STRID(REQ)), 7},
    {STRID(CreateRegisterID_1), STRID(CNF)), 7,
     STRID(F_STRING_TO_WSTRING_2), STRID(REQ)), 8},
    {STRID(F_STRING_TO_WSTRING_2), STRID(CNF)), 8,
     STRID(ANYToJSON), STRID(REQ)), 5},
    {STRID(InitFlagReg), STRID(EO)), 0,
     STRID(Register), STRID(INIT)), 9},
    {STRID(E_PERMIT), STRID(EO)), 1,
     STRID(Register), STRID(REQ)), 9},
    {STRID(Register), STRID(INITO)), 9,
     STRID(E_PERMIT), STRID(EI)), 1},
    {STRID(InitFlagUnReg), STRID(EO)), 2,
     STRID(Unregister), STRID(INIT)), 10},
    {STRID(E_PERMIT_1), STRID(EO)), 6,
     STRID(Unregister), STRID(REQ)), 10},
    {STRID(Unregister), STRID(INITO)), 10,
     STRID(E_PERMIT_1), STRID(EI)), 6},
    {STRID(E_SWITCH_2), STRID(EO1)), 13,
     STRID(E_CTU_1), STRID(R)), 14},
    {STRID(E_SWITCH_1_1), STRID(EO1)), 12,
     STRID(E_CTU_1), STRID(R)), 14},
    {STRID(E_CTU_1), STRID(CUO)), 14,
     STRID(E_SWITCH_1_1), STRID(EI)), 12},
    {STRID(E_SWITCH_2), STRID(EO0)), 13,
     STRID(E_CTU_1), STRID(CU)), 14},
    {STRID(E_SWITCH_1_1), STRID(EO0)), 12,
     STRID(E_DELAY_1), STRID(START)), 11},
    {STRID(E_SWITCH_2_1), STRID(EO1)), 17,
     STRID(E_CTU_1_1), STRID(R)), 18},
    {STRID(E_SWITCH_1_1_1), STRID(EO1)), 16,
     STRID(E_CTU_1_1), STRID(R)), 18},
    {STRID(E_CTU_1_1), STRID(CUO)), 18,
     STRID(E_SWITCH_1_1_1), STRID(EI)), 16},
    {STRID(E_SWITCH_2_1), STRID(EO0)), 17,
     STRID(E_CTU_1_1), STRID(CU)), 18},
    {STRID(E_SWITCH_1_1_1), STRID(EO0)), 16,
     STRID(E_DELAY_1_1), STRID(START)), 15},
    {STRID(Unregister), STRID(CNF)), 10,
     STRID(E_SWITCH_2), STRID(EI)), 13},
    {STRID(E_DELAY_1), STRID(EO)), 11,
     STRID(Unregister), STRID(REQ)), 10},
    {STRID(Register), STRID(CNF)), 9,
     STRID(E_SWITCH_2_1), STRID(EI)), 17},
    {STRID(E_DELAY_1_1), STRID(EO)), 15,
     STRID(Register), STRID(REQ)), 9},
};

const SCFB_FBFannedOutConnectionData FORTE_SubscribeEventOpcUa::scmFannedOutEventConnections[] = {
    {1, STRID(InitFlagReg), STRID(S)), 0},
    {5, STRID(InitFlagUnReg), STRID(S)), 2},
    {12, STRID(SubscribeEventAdp), STRID(Unsubscribed)),
     CCompositeFB::scmAdapterMarker | 0},
    {12, STRID(InitFlagUnReg), STRID(R)), 2},
    {13, STRID(InitFlagUnReg), STRID(R)), 2},
    {13, STRID(SubscribeEventAdp), STRID(Unsubscribed)),
     CCompositeFB::scmAdapterMarker | 0},
    {17, STRID(SubscribeEventAdp), STRID(Subscribed)),
     CCompositeFB::scmAdapterMarker | 0},
    {17, STRID(InitFlagReg), STRID(R)), 0},
    {18, STRID(InitFlagReg), STRID(R)), 0},
    {18, STRID(SubscribeEventAdp), STRID(Subscribed)),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBConnectionData FORTE_SubscribeEventOpcUaauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(InitFlagUnReg), STRID(Q)), 2,
     STRID(E_PERMIT_1), STRID(PERMIT)), 6},
    {STRID(CreateRegisterID), STRID(OUT)), 4,
     STRID(F_STRING_TO_WSTRING), STRID(IN)), 3},
    {STRID(InitFlagReg), STRID(Q)), 0,
     STRID(E_PERMIT), STRID(PERMIT)), 1},
    {STRID(SubscribeEventAdp), STRID(endpoint)), CCompositeFB::scmAdapterMarker | 0,
     STRID(CreateRegisterID), STRID(IN_2)), 4},
    {STRID(SubscribeEventAdp), STRID(eventFilter)),
     CCompositeFB::scmAdapterMarker | 0, STRID(ANYToJSON), STRID(input)), 5},
    {STRID(CreateRegisterID_1), STRID(OUT)), 7,
     STRID(F_STRING_TO_WSTRING_2), STRID(IN)), 8},
    {STRID(F_STRING_TO_WSTRING), STRID(OUT)), 3,
     STRID(Register), STRID(ID)), 9},
    {STRID(ANYToJSON), STRID(output)), 5,
     STRID(Register), STRID(SD_1)), 9},
    {STRID(F_STRING_TO_WSTRING_2), STRID(OUT)), 8,
     STRID(Unregister), STRID(ID)), 10},
    {STRID(E_CTU_1), STRID(Q)), 14,
     STRID(E_SWITCH_1_1), STRID(G)), 12},
    {STRID(E_CTU_1_1), STRID(Q)), 18,
     STRID(E_SWITCH_1_1_1), STRID(G)), 16},
    {STRID(Unregister), STRID(QO)), 10,
     STRID(E_SWITCH_2), STRID(G)), 13},
    {STRID(Register), STRID(QO)), 9,
     STRID(E_SWITCH_2_1), STRID(G)), 17},
};

const SCFB_FBFannedOutConnectionData FORTE_SubscribeEventOpcUa::scmFannedOutDataConnections[] = {
    {0, STRID(Unregister), STRID(QI)), 10},
    {2, STRID(Register), STRID(QI)), 9},
    {3, STRID(CreateRegisterID_1), STRID(IN_2)), 7},
    {7, STRID(Unregister), STRID(SD_1)), 10},
};

const SCFB_FBNData FORTE_SubscribeEventOpcUa::scmFBNData = {
    19, scmInternalFBs,     26, scmEventConnections,         10, scmFannedOutEventConnections,
    13, scmDataConnections, 4,  scmFannedOutDataConnections, 0,  nullptr,
    8,  scmParamters};
