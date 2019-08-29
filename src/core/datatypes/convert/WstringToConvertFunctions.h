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
inline const CIEC_BOOL WSTRING_TO_BOOL(const CIEC_WSTRING &paVal){
  CIEC_BOOL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_BYTE WSTRING_TO_BYTE(const CIEC_WSTRING &paVal){
  CIEC_BYTE tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_DWORD WSTRING_TO_DWORD(const CIEC_WSTRING &paVal){
  CIEC_DWORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_WORD WSTRING_TO_WORD(const CIEC_WSTRING &paVal){
  CIEC_WORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_DINT WSTRING_TO_DINT(const CIEC_WSTRING &paVal){
  CIEC_DINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_INT WSTRING_TO_INT(const CIEC_WSTRING &paVal){
  CIEC_INT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_UINT WSTRING_TO_UINT(const CIEC_WSTRING &paVal){
  CIEC_UINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_UDINT WSTRING_TO_UDINT(const CIEC_WSTRING &paVal){
  CIEC_UDINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_USINT WSTRING_TO_USINT(const CIEC_WSTRING &paVal){
  CIEC_USINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_STRING WSTRING_TO_STRING(const CIEC_WSTRING &paVal){
  CIEC_STRING tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_TIME WSTRING_TO_TIME(const CIEC_WSTRING &paVal){
  CIEC_TIME tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_SINT WSTRING_TO_SINT(const CIEC_WSTRING &paVal){
  CIEC_SINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL WSTRING_TO_REAL(const CIEC_WSTRING &paVal){
  CIEC_REAL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL WSTRING_TO_LREAL(const CIEC_WSTRING &paVal){
  CIEC_LREAL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT WSTRING_TO_LINT(const CIEC_WSTRING &paVal){
  CIEC_LINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_LWORD WSTRING_TO_LWORD(const CIEC_WSTRING &paVal){
  CIEC_LWORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_ULINT WSTRING_TO_ULINT(const CIEC_WSTRING &paVal){
  CIEC_ULINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif
#endif



#endif /* SRC_CORE_DATATYPES_CONVERT_WSTRINGTOCONVERTFUNCTIONS_H_ */
