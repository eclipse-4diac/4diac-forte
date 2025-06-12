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

USE_STRING_ID(ArrowheadPublish);
USE_STRING_ID(ArrowheadPublishAdp);
USE_STRING_ID(endpoint);
USE_STRING_ID(Event);
USE_STRING_ID(publish);
USE_STRING_ID(published);
USE_STRING_ID(publishEvent);
USE_STRING_ID(PublishEvent);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_ArrowheadPublish, STRID(ArrowheadPublish))

namespace {
  const auto cDataInputNames = std::array{STRID(publishEvent), STRID(endpoint)};
  const auto cEventInputNames = std::array{STRID(publish)};
  const auto cEventInputTypeIds = std::array{STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(published)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};

  const auto cPlugNameIds = std::array{STRID(ArrowheadPublishAdp)};

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

const SCFB_FBConnectionData FORTE_ArrowheadPublish::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(publish)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ArrowheadPublishAdp), STRID(publish)), CCompositeFB::scmAdapterMarker | 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ArrowheadPublishAdp), STRID(published)),
     CCompositeFB::scmAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(published)), -1},
};

const SCFB_FBConnectionData FORTE_ArrowheadPublish::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(publishEvent)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ArrowheadPublishAdp), STRID(publishEvent)),
     CCompositeFB::scmAdapterMarker | 0},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(endpoint)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ArrowheadPublishAdp), STRID(endpoint)),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBNData FORTE_ArrowheadPublish::scmFBNData = {
    0, 0, 2, scmEventConnections, 0, 0, 2, scmDataConnections, 0, 0, 0, nullptr, 0, 0};
