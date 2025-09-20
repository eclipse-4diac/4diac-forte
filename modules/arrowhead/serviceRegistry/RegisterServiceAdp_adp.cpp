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

#include "RegisterServiceAdp_adp.h"

using namespace forte::literals;

DEFINE_ADAPTER_TYPE(FORTE_RegisterServiceAdp, "RegisterServiceAdp"_STRID)

const auto cDataOutputNames = std::array{"serviceRegistryEntry"_STRID, "endpoint"_STRID};

"WSTRING"_STRID
}
;

const auto cEventInputNames = std::array{"doneRegister"_STRID, "doneUnregister"_STRID};

const auto cEventOutputNames = std::array{"registerService"_STRID, "unregisterService"_STRID};

const SFBInterfaceSpec FORTE_RegisterServiceAdp::scmFBInterfaceSpecSocket = {
    2, scmEventInputNames, 0, 2, scmEventOutputNames, 0, 0, 0, 2, scmDataOutputNames, 0, 0};

const SFBInterfaceSpec FORTE_RegisterServiceAdp::scmFBInterfaceSpecPlug = {
    2, scmEventOutputNames, 2, scmEventInputNames, 0, 0, 2, scmDataOutputNames, 0, 0, 0, 0, 0};
