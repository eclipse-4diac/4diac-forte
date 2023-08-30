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

const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scmDataOutputNames[] = {g_nStringIdserviceRegistryEntry, g_nStringIdendpoint};

const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scmDataOutputTypeIds[] = {g_nStringIdServiceRegistryEntry, g_nStringIdWSTRING};

const TForteInt16 FORTE_RegisterServiceAdp::scmEIWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scmEventInputNames[] = {g_nStringIddoneRegister, g_nStringIddoneUnregister};

const TDataIOID FORTE_RegisterServiceAdp::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_RegisterServiceAdp::scmEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scmEventOutputNames[] = {g_nStringIdregisterService, g_nStringIdunregisterService};

const SFBInterfaceSpec FORTE_RegisterServiceAdp::scmFBInterfaceSpecSocket = {
  2,  scmEventInputNames,  0,  scmEIWithIndexes,
  2,  scmEventOutputNames,  scmEOWith, scmEOWithIndexes,  0,  0, 0, 
  2,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_RegisterServiceAdp::scmFBInterfaceSpecPlug = {
  2,  scmEventOutputNames,  scmEOWith,  scmEOWithIndexes,
  2,  scmEventInputNames,  0, 0,  2,  scmDataOutputNames, scmDataOutputTypeIds,
  0,  0, 0,
  0, 0
};



