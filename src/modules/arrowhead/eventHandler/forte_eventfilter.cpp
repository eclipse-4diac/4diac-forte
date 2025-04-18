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

#include "forte_eventfilter.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(BOOL);
USE_STRING_ID(consumer);
USE_STRING_ID(DATE_AND_TIME);
USE_STRING_ID(endDate);
USE_STRING_ID(EventFilter);
USE_STRING_ID(eventType);
USE_STRING_ID(filterMetada);
USE_STRING_ID(matchMetada);
USE_STRING_ID(notifyUri);
USE_STRING_ID(sources);
USE_STRING_ID(startDate);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_DATATYPE(EventFilter, STRID(EventFilter));

CIEC_EventFilter::CIEC_EventFilter() :
    CIEC_STRUCT(STRID(EventFilter), 8, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_EventFilter::scmElementTypes[] = {STRID(WSTRING),
                                                                          STRID(ArrowheadSystem),
                                                                          STRID(ARRAY),
                                                                          10,
                                                                          STRID(ArrowheadSystem),
                                                                          STRID(DATE_AND_TIME),
                                                                          STRID(DATE_AND_TIME),
                                                                          STRID(ARRAY),
                                                                          10,
                                                                          STRID(WSTRING),
                                                                          STRID(WSTRING),
                                                                          STRID(BOOL)};
const CStringDictionary::TStringId CIEC_EventFilter::scmElementNames[] = {
    STRID(eventType), STRID(consumer),     STRID(sources),   STRID(startDate),
    STRID(endDate),   STRID(filterMetada), STRID(notifyUri), STRID(matchMetada)};
