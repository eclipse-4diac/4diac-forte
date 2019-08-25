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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_serviceregistryentry_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(ServiceRegistryEntry, g_nStringIdServiceRegistryEntry);

CIEC_ServiceRegistryEntry::CIEC_ServiceRegistryEntry() :
    CIEC_STRUCT(g_nStringIdServiceRegistryEntry, 7, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ServiceRegistryEntry::scmElementTypes[] = { g_nStringIdArrowheadService, g_nStringIdArrowheadSystem, g_nStringIdWSTRING,
  g_nStringIdDINT, g_nStringIdBOOL, g_nStringIdDINT, g_nStringIdWSTRING };
const CStringDictionary::TStringId CIEC_ServiceRegistryEntry::scmElementNames[] = { g_nStringIdprovidedService, g_nStringIdprovider, g_nStringIdserviceURI,
  g_nStringIdversion, g_nStringIdudp, g_nStringIdttl, g_nStringIdmetadata };
