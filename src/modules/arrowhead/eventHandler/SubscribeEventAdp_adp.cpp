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

USE_STRING_ID(endpoint);
USE_STRING_ID(eventFilter);
USE_STRING_ID(EventFilter);
USE_STRING_ID(Subscribe);
USE_STRING_ID(Subscribed);
USE_STRING_ID(SubscribeEventAdp);
USE_STRING_ID(Unsubscribe);
USE_STRING_ID(Unsubscribed);
USE_STRING_ID(WSTRING);

DEFINE_ADAPTER_TYPE(FORTE_SubscribeEventAdp, STRID(SubscribeEventAdp))

const auto cDataOutputNames = std::array{STRID(eventFilter),
                                                                                    STRID(endpoint)};

                                                                                      STRID(WSTRING)};

const auto cEventInputNames = std::array{STRID(Subscribed),
                                                                                    STRID(Unsubscribed)};

const auto cEventOutputNames = std::array{STRID(Subscribe),
                                                                                     STRID(Unsubscribe)};

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
