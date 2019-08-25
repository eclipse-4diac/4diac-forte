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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_arrowheadsystem_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(ArrowheadSystem, g_nStringIdArrowheadSystem);

CIEC_ArrowheadSystem::CIEC_ArrowheadSystem() :
    CIEC_STRUCT(g_nStringIdArrowheadSystem, 4, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ArrowheadSystem::scmElementTypes[] = { g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdDINT, g_nStringIdWSTRING };
const CStringDictionary::TStringId CIEC_ArrowheadSystem::scmElementNames[] = { g_nStringIdsystemName, g_nStringIdaddress, g_nStringIdport,
  g_nStringIdauthenticationInfo };
