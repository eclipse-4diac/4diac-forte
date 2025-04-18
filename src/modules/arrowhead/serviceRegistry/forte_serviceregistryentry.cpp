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

USE_STRING_ID(ArrowheadService);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(BOOL);
USE_STRING_ID(DINT);
USE_STRING_ID(metadata);
USE_STRING_ID(providedService);
USE_STRING_ID(provider);
USE_STRING_ID(ServiceRegistryEntry);
USE_STRING_ID(serviceURI);
USE_STRING_ID(ttl);
USE_STRING_ID(udp);
USE_STRING_ID(version);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_DATATYPE(ServiceRegistryEntry, STRID(ServiceRegistryEntry));

CIEC_ServiceRegistryEntry::CIEC_ServiceRegistryEntry() :
    CIEC_STRUCT(STRID(ServiceRegistryEntry), 7, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ServiceRegistryEntry::scmElementTypes[] = {
    STRID(ArrowheadService), STRID(ArrowheadSystem), STRID(WSTRING), STRID(DINT), STRID(BOOL), STRID(DINT),
    STRID(WSTRING)};
const CStringDictionary::TStringId CIEC_ServiceRegistryEntry::scmElementNames[] = {
    STRID(providedService), STRID(provider), STRID(serviceURI), STRID(version), STRID(udp), STRID(ttl),
    STRID(metadata)};
