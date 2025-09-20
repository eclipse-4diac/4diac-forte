/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "forte_arrowheadservice.h"

using namespace forte::literals;

DEFINE_FIRMWARE_DATATYPE(ArrowheadService, "ArrowheadService"_STRID);

CIEC_ArrowheadService::CIEC_ArrowheadService() :
    CIEC_STRUCT("ArrowheadService"_STRID, 3, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const forte::StringId CIEC_ArrowheadService::scmElementTypes[] = {
    "WSTRING"_STRID, "ARRAY"_STRID, 10, "WSTRING"_STRID, "ARRAY"_STRID, 10, "WSTRING"_STRID};
const forte::StringId CIEC_ArrowheadService::scmElementNames[] = {"serviceDefinition"_STRID, "interfaces"_STRID,
                                                                  "serviceMetadata"_STRID};
