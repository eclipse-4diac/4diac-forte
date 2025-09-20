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

#include "forte_serviceregistryentry.h"

using namespace forte::literals;

DEFINE_FIRMWARE_DATATYPE(ServiceRegistryEntry, "ServiceRegistryEntry"_STRID);

CIEC_ServiceRegistryEntry::CIEC_ServiceRegistryEntry() :
    CIEC_STRUCT("ServiceRegistryEntry"_STRID, 7, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const forte::StringId CIEC_ServiceRegistryEntry::scmElementTypes[] = {
    "ArrowheadService"_STRID, "ArrowheadSystem"_STRID, "WSTRING"_STRID, "DINT"_STRID, "BOOL"_STRID, "DINT"_STRID,
    "WSTRING"_STRID};
const forte::StringId CIEC_ServiceRegistryEntry::scmElementNames[] = {
    "providedService"_STRID, "provider"_STRID, "serviceURI"_STRID, "version"_STRID, "udp"_STRID, "ttl"_STRID,
    "metadata"_STRID};
