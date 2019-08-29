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

#ifndef SRC_CORE_DATATYPES_CONVERT_LREALTOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_LREALTOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   LREAL_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_LREAL_DATATYPE

inline const CIEC_LWORD LREAL_TO_LWORD(const CIEC_LREAL &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL LREAL_TO_REAL(const CIEC_LREAL &paVal){
  return CIEC_ANY::cast<CIEC_REAL>(paVal);
}
#endif

inline const CIEC_USINT LREAL_TO_USINT(const CIEC_LREAL &paVal){
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_UDINT LREAL_TO_UDINT(const CIEC_LREAL &paVal){
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

inline const CIEC_UINT LREAL_TO_UINT(const CIEC_LREAL &paVal){
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

inline const CIEC_ULINT LREAL_TO_ULINT(const CIEC_LREAL &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}

inline const CIEC_LINT LREAL_TO_LINT(const CIEC_LREAL &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}

inline const CIEC_DINT LREAL_TO_DINT(const CIEC_LREAL &paVal){
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_SINT LREAL_TO_SINT(const CIEC_LREAL &paVal){
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_INT LREAL_TO_INT(const CIEC_LREAL &paVal){
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

inline const CIEC_STRING LREAL_TO_STRING(const CIEC_LREAL &paVal){
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING LREAL_TO_WSTRING(const CIEC_LREAL &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif
#endif


#endif /* SRC_CORE_DATATYPES_CONVERT_LREALTOCONVERTFUNCTIONS_H_ */
