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

using namespace forte::literals;

DEFINE_FIRMWARE_DATATYPE(OrchestrationForm, "OrchestrationForm"_STRID);

CIEC_OrchestrationForm::CIEC_OrchestrationForm() :
    CIEC_STRUCT("OrchestrationForm"_STRID, 7, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const forte::StringId CIEC_OrchestrationForm::scmElementTypes[] = {
    "ArrowheadService"_STRID, "ArrowheadSystem"_STRID, "WSTRING"_STRID, "WSTRING"_STRID,
    "WSTRING"_STRID,          "WSTRING"_STRID,         "ARRAY"_STRID,   10,
    "WSTRING"_STRID};
const forte::StringId CIEC_OrchestrationForm::scmElementNames[] = {
    "service"_STRID,   "provider"_STRID, "serviceURI"_STRID, "instruction"_STRID, "authorizationToken"_STRID,
    "signature"_STRID, "warnings"_STRID};
