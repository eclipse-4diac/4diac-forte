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

#include "RequestOrchestrationForm.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RequestOrchestrationForm_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RequestOrchestrationForm, g_nStringIdRequestOrchestrationForm)

const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scm_anDataInputNames[] = {g_nStringIdserviceRequestForm, g_nStringIdendpoint};

const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scm_anDataInputTypeIds[] = {g_nStringIdServiceRequestForm, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scm_anDataOutputNames[] = {g_nStringIdorchestrationResponse};

const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scm_anDataOutputTypeIds[] = {g_nStringIdARRAY, 10, g_nStringIdOrchestrationForm};

const TForteInt16 FORTE_RequestOrchestrationForm::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_RequestOrchestrationForm::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scm_anEventInputNames[] = {g_nStringIdrequestOrchestator};

const TDataIOID FORTE_RequestOrchestrationForm::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_RequestOrchestrationForm::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scm_anEventOutputNames[] = {g_nStringIdresponseReceived};

const SAdapterInstanceDef FORTE_RequestOrchestrationForm::scm_astAdapterInstances[] = {
{g_nStringIdOrchestratorRequestAdp, g_nStringIdrequestOrchestration, true }};

const SFBInterfaceSpec FORTE_RequestOrchestrationForm::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,scm_astAdapterInstances};


const SCFB_FBConnectionData FORTE_RequestOrchestrationForm::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdrequestOrchestator), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdrequestOrchestator), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdresponseReceived), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdresponseReceived), -1},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationForm::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdserviceRequestForm), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdserviceRequestForm), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdorchestrationResponse), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdorchestrationResponse), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdendpoint), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdrequestOrchestration, g_nStringIdendpoint), CCompositeFB::scm_nAdapterMarker |0},
};

const SCFB_FBNData FORTE_RequestOrchestrationForm::scm_stFBNData = {
  0, 0,
  2, scm_astEventConnections,
  0, 0,
  3, scm_astDataConnections,
  0, 0,
  0, 0
};


