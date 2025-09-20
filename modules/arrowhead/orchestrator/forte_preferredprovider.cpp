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

#include "forte_preferredprovider.h"

using namespace forte::literals;

DEFINE_FIRMWARE_DATATYPE(PreferredProvider, "PreferredProvider"_STRID);

CIEC_PreferredProvider::CIEC_PreferredProvider() :
    CIEC_STRUCT("PreferredProvider"_STRID, 2, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const forte::StringId CIEC_PreferredProvider::scmElementTypes[] = {"ArrowheadSystem"_STRID, "ArrowheadCloud"_STRID};
const forte::StringId CIEC_PreferredProvider::scmElementNames[] = {"providerSystem"_STRID, "providerCloud"_STRID};
