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

#include "forte_orchestrationform.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(ArrowheadService);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(authorizationToken);
USE_STRING_ID(instruction);
USE_STRING_ID(OrchestrationForm);
USE_STRING_ID(provider);
USE_STRING_ID(service);
USE_STRING_ID(serviceURI);
USE_STRING_ID(signature);
USE_STRING_ID(warnings);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_DATATYPE(OrchestrationForm, STRID(OrchestrationForm));

CIEC_OrchestrationForm::CIEC_OrchestrationForm() :
    CIEC_STRUCT(STRID(OrchestrationForm), 7, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_OrchestrationForm::scmElementTypes[] = {
    STRID(ArrowheadService), STRID(ArrowheadSystem), STRID(WSTRING), STRID(WSTRING),
    STRID(WSTRING),          STRID(WSTRING),         STRID(ARRAY),   10,
    STRID(WSTRING)};
const CStringDictionary::TStringId CIEC_OrchestrationForm::scmElementNames[] = {
    STRID(service),   STRID(provider), STRID(serviceURI), STRID(instruction), STRID(authorizationToken),
    STRID(signature), STRID(warnings)};
