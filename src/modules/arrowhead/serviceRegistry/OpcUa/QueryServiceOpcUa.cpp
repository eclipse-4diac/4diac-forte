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

USE_STRING_ID(ANYToJSON);
USE_STRING_ID(APPEND_STRING_3);
USE_STRING_ID(CLIENT_1);
USE_STRING_ID(CNF);
USE_STRING_ID(CU);
USE_STRING_ID(CUO);
USE_STRING_ID(DT);
USE_STRING_ID(E_CTU);
USE_STRING_ID(E_DELAY);
USE_STRING_ID(EI);
USE_STRING_ID(endpoint);
USE_STRING_ID(EO);
USE_STRING_ID(EO0);
USE_STRING_ID(EO1);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_SR);
USE_STRING_ID(E_SWITCH);
USE_STRING_ID(E_SWITCH_1);
USE_STRING_ID(F_STRING_TO_WSTRING);
USE_STRING_ID(G);
USE_STRING_ID(GetArrayResponseFromJSON);
USE_STRING_ID(GetHTTPResult);
USE_STRING_ID(ID);
USE_STRING_ID(IN);
USE_STRING_ID(IN_1);
USE_STRING_ID(IN_2);
USE_STRING_ID(IN_3);
USE_STRING_ID(INIT);
USE_STRING_ID(InitFlag);
USE_STRING_ID(INITO);
USE_STRING_ID(input);
USE_STRING_ID(OUT);
USE_STRING_ID(output);
USE_STRING_ID(PERMIT);
USE_STRING_ID(PV);
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(queried);
USE_STRING_ID(query);
USE_STRING_ID(Query);
USE_STRING_ID(QueryServiceOpcUa);
USE_STRING_ID(queryServices);
USE_STRING_ID(QueryServicesAdp);
USE_STRING_ID(R);
USE_STRING_ID(RD_1);
USE_STRING_ID(REQ);
USE_STRING_ID(response);
USE_STRING_ID(S);
USE_STRING_ID(SD_1);
USE_STRING_ID(serviceEntries);
USE_STRING_ID(serviceQueryForm);
USE_STRING_ID(START);

DEFINE_FIRMWARE_FB(FORTE_QueryServiceOpcUa, STRID(QueryServiceOpcUa))

namespace {
  const auto cSocketNameIds = std::array{STRID(queryServices)};

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
    {STRID(APPEND_STRING_3), STRID(APPEND_STRING_3)},
    {STRID(F_STRING_TO_WSTRING), STRID(F_STRING_TO_WSTRING)},
    {STRID(InitFlag), STRID(E_SR)},
    {STRID(E_PERMIT), STRID(E_PERMIT)},
    {STRID(ANYToJSON), STRID(ANYToJSON)},
    {STRID(GetHTTPResult), STRID(GetArrayResponseFromJSON)},
    {STRID(Query), STRID(CLIENT_1)},
    {STRID(E_SWITCH_1), STRID(E_SWITCH)},
    {STRID(E_SWITCH), STRID(E_SWITCH)},
    {STRID(E_DELAY), STRID(E_DELAY)},
    {STRID(E_CTU), STRID(E_CTU)},
};

const SCFB_FBParameter FORTE_QueryServiceOpcUa::scmParamters[] = {
    {0, STRID(IN_1), "STRING#opc_ua[CALL_METHOD; "},
    {0, STRID(IN_3), "STRING##; /Objects/2:ServiceRegistry/2:query]"},
    {9, STRID(DT), "T#250ms"},
    {10, STRID(PV), "UINT#3"},
};

const SCFB_FBConnectionData FORTE_QueryServiceOpcUa::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(APPEND_STRING_3), STRID(CNF)), 0,
     STRID(F_STRING_TO_WSTRING), STRID(REQ)), 1},
    {STRID(F_STRING_TO_WSTRING), STRID(CNF)), 1,
     STRID(InitFlag), STRID(S)), 2},
    {STRID(ANYToJSON), STRID(CNF)), 4,
     STRID(APPEND_STRING_3), STRID(REQ)), 0},
    {STRID(queryServices), STRID(query)), CCompositeFB::scmAdapterMarker | 0,
     STRID(ANYToJSON), STRID(REQ)), 4},
    {STRID(GetHTTPResult), STRID(CNF)), 5,
     STRID(queryServices), STRID(queried)), CCompositeFB::scmAdapterMarker | 0},
    {STRID(InitFlag), STRID(EO)), 2,
     STRID(Query), STRID(INIT)), 6},
    {STRID(E_PERMIT), STRID(EO)), 3,
     STRID(Query), STRID(REQ)), 6},
    {STRID(Query), STRID(INITO)), 6,
     STRID(E_PERMIT), STRID(EI)), 3},
    {STRID(E_SWITCH_1), STRID(EO0)), 7,
     STRID(E_DELAY), STRID(START)), 9},
    {STRID(E_CTU), STRID(CUO)), 10,
     STRID(E_SWITCH_1), STRID(EI)), 7},
    {STRID(E_SWITCH), STRID(EO0)), 8,
     STRID(E_CTU), STRID(CU)), 10},
    {STRID(E_SWITCH), STRID(EO1)), 8,
     STRID(GetHTTPResult), STRID(REQ)), 5},
    {STRID(Query), STRID(CNF)), 6,
     STRID(E_SWITCH), STRID(EI)), 8},
    {STRID(E_SWITCH_1), STRID(EO1)), 7,
     STRID(InitFlag), STRID(R)), 2},
    {STRID(E_DELAY), STRID(EO)), 9,
     STRID(Query), STRID(REQ)), 6},
};

const SCFB_FBFannedOutConnectionData FORTE_QueryServiceOpcUa::scmFannedOutEventConnections[] = {
    {11, STRID(InitFlag), STRID(R)), 2},
    {11, STRID(E_CTU), STRID(R)), 10},
    {13, STRID(GetHTTPResult), STRID(REQ)), 5},
    {13, STRID(E_CTU), STRID(R)), 10},
};

const SCFB_FBConnectionData FORTE_QueryServiceOpcUaauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(APPEND_STRING_3), STRID(OUT)), 0,
     STRID(F_STRING_TO_WSTRING), STRID(IN)), 1},
    {STRID(InitFlag), STRID(Q)), 2,
     STRID(E_PERMIT), STRID(PERMIT)), 3},
    {STRID(queryServices), STRID(serviceQueryForm)),
     CCompositeFB::scmAdapterMarker | 0, STRID(ANYToJSON), STRID(input)), 4},
    {STRID(queryServices), STRID(endpoint)), CCompositeFB::scmAdapterMarker | 0,
     STRID(APPEND_STRING_3), STRID(IN_2)), 0},
    {STRID(GetHTTPResult), STRID(output)), 5,
     STRID(queryServices), STRID(serviceEntries)),
     CCompositeFB::scmAdapterMarker | 0},
    {STRID(F_STRING_TO_WSTRING), STRID(OUT)), 1,
     STRID(Query), STRID(ID)), 6},
    {STRID(ANYToJSON), STRID(output)), 4,
     STRID(Query), STRID(SD_1)), 6},
    {STRID(Query), STRID(RD_1)), 6,
     STRID(GetHTTPResult), STRID(response)), 5},
    {STRID(E_CTU), STRID(Q)), 10,
     STRID(E_SWITCH_1), STRID(G)), 7},
    {STRID(Query), STRID(QO)), 6,
     STRID(E_SWITCH), STRID(G)), 8},
};

const SCFB_FBFannedOutConnectionData FORTE_QueryServiceOpcUa::scmFannedOutDataConnections[] = {
    {1, STRID(Query), STRID(QI)), 6},
};

const SCFB_FBNData FORTE_QueryServiceOpcUa::scmFBNData = {
    11, scmInternalFBs,     15, scmEventConnections,         4, scmFannedOutEventConnections,
    10, scmDataConnections, 1,  scmFannedOutDataConnections, 0, nullptr,
    4,  scmParamters};
