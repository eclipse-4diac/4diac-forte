/*******************************************************************************
 * Copyright (c) 2010 - 2015 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *    2018-2019 TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Monika Wenger, Carolyn Oates, Patrick Smejkal,
 *    Matthias Plasch,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - fixes DT_TO_TOD
 *    Martin Melik-Merkumians - removes invalid casts, update implementation
 *     to use new cast function
 *******************************************************************************/

#ifndef SRC_CORE_DATATYPES_CONVERT_BOOLTOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_BOOLTOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   BOOL_TO_*  functions
//********************************************************************************************
inline const CIEC_BYTE BOOL_TO_BYTE(const CIEC_BOOL &paVal) {
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_DINT BOOL_TO_DINT(const CIEC_BOOL &paVal) {
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_DWORD BOOL_TO_DWORD(const CIEC_BOOL &paVal) {
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_INT BOOL_TO_INT(const CIEC_BOOL &paVal) {
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT BOOL_TO_LINT(const CIEC_BOOL &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD BOOL_TO_LWORD(const CIEC_BOOL &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}
#endif

inline const CIEC_SINT BOOL_TO_SINT(const CIEC_BOOL &paVal) {
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_UDINT BOOL_TO_UDINT(const CIEC_BOOL &paVal) {
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

inline const CIEC_UINT BOOL_TO_UINT(const CIEC_BOOL &paVal) {
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT BOOL_TO_ULINT(const CIEC_BOOL &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}
#endif

inline const CIEC_USINT BOOL_TO_USINT(const CIEC_BOOL &paVal) {
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_WORD BOOL_TO_WORD(const CIEC_BOOL &paVal) {
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING BOOL_TO_STRING(const CIEC_BOOL &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING BOOL_TO_WSTRING(const CIEC_BOOL &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

#endif /* SRC_CORE_DATATYPES_CONVERT_BOOLTOCONVERTFUNCTIONS_H_ */
