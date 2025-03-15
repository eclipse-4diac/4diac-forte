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

USE_STRING_ID(address);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(authenticationInfo);
USE_STRING_ID(DINT);
USE_STRING_ID(port);
USE_STRING_ID(systemName);
USE_STRING_ID(WSTRING);


DEFINE_FIRMWARE_DATATYPE(ArrowheadSystem, STRID(ArrowheadSystem));

CIEC_ArrowheadSystem::CIEC_ArrowheadSystem() :
    CIEC_STRUCT(STRID(ArrowheadSystem), 4, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ArrowheadSystem::scmElementTypes[] = { STRID(WSTRING), STRID(WSTRING), STRID(DINT), STRID(WSTRING) };
const CStringDictionary::TStringId CIEC_ArrowheadSystem::scmElementNames[] = { STRID(systemName), STRID(address), STRID(port),
  STRID(authenticationInfo) };
