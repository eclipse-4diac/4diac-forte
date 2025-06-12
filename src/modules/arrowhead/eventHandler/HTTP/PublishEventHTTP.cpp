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

USE_STRING_ID(ANYToJSON);
USE_STRING_ID(APPEND_STRING_3);
USE_STRING_ID(ArrowheadPublishAdp);
USE_STRING_ID(CLIENT_1_2);
USE_STRING_ID(CNF);
USE_STRING_ID(EI);
USE_STRING_ID(endpoint);
USE_STRING_ID(EO);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_SR);
USE_STRING_ID(F_STRING_TO_WSTRING);
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
USE_STRING_ID(publish);
USE_STRING_ID(published);
USE_STRING_ID(publishEvent);
USE_STRING_ID(PublishEventHTTP);
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(Query);
USE_STRING_ID(R);
USE_STRING_ID(REQ);
USE_STRING_ID(S);
USE_STRING_ID(SD_1);

DEFINE_FIRMWARE_FB(FORTE_PublishEventHTTP, STRID(PublishEventHTTP))

namespace {
  const auto cSocketNameIds = std::array{STRID(publishEvent)};

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
    {STRID(ANYToJSON), STRID(ANYToJSON)},
    {STRID(InitFlag), STRID(E_SR)},
    {STRID(APPEND_STRING_3), STRID(APPEND_STRING_3)},
    {STRID(F_STRING_TO_WSTRING), STRID(F_STRING_TO_WSTRING)},
    {STRID(Query), STRID(CLIENT_1_2)},
    {STRID(E_PERMIT), STRID(E_PERMIT)},
};

const SCFB_FBParameter FORTE_PublishEventHTTP::scmParamters[] = {
    {2, STRID(IN_1), "STRING#http["},
    {2, STRID(IN_3), "STRING#/publish; POST; application/json]"},
};

const SCFB_FBConnectionData FORTE_PublishEventHTTP::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(EO)), 5,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Query), STRID(REQ)), 4},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(APPEND_STRING_3), STRID(CNF)), 2,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(REQ)), 3},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlag), STRID(EO)), 1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Query), STRID(INIT)), 4},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(CNF)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(APPEND_STRING_3), STRID(REQ)), 2},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(CNF)), 3,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlag), STRID(S)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Query), STRID(INITO)), 4,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(EI)), 5},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Query), STRID(CNF)), 4,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlag), STRID(R)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(publishEvent), STRID(publish)), CCompositeFB::scmAdapterMarker | 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(REQ)), 0},
};

const SCFB_FBFannedOutConnectionData FORTE_PublishEventHTTP::scmFannedOutEventConnections[] = {
    {6, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(publishEvent), STRID(published)), CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBConnectionData FORTE_PublishEventHTTP::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(output)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Query), STRID(SD_1)), 4},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlag), STRID(Q)), 1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Query), STRID(QI)), 4},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(APPEND_STRING_3), STRID(OUT)), 2,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(IN)), 3},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(OUT)), 3,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Query), STRID(ID)), 4},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(publishEvent), STRID(publishEvent)), CCompositeFB::scmAdapterMarker | 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(input)), 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(publishEvent), STRID(endpoint)), CCompositeFB::scmAdapterMarker | 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(APPEND_STRING_3), STRID(IN_2)), 2},
};

const SCFB_FBFannedOutConnectionData FORTE_PublishEventHTTP::scmFannedOutDataConnections[] = {
    {1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(PERMIT)), 5},
};

const SCFB_FBNData FORTE_PublishEventHTTP::scmFBNData = {
    6, scmInternalFBs,     8, scmEventConnections,         1, scmFannedOutEventConnections,
    6, scmDataConnections, 1, scmFannedOutDataConnections, 0, nullptr,
    2, scmParamters};
