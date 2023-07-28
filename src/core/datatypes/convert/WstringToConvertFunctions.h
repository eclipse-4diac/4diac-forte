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

#ifndef SRC_CORE_DATATYPES_CONVERT_WSTRINGTOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_WSTRINGTOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   WSTRING_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_BOOL func_WSTRING_AS_BOOL(const CIEC_WSTRING &paVal){
  CIEC_BOOL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_BYTE func_WSTRING_AS_BYTE(const CIEC_WSTRING &paVal){
  CIEC_BYTE tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_DWORD func_WSTRING_AS_DWORD(const CIEC_WSTRING &paVal){
  CIEC_DWORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_WORD func_WSTRING_AS_WORD(const CIEC_WSTRING &paVal){
  CIEC_WORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_DINT func_WSTRING_AS_DINT(const CIEC_WSTRING &paVal){
  CIEC_DINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_INT func_WSTRING_AS_INT(const CIEC_WSTRING &paVal){
  CIEC_INT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_UINT func_WSTRING_AS_UINT(const CIEC_WSTRING &paVal){
  CIEC_UINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_UDINT func_WSTRING_AS_UDINT(const CIEC_WSTRING &paVal){
  CIEC_UDINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_USINT func_WSTRING_AS_USINT(const CIEC_WSTRING &paVal){
  CIEC_USINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_STRING func_WSTRING_TO_STRING(const CIEC_WSTRING &paVal){
  const char *pacBuffer = paVal.getValue();
  CIEC_STRING tempVal(pacBuffer, strlen(pacBuffer)); // TODO: implementation wrong, needs to be fixed with WSTRING
  return tempVal;
}

inline const CIEC_TIME func_WSTRING_AS_TIME(const CIEC_WSTRING &paVal){
  CIEC_TIME tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_SINT func_WSTRING_AS_SINT(const CIEC_WSTRING &paVal){
  CIEC_SINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_REAL func_WSTRING_AS_REAL(const CIEC_WSTRING &paVal){
  CIEC_REAL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_LREAL func_WSTRING_AS_LREAL(const CIEC_WSTRING &paVal){
  CIEC_LREAL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_LINT func_WSTRING_AS_LINT(const CIEC_WSTRING &paVal){
  CIEC_LINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_LWORD func_WSTRING_AS_LWORD(const CIEC_WSTRING &paVal){
  CIEC_LWORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_ULINT func_WSTRING_AS_ULINT(const CIEC_WSTRING &paVal){
  CIEC_ULINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#endif /* SRC_CORE_DATATYPES_CONVERT_WSTRINGTOCONVERTFUNCTIONS_H_ */
