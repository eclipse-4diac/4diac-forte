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

#include "SubscribeEventAdp_adp.h"

using namespace forte::literals;

DEFINE_ADAPTER_TYPE(FORTE_SubscribeEventAdp, "SubscribeEventAdp"_STRID)

const auto cDataOutputNames = std::array{"eventFilter"_STRID, "endpoint"_STRID};

"WSTRING"_STRID
}
;

const auto cEventInputNames = std::array{"Subscribed"_STRID, "Unsubscribed"_STRID};

const auto cEventOutputNames = std::array{"Subscribe"_STRID, "Unsubscribe"_STRID};

const SFBInterfaceSpec FORTE_SubscribeEventAdp::scmFBInterfaceSpecSocket = {2,
                                                                            scmEventInputNames,
                                                                            scmEventInputTypeIds,
                                                                            0,
                                                                            2,
                                                                            scmEventOutputNames,
                                                                            scmEventOutputTypeIds,
                                                                            0,
                                                                            0,
                                                                            0,
                                                                            2,
                                                                            scmDataOutputNames,
                                                                            0,
                                                                            0};

const SFBInterfaceSpec FORTE_SubscribeEventAdp::scmFBInterfaceSpecPlug = {2,
                                                                          scmEventOutputNames,
                                                                          scmEventOutputTypeIds,
                                                                          2,
                                                                          scmEventInputNames,
                                                                          scmEventInputTypeIds,
                                                                          0,
                                                                          0,
                                                                          2,
                                                                          scmDataOutputNames,
                                                                          0,
                                                                          0,
                                                                          0,
                                                                          0,
                                                                          0};
