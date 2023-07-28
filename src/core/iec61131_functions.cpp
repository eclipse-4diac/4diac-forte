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

  struct tm pstTime; 
  paIN1.getTimeStruct(&pstTime);

  time_t t = static_cast<time_t>(static_cast<TForteUInt64>(paIN2) / 1000ULL);
  pstTime.tm_hour = static_cast<int>(t / 3600);
  pstTime.tm_min = static_cast<int>((t % 3600) / 60);
  pstTime.tm_sec = static_cast<int>(t % 60);

  retVal.setDateAndTime(pstTime, paIN2.getMilliSeconds());
  return retVal;
}

const CIEC_DATE_AND_TIME func_CONCAT_DT(const CIEC_ANY_INT &YEAR, const CIEC_ANY_INT &MONTH, const CIEC_ANY_INT &DAY, const CIEC_ANY_INT &HOUR, const CIEC_ANY_INT &MINUTE, const CIEC_ANY_INT &SECOND, const CIEC_ANY_INT &MILLISECOND) {
  struct tm concatTime {};
  if(YEAR.isSigned()) {
    concatTime.tm_year = static_cast<int>(YEAR.getSignedValue() - 1900);
  } else {
    concatTime.tm_year = static_cast<int>(YEAR.getUnsignedValue() - 1900);
  }

  if (MONTH.isSigned()) {
    concatTime.tm_mon = static_cast<int>(MONTH.getSignedValue() - 1);
  } else {
    concatTime.tm_mon = static_cast<int>(MONTH.getUnsignedValue() - 1);
  }

  if (DAY.isSigned()) {
    concatTime.tm_mday = static_cast<int>(DAY.getSignedValue());
  } else {
    concatTime.tm_mday = static_cast<int>(DAY.getUnsignedValue());
  }

  if (HOUR.isSigned()) {
    concatTime.tm_hour = static_cast<int>(HOUR.getSignedValue());
  } else {
    concatTime.tm_hour = static_cast<int>(HOUR.getUnsignedValue());
  }

  if (MINUTE.isSigned()) {
    concatTime.tm_min = static_cast<int>(MINUTE.getSignedValue());
  } else {
    concatTime.tm_min = static_cast<int>(MINUTE.getUnsignedValue());
  }

  if (SECOND.isSigned()) {
    concatTime.tm_sec = static_cast<int>(SECOND.getSignedValue());
  } else {
    concatTime.tm_sec = static_cast<int>(SECOND.getUnsignedValue());
  }

  unsigned int millisecond = 0;
  if (MILLISECOND.isSigned()) {
    millisecond = static_cast<unsigned int>(MILLISECOND.getSignedValue());
  } else {
    millisecond = static_cast<unsigned int>(MILLISECOND.getUnsignedValue());
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

  struct tm pstTime;
  paIN1.getTimeStruct(&pstTime);

  time_t t = static_cast<time_t>( paIN2 / 1000ULL);
  pstTime.tm_hour = static_cast<int>(t / 3600);
  pstTime.tm_min = static_cast<int>((t % 3600) / 60);
  pstTime.tm_sec = static_cast<int>(t % 60);

  retVal.setDateAndTime(pstTime, paIN2.getMilliSeconds());
  return retVal;
}

const CIEC_LDATE_AND_TIME func_CONCAT_LDT(const CIEC_ANY_INT &YEAR, const CIEC_ANY_INT &MONTH, const CIEC_ANY_INT &DAY, const CIEC_ANY_INT &HOUR, const CIEC_ANY_INT &MINUTE, const CIEC_ANY_INT &SECOND, const CIEC_ANY_INT &MILLISECOND) {
  struct tm concatTime{};

  if(YEAR.isSigned()) {
    concatTime.tm_year = static_cast<int>(YEAR.getSignedValue() - 1900);
  } else {
    concatTime.tm_year = static_cast<int>(YEAR.getUnsignedValue() - 1900);
  }

  if (MONTH.isSigned()) {
    concatTime.tm_mon = static_cast<int>(MONTH.getSignedValue() - 1);
  } else {
    concatTime.tm_mon = static_cast<int>(MONTH.getUnsignedValue() - 1);
  }

  if (DAY.isSigned()) {
    concatTime.tm_mday = static_cast<int>(DAY.getSignedValue());
  } else {
    concatTime.tm_mday = static_cast<int>(DAY.getUnsignedValue());
  }

  if (HOUR.isSigned()) {
    concatTime.tm_hour = static_cast<int>(HOUR.getSignedValue());
  } else {
    concatTime.tm_hour = static_cast<int>(HOUR.getUnsignedValue());
  }

  if (MINUTE.isSigned()) {
    concatTime.tm_min = static_cast<int>(MINUTE.getSignedValue());
  } else {
    concatTime.tm_min = static_cast<int>(MINUTE.getUnsignedValue());
  }

  if (SECOND.isSigned()) {
    concatTime.tm_sec = static_cast<int>(SECOND.getSignedValue());
  } else {
    concatTime.tm_sec = static_cast<int>(SECOND.getUnsignedValue());
  }

  unsigned int millisecond = 0;
  if (MILLISECOND.isSigned()) {
    millisecond = static_cast<unsigned int>(MILLISECOND.getSignedValue());
  } else {
    millisecond = static_cast<unsigned int>(MILLISECOND.getUnsignedValue());
  }
  CIEC_LDATE_AND_TIME retVal;
  retVal.setDateAndTime(concatTime, millisecond);
  return retVal;
}

const CIEC_LDATE func_CONCAT_LDATE(const CIEC_ANY_INT &YEAR, const CIEC_ANY_INT &MONTH, const CIEC_ANY_INT &DAY) {
  return func_LDT_TO_LDATE(func_CONCAT_DT(YEAR, MONTH, DAY, CIEC_LINT(0), CIEC_LINT(0), CIEC_LINT(0), CIEC_LINT(0)));
}

const CIEC_LTIME_OF_DAY func_CONCAT_LTOD(const CIEC_ANY_INT &HOUR, const CIEC_ANY_INT &MINUTE, const CIEC_ANY_INT &SECOND, const CIEC_ANY_INT &MILLISECOND) {
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
  return CIEC_LTIME_OF_DAY(hour + minute + second + millisecond);
}

const CIEC_TIME func_NOW_MONOTONIC() {
  return CIEC_TIME(getNanoSecondsMonotonic());
}

const CIEC_DATE_AND_TIME func_NOW() {
  return CIEC_DATE_AND_TIME(getNanoSecondsRealtime());
}

void func_SPLIT_DATE(const CIEC_DATE &paValue, CIEC_ANY_INT &YEAR, CIEC_ANY_INT &MONTH, CIEC_ANY_INT &DAY) {
  func_SPLIT_LDATE(paValue, YEAR, MONTH, DAY);
}

void func_SPLIT_LDATE(const CIEC_LDATE &paValue, CIEC_ANY_INT &YEAR, CIEC_ANY_INT &MONTH, CIEC_ANY_INT &DAY) {
  struct tm timeStruct;
  paValue.getTimeStruct(&timeStruct);
  YEAR.setUnsignedValue(timeStruct.tm_year + 1900);
  MONTH.setUnsignedValue(timeStruct.tm_mon + 1);
  DAY.setUnsignedValue(timeStruct.tm_mday);
}

void func_SPLIT_TOD(const CIEC_TIME_OF_DAY &paValue, CIEC_ANY_INT &HOUR, CIEC_ANY_INT &MINUTE, CIEC_ANY_INT &SECOND, CIEC_ANY_INT &MILLISECOND) {
  func_SPLIT_LTOD(paValue, HOUR, MINUTE, SECOND, MILLISECOND);
}

void func_SPLIT_LTOD(const CIEC_LTIME_OF_DAY &paValue, CIEC_ANY_INT &HOUR, CIEC_ANY_INT &MINUTE, CIEC_ANY_INT &SECOND, CIEC_ANY_INT &MILLISECOND) {
  CIEC_LTIME_OF_DAY::TValueType nanoSeconds = static_cast<CIEC_LTIME_OF_DAY::TValueType>(paValue);
  CIEC_LTIME_OF_DAY::TValueType hours = nanoSeconds / (3600ULL * CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond);
  HOUR.setUnsignedValue(hours);

  nanoSeconds = nanoSeconds - hours * (3600ULL * CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond);
  CIEC_LTIME_OF_DAY::TValueType minutes = nanoSeconds / (60ULL * CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond);
  MINUTE.setUnsignedValue(minutes);

  nanoSeconds = nanoSeconds - minutes * (60ULL * CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond);
  CIEC_LTIME_OF_DAY::TValueType seconds = nanoSeconds / CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond;
  SECOND.setUnsignedValue(seconds);

  nanoSeconds = nanoSeconds - seconds * CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond;
  CIEC_LTIME_OF_DAY::TValueType milliSeconds = nanoSeconds / (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000ULL);
  MILLISECOND.setUnsignedValue(milliSeconds);
}

void func_SPLIT_DT(const CIEC_DATE_AND_TIME &paValue, CIEC_ANY_INT &YEAR, CIEC_ANY_INT &MONTH, CIEC_ANY_INT &DAY, CIEC_ANY_INT &HOUR, CIEC_ANY_INT &MINUTE, CIEC_ANY_INT &SECOND, CIEC_ANY_INT &MILLISECOND) {
  func_SPLIT_LDT(paValue, YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, MILLISECOND);
}

void func_SPLIT_LDT(const CIEC_LDATE_AND_TIME &paValue, CIEC_ANY_INT &YEAR, CIEC_ANY_INT &MONTH, CIEC_ANY_INT &DAY, CIEC_ANY_INT &HOUR, CIEC_ANY_INT &MINUTE, CIEC_ANY_INT &SECOND, CIEC_ANY_INT &MILLISECOND) {
  struct tm timeStruct;
  paValue.getTimeStruct(&timeStruct);

  YEAR.setUnsignedValue(timeStruct.tm_year + 1900);
  MONTH.setUnsignedValue(timeStruct.tm_mon + 1);
  DAY.setUnsignedValue(timeStruct.tm_mday);
  HOUR.setUnsignedValue(timeStruct.tm_hour);
  MINUTE.setUnsignedValue(timeStruct.tm_min);
  SECOND.setUnsignedValue(timeStruct.tm_sec);
  MILLISECOND.setUnsignedValue(paValue.getMilliSeconds());
}

CIEC_ANY_INT func_DAY_OF_WEEK(const CIEC_LDATE &paValue) {
  struct tm timeStruct;
  paValue.getTimeStruct(&timeStruct);
  return CIEC_ANY_INT(timeStruct.tm_wday);
}

void swapEndianessInplace(CIEC_ARRAY &paValue) {
  for(intmax_t i = paValue.getLowerBound(); i <= paValue.getUpperBound(); i++) {
    CIEC_ANY &element = paValue[i];
    switch (element.getDataTypeID()) {
    case CIEC_ANY::e_BOOL:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_BOOL &>(element)));
      break;
    case CIEC_ANY::e_SINT:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_SINT &>(element)));
      break;
    case CIEC_ANY::e_INT:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_INT &>(element)));
      break;
    case CIEC_ANY::e_DINT:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_DINT &>(element)));
      break;
    case CIEC_ANY::e_LINT:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_LINT &>(element)));
      break;
    case CIEC_ANY::e_USINT:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_USINT &>(element)));
      break;
    case CIEC_ANY::e_UINT:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_UINT &>(element)));
      break;
    case CIEC_ANY::e_UDINT:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_UDINT &>(element)));
      break;
    case CIEC_ANY::e_ULINT:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_ULINT &>(element)));
      break;
    case CIEC_ANY::e_BYTE:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_BYTE &>(element)));
      break;
    case CIEC_ANY::e_WORD:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_WORD &>(element)));
      break;
    case CIEC_ANY::e_DWORD:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_DWORD &>(element)));
      break;
    case CIEC_ANY::e_LWORD:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_LWORD &>(element)));
      break;
    case CIEC_ANY::e_DATE:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_DATE &>(element)));
      break;
    case CIEC_ANY::e_TIME_OF_DAY:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_TIME_OF_DAY &>(element)));
      break;
    case CIEC_ANY::e_DATE_AND_TIME:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_DATE_AND_TIME &>(element)));
      break;
    case CIEC_ANY::e_TIME:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_TIME &>(element)));
      break;
    case CIEC_ANY::e_CHAR:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_CHAR &>(element)));
      break;
    case CIEC_ANY::e_WCHAR:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_WCHAR &>(element)));
      break;
    case CIEC_ANY::e_LDATE:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_LDATE &>(element)));
      break;
    case CIEC_ANY::e_LTIME_OF_DAY:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_LTIME_OF_DAY &>(element)));
      break;
    case CIEC_ANY::e_LDATE_AND_TIME:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_LDATE_AND_TIME &>(element)));
      break;
    case CIEC_ANY::e_LTIME:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_LDATE &>(element)));
      break;
    case CIEC_ANY::e_REAL:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_REAL &>(element)));
      break;
    case CIEC_ANY::e_LREAL:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_LREAL &>(element)));
      break;
    case CIEC_ANY::e_STRING:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_STRING &>(element)));
      break;
    case CIEC_ANY::e_WSTRING:
      element.setValue(swapEndianess(reinterpret_cast<const CIEC_WSTRING &>(element)));
      break;
    case CIEC_ANY::e_ARRAY:
      swapEndianessInplace(reinterpret_cast<CIEC_ARRAY &>(element));
      break;
    case CIEC_ANY::e_STRUCT:
      swapEndianessInplace(reinterpret_cast<CIEC_STRUCT &>(element));
      break;
    default:
      break; // do nothing
    }
  }
}

void swapEndianessInplace(CIEC_STRUCT &paValue) {
  for (size_t i = 0; i < paValue.getStructSize(); ++i) {
    CIEC_ANY &member = *paValue.getMember(i);
    switch (member.getDataTypeID()) {
      case CIEC_ANY::e_BOOL:
        member.setValue(swapEndianess(static_cast<CIEC_BOOL &>(member)));
        break;
      case CIEC_ANY::e_SINT:
        member.setValue(swapEndianess(static_cast<CIEC_SINT &>(member)));
        break;
      case CIEC_ANY::e_INT:
        member.setValue(swapEndianess(static_cast<CIEC_INT &>(member)));
        break;
      case CIEC_ANY::e_DINT:
        member.setValue(swapEndianess(static_cast<CIEC_DINT &>(member)));
        break;
      case CIEC_ANY::e_LINT:
        member.setValue(swapEndianess(static_cast<CIEC_LINT &>(member)));
        break;
      case CIEC_ANY::e_USINT:
        member.setValue(swapEndianess(static_cast<CIEC_USINT &>(member)));
        break;
      case CIEC_ANY::e_UINT:
        member.setValue(swapEndianess(static_cast<CIEC_UINT &>(member)));
        break;
      case CIEC_ANY::e_UDINT:
        member.setValue(swapEndianess(static_cast<CIEC_UDINT &>(member)));
        break;
      case CIEC_ANY::e_ULINT:
        member.setValue(swapEndianess(static_cast<CIEC_ULINT &>(member)));
        break;
      case CIEC_ANY::e_BYTE:
        member.setValue(swapEndianess(static_cast<CIEC_BYTE &>(member)));
        break;
      case CIEC_ANY::e_WORD:
        member.setValue(swapEndianess(static_cast<CIEC_WORD &>(member)));
        break;
      case CIEC_ANY::e_DWORD:
        member.setValue(swapEndianess(static_cast<CIEC_DWORD &>(member)));
        break;
      case CIEC_ANY::e_LWORD:
        member.setValue(swapEndianess(static_cast<CIEC_LWORD &>(member)));
        break;
      case CIEC_ANY::e_DATE:
        member.setValue(swapEndianess(static_cast<CIEC_DATE &>(member)));
        break;
      case CIEC_ANY::e_TIME_OF_DAY:
        member.setValue(swapEndianess(static_cast<CIEC_TIME_OF_DAY &>(member)));
        break;
      case CIEC_ANY::e_DATE_AND_TIME:
        member.setValue(swapEndianess(static_cast<CIEC_DATE_AND_TIME &>(member)));
        break;
      case CIEC_ANY::e_TIME:
        member.setValue(swapEndianess(static_cast<CIEC_TIME &>(member)));
        break;
      case CIEC_ANY::e_CHAR:
        member.setValue(swapEndianess(static_cast<CIEC_CHAR &>(member)));
        break;
      case CIEC_ANY::e_WCHAR:
        member.setValue(swapEndianess(static_cast<CIEC_WCHAR &>(member)));
        break;
      case CIEC_ANY::e_LDATE:
        member.setValue(swapEndianess(static_cast<CIEC_LDATE &>(member)));
        break;
      case CIEC_ANY::e_LTIME_OF_DAY:
        member.setValue(swapEndianess(static_cast<CIEC_LTIME_OF_DAY &>(member)));
        break;
      case CIEC_ANY::e_LDATE_AND_TIME:
        member.setValue(swapEndianess(static_cast<CIEC_LDATE_AND_TIME &>(member)));
        break;
      case CIEC_ANY::e_LTIME:
        member.setValue(swapEndianess(static_cast<CIEC_LDATE &>(member)));
        break;
      case CIEC_ANY::e_REAL:
        member.setValue(swapEndianess(static_cast<CIEC_REAL &>(member)));
        break;
      case CIEC_ANY::e_LREAL:
        member.setValue(swapEndianess(static_cast<CIEC_LREAL &>(member)));
        break;
      case CIEC_ANY::e_STRING:
        member.setValue(swapEndianess(static_cast<CIEC_STRING &>(member)));
        break;
      case CIEC_ANY::e_WSTRING:
        member.setValue(swapEndianess(static_cast<CIEC_WSTRING &>(member)));
        break;
      case CIEC_ANY::e_ARRAY:
        swapEndianessInplace(static_cast<CIEC_ARRAY &>(member));
        break;
      case CIEC_ANY::e_STRUCT:
        swapEndianessInplace(static_cast<CIEC_STRUCT &>(member));
        break;
      default:
        break; //do nothing
    }
  }
}

#ifdef FORTE_LITTLE_ENDIAN
CIEC_ARRAY_DYNAMIC func_TO_BIG_ENDIAN(const CIEC_ARRAY_DYNAMIC &paValue) {
  CIEC_ARRAY_DYNAMIC reversed(paValue);
  swapEndianessInplace(reversed);
  return reversed;
}

CIEC_ARRAY_DYNAMIC func_FROM_BIG_ENDIAN(const CIEC_ARRAY_DYNAMIC &paValue) {
  CIEC_ARRAY_DYNAMIC reversed(paValue);
  swapEndianessInplace(reversed);
  return reversed;
}
#endif
#ifdef FORTE_BIG_ENDIAN
CIEC_ARRAY_DYNAMIC func_TO_LITTLE_ENDIAN(const CIEC_ARRAY_DYNAMIC &paValue) {
  CIEC_ARRAY_DYNAMIC reversed(paValue);
  swapEndianessInplace(reversed);
  return reversed;
}

CIEC_ARRAY_DYNAMIC func_FROM_LITTLE_ENDIAN(const CIEC_ARRAY_DYNAMIC &paValue) {
  CIEC_ARRAY_DYNAMIC reversed(paValue);
  swapEndianessInplace(reversed);
  return reversed;
}
#endif
