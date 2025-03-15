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

#include "ArrowheadPublish.h"

USE_STRING_ID(ArrowheadPublish);
USE_STRING_ID(ArrowheadPublishAdp);
USE_STRING_ID(endpoint);
USE_STRING_ID(Event);
USE_STRING_ID(publish);
USE_STRING_ID(published);
USE_STRING_ID(publishEvent);
USE_STRING_ID(PublishEvent);
USE_STRING_ID(WSTRING);


DEFINE_FIRMWARE_FB(FORTE_ArrowheadPublish, STRID(ArrowheadPublish))

const CStringDictionary::TStringId FORTE_ArrowheadPublish::scmDataInputNames[] = {STRID(publishEvent), STRID(endpoint)};

const CStringDictionary::TStringId FORTE_ArrowheadPublish::scmDataInputTypeIds[] = {STRID(PublishEvent), STRID(WSTRING)};

const TForteInt16 FORTE_ArrowheadPublish::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_ArrowheadPublish::scmEIWith[] = {0, 1, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_ArrowheadPublish::scmEventInputNames[] = {STRID(publish)};
const CStringDictionary::TStringId FORTE_ArrowheadPublish::scmEventInputTypeIds[] = {STRID(Event)};

const TForteInt16 FORTE_ArrowheadPublish::scmEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_ArrowheadPublish::scmEventOutputNames[] = {STRID(published)};
const CStringDictionary::TStringId FORTE_ArrowheadPublish::scmEventOutputTypeIds[] = {STRID(Event)};

const SAdapterInstanceDef FORTE_ArrowheadPublish::scmAdapterInstances[] = {
{STRID(ArrowheadPublishAdp), STRID(ArrowheadPublishAdp), true }};

const SFBInterfaceSpec FORTE_ArrowheadPublish::scmFBInterfaceSpec = {
  1,  scmEventInputNames, scmEventInputTypeIds,  scmEIWith,  scmEIWithIndexes,
  1,  scmEventOutputNames, scmEventOutputTypeIds,  0, 0,  2,  scmDataInputNames, scmDataInputTypeIds,
  0,  0, 0,
  1,scmAdapterInstances};


const SCFB_FBConnectionData FORTE_ArrowheadPublish::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(publish)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ArrowheadPublishAdp), STRID(publish)), CCompositeFB::scmAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ArrowheadPublishAdp), STRID(published)), CCompositeFB::scmAdapterMarker |0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(published)), -1},
};

const SCFB_FBConnectionData FORTE_ArrowheadPublish::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(publishEvent)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ArrowheadPublishAdp), STRID(publishEvent)), CCompositeFB::scmAdapterMarker |0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(endpoint)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ArrowheadPublishAdp), STRID(endpoint)), CCompositeFB::scmAdapterMarker |0},
};

const SCFB_FBNData FORTE_ArrowheadPublish::scmFBNData = {
  0, 0,
  2, scmEventConnections,
  0, 0,
  2, scmDataConnections,
  0, 0,
  0, nullptr,
  0, 0
};


