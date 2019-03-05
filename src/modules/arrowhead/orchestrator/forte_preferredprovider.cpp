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

#include "forte_preferredprovider.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_preferredprovider_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(PreferredProvider, g_nStringIdPreferredProvider);

CIEC_PreferredProvider::CIEC_PreferredProvider() :
    CIEC_STRUCT(g_nStringIdPreferredProvider, 2, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_PreferredProvider::scmElementTypes[] = { g_nStringIdArrowheadSystem, g_nStringIdArrowheadCloud };
const CStringDictionary::TStringId CIEC_PreferredProvider::scmElementNames[] = { g_nStringIdproviderSystem, g_nStringIdproviderCloud };
