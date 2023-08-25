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

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmDataInputNames[] = {g_nStringIdserviceEntries};

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmDataInputTypeIds[] = {g_nStringIdARRAY, 10, g_nStringIdServiceRegistryEntry};

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmDataOutputNames[] = {g_nStringIdserviceQueryForm, g_nStringIdendpoint};

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmDataOutputTypeIds[] = {g_nStringIdServiceQueryForm, g_nStringIdWSTRING};

const TDataIOID FORTE_QueryServicesAdp::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_QueryServicesAdp::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmEventInputNames[] = {g_nStringIdqueried};

const TDataIOID FORTE_QueryServicesAdp::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_QueryServicesAdp::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmEventOutputNames[] = {g_nStringIdquery};

const SFBInterfaceSpec FORTE_QueryServicesAdp::scmFBInterfaceSpecSocket = {
  1,  scmEventInputNames,  scmEIWith,  scmEIWithIndexes,
  1,  scmEventOutputNames,  scmEOWith, scmEOWithIndexes,  1,  scmDataInputNames, scmDataInputTypeIds,
  2,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_QueryServicesAdp::scmFBInterfaceSpecPlug = {
  1,  scmEventOutputNames,  scmEOWith,  scmEOWithIndexes,
  1,  scmEventInputNames,  scmEIWith, scmEIWithIndexes,  2,  scmDataOutputNames, scmDataOutputTypeIds,
  1,  scmDataInputNames, scmDataInputTypeIds,
  0, 0
};



