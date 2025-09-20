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

using namespace forte::literals;

DEFINE_FIRMWARE_DATATYPE(PublishEvent, "PublishEvent"_STRID);

CIEC_PublishEvent::CIEC_PublishEvent() :
    CIEC_STRUCT("PublishEvent"_STRID, 3, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const forte::StringId CIEC_PublishEvent::scmElementTypes[] = {"ArrowheadSystem"_STRID, "ArrowheadEvent"_STRID,
                                                              "WSTRING"_STRID};
const forte::StringId CIEC_PublishEvent::scmElementNames[] = {"source"_STRID, "event"_STRID,
                                                              "deliveryCompleteUri"_STRID};
