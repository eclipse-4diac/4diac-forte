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
inline const CIEC_INT func_STRING_TO_INT(const CIEC_STRING &paVal) {
  CIEC_INT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_UINT func_STRING_TO_UINT(const CIEC_STRING &paVal) {
  CIEC_UINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_USINT func_STRING_TO_USINT(const CIEC_STRING &paVal) {
  CIEC_USINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_SINT func_STRING_TO_SINT(const CIEC_STRING &paVal) {
  CIEC_SINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_DINT func_STRING_TO_DINT(const CIEC_STRING &paVal) {
  CIEC_DINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_UDINT func_STRING_TO_UDINT(const CIEC_STRING &paVal) {
  CIEC_UDINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_BOOL func_STRING_TO_BOOL(const CIEC_STRING &paVal) {
  CIEC_BOOL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_BYTE func_STRING_TO_BYTE(const CIEC_STRING &paVal) {
  CIEC_BYTE tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_WORD func_STRING_TO_WORD(const CIEC_STRING &paVal) {
  CIEC_WORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_DWORD func_STRING_TO_DWORD(const CIEC_STRING &paVal) {
  CIEC_DWORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_TIME func_STRING_TO_TIME(const CIEC_STRING &paVal) {
  CIEC_TIME tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL func_STRING_TO_REAL(const CIEC_STRING &paVal){
  CIEC_REAL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL func_STRING_TO_LREAL(const CIEC_STRING &paVal){
  CIEC_LREAL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT func_STRING_TO_ULINT(const CIEC_STRING &paVal){
  CIEC_ULINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_LINT func_STRING_TO_LINT(const CIEC_STRING &paVal){
  CIEC_LINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_LWORD func_STRING_TO_LWORD(const CIEC_STRING &paVal){
  CIEC_LWORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING func_STRING_TO_WSTRING(const CIEC_STRING &paVal){
  CIEC_WSTRING tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#endif /* SRC_CORE_DATATYPES_CONVERT_STRINGTOCONVERTFUNCTIONS_H_ */
