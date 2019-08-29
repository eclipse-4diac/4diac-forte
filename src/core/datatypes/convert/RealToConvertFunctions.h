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

#ifndef SRC_CORE_DATATYPES_CONVERT_REALTOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_REALTOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   REAL_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT REAL_TO_LINT(const CIEC_REAL &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}

inline const CIEC_ULINT REAL_TO_ULINT(const CIEC_REAL &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}

inline const CIEC_LREAL REAL_TO_LREAL(const CIEC_REAL &paVal){
  return CIEC_ANY::cast<CIEC_LREAL>(paVal);
}
#endif

inline const CIEC_DINT REAL_TO_DINT(const CIEC_REAL &paVal){
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_SINT REAL_TO_SINT(const CIEC_REAL &paVal){
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_INT REAL_TO_INT(const CIEC_REAL &paVal){
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

inline const CIEC_USINT REAL_TO_USINT(const CIEC_REAL &paVal){
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_UINT REAL_TO_UINT(const CIEC_REAL &paVal){
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

inline const CIEC_UDINT REAL_TO_UDINT(const CIEC_REAL &paVal){
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

inline const CIEC_STRING REAL_TO_STRING(const CIEC_REAL &paVal){
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING REAL_TO_WSTRING(const CIEC_REAL &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

inline const CIEC_DWORD REAL_TO_DWORD(const CIEC_REAL &paVal){
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}
#endif

#endif /* SRC_CORE_DATATYPES_CONVERT_REALTOCONVERTFUNCTIONS_H_ */
