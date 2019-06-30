/*******************************************************************************
 * Copyright (c) 2010 - 2015 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *    2018-2019 TU Vienna/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Monika Wenger, Carolyn Oates, Patrick Smejkal,
 *    Matthias Plasch,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - fixes DT_TO_TOD
 *    Martin Melik-Merkumians - removes invalid casts, update implementation
 *     to use new cast function
 *    Martin Melik-Merkumians - adds non standard time to number conversions
 *******************************************************************************/
#ifndef CONVERT_FUNCTIONS_H_
#define CONVERT_FUNCTIONS_H_

#include "datatypes/forte_any.h"
#include "datatypes/forte_bool.h"
#include "datatypes/forte_byte.h"
#include "datatypes/forte_dint.h"
#include "datatypes/forte_dword.h"
#include "datatypes/forte_int.h"
#include "datatypes/forte_lint.h"
#include "datatypes/forte_lreal.h"
#include "datatypes/forte_lword.h"
#include "datatypes/forte_real.h"
#include "datatypes/forte_sint.h"
#include "datatypes/forte_udint.h"
#include "datatypes/forte_uint.h"
#include "datatypes/forte_ulint.h"
#include "datatypes/forte_usint.h"
#include "datatypes/forte_word.h"
#include "datatypes/forte_string.h"
#include "datatypes/forte_wstring.h"
#include "datatypes/forte_time.h"
#include "datatypes/forte_time_of_day.h"
#include "datatypes/forte_date_and_time.h"
#include "datatypes/forte_date.h"
#include "datatypes/forte_struct.h"
#include "datatypes/forte_array.h"
#include <math.h>

//some compilers don't have this definition or is behind __STDC_CONSTANT_MACROS.
//since this definition is only used here, we define it instead of enabling __STDC_CONSTANT_MACROS globally,
//because it's not easy to detect which compilers will or not have this definition
#ifndef UINT64_C
# define UINT64_C(x)   (x##ULL)
#endif

/*! \file
 * This file implements the type conversion functions as defined by IEC 61131-3 in subclause 2.5.1.5.1
 */

inline void stringConverter(CIEC_ANY_STRING &paString, const CIEC_ANY &paVal) {
  TForteUInt16 bufferSize = CIEC_ANY::csmStringBufferSize[paVal.getDataTypeID()];
  paString.reserve(static_cast<TForteUInt16>(bufferSize));
  char *pacBuffer = paString.getValue();
  int nWrittenBytes = paVal.toString(pacBuffer, bufferSize);
  nWrittenBytes = nWrittenBytes > -1 ? nWrittenBytes : 0;
  paString.assign(pacBuffer, static_cast<TForteUInt16>(nWrittenBytes));
}

//********************************************************************************************
//   DT_TO_**  functions
//********************************************************************************************
#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_DATE DT_TO_DATE(const CIEC_DATE_AND_TIME &paVal){
  TForteUInt64 nBuffer = paVal;
  time_t t = static_cast<time_t>(nBuffer / 1000);
  struct tm *ptm = forte_localtime(&t);

  if(0 == ptm){
    return CIEC_DATE(0);
  }

  ptm->tm_hour = 0;
  ptm->tm_min = 0;
  ptm->tm_sec = 0;

  t = forte_mktime(ptm);
  if((time_t) -1 == t){
    return CIEC_DATE(0);
  }

  return CIEC_DATE(t * 1000ULL);
}

inline const CIEC_DATE_AND_TIME DATE_TO_DT(const CIEC_DATE &paVal){
  return CIEC_DATE_AND_TIME((TForteUInt64) paVal);
}

inline const CIEC_TIME_OF_DAY DT_TO_TOD(const CIEC_DATE_AND_TIME &paVal){
  TForteUInt64 nBuffer = paVal;
  time_t t = static_cast<time_t>(nBuffer / 1000);
  struct tm *ptm = forte_localtime(&t);

  if(0 == ptm){
    return CIEC_TIME_OF_DAY(0);
  }

  return CIEC_TIME_OF_DAY(static_cast<TForteUInt64>((ptm->tm_hour * UINT64_C(3600) + ptm->tm_min * UINT64_C(60) + ptm->tm_sec) * UINT64_C(1000) + (nBuffer % UINT64_C(1000))));
}
#endif

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

//********************************************************************************************
//   STRING_TO_*  functions
//********************************************************************************************
inline const CIEC_INT STRING_TO_INT(const CIEC_STRING &paVal) {
  CIEC_INT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_UINT STRING_TO_UINT(const CIEC_STRING &paVal) {
  CIEC_UINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_USINT STRING_TO_USINT(const CIEC_STRING &paVal) {
  CIEC_USINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_SINT STRING_TO_SINT(const CIEC_STRING &paVal) {
  CIEC_SINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_DINT STRING_TO_DINT(const CIEC_STRING &paVal) {
  CIEC_DINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_UDINT STRING_TO_UDINT(const CIEC_STRING &paVal) {
  CIEC_UDINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_BOOL STRING_TO_BOOL(const CIEC_STRING &paVal) {
  CIEC_BOOL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_BYTE STRING_TO_BYTE(const CIEC_STRING &paVal) {
  CIEC_BYTE tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_WORD STRING_TO_WORD(const CIEC_STRING &paVal) {
  CIEC_WORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_DWORD STRING_TO_DWORD(const CIEC_STRING &paVal) {
  CIEC_DWORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_TIME STRING_TO_TIME(const CIEC_STRING &paVal) {
  CIEC_TIME tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL STRING_TO_REAL(const CIEC_STRING &paVal){
  CIEC_REAL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL STRING_TO_LREAL(const CIEC_STRING &paVal){
  CIEC_LREAL tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT STRING_TO_ULINT(const CIEC_STRING &paVal){
  CIEC_ULINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_LINT STRING_TO_LINT(const CIEC_STRING &paVal){
  CIEC_LINT tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_LWORD STRING_TO_LWORD(const CIEC_STRING &paVal){
  CIEC_LWORD tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING STRING_TO_WSTRING(const CIEC_STRING &paVal){
  CIEC_WSTRING tempVal;
  const char *pacBuffer = paVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

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

//********************************************************************************************
//   TIME_TO_*  functions
//********************************************************************************************
inline const CIEC_STRING TIME_TO_STRING(const CIEC_TIME &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING TIME_TO_WSTRING(const CIEC_TIME &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

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

//********************************************************************************************
//   BYTE_TO_*  functions
//********************************************************************************************

inline const CIEC_DINT BYTE_TO_DINT(const CIEC_BYTE &paVal) {
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_DWORD BYTE_TO_DWORD(const CIEC_BYTE &paVal) {
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_INT BYTE_TO_INT(const CIEC_BYTE &paVal) {
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT BYTE_TO_LINT(const CIEC_BYTE &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD BYTE_TO_LWORD(const CIEC_BYTE &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}
#endif

inline const CIEC_SINT BYTE_TO_SINT(const CIEC_BYTE &paVal) {
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_UDINT BYTE_TO_UDINT(const CIEC_BYTE &paVal) {
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

inline const CIEC_UINT BYTE_TO_UINT(const CIEC_BYTE &paVal) {
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT BYTE_TO_ULINT(const CIEC_BYTE &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}
#endif

inline const CIEC_USINT BYTE_TO_USINT(const CIEC_BYTE &paVal) {
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_WORD BYTE_TO_WORD(const CIEC_BYTE &paVal) {
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING BYTE_TO_STRING(const CIEC_BYTE &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING BYTE_TO_WSTRING(const CIEC_BYTE &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

//********************************************************************************************
//   DINT_TO_*  functions
//********************************************************************************************
inline const CIEC_BYTE DINT_TO_BYTE(const CIEC_DINT &paVal) {
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_DWORD DINT_TO_DWORD(const CIEC_DINT &paVal) {
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_INT DINT_TO_INT(const CIEC_DINT &paVal) {
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT DINT_TO_LINT(const CIEC_DINT &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL DINT_TO_LREAL(const CIEC_DINT &paVal){
  return CIEC_ANY::cast<CIEC_LREAL>(paVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD DINT_TO_LWORD(const CIEC_DINT &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL DINT_TO_REAL(const CIEC_DINT &paVal){
  return CIEC_ANY::cast<CIEC_REAL>(paVal);
}
#endif

inline const CIEC_SINT DINT_TO_SINT(const CIEC_DINT &paVal) {
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_UDINT DINT_TO_UDINT(const CIEC_DINT &paVal) {
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

inline const CIEC_UINT DINT_TO_UINT(const CIEC_DINT &paVal) {
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT DINT_TO_ULINT(const CIEC_DINT &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}
#endif

inline const CIEC_USINT DINT_TO_USINT(const CIEC_DINT &paVal) {
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_WORD DINT_TO_WORD(const CIEC_DINT &paVal) {
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING DINT_TO_STRING(const CIEC_DINT &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING DINT_TO_WSTRING(const CIEC_DINT &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

//********************************************************************************************
//   DWORD_TO_*  functions
//********************************************************************************************
inline const CIEC_DINT DWORD_TO_DINT(const CIEC_DWORD &paVal) {
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_BYTE DWORD_TO_BYTE(const CIEC_DWORD &paVal) {
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_INT DWORD_TO_INT(const CIEC_DWORD &paVal) {
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT DWORD_TO_LINT(const CIEC_DWORD &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD DWORD_TO_LWORD(const CIEC_DWORD &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL DWORD_TO_REAL(const CIEC_DWORD &paVal){
  return CIEC_ANY::cast<CIEC_REAL>(paVal);
}
#endif

inline const CIEC_SINT DWORD_TO_SINT(const CIEC_DWORD &paVal) {
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_UDINT DWORD_TO_UDINT(const CIEC_DWORD &paVal) {
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

inline const CIEC_UINT DWORD_TO_UINT(const CIEC_DWORD &paVal) {
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT DWORD_TO_ULINT(const CIEC_DWORD &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}
#endif

inline const CIEC_USINT DWORD_TO_USINT(const CIEC_DWORD &paVal) {
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_WORD DWORD_TO_WORD(const CIEC_DWORD &paVal) {
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING DWORD_TO_STRING(const CIEC_DWORD &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING DWORD_TO_WSTRING(const CIEC_DWORD &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

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

//********************************************************************************************
//   UDINT_TO_*  functions
//********************************************************************************************
inline const CIEC_DINT UDINT_TO_DINT(const CIEC_UDINT &paVal) {
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_BYTE UDINT_TO_BYTE(const CIEC_UDINT &paVal) {
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_INT UDINT_TO_INT(const CIEC_UDINT &paVal) {
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT UDINT_TO_LINT(const CIEC_UDINT &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL UDINT_TO_LREAL(const CIEC_UDINT &paVal){
  return CIEC_ANY::cast<CIEC_LREAL>(paVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD UDINT_TO_LWORD(const CIEC_UDINT &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL UDINT_TO_REAL(const CIEC_UDINT &paVal){
  return CIEC_ANY::cast<CIEC_REAL>(paVal);
}
#endif

inline const CIEC_SINT UDINT_TO_SINT(const CIEC_UDINT &paVal) {
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_DWORD UDINT_TO_DWORD(const CIEC_UDINT &paVal) {
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_UINT UDINT_TO_UINT(const CIEC_UDINT &paVal) {
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT UDINT_TO_ULINT(const CIEC_UDINT &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}
#endif

inline const CIEC_USINT UDINT_TO_USINT(const CIEC_UDINT &paVal) {
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_WORD UDINT_TO_WORD(const CIEC_UDINT &paVal) {
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING UDINT_TO_STRING(const CIEC_UDINT &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING UDINT_TO_WSTRING(const CIEC_UDINT &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

//********************************************************************************************
//   UINT_TO_*  functions
//********************************************************************************************
inline const CIEC_DINT UINT_TO_DINT(const CIEC_UINT &paVal) {
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_INT UINT_TO_INT(const CIEC_UINT &paVal) {
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

inline const CIEC_BYTE UINT_TO_BYTE(const CIEC_UINT &paVal) {
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT UINT_TO_LINT(const CIEC_UINT &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL UINT_TO_LREAL(const CIEC_UINT &paVal){
  return CIEC_ANY::cast<CIEC_LREAL>(paVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD UINT_TO_LWORD(const CIEC_UINT &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL UINT_TO_REAL(const CIEC_UINT &paVal){
  return CIEC_ANY::cast<CIEC_REAL>(paVal);
}
#endif

inline const CIEC_SINT UINT_TO_SINT(const CIEC_UINT &paVal) {
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_DWORD UINT_TO_DWORD(const CIEC_UINT &paVal) {
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_UDINT UINT_TO_UDINT(const CIEC_UINT &paVal) {
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT UINT_TO_ULINT(const CIEC_UINT &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}
#endif

inline const CIEC_USINT UINT_TO_USINT(const CIEC_UINT &paVal) {
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_WORD UINT_TO_WORD(const CIEC_UINT &paVal) {
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING UINT_TO_STRING(const CIEC_UINT &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING UINT_TO_WSTRING(const CIEC_UINT &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

//********************************************************************************************
//   ULINT_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_64BIT_DATATYPES

inline const CIEC_DINT ULINT_TO_DINT(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_BYTE ULINT_TO_BYTE(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_INT ULINT_TO_INT(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

inline const CIEC_LINT ULINT_TO_LINT(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL ULINT_TO_LREAL(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_LREAL>(paVal);
}
#endif

inline const CIEC_DWORD ULINT_TO_DWORD(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL ULINT_TO_REAL(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_REAL>(paVal);
}
#endif

inline const CIEC_SINT ULINT_TO_SINT(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_UDINT ULINT_TO_UDINT(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

inline const CIEC_UINT ULINT_TO_UINT(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

inline const CIEC_LWORD ULINT_TO_LWORD(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}

inline const CIEC_USINT ULINT_TO_USINT(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_WORD ULINT_TO_WORD(const CIEC_ULINT &paVal){
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING ULINT_TO_STRING(const CIEC_ULINT &paVal){
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING ULINT_TO_WSTRING(const CIEC_ULINT &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

#endif

//********************************************************************************************
//   USINT_TO_*  functions
//********************************************************************************************
inline const CIEC_DINT USINT_TO_DINT(const CIEC_USINT &paVal) {
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_BYTE USINT_TO_BYTE(const CIEC_USINT &paVal) {
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_INT USINT_TO_INT(const CIEC_USINT &paVal) {
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT USINT_TO_LINT(const CIEC_USINT &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL USINT_TO_LREAL(const CIEC_USINT &paVal){
  return CIEC_ANY::cast<CIEC_LREAL>(paVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD USINT_TO_LWORD(const CIEC_USINT &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL USINT_TO_REAL(const CIEC_USINT &paVal){
  return CIEC_ANY::cast<CIEC_REAL>(paVal);
}
#endif

inline const CIEC_SINT USINT_TO_SINT(const CIEC_USINT &paVal) {
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_DWORD USINT_TO_DWORD(const CIEC_USINT &paVal) {
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_UDINT USINT_TO_UDINT(const CIEC_USINT &paVal) {
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT USINT_TO_ULINT(const CIEC_USINT &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}
#endif

inline const CIEC_UINT USINT_TO_UINT(const CIEC_USINT &paVal) {
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

inline const CIEC_WORD USINT_TO_WORD(const CIEC_USINT &paVal) {
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING USINT_TO_STRING(const CIEC_USINT &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING USINT_TO_WSTRING(const CIEC_USINT &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

//********************************************************************************************
//   WORD_TO_*  functions
//********************************************************************************************
inline const CIEC_DINT WORD_TO_DINT(const CIEC_WORD &paVal) {
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_BYTE WORD_TO_BYTE(const CIEC_WORD &paVal) {
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_INT WORD_TO_INT(const CIEC_WORD &paVal) {
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT WORD_TO_LINT(const CIEC_WORD &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD WORD_TO_LWORD(const CIEC_WORD &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}
#endif

inline const CIEC_SINT WORD_TO_SINT(const CIEC_WORD &paVal) {
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_DWORD WORD_TO_DWORD(const CIEC_WORD &paVal) {
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_UDINT WORD_TO_UDINT(const CIEC_WORD &paVal) {
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT WORD_TO_ULINT(const CIEC_WORD &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}
#endif

inline const CIEC_USINT WORD_TO_USINT(const CIEC_WORD &paVal) {
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_UINT WORD_TO_UINT(const CIEC_WORD &paVal) {
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

inline const CIEC_STRING WORD_TO_STRING(const CIEC_WORD &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING WORD_TO_WSTRING(const CIEC_WORD &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

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

//********************************************************************************************
//   LINT_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_64BIT_DATATYPES

inline const CIEC_BYTE LINT_TO_BYTE(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_DWORD LINT_TO_DWORD(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_DINT LINT_TO_DINT(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

inline const CIEC_INT LINT_TO_INT(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL LINT_TO_LREAL(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_LREAL>(paVal);
}
#endif

inline const CIEC_LWORD LINT_TO_LWORD(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL LINT_TO_REAL(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_REAL>(paVal);
}
#endif

inline const CIEC_SINT LINT_TO_SINT(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_SINT>(paVal);
}

inline const CIEC_UDINT LINT_TO_UDINT(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

inline const CIEC_UINT LINT_TO_UINT(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

inline const CIEC_ULINT LINT_TO_ULINT(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}

inline const CIEC_USINT LINT_TO_USINT(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_WORD LINT_TO_WORD(const CIEC_LINT &paVal){
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING LINT_TO_STRING(const CIEC_LINT &paVal){
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING LINT_TO_WSTRING(const CIEC_LINT &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

#endif

//********************************************************************************************
//   SINT_TO_*  functions
//********************************************************************************************
inline const CIEC_BYTE SINT_TO_BYTE(const CIEC_SINT &paVal) {
  return CIEC_ANY::cast<CIEC_BYTE>(paVal);
}

inline const CIEC_DWORD SINT_TO_DWORD(const CIEC_SINT &paVal) {
  return CIEC_ANY::cast<CIEC_DWORD>(paVal);
}

inline const CIEC_DINT SINT_TO_DINT(const CIEC_SINT &paVal) {
  return CIEC_ANY::cast<CIEC_DINT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT SINT_TO_LINT(const CIEC_SINT &paVal){
  return CIEC_ANY::cast<CIEC_LINT>(paVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL SINT_TO_LREAL(const CIEC_SINT &paVal){
  return CIEC_ANY::cast<CIEC_LREAL>(paVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD SINT_TO_LWORD(const CIEC_SINT &paVal){
  return CIEC_ANY::cast<CIEC_LWORD>(paVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL SINT_TO_REAL(const CIEC_SINT &paVal){
  return CIEC_ANY::cast<CIEC_REAL>(paVal);
}
#endif

inline const CIEC_INT SINT_TO_INT(const CIEC_SINT &paVal) {
  return CIEC_ANY::cast<CIEC_INT>(paVal);
}

inline const CIEC_UDINT SINT_TO_UDINT(const CIEC_SINT &paVal) {
  return CIEC_ANY::cast<CIEC_UDINT>(paVal);
}

inline const CIEC_UINT SINT_TO_UINT(const CIEC_SINT &paVal) {
  return CIEC_ANY::cast<CIEC_UINT>(paVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT SINT_TO_ULINT(const CIEC_SINT &paVal){
  return CIEC_ANY::cast<CIEC_ULINT>(paVal);
}
#endif

inline const CIEC_USINT SINT_TO_USINT(const CIEC_SINT &paVal) {
  return CIEC_ANY::cast<CIEC_USINT>(paVal);
}

inline const CIEC_WORD SINT_TO_WORD(const CIEC_SINT &paVal) {
  return CIEC_ANY::cast<CIEC_WORD>(paVal);
}

inline const CIEC_STRING SINT_TO_STRING(const CIEC_SINT &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING SINT_TO_WSTRING(const CIEC_SINT &paVal){
  CIEC_WSTRING string;
  stringConverter(string, paVal);
  return string;
}
#endif

//********************************************************************************************
//   *_BCD_TO_**  functions
//********************************************************************************************
inline const CIEC_USINT BYTE_BCD_TO_USINT(const CIEC_BYTE &paVal) {
  return CIEC_USINT(static_cast<TForteUInt8>((paVal / 16 * 10) + (paVal % 16)));
}

inline const CIEC_UINT WORD_BCD_TO_UINT(const CIEC_WORD &paVal) {
  return CIEC_UINT(
    static_cast<TForteUInt16>(BYTE_BCD_TO_USINT(CIEC_BYTE(static_cast<TForteByte>(paVal >> 8))) * 100
      + BYTE_BCD_TO_USINT(CIEC_BYTE(static_cast<TForteByte>(paVal & 0xff)))));
}

inline const CIEC_UDINT DWORD_BCD_TO_UDINT(const CIEC_DWORD &paVal) {
  return CIEC_UDINT(
    static_cast<TForteUInt32>(WORD_BCD_TO_UINT(CIEC_WORD(static_cast<TForteWord>(paVal >> 16))) * 10000
      + WORD_BCD_TO_UINT(CIEC_WORD(static_cast<TForteWord>(paVal & 0xffff)))));
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT LWORD_BCD_TO_ULINT(const CIEC_LWORD &paVal){
  return CIEC_ULINT(static_cast<TForteUInt64>(DWORD_BCD_TO_UDINT(CIEC_DWORD(static_cast<TForteDWord>(paVal >> 32))) * 100000000 + DWORD_BCD_TO_UDINT(CIEC_DWORD(static_cast<TForteDWord>(paVal & 0xffffffff)))));
}
#endif

//********************************************************************************************
//   **_TO_BCD_*  functions
//********************************************************************************************
inline const CIEC_BYTE USINT_TO_BCD_BYTE(const CIEC_USINT &paVal) {
  return CIEC_BYTE(static_cast<TForteByte>(paVal / 10 * 16 + paVal % 10));
}

inline const CIEC_WORD UINT_TO_BCD_WORD(const CIEC_UINT &paVal) {
  return CIEC_WORD(
    static_cast<TForteWord>((static_cast<TForteWord>(USINT_TO_BCD_BYTE(static_cast<TForteUInt8>(paVal / 100))) << 8)
      + static_cast<TForteWord>(USINT_TO_BCD_BYTE(static_cast<TForteUInt8>(paVal % 100)))));
}

inline const CIEC_DWORD UDINT_TO_BCD_DWORD(const CIEC_UDINT &paVal) {
  return CIEC_DWORD(
    static_cast<TForteDWord>((static_cast<TForteDWord>(UINT_TO_BCD_WORD(static_cast<TForteUInt16>(paVal / 10000))) << 16)
      + static_cast<TForteDWord>(UINT_TO_BCD_WORD(static_cast<TForteUInt16>(paVal % 10000)))));
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD ULINT_TO_BCD_LWORD(const CIEC_ULINT &paVal){
  return CIEC_LWORD(static_cast<TForteLWord>((static_cast<TForteLWord>(UDINT_TO_BCD_DWORD(static_cast<TForteUInt32>(paVal / 100000000))) << 32) + static_cast<TForteLWord>(UDINT_TO_BCD_DWORD(static_cast<TForteUInt32>(paVal % 100000000)))));
}
#endif

/*********************************************************************************************
 *   Non-standard convert functions
 *********************************************************************************************/

/*********************************************************************************************
 *   TIME_IN_<RESOLUTION>_TO_* functions
 *********************************************************************************************/
#ifdef FORTE_USE_64BIT_DATATYPES

inline const CIEC_LINT TIME_IN_S_TO_LINT(const CIEC_TIME &paValue) {
  return CIEC_LINT(paValue.getInSeconds());
}

inline const CIEC_LINT TIME_IN_MS_TO_LINT(const CIEC_TIME &paValue) {
  return CIEC_LINT(paValue.getInMilliSeconds());
}

#endif /* FORTE_USE_64BIT_DATATYPES */

#endif /* CONVERT_FUNCTIONS_H_ */
