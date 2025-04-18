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

const CStringDictionary::TStringId FORTE_SubscribeEventAdp::scmDataOutputNames[] = {STRID(eventFilter),
                                                                                    STRID(endpoint)};

const CStringDictionary::TStringId FORTE_SubscribeEventAdp::scmDataOutputTypeIds[] = {STRID(EventFilter),
                                                                                      STRID(WSTRING)};

const TForteInt16 FORTE_SubscribeEventAdp::scmEIWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_SubscribeEventAdp::scmEventInputNames[] = {STRID(Subscribed),
                                                                                    STRID(Unsubscribed)};

const TDataIOID FORTE_SubscribeEventAdp::scmEOWith[] = {0, 1, scmWithListDelimiter, 1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_SubscribeEventAdp::scmEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_SubscribeEventAdp::scmEventOutputNames[] = {STRID(Subscribe),
                                                                                     STRID(Unsubscribe)};

const SFBInterfaceSpec FORTE_SubscribeEventAdp::scmFBInterfaceSpecSocket = {2,
                                                                            scmEventInputNames,
                                                                            scmEventInputTypeIds,
                                                                            0,
                                                                            scmEIWithIndexes,
                                                                            2,
                                                                            scmEventOutputNames,
                                                                            scmEventOutputTypeIds,
                                                                            scmEOWith,
                                                                            scmEOWithIndexes,
                                                                            0,
                                                                            0,
                                                                            0,
                                                                            2,
                                                                            scmDataOutputNames,
                                                                            scmDataOutputTypeIds,
                                                                            0,
                                                                            0};

const SFBInterfaceSpec FORTE_SubscribeEventAdp::scmFBInterfaceSpecPlug = {2,
                                                                          scmEventOutputNames,
                                                                          scmEventOutputTypeIds,
                                                                          scmEOWith,
                                                                          scmEOWithIndexes,
                                                                          2,
                                                                          scmEventInputNames,
                                                                          scmEventInputTypeIds,
                                                                          0,
                                                                          0,
                                                                          2,
                                                                          scmDataOutputNames,
                                                                          scmDataOutputTypeIds,
                                                                          0,
                                                                          0,
                                                                          0,
                                                                          0,
                                                                          0};
