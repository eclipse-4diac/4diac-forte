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

#include "ArrowheadPublish.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_ArrowheadPublish, "ArrowheadPublish"_STRID)

namespace {
  const auto cDataInputNames = std::array{"publishEvent"_STRID, "endpoint"_STRID};
  const auto cEventInputNames = std::array{"publish"_STRID};
  const auto cEventInputTypeIds = std::array{"Event"_STRID};
  const auto cEventOutputNames = std::array{"published"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID};

  const auto cPlugNameIds = std::array{"ArrowheadPublishAdp"_STRID};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = cPlugNameIds,
  };
} // namespace

const SCFB_FBConnectionData FORTE_ArrowheadPublish::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {{}, "publish"_STRID, -1,
     "ArrowheadPublishAdp"_STRID, "publish"_STRID), CCompositeFB::scmAdapterMarker | 0},
    {"ArrowheadPublishAdp"_STRID, "published"_STRID),
     CCompositeFB::scmAdapterMarker | 0, {}, "published"_STRID, -1},
};

const SCFB_FBConnectionData FORTE_ArrowheadPublishauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {{}, "publishEvent"_STRID, -1,
     "ArrowheadPublishAdp"_STRID, "publishEvent"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {{}, "endpoint"_STRID, -1,
     "ArrowheadPublishAdp"_STRID, "endpoint"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBNData FORTE_ArrowheadPublish::scmFBNData = {
    0, 0, 2, scmEventConnections, 0, 0, 2, scmDataConnections, 0, 0, 0, nullptr, 0, 0};
