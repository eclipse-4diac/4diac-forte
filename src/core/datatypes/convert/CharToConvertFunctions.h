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

#ifndef SRC_CORE_DATATYPES_CONVERT_CHARTOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_CHARTOCONVERTFUNCTIONS_H_

#include "forte_any.h"
#include "forte_sint.h"
#include "forte_byte.h"
#include "forte_word.h"
#include "forte_dword.h"
#include "forte_lword.h"
#include "forte_char.h"
#include "forte_wchar.h"
#include "forte_string.h"

inline const CIEC_USINT func_CHAR_TO_USINT(const CIEC_CHAR &paVal) {
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_BYTE func_CHAR_TO_BYTE(const CIEC_CHAR &paVal) {
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_WORD func_CHAR_TO_WORD(const CIEC_CHAR &paVal) {
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_DWORD func_CHAR_TO_DWORD(const CIEC_CHAR &paVal) {
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_LWORD func_CHAR_TO_LWORD(const CIEC_CHAR &paVal) {
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}

inline const CIEC_STRING func_CHAR_TO_STRING(const CIEC_CHAR &paVal) {
  CIEC_STRING stringCast(paVal);
  return stringCast;
}

inline const CIEC_WCHAR func_CHAR_TO_WCHAR(const CIEC_CHAR &paVal) {
  return CIEC_ANY::cast<CIEC_WCHAR>(paVal);
}

#endif /* SRC_CORE_DATATYPES_CONVERT_CHARTOCONVERTFUNCTIONS_H_ */