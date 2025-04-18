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

USE_STRING_ID(address);
USE_STRING_ID(ARRAY);
USE_STRING_ID(endpoint);
USE_STRING_ID(Event);
USE_STRING_ID(queried);
USE_STRING_ID(query);
USE_STRING_ID(QueryService);
USE_STRING_ID(queryServices);
USE_STRING_ID(QueryServicesAdp);
USE_STRING_ID(serviceEntries);
USE_STRING_ID(serviceQueryForm);
USE_STRING_ID(ServiceQueryForm);
USE_STRING_ID(ServiceRegistryEntry);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_QueryService, STRID(QueryService))

const CStringDictionary::TStringId FORTE_QueryService::scmDataInputNames[] = {STRID(serviceQueryForm), STRID(address)};

const CStringDictionary::TStringId FORTE_QueryService::scmDataInputTypeIds[] = {STRID(ServiceQueryForm),
                                                                                STRID(WSTRING)};

const CStringDictionary::TStringId FORTE_QueryService::scmDataOutputNames[] = {STRID(serviceEntries)};

const CStringDictionary::TStringId FORTE_QueryService::scmDataOutputTypeIds[] = {STRID(ARRAY), 10,
                                                                                 STRID(ServiceRegistryEntry)};

const TForteInt16 FORTE_QueryService::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_QueryService::scmEIWith[] = {0, 1, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_QueryService::scmEventInputNames[] = {STRID(query)};
const CStringDictionary::TStringId FORTE_QueryService::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_QueryService::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_QueryService::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_QueryService::scmEventOutputNames[] = {STRID(queried)};
const CStringDictionary::TStringId FORTE_QueryService::scmEventOutputTypeIds[] = {STRID(Event)};

const SAdapterInstanceDef FORTE_QueryService::scmAdapterInstances[] = {
    {STRID(QueryServicesAdp), STRID(queryServices), true}};

const SFBInterfaceSpec FORTE_QueryService::scmFBInterfaceSpec = {1,
                                                                 scmEventInputNames,
                                                                 scmEventInputTypeIds,
                                                                 scmEIWith,
                                                                 scmEIWithIndexes,
                                                                 1,
                                                                 scmEventOutputNames,
                                                                 scmEventOutputTypeIds,
                                                                 scmEOWith,
                                                                 scmEOWithIndexes,
                                                                 2,
                                                                 scmDataInputNames,
                                                                 scmDataInputTypeIds,
                                                                 1,
                                                                 scmDataOutputNames,
                                                                 scmDataOutputTypeIds,
                                                                 1,
                                                                 scmAdapterInstances};

const SCFB_FBConnectionData FORTE_QueryService::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(query)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(queryServices), STRID(query)), CCompositeFB::scmAdapterMarker | 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(queryServices), STRID(queried)), CCompositeFB::scmAdapterMarker | 0,
     GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(queried)), -1},
};

const SCFB_FBConnectionData FORTE_QueryService::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(serviceQueryForm)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(queryServices), STRID(serviceQueryForm)),
     CCompositeFB::scmAdapterMarker | 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(queryServices), STRID(serviceEntries)), CCompositeFB::scmAdapterMarker | 0,
     GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(serviceEntries)), -1},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(address)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(queryServices), STRID(endpoint)), CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBNData FORTE_QueryService::scmFBNData = {
    0, 0, 2, scmEventConnections, 0, 0, 3, scmDataConnections, 0, 0, 0, nullptr, 0, 0};
