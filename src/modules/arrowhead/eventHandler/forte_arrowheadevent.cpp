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

#include "forte_arrowheadevent.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(ArrowheadEvent);
USE_STRING_ID(DATE_AND_TIME);
USE_STRING_ID(eventMetadata);
USE_STRING_ID(payload);
USE_STRING_ID(timestamp);
USE_STRING_ID(type);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_DATATYPE(ArrowheadEvent, STRID(ArrowheadEvent));

CIEC_ArrowheadEvent::CIEC_ArrowheadEvent() :
    CIEC_STRUCT(STRID(ArrowheadEvent), 4, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ArrowheadEvent::scmElementTypes[] = {
    STRID(WSTRING), STRID(WSTRING), STRID(DATE_AND_TIME), STRID(ARRAY), 10, STRID(WSTRING)};
const CStringDictionary::TStringId CIEC_ArrowheadEvent::scmElementNames[] = {STRID(type), STRID(payload),
                                                                             STRID(timestamp), STRID(eventMetadata)};
