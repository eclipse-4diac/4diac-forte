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
