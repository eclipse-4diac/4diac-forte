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

#include "forte_servicequeryform.h"

USE_STRING_ID(ArrowheadService);
USE_STRING_ID(BOOL);
USE_STRING_ID(DINT);
USE_STRING_ID(metadataSearch);
USE_STRING_ID(pingProviders);
USE_STRING_ID(service);
USE_STRING_ID(ServiceQueryForm);
USE_STRING_ID(version);

DEFINE_FIRMWARE_DATATYPE(ServiceQueryForm, STRID(ServiceQueryForm));

CIEC_ServiceQueryForm::CIEC_ServiceQueryForm() :
    CIEC_STRUCT(STRID(ServiceQueryForm), 4, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ServiceQueryForm::scmElementTypes[] = {STRID(ArrowheadService), STRID(BOOL),
                                                                               STRID(BOOL), STRID(DINT)};
const CStringDictionary::TStringId CIEC_ServiceQueryForm::scmElementNames[] = {STRID(service), STRID(metadataSearch),
                                                                               STRID(pingProviders), STRID(version)};
