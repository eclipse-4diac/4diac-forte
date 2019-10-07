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

#ifndef SRC_CORE_DATATYPES_CONVERT_INTTOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_INTTOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   INT_TO_*  functions
//********************************************************************************************
inline const CIEC_BYTE INT_TO_BYTE(const CIEC_INT &paVal) {
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_DWORD INT_TO_DWORD(const CIEC_INT &paVal) {
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_DINT INT_TO_DINT(const CIEC_INT &paVal) {
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT INT_TO_LINT(const CIEC_INT &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL INT_TO_LREAL(const CIEC_INT &paVal){
  return CIEC_ANY::cast<CIEC_LREAL>(paVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD INT_TO_LWORD(const CIEC_INT &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL INT_TO_REAL(const CIEC_INT &paVal){
  return CIEC_ANY::cast<CIEC_REAL>(paVal);
}
#endif

inline const CIEC_SINT INT_TO_SINT(const CIEC_INT &paVal) {
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_UDINT INT_TO_UDINT(const CIEC_INT &paVal) {
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

inline const CIEC_INT INT_TO_UINT(const CIEC_INT &paVal) {
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT INT_TO_ULINT(const CIEC_INT &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}
#endif

inline const CIEC_USINT INT_TO_USINT(const CIEC_INT &paVal) {
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_WORD INT_TO_WORD(const CIEC_INT &paVal) {
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING INT_TO_STRING(const CIEC_INT &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING INT_TO_WSTRING(const CIEC_INT &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

#endif /* SRC_CORE_DATATYPES_CONVERT_INTTOCONVERTFUNCTIONS_H_ */
