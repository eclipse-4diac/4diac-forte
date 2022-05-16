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

#ifdef FORTE_USE_REAL_DATATYPE
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
#endif

#ifdef FORTE_USE_LREAL_DATATYPE
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
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
const CIEC_TIME_OF_DAY func_ADD_TOD_TIME(const CIEC_TIME_OF_DAY& pa_roIN1, const CIEC_TIME& pa_roIN2){
  return CIEC_TIME_OF_DAY(static_cast<TForteUInt64>(pa_roIN1 + pa_roIN2.getInMilliSeconds()));
}

const CIEC_DATE_AND_TIME func_ADD_DT_TIME(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_TIME& pa_roIN2){
  return CIEC_DATE_AND_TIME(static_cast<TForteUInt64>(pa_roIN1 + pa_roIN2.getInMilliSeconds()));
}

const CIEC_TIME_OF_DAY func_SUB_TOD_TIME(const CIEC_TIME_OF_DAY& pa_roIN1, const CIEC_TIME& pa_roIN2){
  return CIEC_TIME_OF_DAY(static_cast<TForteUInt64>(pa_roIN1 - pa_roIN2.getInMilliSeconds()));
}

const CIEC_DATE_AND_TIME func_SUB_DT_TIME(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_TIME& pa_roIN2){
  return CIEC_DATE_AND_TIME(static_cast<TForteUInt64>(pa_roIN1 - pa_roIN2.getInMilliSeconds()));
}

const CIEC_TIME func_SUB_DT_DT(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_DATE_AND_TIME& pa_roIN2){
  CIEC_TIME temp;
  temp.setFromMilliSeconds(pa_roIN1 - pa_roIN2);
  return temp;
}

const CIEC_TIME func_SUB_TOD_TOD(const CIEC_TIME_OF_DAY &pa_roIN1, const CIEC_TIME_OF_DAY &pa_roIN2){
  CIEC_TIME temp;
  temp.setFromMilliSeconds(pa_roIN1 - pa_roIN2);
  return temp;
}

const CIEC_TIME func_SUB_DATE_DATE(const CIEC_DATE &pa_roIN1, const CIEC_DATE &pa_roIN2){
  CIEC_TIME temp;
  temp.setFromMilliSeconds(pa_roIN1 - pa_roIN2);
  return temp;
}

const CIEC_DATE_AND_TIME func_CONCAT_DATE_TOD(const CIEC_DATE& pa_roIN1, const CIEC_TIME_OF_DAY& pa_roIN2){
  CIEC_DATE_AND_TIME retVal;

  struct tm *pstTime = pa_roIN1.getTimeStruct();

  time_t t = static_cast<time_t>( pa_roIN2 / 1000ULL);
  pstTime->tm_hour = (int) (t / 3600);
  pstTime->tm_min = (int) ((t % 3600) / 60);
  pstTime->tm_sec = (int) (t % 60);

  retVal.setDateAndTime(*pstTime, pa_roIN2.getMilliSeconds());
  return retVal;
}

CIEC_ANY_INT func_FIND(const CIEC_ANY_STRING& pa_rsIn1, const CIEC_ANY_STRING& pa_rsIn2){
  const char* pc_Find = strstr(pa_rsIn1.getValue(), pa_rsIn2.getValue());
  CIEC_ANY_INT temp;
  if (0 != pc_Find){
    temp.setSignedValue(pc_Find - pa_rsIn1.getValue() + 1);
  }
  return temp;
}

const CIEC_TIME func_NOW_MONOTONIC() {
  CIEC_TIME now;
  now.setFromNanoSeconds(getNanoSecondsMonotonic());
  return now;
}

const CIEC_DATE_AND_TIME func_NOW() {
  return CIEC_DATE_AND_TIME(forte_time() * 1000ULL);
}

#endif
