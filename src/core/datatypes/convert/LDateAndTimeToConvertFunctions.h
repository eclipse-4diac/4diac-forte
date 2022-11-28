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

#ifndef SRC_CORE_DATATYPES_CONVERT_LDATEANDTIMETOCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_LDATEANDTIMETOCONVERTFUNCTIONS_H_

//********************************************************************************************
//   LDT_TO_**  functions
//********************************************************************************************
inline const CIEC_LDATE func_LDT_TO_LDATE(const CIEC_LDATE_AND_TIME &paVal){
  TForteUInt64 nBuffer = paVal;
  time_t t = static_cast<time_t>(nBuffer / 1000000000ULL);
  struct tm *ptm = forte_localtime(&t);

  if(nullptr == ptm){
    return CIEC_DATE(0);
  }

  ptm->tm_hour = 0;
  ptm->tm_min = 0;
  ptm->tm_sec = 0;

  t = forte_mktime(ptm);
  if((time_t) -1 == t){
    return CIEC_LDATE(0);
  }

  return CIEC_LDATE(t * 1000000000ULL);
}

#endif /* SRC_CORE_DATATYPES_CONVERT_LDATEANDTIMETOCONVERTFUNCTIONS_H_ */
