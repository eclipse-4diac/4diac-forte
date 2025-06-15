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

USE_STRING_ID(doneRegister);
USE_STRING_ID(doneUnregister);
USE_STRING_ID(endpoint);
USE_STRING_ID(Event);
USE_STRING_ID(register);
USE_STRING_ID(registerService);
USE_STRING_ID(RegisterService);
USE_STRING_ID(RegisterServiceAdp);
USE_STRING_ID(serviceRegistryEntry);
USE_STRING_ID(ServiceRegistryEntry);
USE_STRING_ID(unregister);
USE_STRING_ID(unregisterService);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_RegisterService, STRID(RegisterService))

namespace {
  const auto cDataInputNames = std::array{STRID(serviceRegistryEntry), STRID(endpoint)};

  const auto cEventInputNames = std::array{STRID(register), STRID(unregister)};
  const auto cEventInputTypeIds = std::array{STRID(Event), STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(doneRegister), STRID(doneUnregister)};
  const auto cEventOutputTypeIds = std::array{STRID(Event), STRID(Event)};

  const auto cPlugNameIds = std::array{STRID(registerService)};

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
      {CStringDictionary::scmInvalidStringId, STRID(register), STRID(registerService), STRID(registerService)},
      {CStringDictionary::scmInvalidStringId, STRID(unregister), STRID(registerService), STRID(unregisterService)},
      {STRID(registerService), STRID(doneRegister), CStringDictionary::scmInvalidStringId, STRID(doneRegister)},
      {STRID(registerService), STRID(doneUnregister), CStringDictionary::scmInvalidStringId, STRID(doneUnregister)},
  });

  const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(serviceRegistryEntry), STRID(registerService),
       STRID(serviceRegistryEntry)},
      {CStringDictionary::scmInvalidStringId, STRID(endpoint), STRID(registerService), STRID(endpoint)},
  });
} // namespace

const SCFB_FBNData FORTE_RegisterService::scmFBNData = {0, 0, cEventConnections, cDataConnections, {}, 0, 0};
