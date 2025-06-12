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

USE_STRING_ID(doneRegister);
USE_STRING_ID(doneUnregister);
USE_STRING_ID(endpoint);
USE_STRING_ID(registerService);
USE_STRING_ID(RegisterServiceAdp);
USE_STRING_ID(serviceRegistryEntry);
USE_STRING_ID(ServiceRegistryEntry);
USE_STRING_ID(unregisterService);
USE_STRING_ID(WSTRING);

DEFINE_ADAPTER_TYPE(FORTE_RegisterServiceAdp, STRID(RegisterServiceAdp))

const auto cDataOutputNames = std::array{STRID(serviceRegistryEntry),
                                                                                     STRID(endpoint)};

                                                                                       STRID(WSTRING)};

const auto cEventInputNames = std::array{STRID(doneRegister),
                                                                                     STRID(doneUnregister)};

const auto cEventOutputNames = std::array{STRID(registerService),
                                                                                      STRID(unregisterService)};

const SFBInterfaceSpec FORTE_RegisterServiceAdp::scmFBInterfaceSpecSocket = {2,
                                                                             scmEventInputNames,
                                                                             0,
                                                                             2,
                                                                             scmEventOutputNames,
                                                                             0,
                                                                             0,
                                                                             0,
                                                                             2,
                                                                             scmDataOutputNames,
                                                                             0,
                                                                             0};

const SFBInterfaceSpec FORTE_RegisterServiceAdp::scmFBInterfaceSpecPlug = {2,
                                                                           scmEventOutputNames,
                                                                           2,
                                                                           scmEventInputNames,
                                                                           0,
                                                                           0,
                                                                           2,
                                                                           scmDataOutputNames,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           0,
                                                                           0};
