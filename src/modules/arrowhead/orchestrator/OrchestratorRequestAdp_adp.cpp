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

USE_STRING_ID(ARRAY);
USE_STRING_ID(endpoint);
USE_STRING_ID(OrchestrationForm);
USE_STRING_ID(orchestrationResponse);
USE_STRING_ID(OrchestratorRequestAdp);
USE_STRING_ID(requestOrchestator);
USE_STRING_ID(responseReceived);
USE_STRING_ID(serviceRequestForm);
USE_STRING_ID(ServiceRequestForm);
USE_STRING_ID(WSTRING);

DEFINE_ADAPTER_TYPE(FORTE_OrchestratorRequestAdp, STRID(OrchestratorRequestAdp))

const auto cDataInputNames = std::array{STRID(orchestrationResponse)};

                                                                                          STRID(OrchestrationForm)};

const auto cDataOutputNames = std::array{STRID(serviceRequestForm),
                                                                                         STRID(endpoint)};

                                                                                           STRID(WSTRING)};

const auto cEventInputNames = std::array{STRID(responseReceived)};

const auto cEventOutputNames = std::array{STRID(requestOrchestator)};

const SFBInterfaceSpec FORTE_OrchestratorRequestAdp::scmFBInterfaceSpecSocket = {1,
                                                                                 scmEventInputNames,
                                                                                 1,
                                                                                 scmEventOutputNames,
                                                                                 1,
                                                                                 scmDataInputNames,
                                                                                 2,
                                                                                 scmDataOutputNames,
                                                                                 0,
                                                                                 0};

const SFBInterfaceSpec FORTE_OrchestratorRequestAdp::scmFBInterfaceSpecPlug = {1,
                                                                               scmEventOutputNames,
                                                                               1,
                                                                               scmEventInputNames,
                                                                               2,
                                                                               scmDataOutputNames,
                                                                               1,
                                                                               scmDataInputNames,
                                                                               0,
                                                                               0};
