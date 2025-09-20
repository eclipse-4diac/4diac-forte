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

using namespace forte::literals;

DEFINE_FIRMWARE_DATATYPE(EventFilter, "EventFilter"_STRID);

CIEC_EventFilter::CIEC_EventFilter() :
    CIEC_STRUCT("EventFilter"_STRID, 8, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const forte::StringId CIEC_EventFilter::scmElementTypes[] = {"WSTRING"_STRID,
                                                             "ArrowheadSystem"_STRID,
                                                             "ARRAY"_STRID,
                                                             10,
                                                             "ArrowheadSystem"_STRID,
                                                             "DATE_AND_TIME"_STRID,
                                                             "DATE_AND_TIME"_STRID,
                                                             "ARRAY"_STRID,
                                                             10,
                                                             "WSTRING"_STRID,
                                                             "WSTRING"_STRID,
                                                             "BOOL"_STRID};
const forte::StringId CIEC_EventFilter::scmElementNames[] = {
    "eventType"_STRID, "consumer"_STRID,     "sources"_STRID,   "startDate"_STRID,
    "endDate"_STRID,   "filterMetada"_STRID, "notifyUri"_STRID, "matchMetada"_STRID};
