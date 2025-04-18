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

#include "RequestOrchestrationOpcUa.h"

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
USE_STRING_ID(GetHTTPOrchestrationForm);
USE_STRING_ID(ID);
USE_STRING_ID(IN);
USE_STRING_ID(IN_1);
USE_STRING_ID(IN_2);
USE_STRING_ID(IN_3);
USE_STRING_ID(INIT);
USE_STRING_ID(InitFlag);
USE_STRING_ID(INITO);
USE_STRING_ID(input);
USE_STRING_ID(orchestrationResponse);
USE_STRING_ID(OrchestratorRequestAdp);
USE_STRING_ID(OUT);
USE_STRING_ID(output);
USE_STRING_ID(PERMIT);
USE_STRING_ID(PV);
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(R);
USE_STRING_ID(RD_1);
USE_STRING_ID(REQ);
USE_STRING_ID(Request);
USE_STRING_ID(requestOrchestator);
USE_STRING_ID(requestOrchestration);
USE_STRING_ID(RequestOrchestrationOpcUa);
USE_STRING_ID(response);
USE_STRING_ID(responseReceived);
USE_STRING_ID(S);
USE_STRING_ID(SD_1);
USE_STRING_ID(serviceRequestForm);
USE_STRING_ID(START);

DEFINE_FIRMWARE_FB(FORTE_RequestOrchestrationOpcUa, STRID(RequestOrchestrationOpcUa))

const TForteInt16 FORTE_RequestOrchestrationOpcUa::scmEOWithIndexes[] = {-1};
const SAdapterInstanceDef FORTE_RequestOrchestrationOpcUa::scmAdapterInstances[] = {
    {STRID(OrchestratorRequestAdp), STRID(requestOrchestration), false}};

const SFBInterfaceSpec FORTE_RequestOrchestrationOpcUa::scmFBInterfaceSpec = {
    0, 0, nullptr, 0, 0, 0, 0, nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 1, scmAdapterInstances};

const SCFB_FBInstanceData FORTE_RequestOrchestrationOpcUa::scmInternalFBs[] = {
    {STRID(APPEND_STRING_3), STRID(APPEND_STRING_3)},
    {STRID(InitFlag), STRID(E_SR)},
    {STRID(E_PERMIT), STRID(E_PERMIT)},
    {STRID(F_STRING_TO_WSTRING), STRID(F_STRING_TO_WSTRING)},
    {STRID(ANYToJSON), STRID(ANYToJSON)},
    {STRID(GetHTTPOrchestrationForm), STRID(GetArrayResponseFromJSON)},
    {STRID(Request), STRID(CLIENT_1)},
    {STRID(E_SWITCH), STRID(E_SWITCH)},
    {STRID(E_SWITCH_1), STRID(E_SWITCH)},
    {STRID(E_CTU), STRID(E_CTU)},
    {STRID(E_DELAY), STRID(E_DELAY)},
};

const SCFB_FBParameter FORTE_RequestOrchestrationOpcUa::scmParamters[] = {
    {0, STRID(IN_1), "STRING#opc_ua[CALL_METHOD; "},
    {0, STRID(IN_3), "STRING##; /Objects/2:Orchestrator/2:orchestration]"},
    {9, STRID(PV), "UINT#3"},
    {10, STRID(DT), "T#250ms"},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationOpcUa::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(APPEND_STRING_3), STRID(CNF)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(REQ)), 3},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(CNF)), 3,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlag), STRID(S)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(CNF)), 4,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(APPEND_STRING_3), STRID(REQ)), 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(requestOrchestration), STRID(requestOrchestator)),
     CCompositeFB::scmAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(REQ)), 4},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(GetHTTPOrchestrationForm), STRID(CNF)), 5,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(requestOrchestration), STRID(responseReceived)),
     CCompositeFB::scmAdapterMarker | 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlag), STRID(EO)), 1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Request), STRID(INIT)), 6},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(EO)), 2,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Request), STRID(REQ)), 6},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Request), STRID(INITO)), 6,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(EI)), 2},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EO1)), 7,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(GetHTTPOrchestrationForm), STRID(REQ)), 5},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Request), STRID(CNF)), 6,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EI)), 7},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EO0)), 7,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(CU)), 9},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(EO)), 10,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Request), STRID(REQ)), 6},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(CUO)), 9,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH_1), STRID(EI)), 8},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH_1), STRID(EO0)), 8,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(START)), 10},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH_1), STRID(EO1)), 8,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlag), STRID(R)), 1},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationOpcUa::scmFannedOutEventConnections[] = {
    {8, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlag), STRID(R)), 1},
    {8, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(R)), 9},
    {14, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(GetHTTPOrchestrationForm), STRID(REQ)), 5},
    {14, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(R)), 9},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationOpcUa::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(APPEND_STRING_3), STRID(OUT)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(IN)), 3},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(InitFlag), STRID(Q)), 1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(PERMIT)), 2},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(requestOrchestration), STRID(serviceRequestForm)),
     CCompositeFB::scmAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(input)), 4},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(requestOrchestration), STRID(endpoint)),
     CCompositeFB::scmAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(APPEND_STRING_3), STRID(IN_2)), 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(GetHTTPOrchestrationForm), STRID(output)), 5,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(requestOrchestration), STRID(orchestrationResponse)),
     CCompositeFB::scmAdapterMarker | 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(F_STRING_TO_WSTRING), STRID(OUT)), 3,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Request), STRID(ID)), 6},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ANYToJSON), STRID(output)), 4,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Request), STRID(SD_1)), 6},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Request), STRID(RD_1)), 6,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(GetHTTPOrchestrationForm), STRID(response)), 5},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Request), STRID(QO)), 6,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(G)), 7},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_CTU), STRID(Q)), 9,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH_1), STRID(G)), 8},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationOpcUa::scmFannedOutDataConnections[] = {
    {1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(Request), STRID(QI)), 6},
};

const SCFB_FBNData FORTE_RequestOrchestrationOpcUa::scmFBNData = {
    11, scmInternalFBs,     15, scmEventConnections,         4, scmFannedOutEventConnections,
    10, scmDataConnections, 1,  scmFannedOutDataConnections, 0, nullptr,
    4,  scmParamters};
