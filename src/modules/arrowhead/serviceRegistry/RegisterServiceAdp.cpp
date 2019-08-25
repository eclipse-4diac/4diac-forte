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

#include "RegisterServiceAdp.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RegisterServiceAdp_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_RegisterServiceAdp, g_nStringIdRegisterServiceAdp)

const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scm_anDataOutputNames[] = {g_nStringIdserviceRegistryEntry, g_nStringIdendpoint};

const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scm_anDataOutputTypeIds[] = {g_nStringIdServiceRegistryEntry, g_nStringIdWSTRING};

const TForteInt16 FORTE_RegisterServiceAdp::scm_anEIWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scm_anEventInputNames[] = {g_nStringIddoneRegister, g_nStringIddoneUnregister};

const TDataIOID FORTE_RegisterServiceAdp::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
const TForteInt16 FORTE_RegisterServiceAdp::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scm_anEventOutputNames[] = {g_nStringIdregisterService, g_nStringIdunregisterService};

const SFBInterfaceSpec FORTE_RegisterServiceAdp::scm_stFBInterfaceSpecSocket = {
  2,  scm_anEventInputNames,  0,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  0,  0, 0, 
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_RegisterServiceAdp::scm_stFBInterfaceSpecPlug = {
  2,  scm_anEventOutputNames,  scm_anEOWith,  scm_anEOWithIndexes,
  2,  scm_anEventInputNames,  0, 0,  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0,  0, 0,
  0, 0
};



