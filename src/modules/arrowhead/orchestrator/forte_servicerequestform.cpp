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

#include "forte_servicerequestform.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(ArrowheadCloud);
USE_STRING_ID(ArrowheadService);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(orchestrationFlags);
USE_STRING_ID(PreferredProvider);
USE_STRING_ID(preferredProviders);
USE_STRING_ID(requestedQoS);
USE_STRING_ID(requestedService);
USE_STRING_ID(requesterCloud);
USE_STRING_ID(requesterSystem);
USE_STRING_ID(ServiceRequestForm);
USE_STRING_ID(WSTRING);


DEFINE_FIRMWARE_DATATYPE(ServiceRequestForm, STRID(ServiceRequestForm));

CIEC_ServiceRequestForm::CIEC_ServiceRequestForm() :
    CIEC_STRUCT(STRID(ServiceRequestForm), 6, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ServiceRequestForm::scmElementTypes[] = { STRID(ArrowheadSystem), STRID(ArrowheadCloud),
  STRID(ArrowheadService), STRID(ARRAY), 10, STRID(WSTRING), STRID(ARRAY), 10, STRID(PreferredProvider), STRID(ARRAY), 10,
  STRID(WSTRING) };
const CStringDictionary::TStringId CIEC_ServiceRequestForm::scmElementNames[] = { STRID(requesterSystem), STRID(requesterCloud),
  STRID(requestedService), STRID(orchestrationFlags), STRID(preferredProviders), STRID(requestedQoS) };
