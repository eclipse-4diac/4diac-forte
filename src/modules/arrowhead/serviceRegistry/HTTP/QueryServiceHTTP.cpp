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

USE_STRING_ID(ANYToJSON);
USE_STRING_ID(APPEND_STRING_3);
USE_STRING_ID(CLIENT_1_2);
USE_STRING_ID(CNF);
USE_STRING_ID(EI);
USE_STRING_ID(endpoint);
USE_STRING_ID(EO);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_SR);
USE_STRING_ID(F_STRING_TO_WSTRING);
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
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(queried);
USE_STRING_ID(query);
USE_STRING_ID(Query);
USE_STRING_ID(QueryServiceHTTP);
USE_STRING_ID(queryServices);
USE_STRING_ID(QueryServicesAdp);
USE_STRING_ID(R);
USE_STRING_ID(RD_2);
USE_STRING_ID(REQ);
USE_STRING_ID(response);
USE_STRING_ID(S);
USE_STRING_ID(SD_1);
USE_STRING_ID(serviceEntries);
USE_STRING_ID(serviceQueryForm);

DEFINE_FIRMWARE_FB(FORTE_QueryServiceHTTP, STRID(QueryServiceHTTP))

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

const SCFB_FBInstanceData FORTE_QueryServiceHTTP::scmInternalFBs[] = {
    {STRID(APPEND_STRING_3), STRID(APPEND_STRING_3)},
    {STRID(Query), STRID(CLIENT_1_2)},
    {STRID(F_STRING_TO_WSTRING), STRID(F_STRING_TO_WSTRING)},
    {STRID(InitFlag), STRID(E_SR)},
    {STRID(E_PERMIT), STRID(E_PERMIT)},
    {STRID(ANYToJSON), STRID(ANYToJSON)},
    {STRID(GetHTTPResult), STRID(GetArrayResponseFromJSON)},
};

const SCFB_FBParameter FORTE_QueryServiceHTTP::scmParamters[] = {
    {0, STRID(IN_1), "STRING#http["},
    {0, STRID(IN_3), "STRING#/query; PUT; application/json]"},
};

const SCFB_FBConnectionData FORTE_QueryServiceHTTP::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(APPEND_STRING_3), STRID(CNF)), 0,
     STRID(F_STRING_TO_WSTRING), STRID(REQ)), 2},
    {STRID(F_STRING_TO_WSTRING), STRID(CNF)), 2,
     STRID(InitFlag), STRID(S)), 3},
    {STRID(InitFlag), STRID(EO)), 3,
     STRID(Query), STRID(INIT)), 1},
    {STRID(Query), STRID(INITO)), 1,
     STRID(E_PERMIT), STRID(EI)), 4},
    {STRID(E_PERMIT), STRID(EO)), 4,
     STRID(Query), STRID(REQ)), 1},
    {STRID(Query), STRID(CNF)), 1,
     STRID(InitFlag), STRID(R)), 3},
    {STRID(ANYToJSON), STRID(CNF)), 5,
     STRID(APPEND_STRING_3), STRID(REQ)), 0},
    {STRID(queryServices), STRID(query)), CCompositeFB::scmAdapterMarker | 0,
     STRID(ANYToJSON), STRID(REQ)), 5},
    {STRID(GetHTTPResult), STRID(CNF)), 6,
     STRID(queryServices), STRID(queried)), CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBFannedOutConnectionData FORTE_QueryServiceHTTP::scmFannedOutEventConnections[] = {
    {5, STRID(GetHTTPResult), STRID(REQ)), 6},
};

const SCFB_FBConnectionData FORTE_QueryServiceHTTPauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {STRID(APPEND_STRING_3), STRID(OUT)), 0,
     STRID(F_STRING_TO_WSTRING), STRID(IN)), 2},
    {STRID(F_STRING_TO_WSTRING), STRID(OUT)), 2,
     STRID(Query), STRID(ID)), 1},
    {STRID(InitFlag), STRID(Q)), 3,
     STRID(E_PERMIT), STRID(PERMIT)), 4},
    {STRID(ANYToJSON), STRID(output)), 5,
     STRID(Query), STRID(SD_1)), 1},
    {STRID(queryServices), STRID(serviceQueryForm)),
     CCompositeFB::scmAdapterMarker | 0, STRID(ANYToJSON), STRID(input)), 5},
    {STRID(queryServices), STRID(endpoint)), CCompositeFB::scmAdapterMarker | 0,
     STRID(APPEND_STRING_3), STRID(IN_2)), 0},
    {STRID(Query), STRID(RD_2)), 1,
     STRID(GetHTTPResult), STRID(response)), 6},
    {STRID(GetHTTPResult), STRID(output)), 6,
     STRID(queryServices), STRID(serviceEntries)),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBFannedOutConnectionData FORTE_QueryServiceHTTP::scmFannedOutDataConnections[] = {
    {2, STRID(Query), STRID(QI)), 1},
};

const SCFB_FBNData FORTE_QueryServiceHTTP::scmFBNData = {
    7, scmInternalFBs,     9, scmEventConnections,         1, scmFannedOutEventConnections,
    8, scmDataConnections, 1, scmFannedOutDataConnections, 0, nullptr,
    2, scmParamters};
