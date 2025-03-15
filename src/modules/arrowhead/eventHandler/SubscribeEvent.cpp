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

USE_STRING_ID(endpoint);
USE_STRING_ID(Event);
USE_STRING_ID(eventFilter);
USE_STRING_ID(EventFilter);
USE_STRING_ID(Subscribe);
USE_STRING_ID(Subscribed);
USE_STRING_ID(SubscribeEvent);
USE_STRING_ID(SubscribeEventAdp);
USE_STRING_ID(Unsubscribe);
USE_STRING_ID(Unsubscribed);
USE_STRING_ID(WSTRING);


DEFINE_FIRMWARE_FB(FORTE_SubscribeEvent, STRID(SubscribeEvent))

const CStringDictionary::TStringId FORTE_SubscribeEvent::scmDataInputNames[] = {STRID(eventFilter), STRID(endpoint)};

const CStringDictionary::TStringId FORTE_SubscribeEvent::scmDataInputTypeIds[] = {STRID(EventFilter), STRID(WSTRING)};

const TForteInt16 FORTE_SubscribeEvent::scmEIWithIndexes[] = {0, 3};
const TDataIOID FORTE_SubscribeEvent::scmEIWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_SubscribeEvent::scmEventInputNames[] = {STRID(Subscribe), STRID(Unsubscribe)};
const CStringDictionary::TStringId FORTE_SubscribeEvent::scmEventInputTypeIds[] = {STRID(Event), STRID(Event)};

const TForteInt16 FORTE_SubscribeEvent::scmEOWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_SubscribeEvent::scmEventOutputNames[] = {STRID(Subscribed), STRID(Unsubscribed)};
const CStringDictionary::TStringId FORTE_SubscribeEvent::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};

const SAdapterInstanceDef FORTE_SubscribeEvent::scmAdapterInstances[] = {
{STRID(SubscribeEventAdp), STRID(SubscribeEventAdp), true }};

const SFBInterfaceSpec FORTE_SubscribeEvent::scmFBInterfaceSpec = {
  2,  scmEventInputNames, scmEventInputTypeIds,  scmEIWith,  scmEIWithIndexes,
  2,  scmEventOutputNames, scmEventOutputTypeIds,  0, 0,  2,  scmDataInputNames, scmDataInputTypeIds,
  0,  0, 0,
  1,scmAdapterInstances};


const SCFB_FBConnectionData FORTE_SubscribeEvent::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Unsubscribe)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(Unsubscribe)), CCompositeFB::scmAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Subscribe)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(Subscribe)), CCompositeFB::scmAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(Subscribed)), CCompositeFB::scmAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Subscribed)), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(Unsubscribed)), CCompositeFB::scmAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Unsubscribed)), -1},
};

const SCFB_FBConnectionData FORTE_SubscribeEvent::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(eventFilter)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(eventFilter)), CCompositeFB::scmAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(endpoint)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SubscribeEventAdp), STRID(endpoint)), CCompositeFB::scmAdapterMarker |0},
};

const SCFB_FBNData FORTE_SubscribeEvent::scmFBNData = {
  0, 0,
  4, scmEventConnections,
  0, 0,
  2, scmDataConnections,
  0, 0,
  0, nullptr,
  0, 0
};


