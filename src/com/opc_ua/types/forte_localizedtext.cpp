/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "forte_localizedtext.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_localizedtext_gen.cpp"
#endif

using namespace std::string_literals;

DEFINE_FIRMWARE_DATATYPE(LocalizedText, g_nStringIdLocalizedText);

CIEC_LocalizedText::CIEC_LocalizedText() :
    CIEC_STRUCT(),
    var_locale(""s),
    var_text(""s) {
}

CStringDictionary::TStringId CIEC_LocalizedText::getStructTypeNameID() const {
  return g_nStringIdLocalizedText;
}

CIEC_ANY *CIEC_LocalizedText::getMember(size_t paMemberIndex) {
  switch (paMemberIndex) {
    case 0: return &var_locale;
    case 1: return &var_text;
  }
  return nullptr;
}

const CIEC_ANY *CIEC_LocalizedText::getMember(size_t paMemberIndex) const {
  switch (paMemberIndex) {
    case 0: return &var_locale;
    case 1: return &var_text;
  }
  return nullptr;
}

const CStringDictionary::TStringId CIEC_LocalizedText::scmElementNames[] = { g_nStringIdlocale, g_nStringIdtext };
