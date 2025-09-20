/*******************************************************************************
 * Copyright (c) 2019, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "ArrowheadPublishAdp_adp.h"

using namespace forte::literals;

DEFINE_ADAPTER_TYPE(FORTE_ArrowheadPublishAdp, "ArrowheadPublishAdp"_STRID)

const auto cDataOutputNames = std::array{"publishEvent"_STRID, "endpoint"_STRID};

"WSTRING"_STRID
}
;

const auto cEventInputNames = std::array{"published"_STRID};

const auto cEventOutputNames = std::array{"publish"_STRID};

const SFBInterfaceSpec FORTE_ArrowheadPublishAdp::scmFBInterfaceSpecSocket = {
    1, scmEventInputNames, scmEventInputTypeIds, 0, 1, scmEventOutputNames, 0, 0, 0, 2, scmDataOutputNames, 0, 0};

const SFBInterfaceSpec FORTE_ArrowheadPublishAdp::scmFBInterfaceSpecPlug = {
    1, scmEventOutputNames, 1, scmEventInputNames, scmEventInputTypeIds, 0, 0, 2, scmDataOutputNames, 0, 0, 0, 0, 0};
