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

#include "forte_arrowheadcloud.h"

using namespace forte::literals;

DEFINE_FIRMWARE_DATATYPE(ArrowheadCloud, "ArrowheadCloud"_STRID);

CIEC_ArrowheadCloud::CIEC_ArrowheadCloud() :
    CIEC_STRUCT("ArrowheadCloud"_STRID, 7, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const forte::StringId CIEC_ArrowheadCloud::scmElementTypes[] = {
    "WSTRING"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "DINT"_STRID, "WSTRING"_STRID, "WSTRING"_STRID, "BOOL"_STRID};
const forte::StringId CIEC_ArrowheadCloud::scmElementNames[] = {
    "operator"_STRID,           "cloudName"_STRID, "address"_STRID, "port"_STRID, "gatekeeperServiceURI"_STRID,
    "authenticationInfo"_STRID, "secure"_STRID};
