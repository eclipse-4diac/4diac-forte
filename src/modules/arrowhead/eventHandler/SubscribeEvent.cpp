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

USE_STRING_ID(endpoint);
USE_STRING_ID(Event);
USE_STRING_ID(eventFilter);
USE_STRING_ID(EventFilter);
USE_STRING_ID(Subscribe);
USE_STRING_ID(Subscribed);
USE_STRING_ID(SubscribeEvent);
USE_STRING_ID(SubscribeEventAdp);
USE_STRING_ID(Unsubscribe);
USE_STRING_ID(Unsubscribed);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_SubscribeEvent, STRID(SubscribeEvent))

namespace {
  const auto cDataInputNames = std::array{STRID(eventFilter), STRID(endpoint)};
  const auto cEventInputNames = std::array{STRID(Subscribe), STRID(Unsubscribe)};
  const auto cEventInputTypeIds = std::array{STRID(Event), STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(Subscribed), STRID(Unsubscribed)};
  const auto cEventOutputTypeIds = std::array{STRID(Event), STRID(Event)};
  const auto cPlugNameIds = std::array{STRID(SubscribeEventAdp)};

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

const SCFB_FBConnectionData FORTE_SubscribeEvent::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Unsubscribe)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(Unsubscribe)),
     CCompositeFB::scmAdapterMarker | 0},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Subscribe)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(Subscribe)), CCompositeFB::scmAdapterMarker | 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(Subscribed)), CCompositeFB::scmAdapterMarker | 0,
     GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Subscribed)), -1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(Unsubscribed)),
     CCompositeFB::scmAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Unsubscribed)), -1},
};

const SCFB_FBConnectionData FORTE_SubscribeEvent::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(eventFilter)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(eventFilter)),
     CCompositeFB::scmAdapterMarker | 0},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(endpoint)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(endpoint)), CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBNData FORTE_SubscribeEvent::scmFBNData = {
    0, 0, 4, scmEventConnections, 0, 0, 2, scmDataConnections, 0, 0, 0, nullptr, 0, 0};
