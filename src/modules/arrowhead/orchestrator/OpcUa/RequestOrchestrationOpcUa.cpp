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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RequestOrchestrationOpcUa_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RequestOrchestrationOpcUa, g_nStringIdRequestOrchestrationOpcUa)

const TForteInt16 FORTE_RequestOrchestrationOpcUa::scm_anEOWithIndexes[] = {-1};
const SAdapterInstanceDef FORTE_RequestOrchestrationOpcUa::scm_astAdapterInstances[] = {
{g_nStringIdOrchestratorRequestAdp, g_nStringIdrequestOrchestration, false }};

const SFBInterfaceSpec FORTE_RequestOrchestrationOpcUa::scm_stFBInterfaceSpec = {
  0,  0,  0,  0,
  0,  0,   0, 0,  0,  0, 0, 
  0,  0, 0,
  1,scm_astAdapterInstances};


const SCFB_FBInstanceData FORTE_RequestOrchestrationOpcUa::scm_astInternalFBs[] = {
  {g_nStringIdAPPEND_STRING_3, g_nStringIdAPPEND_STRING_3},
  {g_nStringIdInitFlag, g_nStringIdE_SR},
  {g_nStringIdE_PERMIT, g_nStringIdE_PERMIT},
  {g_nStringIdF_STRING_TO_WSTRING, g_nStringIdF_STRING_TO_WSTRING},
  {g_nStringIdANYToJSON, g_nStringIdANYToJSON},
  {g_nStringIdGetHTTPOrchestrationForm, g_nStringIdGetArrayResponseFromJSON},
  {g_nStringIdRequest, g_nStringIdCLIENT_1},
  {g_nStringIdE_SWITCH, g_nStringIdE_SWITCH},
  {g_nStringIdE_SWITCH_1, g_nStringIdE_SWITCH},
  {g_nStringIdE_CTU, g_nStringIdE_CTU},
  {g_nStringIdE_DELAY, g_nStringIdE_DELAY},
};

const SCFB_FBParameter FORTE_RequestOrchestrationOpcUa::scm_astParamters[] = {
  {0, g_nStringIdIN_1, "STRING#opc_ua[CALL_METHOD; "},
  {0, g_nStringIdIN_3, "STRING##; /Objects/2:Orchestrator/2:orchestration]"},
  {9, g_nStringIdPV, "UINT#3"},
  {10, g_nStringIdDT, "T#250ms"},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationOpcUa::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdAPPEND_STRING_3, g_nStringIdCNF), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_STRING_TO_WSTRING, g_nStringIdREQ), 3},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_STRING_TO_WSTRING, g_nStringIdCNF), 3, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdInitFlag, g_nStringIdS), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdANYToJSON, g_nStringIdCNF), 4, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdAPPEND_STRING_3, g_nStringIdREQ), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdrequestOrchestator), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdANYToJSON, g_nStringIdREQ), 4},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGetHTTPOrchestrationForm, g_nStringIdCNF), 5, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdresponseReceived), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdInitFlag, g_nStringIdEO), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdINIT), 6},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_PERMIT, g_nStringIdEO), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdREQ), 6},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdINITO), 6, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_PERMIT, g_nStringIdEI), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH, g_nStringIdEO1), 7, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGetHTTPOrchestrationForm, g_nStringIdREQ), 5},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdCNF), 6, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH, g_nStringIdEI), 7},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH, g_nStringIdEO0), 7, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdCU), 9},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DELAY, g_nStringIdEO), 10, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdREQ), 6},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdCUO), 9, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH_1, g_nStringIdEI), 8},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH_1, g_nStringIdEO0), 8, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DELAY, g_nStringIdSTART), 10},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH_1, g_nStringIdEO1), 8, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdInitFlag, g_nStringIdR), 1},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationOpcUa::scm_astFannedOutEventConnections[] = {
  {8, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdInitFlag, g_nStringIdR), 1},
  {8, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdR), 9},
  {14, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGetHTTPOrchestrationForm, g_nStringIdREQ), 5},
  {14, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdR), 9},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationOpcUa::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdAPPEND_STRING_3, g_nStringIdOUT), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_STRING_TO_WSTRING, g_nStringIdIN), 3},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdInitFlag, g_nStringIdQ), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_PERMIT, g_nStringIdPERMIT), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdserviceRequestForm), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdANYToJSON, g_nStringIdinput), 4},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdendpoint), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdAPPEND_STRING_3, g_nStringIdIN_2), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGetHTTPOrchestrationForm, g_nStringIdoutput), 5, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdorchestrationResponse), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_STRING_TO_WSTRING, g_nStringIdOUT), 3, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdID), 6},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdANYToJSON, g_nStringIdoutput), 4, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdSD_1), 6},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdRD_1), 6, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGetHTTPOrchestrationForm, g_nStringIdresponse), 5},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdQO), 6, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH, g_nStringIdG), 7},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdQ), 9, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH_1, g_nStringIdG), 8},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationOpcUa::scm_astFannedOutDataConnections[] = {
  {1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdQI), 6},
};

const SCFB_FBNData FORTE_RequestOrchestrationOpcUa::scm_stFBNData = {
  11, scm_astInternalFBs,
  15, scm_astEventConnections,
  4, scm_astFannedOutEventConnections,
  10, scm_astDataConnections,
  1, scm_astFannedOutDataConnections,
  4, scm_astParamters
};


