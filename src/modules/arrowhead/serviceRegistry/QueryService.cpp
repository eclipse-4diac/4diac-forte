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

namespace {
  const auto cDataInputNames = std::array{STRID(serviceQueryForm), STRID(address)};
  const auto cDataOutputNames = std::array{STRID(serviceEntries)};
  const auto cEventInputNames = std::array{STRID(query)};
  const auto cEventInputTypeIds = std::array{STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(queried)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};
  const auto cPlugNameIds = std::array{STRID(queryServices)};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = cPlugNameIds,
  };
} // namespace

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
