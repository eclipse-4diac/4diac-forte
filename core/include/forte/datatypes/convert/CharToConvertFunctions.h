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
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wchar.h"

namespace forte {
  inline CIEC_USINT func_CHAR_TO_USINT(const CIEC_CHAR &paVal) {
    return CIEC_ANY::cast<CIEC_USINT>(paVal);
  }

  inline CIEC_BYTE func_CHAR_TO_BYTE(const CIEC_CHAR &paVal) {
    return CIEC_ANY::cast<CIEC_BYTE>(paVal);
  }

  inline CIEC_WORD func_CHAR_TO_WORD(const CIEC_CHAR &paVal) {
    return CIEC_ANY::cast<CIEC_WORD>(paVal);
  }

  inline CIEC_DWORD func_CHAR_TO_DWORD(const CIEC_CHAR &paVal) {
    return CIEC_ANY::cast<CIEC_DWORD>(paVal);
  }

  inline CIEC_LWORD func_CHAR_TO_LWORD(const CIEC_CHAR &paVal) {
    return CIEC_ANY::cast<CIEC_LWORD>(paVal);
  }

  inline CIEC_STRING func_CHAR_TO_STRING(const CIEC_CHAR &paVal) {
    CIEC_STRING stringCast(paVal);
    return stringCast;
  }

  inline CIEC_WCHAR func_CHAR_TO_WCHAR(const CIEC_CHAR &paVal) {
    return CIEC_ANY::cast<CIEC_WCHAR>(paVal);
  }
} // namespace forte
