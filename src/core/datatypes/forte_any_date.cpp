/*******************************************************************************
 * Copyright (c) 2013 ACIN, fortiss GmbH, nxtControl GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Monika Wener, Alois Zoitl, Stansilav Meduna
 *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include <stdlib.h>
#include "forte_any_date.h"

#include <forte_architecture_time.h>

TForteInt32 CIEC_ANY_DATE::sm_nTimeZoneOffset = -1;

TForteInt32 CIEC_ANY_DATE::getTimeZoneOffset(){
  if(sm_nTimeZoneOffset == -1){
    time_t t = 24 * 60 * 60; // 2. 1. 1970 00:00:00 for UTC

    struct tm *ptm = forte_localtime(&t);
    if(ptm->tm_mday < 2) {
      sm_nTimeZoneOffset = 60 * ptm->tm_hour + ptm->tm_min - 24 * 60;
    } else {
      sm_nTimeZoneOffset = 60 * ptm->tm_hour + ptm->tm_min;
    }
  }

  return sm_nTimeZoneOffset;
}

bool CIEC_ANY_DATE::setDateAndTime(struct tm &pa_rstTM, unsigned int pa_unMilliSec){

  pa_rstTM.tm_isdst = -1;

  time_t nTime;
  nTime = forte_mktime(&pa_rstTM);

  if(nTime == (time_t) -1) {
    return false;
  }

  setTUINT64(nTime * 1000ULL + pa_unMilliSec);
  return true;
}

struct tm *CIEC_ANY_DATE::getTimeStruct() const {
  TForteUInt64 nToStringBuffer = getTUINT64();
  time_t nTime = static_cast<time_t>(nToStringBuffer / 1000ULL);

  struct tm *pstRetVal;

  pstRetVal = forte_localtime(&nTime);
  return pstRetVal;
}

unsigned int CIEC_ANY_DATE::getMilliSeconds() const{
  return static_cast<unsigned int>(getTUINT64() % 1000ULL);
}
