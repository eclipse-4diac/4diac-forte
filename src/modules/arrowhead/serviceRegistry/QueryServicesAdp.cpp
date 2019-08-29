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

#include "QueryServicesAdp.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "QueryServicesAdp_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_QueryServicesAdp, g_nStringIdQueryServicesAdp)

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scm_anDataInputNames[] = {g_nStringIdserviceEntries};

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scm_anDataInputTypeIds[] = {g_nStringIdARRAY, 10, g_nStringIdServiceRegistryEntry};

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scm_anDataOutputNames[] = {g_nStringIdserviceQueryForm, g_nStringIdendpoint};

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scm_anDataOutputTypeIds[] = {g_nStringIdServiceQueryForm, g_nStringIdWSTRING};

const TDataIOID FORTE_QueryServicesAdp::scm_anEIWith[] = {0, 255};
const TForteInt16 FORTE_QueryServicesAdp::scm_anEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_QueryServicesAdp::scm_anEventInputNames[] = {g_nStringIdqueried};

const TDataIOID FORTE_QueryServicesAdp::scm_anEOWith[] = {0, 1, 255};
const TForteInt16 FORTE_QueryServicesAdp::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_QueryServicesAdp::scm_anEventOutputNames[] = {g_nStringIdquery};

const SFBInterfaceSpec FORTE_QueryServicesAdp::scm_stFBInterfaceSpecSocket = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_QueryServicesAdp::scm_stFBInterfaceSpecPlug = {
  1,  scm_anEventOutputNames,  scm_anEOWith,  scm_anEOWithIndexes,
  1,  scm_anEventInputNames,  scm_anEIWith, scm_anEIWithIndexes,  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0, 0
};



