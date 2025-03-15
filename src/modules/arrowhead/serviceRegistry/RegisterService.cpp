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

USE_STRING_ID(doneRegister);
USE_STRING_ID(doneUnregister);
USE_STRING_ID(endpoint);
USE_STRING_ID(Event);
USE_STRING_ID(register);
USE_STRING_ID(registerService);
USE_STRING_ID(RegisterService);
USE_STRING_ID(RegisterServiceAdp);
USE_STRING_ID(serviceRegistryEntry);
USE_STRING_ID(ServiceRegistryEntry);
USE_STRING_ID(unregister);
USE_STRING_ID(unregisterService);
USE_STRING_ID(WSTRING);


DEFINE_FIRMWARE_FB(FORTE_RegisterService, STRID(RegisterService))

const CStringDictionary::TStringId FORTE_RegisterService::scmDataInputNames[] = {STRID(serviceRegistryEntry), STRID(endpoint)};

const CStringDictionary::TStringId FORTE_RegisterService::scmDataInputTypeIds[] = {STRID(ServiceRegistryEntry), STRID(WSTRING)};

const TForteInt16 FORTE_RegisterService::scmEIWithIndexes[] = {0, 3};
const TDataIOID FORTE_RegisterService::scmEIWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_RegisterService::scmEventInputNames[] = {STRID(register), STRID(unregister)};
const CStringDictionary::TStringId FORTE_RegisterService::scmEventInputTypeIds[] = {STRID(Event), STRID(Event)};

const TForteInt16 FORTE_RegisterService::scmEOWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_RegisterService::scmEventOutputNames[] = {STRID(doneRegister), STRID(doneUnregister)};
const CStringDictionary::TStringId FORTE_RegisterService::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};

const SAdapterInstanceDef FORTE_RegisterService::scmAdapterInstances[] = {
{STRID(RegisterServiceAdp), STRID(registerService), true }};

const SFBInterfaceSpec FORTE_RegisterService::scmFBInterfaceSpec = {
  2,  scmEventInputNames, scmEventInputTypeIds,  scmEIWith,  scmEIWithIndexes,
  2,  scmEventOutputNames, scmEventOutputTypeIds,  0, 0,  2,  scmDataInputNames, scmDataInputTypeIds,
  0,  0, 0,
  1,scmAdapterInstances};


const SCFB_FBConnectionData FORTE_RegisterService::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(register)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(registerService)), CCompositeFB::scmAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(unregister)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(unregisterService)), CCompositeFB::scmAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(doneRegister)), CCompositeFB::scmAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(doneRegister)), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(doneUnregister)), CCompositeFB::scmAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(doneUnregister)), -1},
};

const SCFB_FBConnectionData FORTE_RegisterService::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(serviceRegistryEntry)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(serviceRegistryEntry)), CCompositeFB::scmAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(endpoint)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(registerService), STRID(endpoint)), CCompositeFB::scmAdapterMarker |0},
};

const SCFB_FBNData FORTE_RegisterService::scmFBNData = {
  0, 0,
  4, scmEventConnections,
  0, 0,
  2, scmDataConnections,
  0, 0,
  0, nullptr,
  0, 0
};


