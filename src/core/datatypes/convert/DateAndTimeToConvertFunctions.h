/*******************************************************************************
 * Copyright (c) 2010-2019 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *               2022 Primetals Technologies Austria GmbH
 *              
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

#ifndef SRC_CORE_DATATYPES_CONVERT_DATEANDTIMETOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_DATEANDTIMETOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   DT_TO_**  functions
//********************************************************************************************
inline const CIEC_LDATE_AND_TIME func_DT_TO_LDT(const CIEC_DATE_AND_TIME &paValue) {
  return CIEC_LDATE_AND_TIME(paValue);
}

inline const CIEC_DATE func_DT_TO_DATE(const CIEC_DATE_AND_TIME &paVal) {
  TForteUInt64 nBuffer = paVal;
  time_t t = static_cast<time_t>(nBuffer / 1000000000ULL);
  struct tm ptm;

  if(nullptr == forte_gmtime(&t, &ptm)) {
    return CIEC_DATE(0);
  }

  ptm.tm_hour = 0;
  ptm.tm_min = 0;
  ptm.tm_sec = 0;

  t = forte_timegm(&ptm);
  if(static_cast<time_t>(-1) == t){
    return CIEC_DATE(0);
  }

  return CIEC_DATE(t * 1000000000ULL);
}

inline const CIEC_TIME_OF_DAY func_DT_TO_TOD(const CIEC_DATE_AND_TIME &paVal){
  TForteUInt64 nBuffer = paVal;
  time_t t = static_cast<time_t>(nBuffer/1000000000ULL);
  struct tm ptm;

  if(nullptr == forte_gmtime(&t, &ptm)) {
    return CIEC_TIME_OF_DAY(0);
  }

  return CIEC_TIME_OF_DAY(static_cast<TForteUInt64>((ptm.tm_hour * UINT64_C(3600) + ptm.tm_min * UINT64_C(60) + ptm.tm_sec) * UINT64_C(1000000000) + (nBuffer % UINT64_C(1000000000))));
}

inline const CIEC_LTIME_OF_DAY func_DT_TO_LTOD(const CIEC_DATE_AND_TIME &paValue) {
  return CIEC_LTIME_OF_DAY(func_DT_TO_TOD(paValue));
}

#endif /* SRC_CORE_DATATYPES_CONVERT_DATEANDTIMETOCONVERTFUNCTIONS_H_ */
