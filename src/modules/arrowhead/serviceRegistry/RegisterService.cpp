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

#include "RegisterService.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RegisterService_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RegisterService, g_nStringIdRegisterService)

const CStringDictionary::TStringId FORTE_RegisterService::scm_anDataInputNames[] = {g_nStringIdserviceRegistryEntry, g_nStringIdendpoint};

const CStringDictionary::TStringId FORTE_RegisterService::scm_anDataInputTypeIds[] = {g_nStringIdServiceRegistryEntry, g_nStringIdWSTRING};

const TForteInt16 FORTE_RegisterService::scm_anEIWithIndexes[] = {0, 3};
const TDataIOID FORTE_RegisterService::scm_anEIWith[] = {0, 1, 255, 0, 1, 255};
const CStringDictionary::TStringId FORTE_RegisterService::scm_anEventInputNames[] = {g_nStringIdregister, g_nStringIdunregister};

const TForteInt16 FORTE_RegisterService::scm_anEOWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_RegisterService::scm_anEventOutputNames[] = {g_nStringIddoneRegister, g_nStringIddoneUnregister};

const SAdapterInstanceDef FORTE_RegisterService::scm_astAdapterInstances[] = {
{g_nStringIdRegisterServiceAdp, g_nStringIdregisterService, true }};

const SFBInterfaceSpec FORTE_RegisterService::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  0, 0,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  1,scm_astAdapterInstances};


const SCFB_FBConnectionData FORTE_RegisterService::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdregister), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdregisterService, g_nStringIdregisterService), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdunregister), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdregisterService, g_nStringIdunregisterService), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdregisterService, g_nStringIddoneRegister), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIddoneRegister), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdregisterService, g_nStringIddoneUnregister), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIddoneUnregister), -1},
};

const SCFB_FBConnectionData FORTE_RegisterService::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdserviceRegistryEntry), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdregisterService, g_nStringIdserviceRegistryEntry), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdendpoint), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdregisterService, g_nStringIdendpoint), CCompositeFB::scm_nAdapterMarker |0},
};

const SCFB_FBNData FORTE_RegisterService::scm_stFBNData = {
  0, 0,
  4, scm_astEventConnections,
  0, 0,
  2, scm_astDataConnections,
  0, 0,
  0, 0
};


