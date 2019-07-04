/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "forte_localizedtext.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_localizedtext_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(LocalizedText, g_nStringIdLocalizedText);

CIEC_LocalizedText::CIEC_LocalizedText() :
    CIEC_STRUCT(g_nStringIdLocalizedText, 2, scmElementTypes, scmElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_LocalizedText::scmElementTypes[] = { g_nStringIdSTRING, g_nStringIdSTRING };
const CStringDictionary::TStringId CIEC_LocalizedText::scmElementNames[] = { g_nStringIdtext, g_nStringIdlocale };
