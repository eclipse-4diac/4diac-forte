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

#include "RegisterServiceAdp_adp.h"

USE_STRING_ID(doneRegister);
USE_STRING_ID(doneUnregister);
USE_STRING_ID(endpoint);
USE_STRING_ID(registerService);
USE_STRING_ID(RegisterServiceAdp);
USE_STRING_ID(serviceRegistryEntry);
USE_STRING_ID(ServiceRegistryEntry);
USE_STRING_ID(unregisterService);
USE_STRING_ID(WSTRING);


DEFINE_ADAPTER_TYPE(FORTE_RegisterServiceAdp, STRID(RegisterServiceAdp))

const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scmDataOutputNames[] = {STRID(serviceRegistryEntry), STRID(endpoint)};

const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scmDataOutputTypeIds[] = {STRID(ServiceRegistryEntry), STRID(WSTRING)};

const TForteInt16 FORTE_RegisterServiceAdp::scmEIWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scmEventInputNames[] = {STRID(doneRegister), STRID(doneUnregister)};

const TDataIOID FORTE_RegisterServiceAdp::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_RegisterServiceAdp::scmEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_RegisterServiceAdp::scmEventOutputNames[] = {STRID(registerService), STRID(unregisterService)};

const SFBInterfaceSpec FORTE_RegisterServiceAdp::scmFBInterfaceSpecSocket = {
  2,  scmEventInputNames,  0,  scmEIWithIndexes,
  2,  scmEventOutputNames,  scmEOWith, scmEOWithIndexes,  0,  0, 0, 
  2,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_RegisterServiceAdp::scmFBInterfaceSpecPlug = {
  2,  scmEventOutputNames,  scmEOWith,  scmEOWithIndexes,
  2,  scmEventInputNames,  0, 0,  2,  scmDataOutputNames, scmDataOutputTypeIds,
  0,  0, 0,
  0, 0
};



