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

#include "RequestOrchestrationHTTP.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RequestOrchestrationHTTP_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RequestOrchestrationHTTP, g_nStringIdRequestOrchestrationHTTP)

const TForteInt16 FORTE_RequestOrchestrationHTTP::scm_anEOWithIndexes[] = {-1};
const SAdapterInstanceDef FORTE_RequestOrchestrationHTTP::scm_astAdapterInstances[] = {
{g_nStringIdOrchestratorRequestAdp, g_nStringIdrequestOrchestration, false }};

const SFBInterfaceSpec FORTE_RequestOrchestrationHTTP::scm_stFBInterfaceSpec = {
  0,  0,  0,  0,
  0,  0,   0, 0,  0,  0, 0, 
  0,  0, 0,
  1,scm_astAdapterInstances};


const SCFB_FBInstanceData FORTE_RequestOrchestrationHTTP::scm_astInternalFBs[] = {
  {g_nStringIdAPPEND_STRING_3, g_nStringIdAPPEND_STRING_3},
  {g_nStringIdRequest, g_nStringIdCLIENT_1_2},
  {g_nStringIdInitFlag, g_nStringIdE_SR},
  {g_nStringIdE_PERMIT, g_nStringIdE_PERMIT},
  {g_nStringIdF_STRING_TO_WSTRING, g_nStringIdF_STRING_TO_WSTRING},
  {g_nStringIdANYToJSON, g_nStringIdANYToJSON},
  {g_nStringIdGetHTTPOrchestrationForm, g_nStringIdGetArrayResponseFromJSON},
};

const SCFB_FBParameter FORTE_RequestOrchestrationHTTP::scm_astParamters[] = {
  {0, g_nStringIdIN_1, "STRING#http["},
  {0, g_nStringIdIN_3, "STRING#/orchestration; POST; application/json]"},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationHTTP::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdAPPEND_STRING_3, g_nStringIdCNF), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_STRING_TO_WSTRING, g_nStringIdREQ), 4},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_STRING_TO_WSTRING, g_nStringIdCNF), 4, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdInitFlag, g_nStringIdS), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdInitFlag, g_nStringIdEO), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdINIT), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_PERMIT, g_nStringIdEO), 3, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdREQ), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdANYToJSON, g_nStringIdCNF), 5, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdAPPEND_STRING_3, g_nStringIdREQ), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdCNF), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdInitFlag, g_nStringIdR), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdINITO), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_PERMIT, g_nStringIdEI), 3},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdrequestOrchestator), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdANYToJSON, g_nStringIdREQ), 5},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGetHTTPOrchestrationForm, g_nStringIdCNF), 6, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdresponseReceived), CCompositeFB::scm_nAdapterMarker |0},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationHTTP::scm_astFannedOutEventConnections[] = {
  {5, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGetHTTPOrchestrationForm, g_nStringIdREQ), 6},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationHTTP::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdAPPEND_STRING_3, g_nStringIdOUT), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_STRING_TO_WSTRING, g_nStringIdIN), 4},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_STRING_TO_WSTRING, g_nStringIdOUT), 4, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdID), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdInitFlag, g_nStringIdQ), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_PERMIT, g_nStringIdPERMIT), 3},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdANYToJSON, g_nStringIdoutput), 5, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdSD_1), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdserviceRequestForm), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdANYToJSON, g_nStringIdinput), 5},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdendpoint), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdAPPEND_STRING_3, g_nStringIdIN_2), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdRD_2), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGetHTTPOrchestrationForm, g_nStringIdresponse), 6},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGetHTTPOrchestrationForm, g_nStringIdoutput), 6, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdorchestrationResponse), CCompositeFB::scm_nAdapterMarker |0},
};

const SCFB_FBFannedOutConnectionData FORTE_RequestOrchestrationHTTP::scm_astFannedOutDataConnections[] = {
  {2, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRequest, g_nStringIdQI), 1},
};

const SCFB_FBNData FORTE_RequestOrchestrationHTTP::scm_stFBNData = {
  7, scm_astInternalFBs,
  9, scm_astEventConnections,
  1, scm_astFannedOutEventConnections,
  8, scm_astDataConnections,
  1, scm_astFannedOutDataConnections,
  2, scm_astParamters
};


