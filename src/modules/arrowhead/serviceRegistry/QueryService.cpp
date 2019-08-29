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

#include "QueryService.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "QueryService_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_QueryService, g_nStringIdQueryService)

const CStringDictionary::TStringId FORTE_QueryService::scm_anDataInputNames[] = {g_nStringIdserviceQueryForm, g_nStringIdaddress};

const CStringDictionary::TStringId FORTE_QueryService::scm_anDataInputTypeIds[] = {g_nStringIdServiceQueryForm, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_QueryService::scm_anDataOutputNames[] = {g_nStringIdserviceEntries};

const CStringDictionary::TStringId FORTE_QueryService::scm_anDataOutputTypeIds[] = {g_nStringIdARRAY, 10, g_nStringIdServiceRegistryEntry};

const TForteInt16 FORTE_QueryService::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_QueryService::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FORTE_QueryService::scm_anEventInputNames[] = {g_nStringIdquery};

const TDataIOID FORTE_QueryService::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_QueryService::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_QueryService::scm_anEventOutputNames[] = {g_nStringIdqueried};

const SAdapterInstanceDef FORTE_QueryService::scm_astAdapterInstances[] = {
{g_nStringIdQueryServicesAdp, g_nStringIdqueryServices, true }};

const SFBInterfaceSpec FORTE_QueryService::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,scm_astAdapterInstances};


const SCFB_FBConnectionData FORTE_QueryService::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdquery), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdqueryServices, g_nStringIdquery), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdqueryServices, g_nStringIdqueried), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdqueried), -1},
};

const SCFB_FBConnectionData FORTE_QueryService::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdserviceQueryForm), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdqueryServices, g_nStringIdserviceQueryForm), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdqueryServices, g_nStringIdserviceEntries), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdserviceEntries), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdaddress), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdqueryServices, g_nStringIdendpoint), CCompositeFB::scm_nAdapterMarker |0},
};

const SCFB_FBNData FORTE_QueryService::scm_stFBNData = {
  0, 0,
  2, scm_astEventConnections,
  0, 0,
  3, scm_astDataConnections,
  0, 0,
  0, 0
};


