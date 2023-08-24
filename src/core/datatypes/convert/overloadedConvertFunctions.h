/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 * 
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

#ifndef SRC_CORE_DATATYPES_CONVERT_OVERLOADEDCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_OVERLOADEDCONVERTFUNCTIONS_H_

#include "forte_bool.h"
#include "forte_byte.h"
#include "forte_dint.h"
#include "forte_dword.h"
#include "forte_int.h"
#include "forte_lint.h"
#include "forte_lreal.h"
#include "forte_lword.h"
#include "forte_real.h"
#include "forte_sint.h"
#include "forte_udint.h"
#include "forte_uint.h"
#include "forte_ulint.h"
#include "forte_usint.h"
#include "forte_word.h"
#include "forte_string.h"
#include "forte_wstring.h"
#include "forte_time.h"
#include "forte_time_of_day.h"
#include "forte_date_and_time.h"
#include "forte_date.h"
#include "forte_ltime.h"
#include "forte_ltime_of_day.h"
#include "forte_ldate_and_time.h"
#include "forte_ldate.h"
#include "iec61131_cast_helper.h"

template <typename T>
inline auto func_TO_LREAL(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_LREAL>);
  return CIEC_ANY::cast<CIEC_LREAL>(paVal);
}

template <typename T>
inline auto func_TO_REAL(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_REAL>);
  return CIEC_ANY::cast<CIEC_REAL>(paVal);
}

template <typename T>
inline auto func_TO_LINT(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_LINT>);
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}

template <typename T>
inline auto func_TO_DINT(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_DINT>);
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

template <typename T>
inline auto func_TO_INT(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_INT>);
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

template <typename T>
inline auto func_TO_SINT(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_SINT>);
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

template <typename T>
inline auto func_TO_ULINT(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_ULINT>);
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}

template <typename T>
inline auto func_TO_UDINT(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_UDINT>);
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

template <typename T>
inline auto func_TO_UINT(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_UINT>);
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

template <typename T>
inline auto func_TO_USINT(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_USINT>);
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

template <typename T>
inline auto func_TO_LWORD(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_LWORD>);
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}

template <typename T>
inline auto func_TO_DWORD(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_DWORD>);
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

template <typename T>
inline auto func_TO_WORD(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_WORD>);
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

template <typename T>
inline auto func_TO_BYTE(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_BYTE>);
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

template <typename T>
inline auto func_TO_LTIME(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_LTIME>);
  return CIEC_ANY::cast<CIEC_LTIME>(paVal);
}

template <typename T>
inline auto func_TO_TIME(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_TIME>);
  return CIEC_ANY::cast<CIEC_TIME>(paVal);
}

template <typename T>
inline auto func_TO_LDT(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_LDATE_AND_TIME>);
  return CIEC_ANY::cast<CIEC_LDATE_AND_TIME>(paVal);
}

template <typename T>
inline auto func_TO_DT(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_DATE_AND_TIME>);
  return CIEC_ANY::cast<CIEC_DATE_AND_TIME>(paVal);
}

template <typename T>
inline auto func_TO_LDATE(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_LDATE>);
  return CIEC_ANY::cast<CIEC_LDATE>(paVal);
}

template <typename T>
inline auto func_TO_DATE(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_DATE>);
  return CIEC_ANY::cast<CIEC_DATE>(paVal);
}

template <typename T>
inline auto func_TO_LTOD(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_LTIME_OF_DAY>);
  return CIEC_ANY::cast<CIEC_LTIME_OF_DAY>(paVal);
}

template <typename T>
inline auto func_TO_TOD(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_TIME_OF_DAY>);
  return CIEC_ANY::cast<CIEC_TIME_OF_DAY>(paVal);
}

template <typename T>
inline auto func_TO_WSTRING(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_WSTRING>);
  return CIEC_ANY::cast<CIEC_WSTRING>(paVal);
}

template <typename T>
inline auto func_TO_STRING(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_STRING>);
  return CIEC_ANY::cast<CIEC_STRING>(paVal);
}

template <typename T>
inline auto func_TO_WCHAR(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_WCHAR>);
  return CIEC_ANY::cast<CIEC_WCHAR>(paVal);
}

template <typename T>
inline auto func_TO_CHAR(const T &paVal) {
  static_assert(forte::core::mpl::is_implicit_or_explicit_castable_v<T, CIEC_CHAR>);
  return CIEC_ANY::cast<CIEC_CHAR>(paVal);
}




#endif /* SRC_CORE_DATATYPES_CONVERT_OVERLOADEDCONVERTFUNCTIONS_H_ */