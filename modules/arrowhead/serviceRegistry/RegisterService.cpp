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

#include "RegisterService.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_RegisterService, "RegisterService"_STRID)

namespace {
  const auto cDataInputNames = std::array{"serviceRegistryEntry"_STRID, "endpoint"_STRID};

  const auto cEventInputNames = std::array{"register"_STRID, "unregister"_STRID};
  const auto cEventInputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
  const auto cEventOutputNames = std::array{"doneRegister"_STRID, "doneUnregister"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};

  const auto cPlugNameIds = std::array{"registerService"_STRID};

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

  const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
      {{}, "register"_STRID, "registerService"_STRID, "registerService"_STRID},
      {{}, "unregister"_STRID, "registerService"_STRID, "unregisterService"_STRID},
      {"registerService"_STRID, "doneRegister"_STRID, {}, "doneRegister"_STRID},
      {"registerService"_STRID, "doneUnregister"_STRID, {}, "doneUnregister"_STRID},
  });

  const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
      {{}, "serviceRegistryEntry"_STRID, "registerService"_STRID, "serviceRegistryEntry"_STRID},
      {{}, "endpoint"_STRID, "registerService"_STRID, "endpoint"_STRID},
  });
} // namespace

const SCFB_FBNData FORTE_RegisterService::scmFBNData = {0, 0, cEventConnections, cDataConnections, {}, 0, 0};
