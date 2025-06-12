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

const auto cDataInputNames = std::array{STRID(serviceEntries)};

                                                                                    STRID(ServiceRegistryEntry)};

const auto cDataOutputNames = std::array{STRID(serviceQueryForm),
                                                                                   STRID(endpoint)};

                                                                                     STRID(WSTRING)};

const auto cEventInputNames = std::array{STRID(queried)};

const auto cEventOutputNames = std::array{STRID(query)};

const SFBInterfaceSpec FORTE_QueryServicesAdp::scmFBInterfaceSpecSocket = {1,
                                                                           scmEventInputNames,
                                                                           1,
                                                                           scmEventOutputNames,
                                                                           1,
                                                                           scmDataInputNames,
                                                                           2,
                                                                           scmDataOutputNames,
                                                                           0,
                                                                           0};

const SFBInterfaceSpec FORTE_QueryServicesAdp::scmFBInterfaceSpecPlug = {1,
                                                                         scmEventOutputNames,
                                                                         1,
                                                                         scmEventInputNames,
                                                                         2,
                                                                         scmDataOutputNames,
                                                                         1,
                                                                         scmDataInputNames,
                                                                         0,
                                                                         0};
