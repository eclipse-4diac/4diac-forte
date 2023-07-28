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


#ifndef SRC_CORE_DATATYPES_CONVERT_STRINGTOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_STRINGTOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   STRING_TO_*  functions
//********************************************************************************************

template<typename T>
T func_STRING_AS_X_helper(const CIEC_STRING &paVal) {
  T tempVal;
  const char *pacBuffer = paVal.getStorage().c_str();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_USINT func_STRING_AS_USINT(const CIEC_STRING &paVal) {
  return func_STRING_AS_X_helper<CIEC_USINT>(paVal);
}

inline const CIEC_UINT func_STRING_AS_UINT(const CIEC_STRING &paVal) {
  return func_STRING_AS_X_helper<CIEC_UINT>(paVal);
}

inline const CIEC_UDINT func_STRING_AS_UDINT(const CIEC_STRING &paVal) {
  return func_STRING_AS_X_helper<CIEC_UDINT>(paVal);
}

inline const CIEC_ULINT func_STRING_AS_ULINT(const CIEC_STRING &paVal){
  return func_STRING_AS_X_helper<CIEC_ULINT>(paVal);
}

inline const CIEC_SINT func_STRING_AS_SINT(const CIEC_STRING &paVal) {
  return func_STRING_AS_X_helper<CIEC_SINT>(paVal);
}

inline const CIEC_INT func_STRING_AS_INT(const CIEC_STRING &paVal) {
  return func_STRING_AS_X_helper<CIEC_INT>(paVal);
}

inline const CIEC_DINT func_STRING_AS_DINT(const CIEC_STRING &paVal) {
  return func_STRING_AS_X_helper<CIEC_DINT>(paVal);
}

inline const CIEC_LINT func_STRING_AS_LINT(const CIEC_STRING &paVal){
  return func_STRING_AS_X_helper<CIEC_LINT>(paVal);
}

inline const CIEC_BOOL func_STRING_AS_BOOL(const CIEC_STRING &paVal) {
  return func_STRING_AS_X_helper<CIEC_BOOL>(paVal);
}

inline const CIEC_BYTE func_STRING_AS_BYTE(const CIEC_STRING &paVal) {
  return func_STRING_AS_X_helper<CIEC_BYTE>(paVal);
}

inline const CIEC_WORD func_STRING_AS_WORD(const CIEC_STRING &paVal) {
  return func_STRING_AS_X_helper<CIEC_WORD>(paVal);
}

inline const CIEC_DWORD func_STRING_AS_DWORD(const CIEC_STRING &paVal) {
  return func_STRING_AS_X_helper<CIEC_DWORD>(paVal);
}

inline const CIEC_LWORD func_STRING_AS_LWORD(const CIEC_STRING &paVal){
  return func_STRING_AS_X_helper<CIEC_LWORD>(paVal);
}

inline const CIEC_TIME func_STRING_AS_TIME(const CIEC_STRING &paVal) {
  return func_STRING_AS_X_helper<CIEC_TIME>(paVal);
}

inline const CIEC_REAL func_STRING_AS_REAL(const CIEC_STRING &paVal){
  return func_STRING_AS_X_helper<CIEC_REAL>(paVal);
}

inline const CIEC_LREAL func_STRING_AS_LREAL(const CIEC_STRING &paVal){
  return func_STRING_AS_X_helper<CIEC_LREAL>(paVal);
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING func_STRING_TO_WSTRING(const CIEC_STRING &paVal){
  return func_STRING_AS_X_helper<CIEC_WSTRING>(paVal);
}
#endif

#endif /* SRC_CORE_DATATYPES_CONVERT_STRINGTOCONVERTFUNCTIONS_H_ */
