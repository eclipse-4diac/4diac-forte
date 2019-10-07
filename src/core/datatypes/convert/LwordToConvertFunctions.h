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

#ifndef SRC_CORE_DATATYPES_CONVERT_LWORDTOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_LWORDTOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   LWORD_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_64BIT_DATATYPES

inline const CIEC_DINT LWORD_TO_DINT(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_BYTE LWORD_TO_BYTE(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_INT LWORD_TO_INT(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

inline const CIEC_LINT LWORD_TO_LINT(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL LWORD_TO_LREAL(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_LREAL>(paVal);
}
#endif

inline const CIEC_DWORD LWORD_TO_DWORD(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_SINT LWORD_TO_SINT(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_UDINT LWORD_TO_UDINT(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

inline const CIEC_UINT LWORD_TO_UINT(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

inline const CIEC_ULINT LWORD_TO_ULINT(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}

inline const CIEC_USINT LWORD_TO_USINT(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_WORD LWORD_TO_WORD(const CIEC_LWORD &paVal){
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING LWORD_TO_STRING(const CIEC_LWORD &paVal){
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING LWORD_TO_WSTRING(const CIEC_LWORD &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

#endif



#endif /* SRC_CORE_DATATYPES_CONVERT_LWORDTOCONVERTFUNCTIONS_H_ */
