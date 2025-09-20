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

#include "SubscribeEvent.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_SubscribeEvent, "SubscribeEvent"_STRID)

namespace {
  const auto cDataInputNames = std::array{"eventFilter"_STRID, "endpoint"_STRID};
  const auto cEventInputNames = std::array{"Subscribe"_STRID, "Unsubscribe"_STRID};
  const auto cEventInputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
  const auto cEventOutputNames = std::array{"Subscribed"_STRID, "Unsubscribed"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
  const auto cPlugNameIds = std::array{"SubscribeEventAdp"_STRID};

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

const SCFB_FBConnectionData FORTE_SubscribeEvent::auto cEventConnections = std::to_array<SCFB_FBConnectionData>{
    {{}, "Unsubscribe"_STRID, -1,
     "SubscribeEventAdp"_STRID, "Unsubscribe"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {{}, "Subscribe"_STRID, -1,
     "SubscribeEventAdp"_STRID, "Subscribe"_STRID), CCompositeFB::scmAdapterMarker | 0},
    {"SubscribeEventAdp"_STRID, "Subscribed"_STRID), CCompositeFB::scmAdapterMarker | 0,
     {}, "Subscribed"_STRID, -1},
    {"SubscribeEventAdp"_STRID, "Unsubscribed"_STRID),
     CCompositeFB::scmAdapterMarker | 0, {}, "Unsubscribed"_STRID, -1},
};

const SCFB_FBConnectionData FORTE_SubscribeEventauto cDataConnections = std::to_array<SCFB_FBConnectionData>{
    {{}, "eventFilter"_STRID, -1,
     "SubscribeEventAdp"_STRID, "eventFilter"_STRID),
     CCompositeFB::scmAdapterMarker | 0},
    {{}, "endpoint"_STRID, -1,
     "SubscribeEventAdp"_STRID, "endpoint"_STRID), CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBNData FORTE_SubscribeEvent::scmFBNData = {
    0, 0, 4, scmEventConnections, 0, 0, 2, scmDataConnections, 0, 0, 0, nullptr, 0, 0};
