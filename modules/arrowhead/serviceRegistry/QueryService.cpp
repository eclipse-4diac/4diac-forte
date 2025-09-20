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

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_QueryService, "QueryService"_STRID)

namespace {
  const auto cDataInputNames = std::array{"serviceQueryForm"_STRID, "address"_STRID};
  const auto cDataOutputNames = std::array{"serviceEntries"_STRID};
  const auto cEventInputNames = std::array{"query"_STRID};
  const auto cEventInputTypeIds = std::array{"Event"_STRID};
  const auto cEventOutputNames = std::array{"queried"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID};
  const auto cPlugNameIds = std::array{"queryServices"_STRID};

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

  const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
      {{}, "query"_STRID, "queryServices"_STRID, "query"_STRID},
      {"queryServices"_STRID, "queried"_STRID, {}, "queried"_STRID},
  });

  const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
      {{}, "serviceQueryForm"_STRID, "queryServices"_STRID, "serviceQueryForm"_STRID},
      {"queryServices"_STRID, "serviceEntries"_STRID, {}, "serviceEntries"_STRID},
      {{}, "address"_STRID, "queryServices"_STRID, "endpoint"_STRID},
  });
} // namespace

const SCFB_FBNData FORTE_QueryService::scmFBNData = {0, 0, cEventConnections, cDataConnections, {}, 0, 0};
