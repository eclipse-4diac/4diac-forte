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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_arrowheadservice_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(ArrowheadService, g_nStringIdArrowheadService);

CIEC_ArrowheadService::CIEC_ArrowheadService() :
    CIEC_STRUCT(g_nStringIdArrowheadService, 3, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_ArrowheadService::scmElementTypes[] = { g_nStringIdWSTRING, g_nStringIdARRAY, 10, g_nStringIdWSTRING, g_nStringIdARRAY,
  10, g_nStringIdWSTRING };
const CStringDictionary::TStringId CIEC_ArrowheadService::scmElementNames[] =
  { g_nStringIdserviceDefinition, g_nStringIdinterfaces, g_nStringIdserviceMetadata };
