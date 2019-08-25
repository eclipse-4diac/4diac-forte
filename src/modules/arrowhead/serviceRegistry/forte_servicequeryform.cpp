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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_servicequeryform_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(ServiceQueryForm, g_nStringIdServiceQueryForm);

CIEC_ServiceQueryForm::CIEC_ServiceQueryForm() :
    CIEC_STRUCT(g_nStringIdServiceQueryForm, 4, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ServiceQueryForm::scmElementTypes[] = { g_nStringIdArrowheadService, g_nStringIdBOOL, g_nStringIdBOOL,
  g_nStringIdDINT };
const CStringDictionary::TStringId CIEC_ServiceQueryForm::scmElementNames[] = { g_nStringIdservice, g_nStringIdmetadataSearch, g_nStringIdpingProviders,
  g_nStringIdversion };
