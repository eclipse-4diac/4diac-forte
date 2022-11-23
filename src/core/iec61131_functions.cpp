/*******************************************************************************
 * Copyright (c) 2011 - 2013, 2018 ACIN, fortiss GmbH, 2019 TU Wien/ACIN
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - adds NOW and NOW_MONOTONIC functions, 
 *        changes for removed implicit constructor for primitive types
 *******************************************************************************/
#include "iec61131_functions.h"

#include "forte_architecture_time.h"

/*** TRUNC ***/
const CIEC_SINT func_TRUNC_SINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_SINT>(paIN);
}

const CIEC_INT func_TRUNC_INT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_INT>(paIN);
}

const CIEC_DINT func_TRUNC_DINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_DINT>(paIN);
}

const CIEC_LINT func_TRUNC_LINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_LINT>(paIN);
}

const CIEC_USINT func_TRUNC_USINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_USINT>(paIN);
}

const CIEC_UINT func_TRUNC_UINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_UINT>(paIN);
}

const CIEC_UDINT func_TRUNC_UDINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_UDINT>(paIN);
}

const CIEC_ULINT func_TRUNC_ULINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_ULINT>(paIN);
}

const CIEC_SINT func_REAL_TRUNC_SINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_SINT>(paIN);
}

const CIEC_INT func_REAL_TRUNC_INT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_INT>(paIN);
}

const CIEC_DINT func_REAL_TRUNC_DINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_DINT>(paIN);
}

const CIEC_LINT func_REAL_TRUNC_LINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_LINT>(paIN);
}

const CIEC_USINT func_REAL_TRUNC_USINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_USINT>(paIN);
}

const CIEC_UINT func_REAL_TRUNC_UINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_UINT>(paIN);
}

const CIEC_UDINT func_REAL_TRUNC_UDINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_UDINT>(paIN);
}

const CIEC_ULINT func_REAL_TRUNC_ULINT(const CIEC_REAL &paIN) {
  return func_TRUNC<CIEC_ULINT>(paIN);
}

const CIEC_SINT func_TRUNC_SINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_SINT>(paIN);
}

const CIEC_INT func_TRUNC_INT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_INT>(paIN);
}

const CIEC_DINT func_TRUNC_DINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_DINT>(paIN);
}

const CIEC_LINT func_TRUNC_LINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_LINT>(paIN);
}

const CIEC_USINT func_TRUNC_USINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_USINT>(paIN);
}

const CIEC_UINT func_TRUNC_UINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_UINT>(paIN);
}

const CIEC_UDINT func_TRUNC_UDINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_UDINT>(paIN);
}

const CIEC_ULINT func_TRUNC_ULINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_ULINT>(paIN);
}

const CIEC_SINT func_LREAL_TRUNC_SINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_SINT>(paIN);
}

const CIEC_INT func_LREAL_TRUNC_INT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_INT>(paIN);
}

const CIEC_DINT func_LREAL_TRUNC_DINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_DINT>(paIN);
}

const CIEC_LINT func_LREAL_TRUNC_LINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_LINT>(paIN);
}

const CIEC_USINT func_LREAL_TRUNC_USINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_USINT>(paIN);
}

const CIEC_UINT func_LREAL_TRUNC_UINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_UINT>(paIN);
}

const CIEC_UDINT func_LREAL_TRUNC_UDINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_UDINT>(paIN);
}

const CIEC_ULINT func_LREAL_TRUNC_ULINT(const CIEC_LREAL &paIN) {
  return func_TRUNC<CIEC_ULINT>(paIN);
}

const CIEC_TIME func_ADD_TIME(const CIEC_TIME &paIN1, const CIEC_TIME &paIN2) {
  return func_ADD(paIN1, paIN2);
}

const CIEC_TIME_OF_DAY func_ADD_TOD_TIME(const CIEC_TIME_OF_DAY& paIN1, const CIEC_TIME& paIN2){
  return CIEC_TIME_OF_DAY(static_cast<CIEC_TIME_OF_DAY::TValueType>(paIN1) + static_cast<CIEC_TIME::TValueType>(paIN2));
}

const CIEC_DATE_AND_TIME func_ADD_DT_TIME(const CIEC_DATE_AND_TIME& paIN1, const CIEC_TIME& paIN2){
  return CIEC_DATE_AND_TIME(static_cast<CIEC_DATE_AND_TIME::TValueType>(paIN1) + static_cast<CIEC_TIME::TValueType>(paIN2));
}

const CIEC_TIME func_SUB_TIME(const CIEC_TIME &paIN1, const CIEC_TIME &paIN2) {
  return func_SUB(paIN1, paIN2);
}

const CIEC_TIME_OF_DAY func_SUB_TOD_TIME(const CIEC_TIME_OF_DAY& paIN1, const CIEC_TIME& paIN2){
  return CIEC_TIME_OF_DAY(static_cast<CIEC_TIME_OF_DAY::TValueType>(paIN1) - static_cast<CIEC_TIME::TValueType>(paIN2));
}

const CIEC_DATE_AND_TIME func_SUB_DT_TIME(const CIEC_DATE_AND_TIME& paIN1, const CIEC_TIME& paIN2){
  return CIEC_DATE_AND_TIME(static_cast<CIEC_DATE_AND_TIME::TValueType>(paIN1) - static_cast<CIEC_TIME::TValueType>(paIN2));
}

const CIEC_TIME func_SUB_DT_DT(const CIEC_DATE_AND_TIME& paIN1, const CIEC_DATE_AND_TIME& paIN2){
  return CIEC_TIME(static_cast<CIEC_DATE_AND_TIME::TValueType>(paIN1) - static_cast<TForteUInt64>(paIN2));
}

const CIEC_TIME func_SUB_TOD_TOD(const CIEC_TIME_OF_DAY &paIN1, const CIEC_TIME_OF_DAY &paIN2){
  return CIEC_TIME(static_cast<CIEC_TIME_OF_DAY::TValueType>(paIN1) - static_cast<CIEC_TIME_OF_DAY::TValueType>(paIN2));
}

const CIEC_TIME func_SUB_DATE_DATE(const CIEC_DATE &paIN1, const CIEC_DATE &paIN2){
  return CIEC_TIME(static_cast<CIEC_DATE::TValueType>(paIN1) - static_cast<CIEC_DATE::TValueType>(paIN2));
}

const CIEC_DATE_AND_TIME func_CONCAT_DATE_TOD(const CIEC_DATE& paIN1, const CIEC_TIME_OF_DAY& paIN2){
  CIEC_DATE_AND_TIME retVal;

  struct tm *pstTime = paIN1.getTimeStruct();

  time_t t = static_cast<time_t>(static_cast<TForteUInt64>(paIN2) / 1000ULL);
  pstTime->tm_hour = (int) (t / 3600);
  pstTime->tm_min = (int) ((t % 3600) / 60);
  pstTime->tm_sec = (int) (t % 60);

  retVal.setDateAndTime(*pstTime, paIN2.getMilliSeconds());
  return retVal;
}

const CIEC_DATE_AND_TIME func_CONCAT_DT(const CIEC_ANY_INT &YEAR, const CIEC_ANY_INT &MONTH, const CIEC_ANY_INT &DAY, const CIEC_ANY_INT &HOUR, const CIEC_ANY_INT &MINUTE, const CIEC_ANY_INT &SECOND, const CIEC_ANY_INT &MILLISECOND) {
  struct tm concatTime = {0};
  memset(&concatTime, 0, sizeof(concatTime));
  if(YEAR.isSigned()) {
    concatTime.tm_year = YEAR.getSignedValue() - 1900;
  } else {
    concatTime.tm_year = YEAR.getUnsignedValue() - 1900;
  }

  if (MONTH.isSigned()) {
    concatTime.tm_mon = MONTH.getSignedValue() - 1;
  } else {
    concatTime.tm_mon = MONTH.getUnsignedValue() - 1;
  }

  if (DAY.isSigned()) {
    concatTime.tm_mday = DAY.getSignedValue();
  } else {
    concatTime.tm_mday = DAY.getUnsignedValue();
  }

  if (HOUR.isSigned()) {
    concatTime.tm_hour = HOUR.getSignedValue();
  } else {
    concatTime.tm_hour = HOUR.getUnsignedValue();
  }

  if (MINUTE.isSigned()) {
    concatTime.tm_min = MINUTE.getSignedValue();
  } else {
    concatTime.tm_min = MINUTE.getUnsignedValue();
  }

  if (SECOND.isSigned()) {
    concatTime.tm_sec = SECOND.getSignedValue();
  } else {
    concatTime.tm_sec = SECOND.getUnsignedValue();
  }

  unsigned int millisecond = 0;
  if (MILLISECOND.isSigned()) {
    millisecond = MILLISECOND.getSignedValue();
  } else {
    millisecond = MILLISECOND.getUnsignedValue();
  }
  CIEC_DATE_AND_TIME retVal;
  retVal.setDateAndTime(concatTime, millisecond);
  return retVal;
}

const CIEC_TIME_OF_DAY func_CONCAT_TOD(const CIEC_ANY_INT &HOUR, const CIEC_ANY_INT &MINUTE, const CIEC_ANY_INT &SECOND, const CIEC_ANY_INT &MILLISECOND) {
  // Only unsigned representations need to be checked, as negative signed number will be large positive unsigned numbers, outside the allowed range
  if (HOUR.getUnsignedValue() > 23) {
    DEVLOG_ERROR("CONCAT_TOD HOUR outside of valid range");
    return CIEC_TIME_OF_DAY(0);
  }
  if (MINUTE.getUnsignedValue() > 59) {
    DEVLOG_ERROR("CONCAT_TOD MINUTE outside of valid range");
    return CIEC_TIME_OF_DAY(0);
  }
  if (SECOND.getUnsignedValue() > 59) {
    DEVLOG_ERROR("CONCAT_TOD SECOND outside of valid range");
    return CIEC_TIME_OF_DAY(0);
  }
  if (MILLISECOND.getUnsignedValue() > 999) {
    DEVLOG_ERROR("CONCAT_TOD MILLISECOND outside of valid range");
    return CIEC_TIME_OF_DAY(0);
  }
  uint_fast64_t hour = HOUR.getSignedValue() * 60ULL * 60ULL * 1000ULL * 1000000ULL;
  uint_fast64_t minute = MINUTE.getSignedValue() * 60ULL * 1000ULL * 1000000ULL;
  uint_fast64_t second = SECOND.getSignedValue() * 1000ULL * 1000000ULL;
  uint_fast64_t millisecond = MILLISECOND.getSignedValue() * 1000000ULL;
  return CIEC_TIME_OF_DAY(hour + minute + second + millisecond);
}

const CIEC_DATE func_CONCAT_DATE(const CIEC_ANY_INT &YEAR, const CIEC_ANY_INT &MONTH, const CIEC_ANY_INT &DAY) {
  return func_DT_TO_DATE(func_CONCAT_DT(YEAR, MONTH, DAY, CIEC_LINT(0), CIEC_LINT(0), CIEC_LINT(0), CIEC_LINT(0)));
}

const CIEC_LTIME func_ADD_LTIME(const CIEC_LTIME &paIN1, const CIEC_LTIME &paIN2) {
  return func_ADD(paIN1, paIN2);
}

const CIEC_LTIME_OF_DAY func_ADD_LTOD_LTIME(const CIEC_LTIME_OF_DAY& paIN1, const CIEC_LTIME& paIN2){
  return CIEC_LTIME_OF_DAY(static_cast<TForteUInt64>(paIN1) + static_cast<TForteUInt64>(paIN2));
}

const CIEC_LDATE_AND_TIME func_ADD_LDT_LTIME(const CIEC_LDATE_AND_TIME& paIN1, const CIEC_LTIME& paIN2){
  return CIEC_LDATE_AND_TIME(static_cast<TForteUInt64>(paIN1) + static_cast<TForteUInt64>(paIN2));
}

const CIEC_LTIME func_SUB_LTIME(const CIEC_LTIME &paIN1, const CIEC_LTIME &paIN2) {
  return func_SUB(paIN1, paIN2);
}

const CIEC_LTIME_OF_DAY func_SUB_LTOD_LTIME(const CIEC_LTIME_OF_DAY& paIN1, const CIEC_LTIME& paIN2){
  return CIEC_LTIME_OF_DAY(static_cast<TForteUInt64>(paIN1) - static_cast<TForteUInt64>(paIN2));
}

const CIEC_LDATE_AND_TIME func_SUB_LDT_LTIME(const CIEC_LDATE_AND_TIME& paIN1, const CIEC_LTIME& paIN2){
  return CIEC_LDATE_AND_TIME(static_cast<TForteUInt64>(paIN1) - static_cast<TForteUInt64>(paIN2));
}

const CIEC_LTIME func_SUB_LDT_LDT(const CIEC_LDATE_AND_TIME& paIN1, const CIEC_LDATE_AND_TIME& paIN2){
  return CIEC_LTIME(static_cast<TForteUInt64>(paIN1) - static_cast<TForteUInt64>(paIN2));
}

const CIEC_LTIME func_SUB_LTOD_LTOD(const CIEC_LTIME_OF_DAY &paIN1, const CIEC_LTIME_OF_DAY &paIN2){
  return CIEC_LTIME(static_cast<TForteUInt64>(paIN1) - static_cast<TForteUInt64>(paIN2));
}

const CIEC_LTIME func_SUB_LDATE_LDATE(const CIEC_LDATE &paIN1, const CIEC_LDATE &paIN2){
  return CIEC_LTIME(static_cast<TForteUInt64>(paIN1) - static_cast<TForteUInt64>(paIN2));
}

const CIEC_LDATE_AND_TIME func_CONCAT_LDATE_LTOD(const CIEC_LDATE& paIN1, const CIEC_LTIME_OF_DAY& paIN2){
  CIEC_LDATE_AND_TIME retVal;

  struct tm *pstTime = paIN1.getTimeStruct();

  time_t t = static_cast<time_t>( paIN2 / 1000ULL);
  pstTime->tm_hour = (int) (t / 3600);
  pstTime->tm_min = (int) ((t % 3600) / 60);
  pstTime->tm_sec = (int) (t % 60);

  retVal.setDateAndTime(*pstTime, paIN2.getMilliSeconds());
  return retVal;
}

CIEC_ULINT func_FIND(const CIEC_ANY_STRING& pa_rsIn1, const CIEC_ANY_STRING& pa_rsIn2){
  const char* pc_Find = strstr(pa_rsIn1.getValue(), pa_rsIn2.getValue());
  CIEC_ULINT temp;
  if (nullptr != pc_Find){
    temp.setUnsignedValue(pc_Find - pa_rsIn1.getValue() + 1);
  }
  return temp;
}

const CIEC_TIME func_NOW_MONOTONIC() {
  return CIEC_TIME(getNanoSecondsMonotonic());
}

const CIEC_DATE_AND_TIME func_NOW() {
  return CIEC_DATE_AND_TIME(forte_time() * 1000ULL);
}
