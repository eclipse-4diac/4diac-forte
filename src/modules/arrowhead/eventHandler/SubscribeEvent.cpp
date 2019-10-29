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

#include "SubscribeEvent.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "SubscribeEvent_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_SubscribeEvent, g_nStringIdSubscribeEvent)

const CStringDictionary::TStringId FORTE_SubscribeEvent::scm_anDataInputNames[] = {g_nStringIdeventFilter, g_nStringIdendpoint};

const CStringDictionary::TStringId FORTE_SubscribeEvent::scm_anDataInputTypeIds[] = {g_nStringIdEventFilter, g_nStringIdWSTRING};

const TForteInt16 FORTE_SubscribeEvent::scm_anEIWithIndexes[] = {0, 3};
const TDataIOID FORTE_SubscribeEvent::scm_anEIWith[] = {0, 1, 255, 0, 1, 255};
const CStringDictionary::TStringId FORTE_SubscribeEvent::scm_anEventInputNames[] = {g_nStringIdSubscribe, g_nStringIdUnsubscribe};

const TForteInt16 FORTE_SubscribeEvent::scm_anEOWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_SubscribeEvent::scm_anEventOutputNames[] = {g_nStringIdSubscribed, g_nStringIdUnsubscribed};

const SAdapterInstanceDef FORTE_SubscribeEvent::scm_astAdapterInstances[] = {
{g_nStringIdSubscribeEventAdp, g_nStringIdSubscribeEventAdp, true }};

const SFBInterfaceSpec FORTE_SubscribeEvent::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  0, 0,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  1,scm_astAdapterInstances};


const SCFB_FBConnectionData FORTE_SubscribeEvent::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdUnsubscribe), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSubscribeEventAdp, g_nStringIdUnsubscribe), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSubscribe), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSubscribeEventAdp, g_nStringIdSubscribe), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSubscribeEventAdp, g_nStringIdSubscribed), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSubscribed), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSubscribeEventAdp, g_nStringIdUnsubscribed), CCompositeFB::scm_nAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdUnsubscribed), -1},
};

const SCFB_FBConnectionData FORTE_SubscribeEvent::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdeventFilter), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSubscribeEventAdp, g_nStringIdeventFilter), CCompositeFB::scm_nAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdendpoint), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSubscribeEventAdp, g_nStringIdendpoint), CCompositeFB::scm_nAdapterMarker |0},
};

const SCFB_FBNData FORTE_SubscribeEvent::scm_stFBNData = {
  0, 0,
  4, scm_astEventConnections,
  0, 0,
  2, scm_astDataConnections,
  0, 0,
  0, 0
};


