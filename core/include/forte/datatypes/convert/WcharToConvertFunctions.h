/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_char.h"

namespace forte {
  inline CIEC_CHAR func_WCHAR_TO_CHAR(const CIEC_WCHAR &paVal) {
    return CIEC_ANY::cast<CIEC_CHAR>(paVal);
  }

  inline CIEC_WORD func_WCHAR_TO_WORD(const CIEC_WCHAR &paVal) {
    return CIEC_ANY::cast<CIEC_WORD>(paVal);
  }

  inline CIEC_DWORD func_WCHAR_TO_DWORD(const CIEC_WCHAR &paVal) {
    return CIEC_ANY::cast<CIEC_DWORD>(paVal);
  }

  inline CIEC_LWORD func_WCHAR_TO_LWORD(const CIEC_WCHAR &paVal) {
    return CIEC_ANY::cast<CIEC_LWORD>(paVal);
  }

  inline CIEC_WSTRING func_WCHAR_TO_WSTRING(const CIEC_WCHAR &paVal) {
    CIEC_WSTRING wstringCast(paVal);
    return wstringCast;
  }
} // namespace forte
