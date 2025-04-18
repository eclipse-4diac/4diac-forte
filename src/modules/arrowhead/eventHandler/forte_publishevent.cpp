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

#include "forte_publishevent.h"

USE_STRING_ID(ArrowheadEvent);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(deliveryCompleteUri);
USE_STRING_ID(event);
USE_STRING_ID(PublishEvent);
USE_STRING_ID(source);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_DATATYPE(PublishEvent, STRID(PublishEvent));

CIEC_PublishEvent::CIEC_PublishEvent() :
    CIEC_STRUCT(STRID(PublishEvent), 3, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_PublishEvent::scmElementTypes[] = {STRID(ArrowheadSystem),
                                                                           STRID(ArrowheadEvent), STRID(WSTRING)};
const CStringDictionary::TStringId CIEC_PublishEvent::scmElementNames[] = {STRID(source), STRID(event),
                                                                           STRID(deliveryCompleteUri)};
