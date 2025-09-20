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

using namespace forte::literals;

DEFINE_FIRMWARE_DATATYPE(ArrowheadEvent, "ArrowheadEvent"_STRID);

CIEC_ArrowheadEvent::CIEC_ArrowheadEvent() :
    CIEC_STRUCT("ArrowheadEvent"_STRID, 4, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const forte::StringId CIEC_ArrowheadEvent::scmElementTypes[] = {
    "WSTRING"_STRID, "WSTRING"_STRID, "DATE_AND_TIME"_STRID, "ARRAY"_STRID, 10, "WSTRING"_STRID};
const forte::StringId CIEC_ArrowheadEvent::scmElementNames[] = {"type"_STRID, "payload"_STRID, "timestamp"_STRID,
                                                                "eventMetadata"_STRID};
