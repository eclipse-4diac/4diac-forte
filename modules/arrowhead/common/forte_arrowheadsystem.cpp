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

#include "forte_arrowheadsystem.h"

using namespace forte::literals;

DEFINE_FIRMWARE_DATATYPE(ArrowheadSystem, "ArrowheadSystem"_STRID);

CIEC_ArrowheadSystem::CIEC_ArrowheadSystem() :
    CIEC_STRUCT("ArrowheadSystem"_STRID, 4, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const forte::StringId CIEC_ArrowheadSystem::scmElementTypes[] = {"WSTRING"_STRID, "WSTRING"_STRID, "DINT"_STRID,
                                                                 "WSTRING"_STRID};
const forte::StringId CIEC_ArrowheadSystem::scmElementNames[] = {"systemName"_STRID, "address"_STRID, "port"_STRID,
                                                                 "authenticationInfo"_STRID};
