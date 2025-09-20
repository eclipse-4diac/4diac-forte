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

using namespace forte::literals;

DEFINE_ADAPTER_TYPE(FORTE_QueryServicesAdp, "QueryServicesAdp"_STRID)

const auto cDataInputNames = std::array{"serviceEntries"_STRID};

"ServiceRegistryEntry"_STRID
}
;

const auto cDataOutputNames = std::array{"serviceQueryForm"_STRID, "endpoint"_STRID};

"WSTRING"_STRID
}
;

const auto cEventInputNames = std::array{"queried"_STRID};

const auto cEventOutputNames = std::array{"query"_STRID};

const SFBInterfaceSpec FORTE_QueryServicesAdp::scmFBInterfaceSpecSocket = {
    1, scmEventInputNames, 1, scmEventOutputNames, 1, scmDataInputNames, 2, scmDataOutputNames, 0, 0};

const SFBInterfaceSpec FORTE_QueryServicesAdp::scmFBInterfaceSpecPlug = {
    1, scmEventOutputNames, 1, scmEventInputNames, 2, scmDataOutputNames, 1, scmDataInputNames, 0, 0};
