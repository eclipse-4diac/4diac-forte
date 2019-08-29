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
