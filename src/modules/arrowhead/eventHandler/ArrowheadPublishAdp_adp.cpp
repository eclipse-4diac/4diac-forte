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

#include "ArrowheadPublishAdp_adp.h"

USE_STRING_ID(ArrowheadPublishAdp);
USE_STRING_ID(endpoint);
USE_STRING_ID(publish);
USE_STRING_ID(published);
USE_STRING_ID(publishEvent);
USE_STRING_ID(PublishEvent);
USE_STRING_ID(WSTRING);


DEFINE_ADAPTER_TYPE(FORTE_ArrowheadPublishAdp, STRID(ArrowheadPublishAdp))

const CStringDictionary::TStringId FORTE_ArrowheadPublishAdp::scmDataOutputNames[] = {STRID(publishEvent), STRID(endpoint)};

const CStringDictionary::TStringId FORTE_ArrowheadPublishAdp::scmDataOutputTypeIds[] = {STRID(PublishEvent), STRID(WSTRING)};

const TForteInt16 FORTE_ArrowheadPublishAdp::scmEIWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_ArrowheadPublishAdp::scmEventInputNames[] = {STRID(published)};

const TDataIOID FORTE_ArrowheadPublishAdp::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_ArrowheadPublishAdp::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_ArrowheadPublishAdp::scmEventOutputNames[] = {STRID(publish)};

const SFBInterfaceSpec FORTE_ArrowheadPublishAdp::scmFBInterfaceSpecSocket = {
  1,  scmEventInputNames, scmEventInputTypeIds, 0,  scmEIWithIndexes,
  1,  scmEventOutputNames, scmDataOutputTypeIds, scmEOWith, scmEOWithIndexes,  0,  0, 0,
  2,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_ArrowheadPublishAdp::scmFBInterfaceSpecPlug = {
  1,  scmEventOutputNames, scmDataOutputTypeIds,  scmEOWith,  scmEOWithIndexes,
  1,  scmEventInputNames, scmEventInputTypeIds, 0, 0,  2,  scmDataOutputNames, scmDataOutputTypeIds,
  0,  0, 0,
  0, 0
};



