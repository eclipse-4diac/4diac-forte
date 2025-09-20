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

#include "OrchestratorRequestAdp_adp.h"

using namespace forte::literals;

DEFINE_ADAPTER_TYPE(FORTE_OrchestratorRequestAdp, "OrchestratorRequestAdp"_STRID)

const auto cDataInputNames = std::array{"orchestrationResponse"_STRID};

"OrchestrationForm"_STRID
}
;

const auto cDataOutputNames = std::array{"serviceRequestForm"_STRID, "endpoint"_STRID};

"WSTRING"_STRID
}
;

const auto cEventInputNames = std::array{"responseReceived"_STRID};

const auto cEventOutputNames = std::array{"requestOrchestator"_STRID};

const SFBInterfaceSpec FORTE_OrchestratorRequestAdp::scmFBInterfaceSpecSocket = {
    1, scmEventInputNames, 1, scmEventOutputNames, 1, scmDataInputNames, 2, scmDataOutputNames, 0, 0};

const SFBInterfaceSpec FORTE_OrchestratorRequestAdp::scmFBInterfaceSpecPlug = {
    1, scmEventOutputNames, 1, scmEventInputNames, 2, scmDataOutputNames, 1, scmDataInputNames, 0, 0};
