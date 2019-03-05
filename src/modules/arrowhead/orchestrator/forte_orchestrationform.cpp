/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "forte_orchestrationform.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_orchestrationform_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(OrchestrationForm, g_nStringIdOrchestrationForm);

CIEC_OrchestrationForm::CIEC_OrchestrationForm() :
    CIEC_STRUCT(g_nStringIdOrchestrationForm, 7, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_OrchestrationForm::scmElementTypes[] = { g_nStringIdArrowheadService, g_nStringIdArrowheadSystem, g_nStringIdWSTRING,
  g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdARRAY, 10, g_nStringIdWSTRING };
const CStringDictionary::TStringId CIEC_OrchestrationForm::scmElementNames[] = { g_nStringIdservice, g_nStringIdprovider, g_nStringIdserviceURI,
  g_nStringIdinstruction, g_nStringIdauthorizationToken, g_nStringIdsignature, g_nStringIdwarnings };
