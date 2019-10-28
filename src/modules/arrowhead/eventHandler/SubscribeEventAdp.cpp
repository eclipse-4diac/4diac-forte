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

#include "SubscribeEventAdp.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "SubscribeEventAdp_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_SubscribeEventAdp, g_nStringIdSubscribeEventAdp)

const CStringDictionary::TStringId FORTE_SubscribeEventAdp::scm_anDataOutputNames[] = {g_nStringIdeventFilter, g_nStringIdendpoint};

const CStringDictionary::TStringId FORTE_SubscribeEventAdp::scm_anDataOutputTypeIds[] = {g_nStringIdEventFilter, g_nStringIdWSTRING};

const TForteInt16 FORTE_SubscribeEventAdp::scm_anEIWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_SubscribeEventAdp::scm_anEventInputNames[] = {g_nStringIdSubscribed, g_nStringIdUnsubscribed};

const TDataIOID FORTE_SubscribeEventAdp::scm_anEOWith[] = {0, 1, 255, 1, 0, 255};
const TForteInt16 FORTE_SubscribeEventAdp::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_SubscribeEventAdp::scm_anEventOutputNames[] = {g_nStringIdSubscribe, g_nStringIdUnsubscribe};

const SFBInterfaceSpec FORTE_SubscribeEventAdp::scm_stFBInterfaceSpecSocket = {
  2,  scm_anEventInputNames,  0,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  0,  0, 0, 
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_SubscribeEventAdp::scm_stFBInterfaceSpecPlug = {
  2,  scm_anEventOutputNames,  scm_anEOWith,  scm_anEOWithIndexes,
  2,  scm_anEventInputNames,  0, 0,  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0,  0, 0,
  0, 0
};



