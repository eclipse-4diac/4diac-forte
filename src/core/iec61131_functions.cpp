/*******************************************************************************
 * Copyright (c) 2011 - 2013, 2018 ACIN, fortiss GmbH, 2019 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - adds NOW and NOW_MONOTONIC functions
 *******************************************************************************/
#include "iec61131_functions.h"

#include "forte_architecture_time.h"

template<> const CIEC_BOOL NOT<CIEC_BOOL>(const CIEC_BOOL& pa_roIN){
  return CIEC_BOOL(!pa_roIN);
}

#ifdef FORTE_USE_REAL_DATATYPE
TForteInt32 TRUNC(const CIEC_REAL& pa_roIN){
  return static_cast<TForteInt32>(pa_roIN);
}
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
TForteInt64 TRUNC(const CIEC_LREAL& pa_roIN){
  return static_cast<TForteInt64>(pa_roIN);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
const CIEC_TIME_OF_DAY ADD_TOD_TIME(const CIEC_TIME_OF_DAY& pa_roIN1, const CIEC_TIME& pa_roIN2){
  return static_cast<TForteUInt64>(pa_roIN1 + pa_roIN2.getInMilliSeconds());
}

const CIEC_DATE_AND_TIME ADD_DT_TIME(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_TIME& pa_roIN2){
  return static_cast<TForteUInt64>(pa_roIN1 + pa_roIN2.getInMilliSeconds());
}

const CIEC_TIME_OF_DAY SUB_TOD_TIME(const CIEC_TIME_OF_DAY& pa_roIN1, const CIEC_TIME& pa_roIN2){
  return static_cast<TForteUInt64>(pa_roIN1 - pa_roIN2.getInMilliSeconds());
}

const CIEC_DATE_AND_TIME SUB_DT_TIME(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_TIME& pa_roIN2){
  return static_cast<TForteUInt64>(pa_roIN1 - pa_roIN2.getInMilliSeconds());
}

const CIEC_TIME SUB_DT_DT(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_DATE_AND_TIME& pa_roIN2){
  CIEC_TIME temp;
  temp.setFromMilliSeconds(pa_roIN1 - pa_roIN2);
  return temp;
}

const CIEC_TIME SUB_TOD_TOD(const CIEC_TIME_OF_DAY &pa_roIN1, const CIEC_TIME_OF_DAY &pa_roIN2){
  CIEC_TIME temp;
  temp.setFromMilliSeconds(pa_roIN1 - pa_roIN2);
  return temp;
}

const CIEC_TIME SUB_DATE_DATE(const CIEC_DATE &pa_roIN1, const CIEC_DATE &pa_roIN2){
  CIEC_TIME temp;
  temp.setFromMilliSeconds(pa_roIN1 - pa_roIN2);
  return temp;
}

const CIEC_DATE_AND_TIME CONCAT_DATE_TOD(const CIEC_DATE& pa_roIN1, const CIEC_TIME_OF_DAY& pa_roIN2){
  CIEC_DATE_AND_TIME retVal;

  struct tm *pstTime = pa_roIN1.getTimeStruct();

  time_t t = static_cast<time_t>( pa_roIN2 / 1000ULL);
  pstTime->tm_hour = (int) (t / 3600);
  pstTime->tm_min = (int) ((t % 3600) / 60);
  pstTime->tm_sec = (int) (t % 60);

  retVal.setDateAndTime(*pstTime, pa_roIN2.getMilliSeconds());
  return retVal;
}

CIEC_ANY_INT FIND(const CIEC_ANY_STRING& pa_rsIn1, const CIEC_ANY_STRING& pa_rsIn2){
  const char* pc_Find = strstr(pa_rsIn1.getValue(), pa_rsIn2.getValue());
  CIEC_ANY_INT temp;
  if (0 != pc_Find){
    temp.setSignedValue(pc_Find - pa_rsIn1.getValue() + 1);
  }
  return temp;
}

const CIEC_TIME NOW_MONOTONIC() {
  CIEC_TIME now;
  now.setFromNanoSeconds(getNanoSecondsMonotonic());
  return now;
}

const CIEC_DATE_AND_TIME NOW() {
  return CIEC_DATE_AND_TIME(forte_time() * 1000ULL);
}

#endif
