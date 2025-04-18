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

#include "QueryServicesAdp_adp.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(endpoint);
USE_STRING_ID(queried);
USE_STRING_ID(query);
USE_STRING_ID(QueryServicesAdp);
USE_STRING_ID(serviceEntries);
USE_STRING_ID(serviceQueryForm);
USE_STRING_ID(ServiceQueryForm);
USE_STRING_ID(ServiceRegistryEntry);
USE_STRING_ID(WSTRING);

DEFINE_ADAPTER_TYPE(FORTE_QueryServicesAdp, STRID(QueryServicesAdp))

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmDataInputNames[] = {STRID(serviceEntries)};

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmDataInputTypeIds[] = {STRID(ARRAY), 10,
                                                                                    STRID(ServiceRegistryEntry)};

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmDataOutputNames[] = {STRID(serviceQueryForm),
                                                                                   STRID(endpoint)};

const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmDataOutputTypeIds[] = {STRID(ServiceQueryForm),
                                                                                     STRID(WSTRING)};

const TDataIOID FORTE_QueryServicesAdp::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_QueryServicesAdp::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmEventInputNames[] = {STRID(queried)};

const TDataIOID FORTE_QueryServicesAdp::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_QueryServicesAdp::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_QueryServicesAdp::scmEventOutputNames[] = {STRID(query)};

const SFBInterfaceSpec FORTE_QueryServicesAdp::scmFBInterfaceSpecSocket = {1,
                                                                           scmEventInputNames,
                                                                           scmEIWith,
                                                                           scmEIWithIndexes,
                                                                           1,
                                                                           scmEventOutputNames,
                                                                           scmEOWith,
                                                                           scmEOWithIndexes,
                                                                           1,
                                                                           scmDataInputNames,
                                                                           scmDataInputTypeIds,
                                                                           2,
                                                                           scmDataOutputNames,
                                                                           scmDataOutputTypeIds,
                                                                           0,
                                                                           0};

const SFBInterfaceSpec FORTE_QueryServicesAdp::scmFBInterfaceSpecPlug = {1,
                                                                         scmEventOutputNames,
                                                                         scmEOWith,
                                                                         scmEOWithIndexes,
                                                                         1,
                                                                         scmEventInputNames,
                                                                         scmEIWith,
                                                                         scmEIWithIndexes,
                                                                         2,
                                                                         scmDataOutputNames,
                                                                         scmDataOutputTypeIds,
                                                                         1,
                                                                         scmDataInputNames,
                                                                         scmDataInputTypeIds,
                                                                         0,
                                                                         0};
