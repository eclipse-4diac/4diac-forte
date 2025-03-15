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

USE_STRING_ID(address);
USE_STRING_ID(ArrowheadCloud);
USE_STRING_ID(authenticationInfo);
USE_STRING_ID(BOOL);
USE_STRING_ID(cloudName);
USE_STRING_ID(DINT);
USE_STRING_ID(gatekeeperServiceURI);
USE_STRING_ID(operator);
USE_STRING_ID(port);
USE_STRING_ID(secure);
USE_STRING_ID(WSTRING);


DEFINE_FIRMWARE_DATATYPE(ArrowheadCloud, STRID(ArrowheadCloud));

CIEC_ArrowheadCloud::CIEC_ArrowheadCloud() :
    CIEC_STRUCT(STRID(ArrowheadCloud), 7, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ArrowheadCloud::scmElementTypes[] = { STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(DINT),
  STRID(WSTRING), STRID(WSTRING), STRID(BOOL) };
const CStringDictionary::TStringId CIEC_ArrowheadCloud::scmElementNames[] =
  { STRID(operator), STRID(cloudName), STRID(address), STRID(port),
  STRID(gatekeeperServiceURI), STRID(authenticationInfo), STRID(secure) };
