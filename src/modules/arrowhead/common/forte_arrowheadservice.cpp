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

USE_STRING_ID(ARRAY);
USE_STRING_ID(ArrowheadService);
USE_STRING_ID(interfaces);
USE_STRING_ID(serviceDefinition);
USE_STRING_ID(serviceMetadata);
USE_STRING_ID(WSTRING);


DEFINE_FIRMWARE_DATATYPE(ArrowheadService, STRID(ArrowheadService));

CIEC_ArrowheadService::CIEC_ArrowheadService() :
    CIEC_STRUCT(STRID(ArrowheadService), 3, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ArrowheadService::scmElementTypes[] = { STRID(WSTRING), STRID(ARRAY), 10, STRID(WSTRING), STRID(ARRAY),
  10, STRID(WSTRING) };
const CStringDictionary::TStringId CIEC_ArrowheadService::scmElementNames[] =
  { STRID(serviceDefinition), STRID(interfaces), STRID(serviceMetadata) };
