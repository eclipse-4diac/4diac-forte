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

#ifndef SRC_CORE_DATATYPES_CONVERT_WCHARTOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_WCHARTOCONVERTFUNCTIONS_H_

#include "forte_any.h"
#include "forte_word.h"
#include "forte_dword.h"
#include "forte_lword.h"
#include "forte_char.h"
#include "forte_wchar.h"
#include "forte_wstring.h"

inline const CIEC_CHAR func_WCHAR_TO_CHAR(const CIEC_WCHAR &paVal) {
  return CIEC_ANY::cast<CIEC_CHAR>(paVal);
}

inline const CIEC_WORD func_WCHAR_TO_WORD(const CIEC_WCHAR &paVal) {
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_DWORD func_WCHAR_TO_DWORD(const CIEC_WCHAR &paVal) {
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_LWORD func_WCHAR_TO_LWORD(const CIEC_WCHAR &paVal) {
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}

inline const CIEC_WSTRING func_WCHAR_TO_WSTRING(const CIEC_WCHAR &paVal) {
  CIEC_WSTRING wstringCast(paVal);
  return wstringCast;
}

#endif /* SRC_CORE_DATATYPES_CONVERT_WCHARTOCONVERTFUNCTIONS_H_ */