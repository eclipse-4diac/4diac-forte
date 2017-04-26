/*******************************************************************************
  * Copyright (c) 2010 - 2015 ACIN, Profactor GmbH, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl, Ingo Hegny, Monika Wenger, Carolyn Oates, Patrick Smejkal,
  *    Matthias Plasch,
  *      - initial implementation and rework communication infrastructure
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
//#include <stdio.h>
#include <math.h>

/*! \file
 * This file implements the type conversion functions as defined by IEC 61131-3 in subclause 2.5.1.5.1
 */

//********************************************************************************************
//   DT_TO_**  functions
//********************************************************************************************
#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_DATE DT_TO_DATE(const CIEC_DATE_AND_TIME &pa_roVal){
  TForteUInt64 nBuffer = pa_roVal;
  time_t t = static_cast<time_t>(nBuffer / 1000);
#if ! defined(WINCE)
  struct tm *ptm = localtime(&t);
#else
  struct tm *ptm = wceex_localtime(&t);
#endif

  if(ptm == 0)
    return CIEC_DATE(0);

  ptm->tm_hour = 0;
  ptm->tm_min = 0;
  ptm->tm_sec = 0;

#if ! defined(WINCE)
  t = mktime(ptm);
#else
  t = wceex_mktime(ptm);
#endif

  if(t == (time_t) -1)
    return CIEC_DATE(0);

  return CIEC_DATE(t * 1000ULL);
}

inline const CIEC_DATE_AND_TIME DATE_TO_DT(const CIEC_DATE &pa_roVal){
  //this should be sufficient
  return CIEC_DATE_AND_TIME((TForteUInt64) pa_roVal);
}

inline const CIEC_TIME_OF_DAY DT_TO_TOD(const CIEC_DATE_AND_TIME &pa_roVal){
  TForteUInt64 nBuffer = pa_roVal;
  time_t t = static_cast<time_t>(nBuffer / 1000);
#if ! defined(WINCE)
  struct tm *ptm = localtime(&t);
#else
  struct tm *ptm = wceex_localtime(&t);
#endif

  if(ptm == 0)
    return CIEC_TIME_OF_DAY(0);

  ptm->tm_hour = 0;
  ptm->tm_min = 0;
  ptm->tm_sec = 0;

  return CIEC_TIME_OF_DAY((TForteUInt64) (ptm->tm_hour * 3600 + ptm->tm_min * 60 + ptm->tm_sec) * 1000ULL + (nBuffer % 1000));
}
#endif

//********************************************************************************************
//   WSTRING_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_WSTRING_DATATYPE
  inline const CIEC_BOOL WSTRING_TO_BOOL(const CIEC_WSTRING &pa_roVal){
    CIEC_BOOL tempVal;
    const char *pacBuffer = pa_roVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline const CIEC_BYTE WSTRING_TO_BYTE(const CIEC_WSTRING &pa_roVal){
      CIEC_BYTE tempVal;
      const char *pacBuffer = pa_roVal.getValue();
      tempVal.fromString(pacBuffer);
      return tempVal;
  }

  inline const CIEC_DWORD WSTRING_TO_DWORD(const CIEC_WSTRING &pa_roVal){
      CIEC_DWORD tempVal;
      const char *pacBuffer = pa_roVal.getValue();
      tempVal.fromString(pacBuffer);
      return tempVal;
  }

  inline const CIEC_WORD WSTRING_TO_WORD(const CIEC_WSTRING &pa_roVal){
    CIEC_WORD tempVal;
    const char *pacBuffer = pa_roVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline const CIEC_DINT WSTRING_TO_DINT(const CIEC_WSTRING &pa_roVal){
      CIEC_DINT tempVal;
      const char *pacBuffer = pa_roVal.getValue();
      tempVal.fromString(pacBuffer);
      return tempVal;
  }

  inline const CIEC_INT WSTRING_TO_INT(const CIEC_WSTRING &pa_roVal){
      CIEC_INT tempVal;
      const char *pacBuffer = pa_roVal.getValue();
      tempVal.fromString(pacBuffer);
      return tempVal;
  }

  inline const CIEC_UINT WSTRING_TO_UINT(const CIEC_WSTRING &pa_roVal){
    CIEC_UINT tempVal;
    const char *pacBuffer = pa_roVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline const CIEC_UDINT WSTRING_TO_UDINT(const CIEC_WSTRING &pa_roVal){
    CIEC_UDINT tempVal;
    const char *pacBuffer = pa_roVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline const CIEC_USINT WSTRING_TO_USINT(const CIEC_WSTRING &pa_roVal){
    CIEC_USINT tempVal;
    const char *pacBuffer = pa_roVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline const CIEC_STRING WSTRING_TO_STRING(const CIEC_WSTRING &pa_roVal){
      CIEC_STRING tempVal;
      const char *pacBuffer = pa_roVal.getValue();
      tempVal.fromString(pacBuffer);
      return tempVal;
  }

  inline const CIEC_TIME WSTRING_TO_TIME(const CIEC_WSTRING &pa_roVal){
      CIEC_TIME tempVal;
      const char *pacBuffer = pa_roVal.getValue();
      tempVal.fromString(pacBuffer);
      return tempVal;
  }

  inline const CIEC_SINT WSTRING_TO_SINT(const CIEC_WSTRING &pa_roVal){
      CIEC_SINT tempVal;
      const char *pacBuffer = pa_roVal.getValue();
      tempVal.fromString(pacBuffer);
      return tempVal;
  }

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL WSTRING_TO_REAL(const CIEC_WSTRING &pa_roVal){
      CIEC_REAL tempVal;
      const char *pacBuffer = pa_roVal.getValue();
      tempVal.fromString(pacBuffer);
      return tempVal;
  }
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
  inline const CIEC_LREAL WSTRING_TO_LREAL(const CIEC_WSTRING &pa_roVal){
      CIEC_LREAL tempVal;
      const char *pacBuffer = pa_roVal.getValue();
      tempVal.fromString(pacBuffer);
      return tempVal;
  }
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
  inline const CIEC_LINT WSTRING_TO_LINT(const CIEC_WSTRING &pa_roVal){
      CIEC_LINT tempVal;
      const char *pacBuffer = pa_roVal.getValue();
      tempVal.fromString(pacBuffer);
      return tempVal;
  }

  inline const CIEC_LWORD WSTRING_TO_LWORD(const CIEC_WSTRING &pa_roVal){
      CIEC_LWORD tempVal;
      const char *pacBuffer = pa_roVal.getValue();
      tempVal.fromString(pacBuffer);
      return tempVal;
  }

  inline const CIEC_ULINT WSTRING_TO_ULINT(const CIEC_WSTRING &pa_roVal){
        CIEC_ULINT tempVal;
        const char *pacBuffer = pa_roVal.getValue();
        tempVal.fromString(pacBuffer);
        return tempVal;
  }
#endif
#endif

//********************************************************************************************
//   STRING_TO_*  functions
//********************************************************************************************
inline const CIEC_INT STRING_TO_INT(const CIEC_STRING &pa_roVal){
  CIEC_INT tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_UINT STRING_TO_UINT(const CIEC_STRING &pa_roVal){
  CIEC_UINT tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_USINT STRING_TO_USINT(const CIEC_STRING &pa_roVal){
  CIEC_USINT tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_SINT STRING_TO_SINT(const CIEC_STRING &pa_roVal){
  CIEC_SINT tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_DINT STRING_TO_DINT(const CIEC_STRING &pa_roVal){
  CIEC_DINT tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_UDINT STRING_TO_UDINT(const CIEC_STRING &pa_roVal){
  CIEC_UDINT tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_BOOL STRING_TO_BOOL(const CIEC_STRING &pa_roVal){
  CIEC_BOOL tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_BYTE STRING_TO_BYTE(const CIEC_STRING &pa_roVal){
  CIEC_BYTE tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_WORD STRING_TO_WORD(const CIEC_STRING &pa_roVal){
  CIEC_WORD tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_DWORD STRING_TO_DWORD(const CIEC_STRING &pa_roVal){
  CIEC_DWORD tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_TIME STRING_TO_TIME(const CIEC_STRING &pa_roVal){
  CIEC_TIME tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL STRING_TO_REAL(const CIEC_STRING &pa_roVal){
  CIEC_REAL tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL STRING_TO_LREAL(const CIEC_STRING &pa_roVal){
  CIEC_LREAL tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT STRING_TO_ULINT(const CIEC_STRING &pa_roVal){
  CIEC_ULINT tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_LINT STRING_TO_LINT(const CIEC_STRING &pa_roVal){
  CIEC_LINT tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}

inline const CIEC_LWORD STRING_TO_LWORD(const CIEC_STRING &pa_roVal){
  CIEC_LWORD tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING STRING_TO_WSTRING(const CIEC_STRING &pa_roVal){
  CIEC_WSTRING tempVal;
  const char *pacBuffer = pa_roVal.getValue();
  tempVal.fromString(pacBuffer);
  return tempVal;
}
#endif

//********************************************************************************************
//   LREAL_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_BOOL LREAL_TO_BOOL(const CIEC_LREAL &pa_roVal){
	return CIEC_BOOL((TForteDFloat)0.0 != pa_roVal ? true : false);
}

inline const CIEC_LWORD LREAL_TO_LWORD(const CIEC_LREAL &pa_roVal){
  return CIEC_LWORD( (0 < pa_roVal) ? (TForteLWord)pa_roVal : CIEC_LWORD::scm_nMinVal);
}

inline const CIEC_BYTE LREAL_TO_BYTE(const CIEC_LREAL &pa_roVal){
  TForteByte nBuf = static_cast<TForteByte>(pa_roVal);
  if(CIEC_BYTE::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_BYTE::scm_nMaxVal;
  }
  else{
    if(CIEC_BYTE::scm_nMinVal > pa_roVal){
      nBuf = CIEC_BYTE::scm_nMinVal;
    }
  }
  return CIEC_BYTE(nBuf);
}

inline const CIEC_WORD LREAL_TO_WORD(const CIEC_LREAL &pa_roVal){
  TForteWord nBuf = static_cast<TForteWord>(pa_roVal);
  if(CIEC_WORD::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_WORD::scm_nMaxVal;
  }
  else{
    if(CIEC_WORD::scm_nMinVal > pa_roVal){
      nBuf = CIEC_WORD::scm_nMinVal;
    }
  }
  return CIEC_WORD(nBuf);
}

inline const CIEC_DWORD LREAL_TO_DWORD(const CIEC_LREAL &pa_roVal){
  TForteDWord nBuf = static_cast<TForteDWord>(pa_roVal);
  if(CIEC_DWORD::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_DWORD::scm_nMaxVal;
  }
  else{
    if(CIEC_DWORD::scm_nMinVal > pa_roVal){
      nBuf = CIEC_DWORD::scm_nMinVal;
    }
  }
  return CIEC_DWORD(nBuf);
}

inline const CIEC_REAL LREAL_TO_REAL(const CIEC_LREAL &pa_roVal){
  CIEC_REAL tempVal;
  CIEC_ANY::specialCast(const_cast<CIEC_LREAL &>(pa_roVal), tempVal);
  return tempVal;
}

inline const CIEC_USINT LREAL_TO_USINT(const CIEC_LREAL &pa_roVal){
  CIEC_USINT tempVal;
  if(0 < pa_roVal){
    CIEC_ANY::specialCast(const_cast<CIEC_LREAL &>(pa_roVal), tempVal);
  }
  return tempVal;
}

inline const CIEC_UDINT LREAL_TO_UDINT(const CIEC_LREAL &pa_roVal){
  CIEC_UDINT tempVal;
  if(0 < pa_roVal){
    CIEC_ANY::specialCast(const_cast<CIEC_LREAL &>(pa_roVal), tempVal);
  }
  return tempVal;
}

inline const CIEC_UINT LREAL_TO_UINT(const CIEC_LREAL &pa_roVal){
	CIEC_UINT tempVal;
	if(0 < pa_roVal){
		CIEC_ANY::specialCast(const_cast<CIEC_LREAL &>(pa_roVal), tempVal);
	}
	return tempVal;
}

inline const CIEC_ULINT LREAL_TO_ULINT(const CIEC_LREAL &pa_roVal){
  CIEC_ULINT tempVal;
  if(0 < pa_roVal){
    CIEC_ANY::specialCast(const_cast<CIEC_LREAL &>(pa_roVal), tempVal);
  }
  return tempVal;
}

inline CIEC_ANY::TLargestIntValueType lreal_to_xINT(const CIEC_LREAL &pa_roVal){
  CIEC_ANY::TLargestIntValueType temp;
  #if defined(WIN32)
    temp = (CIEC_ANY::TLargestIntValueType)floor(static_cast<TForteDFloat>(pa_roVal) + 0.5);
  #elif defined(VXWORKS)
    temp = (CIEC_ANY::TLargestIntValueType)floor(static_cast<TForteDFloat>(pa_roVal) + 0.5);
  #else
    temp = lroundf(static_cast<TForteFloat>(pa_roVal));
  #endif
  if(static_cast<TForteDFloat>(pa_roVal) > 0){
    if(((temp % 2) != 0) && ((static_cast<TForteDFloat>(temp) - 0.5f) == static_cast<TForteDFloat>(pa_roVal))){
        temp = temp - 1;
    }
  }else{
    if(((temp % 2) != 0) && ((static_cast<TForteDFloat>(temp) + 0.5f) == static_cast<TForteDFloat>(pa_roVal))){
         temp = temp + 1;
    }
  }
  return temp;
}

inline const CIEC_LINT LREAL_TO_LINT(const CIEC_LREAL &pa_roVal){
  return CIEC_LINT( static_cast<CIEC_LINT::TValueType>(lreal_to_xINT(pa_roVal)) );
}

inline const CIEC_DINT LREAL_TO_DINT(const CIEC_LREAL &pa_roVal){
  return CIEC_DINT( static_cast<CIEC_DINT::TValueType>(lreal_to_xINT(pa_roVal)) );
}

inline const CIEC_SINT LREAL_TO_SINT(const CIEC_LREAL &pa_roVal){
  return CIEC_SINT( static_cast<CIEC_SINT::TValueType>(lreal_to_xINT(pa_roVal)) );
}

inline const CIEC_INT LREAL_TO_INT(const CIEC_LREAL &pa_roVal){
  return CIEC_INT( static_cast<CIEC_INT::TValueType>(lreal_to_xINT(pa_roVal)));
}

inline void stringConverter(CIEC_ANY_STRING &string, const CIEC_ANY &pa_roVal){
  TForteUInt16 bufferSize = pa_roVal.csm_aStringBufferSize[pa_roVal.getDataTypeID()];
  string.reserve(static_cast<TForteUInt16>(bufferSize));
  char *pacBuffer = string.getValue();
  int nWrittenBytes = pa_roVal.toString(pacBuffer, bufferSize);
  nWrittenBytes = nWrittenBytes > -1 ? nWrittenBytes : 0;
  string.assign(pacBuffer, static_cast<TForteUInt16>(nWrittenBytes));
}

inline const CIEC_STRING LREAL_TO_STRING(const CIEC_LREAL &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING LREAL_TO_WSTRING(const CIEC_LREAL &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME LREAL_TO_TIME(const CIEC_LREAL &pa_roVal){
  if(pa_roVal > 0){
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal * FORTE_TIME_BASE_UNITS_PER_SECOND));
  }else{
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));
  }
}
#endif

//********************************************************************************************
//   REAL_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_REAL_DATATYPE
inline CIEC_ANY::TLargestIntValueType real_to_xINT(const CIEC_REAL &pa_roVal){
  CIEC_ANY::TLargestIntValueType temp;
  #if defined(WIN32)
    temp = (CIEC_ANY::TLargestIntValueType)floor(static_cast<TForteFloat>(pa_roVal) + 0.5);
  #elif defined(VXWORKS)
    temp = (CIEC_ANY::TLargestIntValueType)floor(static_cast<TForteFloat>(pa_roVal) + 0.5);
  #else
    temp = lroundf(static_cast<TForteFloat>(pa_roVal));
  #endif
  if(static_cast<TForteFloat>(pa_roVal) > 0){
    if(((temp % 2) != 0) && ((static_cast<TForteFloat>(temp) - 0.5f) == static_cast<TForteFloat>(pa_roVal))){
        temp = temp - 1;
    }
  }else{
    if(((temp % 2) != 0) && ((static_cast<TForteFloat>(temp) + 0.5f) == static_cast<TForteFloat>(pa_roVal))){
         temp = temp + 1;
    }
  }
  return temp;
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT REAL_TO_LINT(const CIEC_REAL &pa_roVal){
  return CIEC_LINT( static_cast<CIEC_LINT::TValueType>(real_to_xINT(pa_roVal)) );
}

inline const CIEC_LWORD REAL_TO_LWORD(const CIEC_REAL &pa_roVal){
  return CIEC_LWORD( (0 < pa_roVal) ? (TForteLWord)pa_roVal : CIEC_LWORD::scm_nMinVal);
}

inline const CIEC_ULINT REAL_TO_ULINT(const CIEC_REAL &pa_roVal){
  CIEC_ULINT tempVal;
  if(0 < pa_roVal){
    CIEC_ANY::specialCast(const_cast<CIEC_REAL &>(pa_roVal), tempVal);
  }
  return tempVal;
}

inline const CIEC_LREAL REAL_TO_LREAL(const CIEC_REAL &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

inline const CIEC_DINT REAL_TO_DINT(const CIEC_REAL &pa_roVal){
  return CIEC_DINT( static_cast<CIEC_DINT::TValueType>(real_to_xINT(pa_roVal)) );
}

inline const CIEC_SINT REAL_TO_SINT(const CIEC_REAL &pa_roVal){
  return CIEC_SINT( static_cast<CIEC_SINT::TValueType>(real_to_xINT(pa_roVal)) );
}

inline const CIEC_INT REAL_TO_INT(const CIEC_REAL &pa_roVal){
  return CIEC_INT( static_cast<CIEC_INT::TValueType>(real_to_xINT(pa_roVal)));
}

inline const CIEC_USINT REAL_TO_USINT(const CIEC_REAL &pa_roVal){
  CIEC_USINT tempVal;
  if(0 < pa_roVal){
    CIEC_ANY::specialCast(const_cast<CIEC_REAL &>(pa_roVal), tempVal);
  }
  return tempVal;
}

inline const CIEC_UINT REAL_TO_UINT(const CIEC_REAL &pa_roVal){
  CIEC_UINT tempVal;
  if(0 < pa_roVal){
    CIEC_ANY::specialCast(const_cast<CIEC_REAL &>(pa_roVal), tempVal);
  }
  return tempVal;
}

inline const CIEC_UDINT REAL_TO_UDINT(const CIEC_REAL &pa_roVal){
  CIEC_UDINT tempVal;
  if(0 < pa_roVal){
    CIEC_ANY::specialCast(const_cast<CIEC_REAL &>(pa_roVal), tempVal);
  }
  return tempVal;
}

inline const CIEC_STRING REAL_TO_STRING(const CIEC_REAL &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING REAL_TO_WSTRING(const CIEC_REAL &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME REAL_TO_TIME(const CIEC_REAL &pa_roVal){
  if(pa_roVal > 0){
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal * FORTE_TIME_BASE_UNITS_PER_SECOND));
  }else{
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));
  }
}

inline const CIEC_BOOL REAL_TO_BOOL(const CIEC_REAL &pa_roVal){
  return CIEC_BOOL((TForteFloat)0.0 != pa_roVal ? true : false);
}

inline const CIEC_BYTE REAL_TO_BYTE(const CIEC_REAL &pa_roVal){
  TForteByte nBuf = static_cast<TForteByte>(pa_roVal);
  if(CIEC_BYTE::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_BYTE::scm_nMaxVal;
  }
  else{
    if(CIEC_BYTE::scm_nMinVal > pa_roVal){
      nBuf = CIEC_BYTE::scm_nMinVal;
    }
  }
  return CIEC_BYTE(nBuf);
}

inline const CIEC_WORD REAL_TO_WORD(const CIEC_REAL &pa_roVal){
  TForteWord nBuf = static_cast<TForteWord>(pa_roVal);
  if(CIEC_WORD::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_WORD::scm_nMaxVal;
  }
  else{
    if(CIEC_WORD::scm_nMinVal > pa_roVal){
      nBuf = CIEC_WORD::scm_nMinVal;
    }
  }
  return CIEC_WORD(nBuf);
}

inline const CIEC_DWORD REAL_TO_DWORD(const CIEC_REAL &pa_roVal){
  TForteDWord nBuf = static_cast<TForteDWord>(pa_roVal);
  if(CIEC_DWORD::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_DWORD::scm_nMaxVal;
  }
  else{
    if(CIEC_DWORD::scm_nMinVal > pa_roVal){
      nBuf = CIEC_DWORD::scm_nMinVal;
    }
  }
  return CIEC_DWORD(nBuf);
}
#endif

//********************************************************************************************
//   TIME_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_BYTE TIME_TO_BYTE(const CIEC_TIME &pa_roVal){
  return CIEC_BYTE(static_cast<TForteByte>(static_cast<TForteDFloat>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_WORD TIME_TO_WORD(const CIEC_TIME &pa_roVal){
  return CIEC_WORD(static_cast<TForteUInt16>(static_cast<TForteDFloat>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_DWORD TIME_TO_DWORD(const CIEC_TIME &pa_roVal){
  return CIEC_DWORD(static_cast<TForteUInt32>(static_cast<TForteDFloat>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_UDINT TIME_TO_UDINT(const CIEC_TIME &pa_roVal){
  return CIEC_UDINT(static_cast<TForteUInt32>(static_cast<TForteDFloat>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_UINT TIME_TO_UINT(const CIEC_TIME &pa_roVal){
  return CIEC_UINT(static_cast<TForteUInt16>(static_cast<TForteDFloat>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_SINT TIME_TO_SINT(const CIEC_TIME &pa_roVal){
  return CIEC_SINT(static_cast<TForteInt8>(static_cast<TForteDFloat>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_USINT TIME_TO_USINT(const CIEC_TIME &pa_roVal){
  return CIEC_USINT(static_cast<TForteUInt8>(static_cast<TForteDFloat>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_DINT TIME_TO_DINT(const CIEC_TIME &pa_roVal){
  return CIEC_DINT(static_cast<TForteInt32>(static_cast<TForteDFloat>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_INT TIME_TO_INT(const CIEC_TIME &pa_roVal){
  return CIEC_INT(static_cast<TForteInt16>(static_cast<TForteDFloat>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}
#else
inline const CIEC_BYTE TIME_TO_BYTE(const CIEC_TIME &pa_roVal){
  return CIEC_BYTE(static_cast<TForteByte>(static_cast<TForteFloat>(pa_roVal.operator TForteInt32()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_WORD TIME_TO_WORD(const CIEC_TIME &pa_roVal){
  return CIEC_WORD(static_cast<TForteUInt16>(static_cast<TForteFloat>(pa_roVal.operator TForteInt32()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_DWORD TIME_TO_DWORD(const CIEC_TIME &pa_roVal){
  return CIEC_DWORD(static_cast<TForteUInt32>(static_cast<TForteFloat>(pa_roVal.operator TForteInt32()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_UDINT TIME_TO_UDINT(const CIEC_TIME &pa_roVal){
  return CIEC_UDINT(static_cast<TForteUInt32>(static_cast<TForteFloat>(pa_roVal.operator TForteInt32()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_UINT TIME_TO_UINT(const CIEC_TIME &pa_roVal){
  return CIEC_UINT(static_cast<TForteUInt16>(static_cast<TForteFloat>(pa_roVal.operator TForteInt32()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_SINT TIME_TO_SINT(const CIEC_TIME &pa_roVal){
  return CIEC_SINT(static_cast<TForteInt8>(static_cast<TForteFloat>(pa_roVal.operator TForteInt32()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_USINT TIME_TO_USINT(const CIEC_TIME &pa_roVal){
  return CIEC_USINT(static_cast<TForteUInt8>(static_cast<TForteFloat>(pa_roVal.operator TForteInt32()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_DINT TIME_TO_DINT(const CIEC_TIME &pa_roVal){
  return CIEC_DINT(static_cast<TForteInt32>(static_cast<TForteFloat>(pa_roVal.operator TForteInt32()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}

inline const CIEC_INT TIME_TO_INT(const CIEC_TIME &pa_roVal){
  return CIEC_INT(static_cast<TForteInt16>(static_cast<TForteFloat>(pa_roVal.operator TForteInt32()) /FORTE_TIME_BASE_UNITS_PER_SECOND));
}
#endif

inline const CIEC_BOOL TIME_TO_BOOL(const CIEC_TIME &pa_roVal){
  return CIEC_BOOL(0 != pa_roVal.getInMiliSeconds() ? true : false);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD TIME_TO_LWORD(const CIEC_TIME &pa_roVal){
  return CIEC_LWORD(static_cast<TForteLWord>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND);
}

inline const CIEC_LINT TIME_TO_LINT(const CIEC_TIME &pa_roVal){
  return CIEC_LINT(static_cast<TForteInt64>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND);
}

inline const CIEC_ULINT TIME_TO_ULINT(const CIEC_TIME &pa_roVal){
  return CIEC_ULINT(static_cast<TForteUInt64>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL TIME_TO_REAL(const CIEC_TIME &pa_roVal){
#ifdef FORTE_USE_64BIT_DATATYPES
  return CIEC_REAL(static_cast<TForteFloat>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND);
#else
  return CIEC_REAL(static_cast<TForteFloat>(pa_roVal.operator TForteInt32()) /FORTE_TIME_BASE_UNITS_PER_SECOND);
#endif
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL TIME_TO_LREAL(const CIEC_TIME &pa_roVal){
#ifdef FORTE_USE_64BIT_DATATYPES
  return CIEC_LREAL(static_cast<TForteDFloat>(pa_roVal.operator TForteInt64()) /FORTE_TIME_BASE_UNITS_PER_SECOND);
#else
  return CIEC_LREAL(static_cast<TForteDFloat>(pa_roVal.operator TForteInt32()) /FORTE_TIME_BASE_UNITS_PER_SECOND);
#endif
}
#endif

inline const CIEC_STRING TIME_TO_STRING(const CIEC_TIME &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING TIME_TO_WSTRING(const CIEC_TIME &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

//********************************************************************************************
//   BOOL_TO_*  functions
//********************************************************************************************
inline const CIEC_BYTE BOOL_TO_BYTE(const CIEC_BOOL &pa_roVal){
  return CIEC_BYTE((TForteByte)((true == pa_roVal) ? 1 : 0));
}

inline const CIEC_DINT BOOL_TO_DINT(const CIEC_BOOL &pa_roVal){
  return CIEC_DINT((TForteInt32)((true == pa_roVal) ? 1 : 0));
}

inline const CIEC_DWORD BOOL_TO_DWORD(const CIEC_BOOL &pa_roVal){
  return CIEC_DWORD((TForteDWord)((true == pa_roVal) ? 1 : 0));
}

inline const CIEC_INT BOOL_TO_INT(const CIEC_BOOL &pa_roVal){
  return CIEC_INT((TForteInt16)((true == pa_roVal) ? 1 : 0));
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT BOOL_TO_LINT(const CIEC_BOOL &pa_roVal){
  return CIEC_LINT((TForteInt64) (true == pa_roVal) ? 1 : 0);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL BOOL_TO_LREAL(const CIEC_BOOL &pa_roVal){
  return CIEC_LREAL((TForteDFloat) (true == pa_roVal) ? 1.0 : 0.0);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD BOOL_TO_LWORD(const CIEC_BOOL &pa_roVal){
  return CIEC_LWORD((TForteLWord) (true == pa_roVal) ? 1 : 0);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL BOOL_TO_REAL(const CIEC_BOOL &pa_roVal){
  return CIEC_REAL((TForteFloat) (true == pa_roVal) ? 1.0f : 0.0f);
}
#endif

inline const CIEC_SINT BOOL_TO_SINT(const CIEC_BOOL &pa_roVal){
  return CIEC_SINT((TForteInt8)((true == pa_roVal) ? 1 : 0));
}

inline const CIEC_UDINT BOOL_TO_UDINT(const CIEC_BOOL &pa_roVal){
  return CIEC_UDINT((TForteUInt32) (true == pa_roVal) ? 1 : 0);
}

inline const CIEC_UINT BOOL_TO_UINT(const CIEC_BOOL &pa_roVal){
  return CIEC_UINT((TForteUInt16)((true == pa_roVal) ? 1 : 0));
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT BOOL_TO_ULINT(const CIEC_BOOL &pa_roVal){
  return CIEC_ULINT((TForteUInt64) (true == pa_roVal) ? 1 : 0);
}
#endif

inline const CIEC_USINT BOOL_TO_USINT(const CIEC_BOOL &pa_roVal){
  return CIEC_USINT((TForteUInt8)( (true == pa_roVal) ? 1 : 0));
}

inline const CIEC_WORD BOOL_TO_WORD(const CIEC_BOOL &pa_roVal){
  return CIEC_WORD((TForteWord)((true == pa_roVal) ? 1 : 0));
}

inline const CIEC_STRING BOOL_TO_STRING(const CIEC_BOOL &pa_roVal){
  CIEC_STRING string;
  stringConverter(string, pa_roVal);
  return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING BOOL_TO_WSTRING(const CIEC_BOOL &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME BOOL_TO_TIME(const CIEC_BOOL &pa_roVal){
  return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(true == pa_roVal ? FORTE_TIME_BASE_UNITS_PER_SECOND : 0));
}

//********************************************************************************************
//   BYTE_TO_*  functions
//********************************************************************************************
inline const CIEC_BOOL BYTE_TO_BOOL(const CIEC_BYTE &pa_roVal){
  return CIEC_BOOL((TForteByte)0 != pa_roVal ? true : false);
}

inline const CIEC_DINT BYTE_TO_DINT(const CIEC_BYTE &pa_roVal){
  return CIEC_DINT((TForteInt32) pa_roVal);
}

inline const CIEC_DWORD BYTE_TO_DWORD(const CIEC_BYTE &pa_roVal){
  return CIEC_DWORD((TForteDWord) pa_roVal);
}

inline const CIEC_INT BYTE_TO_INT(const CIEC_BYTE &pa_roVal){
  return CIEC_INT((TForteInt16) pa_roVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT BYTE_TO_LINT(const CIEC_BYTE &pa_roVal){
  return CIEC_LINT((TForteInt64) pa_roVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL BYTE_TO_LREAL(const CIEC_BYTE &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD BYTE_TO_LWORD(const CIEC_BYTE &pa_roVal){
  return CIEC_LWORD((TForteLWord) pa_roVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL BYTE_TO_REAL(const CIEC_BYTE &pa_roVal){
  return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_SINT BYTE_TO_SINT(const CIEC_BYTE &pa_roVal){
  return CIEC_SINT((TForteInt8) (CIEC_SINT::scm_nMaxVal >= pa_roVal) ? pa_roVal : CIEC_SINT::scm_nMaxVal);
}

inline const CIEC_UDINT BYTE_TO_UDINT(const CIEC_BYTE &pa_roVal){
  return CIEC_UDINT((TForteUInt32) pa_roVal);
}

inline const CIEC_UINT BYTE_TO_UINT(const CIEC_BYTE &pa_roVal){
  return CIEC_UINT((TForteUInt16) pa_roVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT BYTE_TO_ULINT(const CIEC_BYTE &pa_roVal){
  return CIEC_ULINT((TForteUInt64) pa_roVal);
}
#endif

inline const CIEC_USINT BYTE_TO_USINT(const CIEC_BYTE &pa_roVal){
  return CIEC_USINT((TForteUInt8) pa_roVal);
}

inline const CIEC_WORD BYTE_TO_WORD(const CIEC_BYTE &pa_roVal){
  return CIEC_WORD((TForteWord) pa_roVal);
}

inline const CIEC_STRING BYTE_TO_STRING(const CIEC_BYTE &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING BYTE_TO_WSTRING(const CIEC_BYTE &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME BYTE_TO_TIME(const CIEC_BYTE &pa_roVal){
  return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal) * FORTE_TIME_BASE_UNITS_PER_SECOND);
}

//********************************************************************************************
//   DINT_TO_*  functions
//********************************************************************************************
inline const CIEC_BOOL DINT_TO_BOOL(const CIEC_DINT &pa_roVal){
  return CIEC_BOOL((TForteInt32)0 != pa_roVal ? true : false);
}

inline const CIEC_BYTE DINT_TO_BYTE(const CIEC_DINT &pa_roVal){
  TForteByte nBuf = static_cast<TForteByte>(pa_roVal);
  if(CIEC_BYTE::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_BYTE::scm_nMaxVal;
  }
  else{
    if(CIEC_BYTE::scm_nMinVal > pa_roVal){
      nBuf = CIEC_BYTE::scm_nMinVal;
    }
  }
  return CIEC_BYTE(nBuf);
}

inline const CIEC_DWORD DINT_TO_DWORD(const CIEC_DINT &pa_roVal){
  return CIEC_DWORD( ((0 < pa_roVal) ? (TForteDWord)pa_roVal : CIEC_DWORD::scm_nMinVal));
}

inline const CIEC_INT DINT_TO_INT(const CIEC_DINT &pa_roVal){
  TForteInt16 nBuf = static_cast<TForteInt16>(pa_roVal);
  if(CIEC_INT::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_INT::scm_nMaxVal;
  }
  else{
    if(CIEC_INT::scm_nMinVal > pa_roVal){
      nBuf = CIEC_INT::scm_nMinVal;
    }
  }
  return CIEC_INT(nBuf);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT DINT_TO_LINT(const CIEC_DINT &pa_roVal){
  return CIEC_LINT((TForteInt64) pa_roVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL DINT_TO_LREAL(const CIEC_DINT &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD DINT_TO_LWORD(const CIEC_DINT &pa_roVal){
  return CIEC_LWORD( (0 < pa_roVal) ? (TForteLWord)pa_roVal : CIEC_LWORD::scm_nMinVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL DINT_TO_REAL(const CIEC_DINT &pa_roVal){
  return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_SINT DINT_TO_SINT(const CIEC_DINT &pa_roVal){
  TForteInt8 nBuf = static_cast<TForteInt8>(pa_roVal);
  if(CIEC_SINT::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_SINT::scm_nMaxVal;
  }
  else{
    if(CIEC_SINT::scm_nMinVal > pa_roVal){
      nBuf = CIEC_SINT::scm_nMinVal;
    }
  }
  return CIEC_SINT(nBuf);
}

inline const CIEC_UDINT DINT_TO_UDINT(const CIEC_DINT &pa_roVal){
  return CIEC_UDINT( (((CIEC_DINT::TValueType)CIEC_UDINT::scm_nMinVal) < pa_roVal) ? (CIEC_UDINT::TValueType)pa_roVal : CIEC_UDINT::scm_nMinVal);
}

inline const CIEC_UINT DINT_TO_UINT(const CIEC_DINT &pa_roVal){
  TForteUInt16 nBuf = static_cast<TForteUInt16>(pa_roVal);
  if(CIEC_UINT::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_UINT::scm_nMaxVal;
  }
  else{
    if(CIEC_UINT::scm_nMinVal > pa_roVal){
      nBuf = CIEC_UINT::scm_nMinVal;
    }
  }
  return CIEC_UINT(nBuf);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT DINT_TO_ULINT(const CIEC_DINT &pa_roVal){
  return CIEC_ULINT( (0 < pa_roVal) ? (TForteUInt64)pa_roVal : CIEC_ULINT::scm_nMinVal);
}
#endif

inline const CIEC_USINT DINT_TO_USINT(const CIEC_DINT &pa_roVal){
  TForteUInt8 nBuf = static_cast<TForteUInt8>(pa_roVal);
  if(CIEC_USINT::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_USINT::scm_nMaxVal;
  }
  else{
    if(CIEC_USINT::scm_nMinVal > pa_roVal){
      nBuf = CIEC_USINT::scm_nMinVal;
    }
  }
  return CIEC_USINT(nBuf);
}

inline const CIEC_WORD DINT_TO_WORD(const CIEC_DINT &pa_roVal){
  TForteWord nBuf = static_cast<TForteWord>(pa_roVal);
  if(CIEC_WORD::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_WORD::scm_nMaxVal;
  }
  else{
    if(CIEC_WORD::scm_nMinVal > pa_roVal){
      nBuf = CIEC_WORD::scm_nMinVal;
    }
  }
  return CIEC_WORD(nBuf);
}

inline const CIEC_STRING DINT_TO_STRING(const CIEC_DINT &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING DINT_TO_WSTRING(const CIEC_DINT &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME DINT_TO_TIME(const CIEC_DINT &pa_roVal){
  if(pa_roVal > 0){
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal) * FORTE_TIME_BASE_UNITS_PER_SECOND);
  }else{
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));
  }
}

//********************************************************************************************
//   DWORD_TO_*  functions
//********************************************************************************************
inline const CIEC_BOOL DWORD_TO_BOOL(const CIEC_DWORD &pa_roVal){
  return CIEC_BOOL((TForteDWord)0 != pa_roVal ? true : false);
}

inline const CIEC_DINT DWORD_TO_DINT(const CIEC_DWORD &pa_roVal){
  return CIEC_DINT((TForteInt32) static_cast<TForteInt32>(pa_roVal));
}

inline const CIEC_BYTE DWORD_TO_BYTE(const CIEC_DWORD &pa_roVal){
  return CIEC_BYTE((TForteByte) (static_cast<TForteDWord>(CIEC_BYTE::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteByte>(pa_roVal) : CIEC_BYTE::scm_nMaxVal);
}

inline const CIEC_INT DWORD_TO_INT(const CIEC_DWORD &pa_roVal){
  return CIEC_INT((TForteInt16) (static_cast<TForteDWord>(CIEC_INT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteInt16>(pa_roVal) : CIEC_INT::scm_nMaxVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT DWORD_TO_LINT(const CIEC_DWORD &pa_roVal){
  return CIEC_LINT((TForteInt64) pa_roVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL DWORD_TO_LREAL(const CIEC_DWORD &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD DWORD_TO_LWORD(const CIEC_DWORD &pa_roVal){
  return CIEC_LWORD((TForteLWord) pa_roVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL DWORD_TO_REAL(const CIEC_DWORD &pa_roVal){
  //fprintf(stderr,"DWORD_TO_REAL: %ld, %f\n", pa_roVal.operator TForteDWord(), (TForteFloat) pa_roVal);
    return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_SINT DWORD_TO_SINT(const CIEC_DWORD &pa_roVal){
  return CIEC_SINT((TForteInt8) (static_cast<TForteDWord>(CIEC_SINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteInt8>(pa_roVal) : CIEC_SINT::scm_nMaxVal);
}

inline const CIEC_UDINT DWORD_TO_UDINT(const CIEC_DWORD &pa_roVal){
  return CIEC_UDINT((TForteUInt32) pa_roVal);
}

inline const CIEC_UINT DWORD_TO_UINT(const CIEC_DWORD &pa_roVal){
  return CIEC_UINT((TForteUInt16) (static_cast<TForteDWord>(CIEC_UINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteUInt16>(pa_roVal) : CIEC_UINT::scm_nMaxVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT DWORD_TO_ULINT(const CIEC_DWORD &pa_roVal){
  return CIEC_ULINT((TForteUInt64) pa_roVal);
}
#endif

inline const CIEC_USINT DWORD_TO_USINT(const CIEC_DWORD &pa_roVal){
  return CIEC_USINT((TForteUInt8) (static_cast<TForteDWord>(CIEC_USINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteUInt8>(pa_roVal) : CIEC_USINT::scm_nMaxVal);
}

inline const CIEC_WORD DWORD_TO_WORD(const CIEC_DWORD &pa_roVal){
  return CIEC_WORD((TForteWord) (static_cast<TForteDWord>(CIEC_WORD::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteWord>(pa_roVal) : CIEC_WORD::scm_nMaxVal);
}

inline const CIEC_TIME DWORD_TO_TIME(const CIEC_DWORD &pa_roVal){
  return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal)* FORTE_TIME_BASE_UNITS_PER_SECOND);
}

inline const CIEC_STRING DWORD_TO_STRING(const CIEC_DWORD &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING DWORD_TO_WSTRING(const CIEC_DWORD &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

//********************************************************************************************
//   LWORD_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_64BIT_DATATYPES

inline const CIEC_BOOL LWORD_TO_BOOL(const CIEC_LWORD &pa_roVal){
  return CIEC_BOOL((TForteLWord)0 != pa_roVal ? true : false);
}

inline const CIEC_DINT LWORD_TO_DINT(const CIEC_LWORD &pa_roVal){
  return CIEC_DINT((TForteInt32) (static_cast<TForteLWord>(CIEC_DINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteInt32>(pa_roVal) : CIEC_DINT::scm_nMaxVal);
}

inline const CIEC_BYTE LWORD_TO_BYTE(const CIEC_LWORD &pa_roVal){
  return CIEC_BYTE((TForteByte) (static_cast<TForteLWord>(CIEC_BYTE::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteByte>(pa_roVal) : CIEC_BYTE::scm_nMaxVal);
}

inline const CIEC_INT LWORD_TO_INT(const CIEC_LWORD &pa_roVal){
  return CIEC_INT((TForteInt16) (static_cast<TForteLWord>(CIEC_INT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteInt16>(pa_roVal) : CIEC_INT::scm_nMaxVal);
}

inline const CIEC_LINT LWORD_TO_LINT(const CIEC_LWORD &pa_roVal){
  return CIEC_LINT((TForteInt64) pa_roVal);
}


#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL LWORD_TO_LREAL(const CIEC_LWORD &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

inline const CIEC_DWORD LWORD_TO_DWORD(const CIEC_LWORD &pa_roVal){
  return CIEC_DWORD((TForteDWord) (static_cast<TForteLWord>(CIEC_DWORD::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteDWord>(pa_roVal) : CIEC_DWORD::scm_nMaxVal);
}

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL LWORD_TO_REAL(const CIEC_LWORD &pa_roVal){
  return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_SINT LWORD_TO_SINT(const CIEC_LWORD &pa_roVal){
  return CIEC_SINT((TForteInt8) (static_cast<TForteLWord>(CIEC_SINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteInt8>(pa_roVal) : CIEC_SINT::scm_nMaxVal);
}

inline const CIEC_UDINT LWORD_TO_UDINT(const CIEC_LWORD &pa_roVal){
  return CIEC_UDINT((TForteUInt32) (static_cast<TForteLWord>(CIEC_UDINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteUInt32>(pa_roVal) : CIEC_UDINT::scm_nMaxVal);
}

inline const CIEC_UINT LWORD_TO_UINT(const CIEC_LWORD &pa_roVal){
  return CIEC_UINT((TForteUInt16) (static_cast<TForteLWord>(CIEC_UINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteUInt16>(pa_roVal) : CIEC_UINT::scm_nMaxVal);
}

inline const CIEC_ULINT LWORD_TO_ULINT(const CIEC_LWORD &pa_roVal){
  return CIEC_ULINT((TForteUInt64) pa_roVal);
}

inline const CIEC_USINT LWORD_TO_USINT(const CIEC_LWORD &pa_roVal){
  return CIEC_USINT((TForteUInt8) (static_cast<TForteLWord>(CIEC_USINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteUInt8>(pa_roVal) : CIEC_USINT::scm_nMaxVal);
}

inline const CIEC_WORD LWORD_TO_WORD(const CIEC_LWORD &pa_roVal){
  return CIEC_WORD((TForteWord) (static_cast<TForteLWord>(CIEC_WORD::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteWord>(pa_roVal) : CIEC_WORD::scm_nMaxVal);
}

inline const CIEC_STRING LWORD_TO_STRING(const CIEC_LWORD &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING LWORD_TO_WSTRING(const CIEC_LWORD &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME LWORD_TO_TIME(const CIEC_LWORD &pa_roVal){
  return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal) * FORTE_TIME_BASE_UNITS_PER_SECOND);
}
#endif

//********************************************************************************************
//   UDINT_TO_*  functions
//********************************************************************************************
inline const CIEC_BOOL UDINT_TO_BOOL(const CIEC_UDINT &pa_roVal){
	return CIEC_BOOL((TForteUInt32)0 != pa_roVal ? true : false);
}

inline const CIEC_DINT UDINT_TO_DINT(const CIEC_UDINT &pa_roVal){
  return CIEC_DINT((TForteInt32) (static_cast<TForteUInt32>(CIEC_DINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteInt32>(pa_roVal) : CIEC_DINT::scm_nMaxVal);
}

inline const CIEC_BYTE UDINT_TO_BYTE(const CIEC_UDINT &pa_roVal){
  return CIEC_BYTE((TForteByte) (static_cast<TForteUInt32>(CIEC_BYTE::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteByte>(pa_roVal) : CIEC_BYTE::scm_nMaxVal);
}

inline const CIEC_INT UDINT_TO_INT(const CIEC_UDINT &pa_roVal){
  return CIEC_INT((TForteInt16) (static_cast<TForteUInt32>(CIEC_INT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteInt16>(pa_roVal) : CIEC_INT::scm_nMaxVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT UDINT_TO_LINT(const CIEC_UDINT &pa_roVal){
  return CIEC_LINT((TForteInt64) pa_roVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL UDINT_TO_LREAL(const CIEC_UDINT &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD UDINT_TO_LWORD(const CIEC_UDINT &pa_roVal){
  return CIEC_LWORD((TForteLWord) pa_roVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL UDINT_TO_REAL(const CIEC_UDINT &pa_roVal){
  return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_SINT UDINT_TO_SINT(const CIEC_UDINT &pa_roVal){
  return CIEC_SINT((TForteInt8) (static_cast<TForteUInt32>(CIEC_SINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteInt8>(pa_roVal) : CIEC_SINT::scm_nMaxVal);
}

inline const CIEC_UDINT UDINT_TO_DWORD(const CIEC_UDINT &pa_roVal){
  return CIEC_UDINT((TForteDWord) pa_roVal);
}

inline const CIEC_UINT UDINT_TO_UINT(const CIEC_UDINT &pa_roVal){
  return CIEC_UINT((TForteUInt16) (static_cast<TForteUInt32>(CIEC_UINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteUInt16>(pa_roVal) : CIEC_UINT::scm_nMaxVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT UDINT_TO_ULINT(const CIEC_UDINT &pa_roVal){
  return CIEC_ULINT((TForteUInt64) pa_roVal);
}
#endif

inline const CIEC_USINT UDINT_TO_USINT(const CIEC_UDINT &pa_roVal){
  return CIEC_USINT((TForteUInt8) (static_cast<TForteUInt32>(CIEC_USINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteUInt8>(pa_roVal) : CIEC_USINT::scm_nMaxVal);
}

inline const CIEC_WORD UDINT_TO_WORD(const CIEC_UDINT &pa_roVal){
  return CIEC_WORD((TForteWord) (static_cast<TForteUInt32>(CIEC_WORD::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteWord>(pa_roVal) : CIEC_WORD::scm_nMaxVal);
}

inline const CIEC_STRING UDINT_TO_STRING(const CIEC_UDINT &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING UDINT_TO_WSTRING(const CIEC_UDINT &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME UDINT_TO_TIME(const CIEC_UDINT &pa_roVal){
  return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal) * FORTE_TIME_BASE_UNITS_PER_SECOND);
}

//********************************************************************************************
//   UINT_TO_*  functions
//********************************************************************************************
inline const CIEC_BOOL UINT_TO_BOOL(const CIEC_UINT &pa_roVal){
	return CIEC_BOOL((TForteUInt16)0 != pa_roVal ? true : false);
}

inline const CIEC_DINT UINT_TO_DINT(const CIEC_UINT &pa_roVal){
  return CIEC_DINT((TForteInt32) pa_roVal);
}

inline const CIEC_INT UINT_TO_INT(const CIEC_UINT &pa_roVal){
  return CIEC_INT((TForteInt16) (CIEC_INT::scm_nMaxVal >= pa_roVal) ? static_cast<TForteInt16>(pa_roVal) : CIEC_INT::scm_nMaxVal);
}

inline const CIEC_BYTE UINT_TO_BYTE(const CIEC_UINT &pa_roVal){
  return CIEC_BYTE((TForteByte) (CIEC_BYTE::scm_nMaxVal >= pa_roVal) ? static_cast<TForteByte>(pa_roVal) : CIEC_BYTE::scm_nMaxVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT UINT_TO_LINT(const CIEC_UINT &pa_roVal){
  return CIEC_LINT((TForteInt64) pa_roVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL UINT_TO_LREAL(const CIEC_UINT &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD UINT_TO_LWORD(const CIEC_UINT &pa_roVal){
  return CIEC_LWORD((TForteLWord) pa_roVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL UINT_TO_REAL(const CIEC_UINT &pa_roVal){
  return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_SINT UINT_TO_SINT(const CIEC_UINT &pa_roVal){
  return CIEC_SINT((TForteInt8) (CIEC_SINT::scm_nMaxVal >= pa_roVal) ? static_cast<TForteInt8>(pa_roVal) : CIEC_SINT::scm_nMaxVal);
}

inline const CIEC_DWORD UINT_TO_DWORD(const CIEC_UINT &pa_roVal){
  return CIEC_DWORD((TForteDWord) pa_roVal);
}

inline const CIEC_UINT UINT_TO_UDINT(const CIEC_UINT &pa_roVal){
  return CIEC_UINT((TForteUInt32) pa_roVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT UINT_TO_ULINT(const CIEC_UINT &pa_roVal){
  return CIEC_ULINT((TForteUInt64) pa_roVal);
}
#endif

inline const CIEC_USINT UINT_TO_USINT(const CIEC_UINT &pa_roVal){
  return CIEC_USINT((TForteUInt8) (CIEC_USINT::scm_nMaxVal >= pa_roVal) ? static_cast<TForteUInt8>(pa_roVal) : CIEC_USINT::scm_nMaxVal);
}

inline const CIEC_WORD UINT_TO_WORD(const CIEC_UINT &pa_roVal){
  return CIEC_WORD((TForteWord) pa_roVal);
}

inline const CIEC_STRING UINT_TO_STRING(const CIEC_UINT &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING UINT_TO_WSTRING(const CIEC_UINT &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME UINT_TO_TIME(const CIEC_UINT &pa_roVal){
  return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal) * FORTE_TIME_BASE_UNITS_PER_SECOND);
}

//********************************************************************************************
//   ULINT_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_BOOL ULINT_TO_BOOL(const CIEC_ULINT &pa_roVal){
	return CIEC_BOOL((TForteUInt64)0 != pa_roVal ? true : false);
}

inline const CIEC_DINT ULINT_TO_DINT(const CIEC_ULINT &pa_roVal){
  return CIEC_DINT((TForteInt32) (static_cast<TForteUInt64>(CIEC_DINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteInt32>(pa_roVal) : CIEC_DINT::scm_nMaxVal);
}

inline const CIEC_BYTE ULINT_TO_BYTE(const CIEC_ULINT &pa_roVal){
  return CIEC_BYTE((TForteByte) (static_cast<TForteUInt64>(CIEC_BYTE::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteByte>(pa_roVal) : CIEC_BYTE::scm_nMaxVal);
}

inline const CIEC_INT ULINT_TO_INT(const CIEC_ULINT &pa_roVal){
  return CIEC_INT((TForteInt16) (static_cast<TForteUInt64>(CIEC_INT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteInt16>(pa_roVal) : CIEC_INT::scm_nMaxVal);
}

inline const CIEC_LINT ULINT_TO_LINT(const CIEC_ULINT &pa_roVal){
  return CIEC_LINT((TForteInt64) pa_roVal);
}

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL ULINT_TO_LREAL(const CIEC_ULINT &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

inline const CIEC_DWORD ULINT_TO_DWORD(const CIEC_ULINT &pa_roVal){
  return CIEC_DWORD((TForteDWord) (static_cast<TForteUInt64>(CIEC_DWORD::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteDWord>(pa_roVal) : CIEC_DWORD::scm_nMaxVal);
}

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL ULINT_TO_REAL(const CIEC_ULINT &pa_roVal){
  return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_SINT ULINT_TO_SINT(const CIEC_ULINT &pa_roVal){
  return CIEC_SINT((TForteInt8) (static_cast<TForteUInt64>(CIEC_SINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteInt8>(pa_roVal) : CIEC_SINT::scm_nMaxVal);
}

inline const CIEC_UDINT ULINT_TO_UDINT(const CIEC_ULINT &pa_roVal){
  return CIEC_UDINT((TForteUInt32) (static_cast<TForteUInt64>(CIEC_UDINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteUInt32>(pa_roVal) : CIEC_UDINT::scm_nMaxVal);
}

inline const CIEC_UINT ULINT_TO_UINT(const CIEC_ULINT &pa_roVal){
  return CIEC_UINT((TForteUInt16) (static_cast<TForteUInt64>(CIEC_UINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteUInt16>(pa_roVal) : CIEC_UINT::scm_nMaxVal);
}

inline const CIEC_LWORD ULINT_TO_LWORD(const CIEC_ULINT &pa_roVal){
  return CIEC_LWORD((TForteUInt64) pa_roVal);
}

inline const CIEC_USINT ULINT_TO_USINT(const CIEC_ULINT &pa_roVal){
  return CIEC_USINT((TForteUInt8) (static_cast<TForteUInt64>(CIEC_USINT::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteUInt8>(pa_roVal) : CIEC_USINT::scm_nMaxVal);
}

inline const CIEC_WORD ULINT_TO_WORD(const CIEC_ULINT &pa_roVal){
  return CIEC_WORD((TForteWord) (static_cast<TForteUInt64>(CIEC_WORD::scm_nMaxVal) >= pa_roVal) ? static_cast<TForteWord>(pa_roVal) : CIEC_WORD::scm_nMaxVal);
}

inline const CIEC_STRING ULINT_TO_STRING(const CIEC_ULINT &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING ULINT_TO_WSTRING(const CIEC_ULINT &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME ULINT_TO_TIME(const CIEC_ULINT &pa_roVal){
  if(pa_roVal > 0){
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal) * FORTE_TIME_BASE_UNITS_PER_SECOND);
  }else{
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));
  }
}
#endif

//********************************************************************************************
//   USINT_TO_*  functions
//********************************************************************************************
inline const CIEC_BOOL USINT_TO_BOOL(const CIEC_USINT &pa_roVal){
	return CIEC_BOOL((TForteUInt8)0 != pa_roVal ? true : false);
}

inline const CIEC_DINT USINT_TO_DINT(const CIEC_USINT &pa_roVal){
  return CIEC_DINT((TForteInt32) pa_roVal);
}

inline const CIEC_BYTE USINT_TO_BYTE(const CIEC_USINT &pa_roVal){
  return CIEC_BYTE((TForteByte) pa_roVal);
}

inline const CIEC_INT USINT_TO_INT(const CIEC_USINT &pa_roVal){
  return CIEC_INT((TForteInt16) pa_roVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT USINT_TO_LINT(const CIEC_USINT &pa_roVal){
  return CIEC_LINT((TForteInt64) pa_roVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL USINT_TO_LREAL(const CIEC_USINT &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD USINT_TO_LWORD(const CIEC_USINT &pa_roVal){
  return CIEC_LWORD((TForteLWord) pa_roVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL USINT_TO_REAL(const CIEC_USINT &pa_roVal){
  return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_SINT USINT_TO_SINT(const CIEC_USINT &pa_roVal){
  return CIEC_SINT((TForteInt8) (CIEC_SINT::scm_nMaxVal >= pa_roVal) ? static_cast<TForteInt8>(pa_roVal) : CIEC_SINT::scm_nMaxVal);
}

inline const CIEC_DWORD USINT_TO_DWORD(const CIEC_USINT &pa_roVal){
  return CIEC_DWORD((TForteDWord) pa_roVal);
}

inline const CIEC_USINT USINT_TO_UDINT(const CIEC_USINT &pa_roVal){
  return CIEC_USINT((TForteUInt32) pa_roVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT USINT_TO_ULINT(const CIEC_USINT &pa_roVal){
  return CIEC_ULINT((TForteUInt64) pa_roVal);
}
#endif

inline const CIEC_UINT USINT_TO_UINT(const CIEC_USINT &pa_roVal){
  return CIEC_UINT((TForteUInt16) pa_roVal);
}

inline const CIEC_WORD USINT_TO_WORD(const CIEC_USINT &pa_roVal){
  return CIEC_WORD((TForteWord) pa_roVal);
}

inline const CIEC_STRING USINT_TO_STRING(const CIEC_USINT &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING USINT_TO_WSTRING(const CIEC_USINT &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME USINT_TO_TIME(const CIEC_USINT &pa_roVal){
  return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal) * FORTE_TIME_BASE_UNITS_PER_SECOND);
}

//********************************************************************************************
//   WORD_TO_*  functions
//********************************************************************************************
inline const CIEC_BOOL WORD_TO_BOOL(const CIEC_WORD &pa_roVal){
  return CIEC_BOOL((TForteWord)0 != pa_roVal ? true : false);
}

inline const CIEC_DINT WORD_TO_DINT(const CIEC_WORD &pa_roVal){
  return CIEC_DINT((TForteInt32) pa_roVal);
}

inline const CIEC_BYTE WORD_TO_BYTE(const CIEC_WORD &pa_roVal){
  return CIEC_BYTE((TForteByte) (CIEC_BYTE::scm_nMaxVal >= pa_roVal) ? static_cast<TForteByte>(pa_roVal) : CIEC_BYTE::scm_nMaxVal);
}

inline const CIEC_INT WORD_TO_INT(const CIEC_WORD &pa_roVal){
  return CIEC_INT((TForteInt16) static_cast<TForteInt16>(pa_roVal));
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT WORD_TO_LINT(const CIEC_WORD &pa_roVal){
  return CIEC_LINT((TForteInt64) pa_roVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL WORD_TO_LREAL(const CIEC_WORD &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD WORD_TO_LWORD(const CIEC_WORD &pa_roVal){
  return CIEC_LWORD((TForteLWord) pa_roVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL WORD_TO_REAL(const CIEC_WORD &pa_roVal){
  return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_SINT WORD_TO_SINT(const CIEC_WORD &pa_roVal){
  return CIEC_SINT((TForteInt8) (CIEC_SINT::scm_nMaxVal >= pa_roVal) ? static_cast<TForteInt8>(pa_roVal) : CIEC_SINT::scm_nMaxVal);
}

inline const CIEC_DWORD WORD_TO_DWORD(const CIEC_WORD &pa_roVal){
  return CIEC_DWORD((TForteDWord) pa_roVal);
}

inline const CIEC_WORD WORD_TO_UDINT(const CIEC_WORD &pa_roVal){
  return CIEC_WORD((TForteUInt32) pa_roVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT WORD_TO_ULINT(const CIEC_WORD &pa_roVal){
  return CIEC_ULINT((TForteUInt64) pa_roVal);
}
#endif

inline const CIEC_USINT WORD_TO_USINT(const CIEC_WORD &pa_roVal){
  return CIEC_USINT((TForteUInt8) (CIEC_USINT::scm_nMaxVal >= pa_roVal) ? static_cast<TForteUInt8>(pa_roVal) : CIEC_USINT::scm_nMaxVal);
}

inline const CIEC_UINT WORD_TO_UINT(const CIEC_WORD &pa_roVal){
  return CIEC_UINT((TForteUInt16) pa_roVal);
}

inline const CIEC_STRING WORD_TO_STRING(const CIEC_WORD &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING WORD_TO_WSTRING(const CIEC_WORD &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME WORD_TO_TIME(const CIEC_WORD &pa_roVal){
  return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal) * FORTE_TIME_BASE_UNITS_PER_SECOND);
}

//********************************************************************************************
//   INT_TO_*  functions
//********************************************************************************************
inline const CIEC_BOOL INT_TO_BOOL(const CIEC_INT &pa_roVal){
	return CIEC_BOOL((TForteInt16)0 != pa_roVal ? true : false);
}

inline const CIEC_BYTE INT_TO_BYTE(const CIEC_INT &pa_roVal){
  TForteByte nBuf = static_cast<TForteByte>(pa_roVal);
  if(CIEC_BYTE::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_BYTE::scm_nMaxVal;
  }
  else{
    if(CIEC_BYTE::scm_nMinVal > pa_roVal){
      nBuf = CIEC_BYTE::scm_nMinVal;
    }
  }
  return CIEC_BYTE(nBuf);
}

inline const CIEC_DWORD INT_TO_DWORD(const CIEC_INT &pa_roVal){
  return CIEC_DWORD((TForteDWord) ((0 < pa_roVal) ? static_cast<TForteDWord>(pa_roVal) : CIEC_DWORD::scm_nMinVal));
}

inline const CIEC_DINT INT_TO_DINT(const CIEC_INT &pa_roVal){
  return CIEC_DINT((TForteInt32) pa_roVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT INT_TO_LINT(const CIEC_INT &pa_roVal){
  return CIEC_LINT((TForteInt64) pa_roVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL INT_TO_LREAL(const CIEC_INT &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD INT_TO_LWORD(const CIEC_INT &pa_roVal){
  return CIEC_LWORD((TForteLWord) (0 < pa_roVal) ? static_cast<TForteLWord>(pa_roVal) : CIEC_LWORD::scm_nMinVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL INT_TO_REAL(const CIEC_INT &pa_roVal){
  return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_SINT INT_TO_SINT(const CIEC_INT &pa_roVal){
  TForteInt8 nBuf = static_cast<TForteInt8>(pa_roVal);
  if(CIEC_SINT::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_SINT::scm_nMaxVal;
  }
  else{
    if(CIEC_SINT::scm_nMinVal > pa_roVal){
      nBuf = CIEC_SINT::scm_nMinVal;
    }
  }
  return CIEC_SINT(nBuf);
}

inline const CIEC_UDINT INT_TO_UDINT(const CIEC_INT &pa_roVal){
  return CIEC_UDINT((TForteUInt32) (0 < pa_roVal) ? static_cast<TForteUInt32>(pa_roVal) : CIEC_UDINT::scm_nMinVal);
}

inline const CIEC_UINT INT_TO_UINT(const CIEC_INT &pa_roVal){
  return CIEC_UINT((TForteUInt16) (0 < pa_roVal) ? static_cast<TForteUInt16>(pa_roVal) : CIEC_UINT::scm_nMinVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT INT_TO_ULINT(const CIEC_INT &pa_roVal){
  return CIEC_ULINT((TForteUInt64) (0 < pa_roVal) ? static_cast<TForteUInt64>(pa_roVal) : CIEC_ULINT::scm_nMinVal);
}
#endif

inline const CIEC_USINT INT_TO_USINT(const CIEC_INT &pa_roVal){
  TForteUInt8 nBuf = static_cast<TForteUInt8>(pa_roVal);
  if(CIEC_USINT::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_USINT::scm_nMaxVal;
  }
  else{
    if(CIEC_USINT::scm_nMinVal > pa_roVal){
      nBuf = CIEC_USINT::scm_nMinVal;
    }
  }
  return CIEC_USINT(nBuf);
}

inline const CIEC_WORD INT_TO_WORD(const CIEC_INT &pa_roVal){
  return CIEC_WORD((TForteWord) static_cast<TForteWord>(pa_roVal));
}

inline const CIEC_STRING INT_TO_STRING(const CIEC_INT &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

inline const CIEC_TIME INT_TO_TIME(const CIEC_INT &pa_roVal){
  if(pa_roVal > 0 ){
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal) * FORTE_TIME_BASE_UNITS_PER_SECOND);
  }else{
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));
  }
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING INT_TO_WSTRING(const CIEC_INT &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

//********************************************************************************************
//   LINT_TO_*  functions
//********************************************************************************************
#ifdef FORTE_USE_64BIT_DATATYPES

inline const CIEC_BOOL LINT_TO_BOOL(const CIEC_LINT &pa_roVal){
	return CIEC_BOOL((TForteInt64)0 != pa_roVal ? true : false);
}

inline const CIEC_BYTE LINT_TO_BYTE(const CIEC_LINT &pa_roVal){
  TForteByte nBuf = static_cast<TForteByte>(pa_roVal);
  if(CIEC_BYTE::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_BYTE::scm_nMaxVal;
  }
  else{
    if(CIEC_BYTE::scm_nMinVal > pa_roVal){
      nBuf = CIEC_BYTE::scm_nMinVal;
    }
  }
  return CIEC_BYTE(nBuf);
}

inline const CIEC_DWORD LINT_TO_DWORD(const CIEC_LINT &pa_roVal){
  return CIEC_DWORD((TForteDWord) ((0 < pa_roVal) ? static_cast<TForteDWord>(pa_roVal) : CIEC_DWORD::scm_nMinVal));
}

inline const CIEC_DINT LINT_TO_DINT(const CIEC_LINT &pa_roVal){
  return CIEC_DINT((TForteInt32) pa_roVal);
}

inline const CIEC_INT LINT_TO_INT(const CIEC_LINT &pa_roVal){
  TForteInt16 nBuf = static_cast<TForteInt16>(pa_roVal);
  if(CIEC_INT::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_INT::scm_nMaxVal;
  }
  else{
    if(CIEC_INT::scm_nMinVal > pa_roVal){
      nBuf = CIEC_INT::scm_nMinVal;
    }
  }
  return CIEC_INT(nBuf);
}

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL LINT_TO_LREAL(const CIEC_LINT &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

inline const CIEC_LWORD LINT_TO_LWORD(const CIEC_LINT &pa_roVal){
  return CIEC_LWORD((TForteLWord) (0 < pa_roVal) ? static_cast<TForteLWord>(pa_roVal) : CIEC_LWORD::scm_nMinVal);
}

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL LINT_TO_REAL(const CIEC_LINT &pa_roVal){
  return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_SINT LINT_TO_SINT(const CIEC_LINT &pa_roVal){
  TForteInt8 nBuf = static_cast<TForteInt8>(pa_roVal);
  if(CIEC_SINT::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_SINT::scm_nMaxVal;
  }
  else{
    if(CIEC_SINT::scm_nMinVal > pa_roVal){
      nBuf = CIEC_SINT::scm_nMinVal;
    }
  }
  return CIEC_SINT(nBuf);
}

inline const CIEC_UDINT LINT_TO_UDINT(const CIEC_LINT &pa_roVal){
  TForteUInt32 nBuf = static_cast<TForteUInt32>(pa_roVal);
  if(static_cast<TForteInt64>(CIEC_UDINT::scm_nMaxVal) < pa_roVal){
    nBuf = CIEC_UDINT::scm_nMaxVal;
  }
  else{
    if(0 > pa_roVal){
      nBuf = CIEC_UDINT::scm_nMinVal;
    }
  }
  return CIEC_UDINT(nBuf);
}

inline const CIEC_UINT LINT_TO_UINT(const CIEC_LINT &pa_roVal){
  TForteUInt16 nBuf = static_cast<TForteUInt16>(pa_roVal);
  if(CIEC_UINT::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_UINT::scm_nMaxVal;
  }
  else{
    if(CIEC_UINT::scm_nMinVal > pa_roVal){
      nBuf = CIEC_UINT::scm_nMinVal;
    }
  }
  return CIEC_UINT(nBuf);
}

inline const CIEC_ULINT LINT_TO_ULINT(const CIEC_LINT &pa_roVal){
  return CIEC_ULINT((TForteUInt64) (0 < pa_roVal) ? static_cast<TForteUInt64>(pa_roVal) : CIEC_ULINT::scm_nMinVal);
}

inline const CIEC_USINT LINT_TO_USINT(const CIEC_LINT &pa_roVal){
  TForteUInt8 nBuf = static_cast<TForteUInt8>(pa_roVal);
  if(CIEC_USINT::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_USINT::scm_nMaxVal;
  }
  else{
    if(CIEC_USINT::scm_nMinVal > pa_roVal){
      nBuf = CIEC_USINT::scm_nMinVal;
    }
  }
  return CIEC_USINT(nBuf);
}

inline const CIEC_WORD LINT_TO_WORD(const CIEC_LINT &pa_roVal){
  TForteWord nBuf = static_cast<TForteWord>(pa_roVal);
  if(CIEC_WORD::scm_nMaxVal < pa_roVal){
    nBuf = CIEC_WORD::scm_nMaxVal;
  }
  else{
    if(CIEC_WORD::scm_nMinVal > pa_roVal){
      nBuf = CIEC_WORD::scm_nMinVal;
    }
  }
  return CIEC_WORD(nBuf);
}

inline const CIEC_STRING LINT_TO_STRING(const CIEC_LINT &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING LINT_TO_WSTRING(const CIEC_LINT &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

inline const CIEC_TIME LINT_TO_TIME(const CIEC_LINT &pa_roVal){
  if(pa_roVal > 0){
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal) * FORTE_TIME_BASE_UNITS_PER_SECOND);
  }else{
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));
  }
}
#endif

//********************************************************************************************
//   SINT_TO_*  functions
//********************************************************************************************
inline const CIEC_BOOL SINT_TO_BOOL(const CIEC_SINT &pa_roVal){
	return CIEC_BOOL((TForteInt8)0 != pa_roVal ? true : false);
}

inline const CIEC_BYTE SINT_TO_BYTE(const CIEC_SINT &pa_roVal){
  return CIEC_BYTE((TForteByte) ((0 < pa_roVal) ? static_cast<TForteByte>(pa_roVal) : CIEC_BYTE::scm_nMinVal));
}

inline const CIEC_DWORD SINT_TO_DWORD(const CIEC_SINT &pa_roVal){
  return CIEC_DWORD((TForteDWord) ((0 < pa_roVal) ? static_cast<TForteDWord>(pa_roVal) : CIEC_DWORD::scm_nMinVal));
}

inline const CIEC_DINT SINT_TO_DINT(const CIEC_SINT &pa_roVal){
  return CIEC_DINT((TForteInt32) pa_roVal);
}

inline const CIEC_TIME SINT_TO_TIME(const CIEC_SINT &pa_roVal){
  if(pa_roVal > 0){
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(pa_roVal) * FORTE_TIME_BASE_UNITS_PER_SECOND);
  }else{
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));
  }
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LINT SINT_TO_LINT(const CIEC_SINT &pa_roVal){
  return CIEC_LINT((TForteInt64) pa_roVal);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
inline const CIEC_LREAL SINT_TO_LREAL(const CIEC_SINT &pa_roVal){
  return CIEC_LREAL((TForteDFloat) pa_roVal);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD SINT_TO_LWORD(const CIEC_SINT &pa_roVal){
  return CIEC_LWORD((TForteLWord) (0 < pa_roVal) ? static_cast<TForteLWord>(pa_roVal) : CIEC_LWORD::scm_nMinVal);
}
#endif

#ifdef FORTE_USE_REAL_DATATYPE
inline const CIEC_REAL SINT_TO_REAL(const CIEC_SINT &pa_roVal){
  return CIEC_REAL((TForteFloat) pa_roVal);
}
#endif

inline const CIEC_INT SINT_TO_INT(const CIEC_SINT &pa_roVal){
  return CIEC_INT((TForteInt16) pa_roVal);
}

inline const CIEC_UDINT SINT_TO_UDINT(const CIEC_SINT &pa_roVal){
  return CIEC_UDINT((TForteUInt32) (0 < pa_roVal) ? static_cast<TForteUInt32>(pa_roVal) : CIEC_UDINT::scm_nMinVal);
}

inline const CIEC_UINT SINT_TO_UINT(const CIEC_SINT &pa_roVal){
  return CIEC_UINT((TForteUInt16) (0 < pa_roVal) ? static_cast<TForteUInt16>(pa_roVal) : CIEC_UINT::scm_nMinVal);
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT SINT_TO_ULINT(const CIEC_SINT &pa_roVal){
  return CIEC_ULINT((TForteUInt64) (0 < pa_roVal) ? static_cast<TForteUInt64>(pa_roVal) : CIEC_ULINT::scm_nMinVal);
}
#endif

inline const CIEC_USINT SINT_TO_USINT(const CIEC_SINT &pa_roVal){
  return CIEC_USINT((TForteUInt8) (0 < pa_roVal) ? static_cast<TForteUInt8>(pa_roVal) : CIEC_USINT::scm_nMinVal);
}

inline const CIEC_WORD SINT_TO_WORD(const CIEC_SINT &pa_roVal){
  return CIEC_WORD((TForteWord) (0 < pa_roVal) ? static_cast<TForteWord>(pa_roVal) : CIEC_WORD::scm_nMinVal);
}

inline const CIEC_STRING SINT_TO_STRING(const CIEC_SINT &pa_roVal){
	CIEC_STRING string;
	stringConverter(string, pa_roVal);
	return string;
}

#ifdef FORTE_USE_WSTRING_DATATYPE
inline const CIEC_WSTRING SINT_TO_WSTRING(const CIEC_SINT &pa_roVal){
  CIEC_WSTRING string;
  stringConverter(string, pa_roVal);
  return string;
}
#endif

//********************************************************************************************
//   *_BCD_TO_**  functions
//********************************************************************************************
inline const CIEC_USINT BYTE_BCD_TO_USINT(const CIEC_BYTE &pa_roVal){
  return CIEC_USINT(static_cast<TForteUInt8>((pa_roVal / 16 * 10) + (pa_roVal % 16)));
}

inline const CIEC_UINT WORD_BCD_TO_UINT(const CIEC_WORD &pa_roVal){
  return CIEC_UINT(static_cast<TForteUInt16>(BYTE_BCD_TO_USINT(CIEC_BYTE(static_cast<TForteByte>(pa_roVal >> 8))) * 100 + BYTE_BCD_TO_USINT(CIEC_BYTE(static_cast<TForteByte>(pa_roVal & 0xff)))));
}

inline const CIEC_UDINT DWORD_BCD_TO_UDINT(const CIEC_DWORD &pa_roVal){
  return CIEC_UDINT(static_cast<TForteUInt32>(WORD_BCD_TO_UINT(CIEC_WORD(static_cast<TForteWord>(pa_roVal >> 16))) * 10000 + WORD_BCD_TO_UINT(CIEC_WORD(static_cast<TForteWord>(pa_roVal & 0xffff)))));
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_ULINT LWORD_BCD_TO_ULINT(const CIEC_LWORD &pa_roVal){
  return CIEC_ULINT(static_cast<TForteUInt64>(DWORD_BCD_TO_UDINT(CIEC_DWORD(static_cast<TForteDWord>(pa_roVal >> 32))) * 100000000 + DWORD_BCD_TO_UDINT(CIEC_DWORD(static_cast<TForteDWord>(pa_roVal & 0xffffffff)))));
}
#endif

//********************************************************************************************
//   **_TO_BCD_*  functions
//********************************************************************************************
inline const CIEC_BYTE USINT_TO_BCD_BYTE(const CIEC_USINT &pa_roVal){
  return CIEC_BYTE(static_cast<TForteByte>(pa_roVal / 10 * 16 + pa_roVal % 10));
}

inline const CIEC_WORD UINT_TO_BCD_WORD(const CIEC_UINT &pa_roVal){
  return CIEC_WORD(static_cast<TForteWord>((static_cast<TForteWord>(USINT_TO_BCD_BYTE(static_cast<TForteUInt8>(pa_roVal / 100))) << 8) + static_cast<TForteWord>(USINT_TO_BCD_BYTE(static_cast<TForteUInt8>(pa_roVal % 100)))));
}

inline const CIEC_DWORD UDINT_TO_BCD_DWORD(const CIEC_UDINT &pa_roVal){
  return CIEC_DWORD(static_cast<TForteDWord>((static_cast<TForteDWord>(UINT_TO_BCD_WORD(static_cast<TForteUInt16>(pa_roVal / 10000))) << 16) + static_cast<TForteDWord>(UINT_TO_BCD_WORD(static_cast<TForteUInt16>(pa_roVal % 10000)))));
}

#ifdef FORTE_USE_64BIT_DATATYPES
inline const CIEC_LWORD ULINT_TO_BCD_LWORD(const CIEC_ULINT &pa_roVal){
  return CIEC_LWORD(static_cast<TForteLWord>((static_cast<TForteLWord>(UDINT_TO_BCD_DWORD(static_cast<TForteUInt32>(pa_roVal / 100000000))) << 32) + static_cast<TForteLWord>(UDINT_TO_BCD_DWORD(static_cast<TForteUInt32>(pa_roVal % 100000000)))));
}
#endif
#endif /* CONVERT_FUNCTIONS_H_ */
