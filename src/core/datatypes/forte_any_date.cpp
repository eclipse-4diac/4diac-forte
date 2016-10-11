/*******************************************************************************
  * Copyright (c) 2013 ACIN, fortiss GmbH, nxtControl GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Monika Wener, Alois Zoitl, Stansilav Meduna
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include <stdlib.h>
#include <time.h>
#include "forte_any_date.h"

#if defined(WINCE)
#include <wce_time.h>
#endif

TForteInt32 CIEC_ANY_DATE::sm_nTimeZoneOffset = -1;

TForteInt32 CIEC_ANY_DATE::getTimeZoneOffset(){
  if(sm_nTimeZoneOffset == -1){
    time_t t = 24 * 60 * 60; // 2. 1. 1970 00:00:00 for UTC

#if ! defined(WINCE)
    struct tm *ptm = localtime(&t);
#else
    struct tm *ptm = wceex_localtime(&t);
#endif

    if(ptm->tm_mday < 2)
      sm_nTimeZoneOffset = 60 * ptm->tm_hour + ptm->tm_min - 24 * 60;
    else
      sm_nTimeZoneOffset = 60 * ptm->tm_hour + ptm->tm_min;
  }

  return sm_nTimeZoneOffset;
}

bool CIEC_ANY_DATE::setDateAndTime(struct tm &pa_rstTM, unsigned int pa_unMilliSec){

  pa_rstTM.tm_isdst = -1;

  time_t nTime;
#if ! defined(WINCE)
  nTime = mktime(&pa_rstTM);
#else
  nTime = wceex_mktime(&pa_rstTM);
#endif

  if(nTime == (time_t) -1)
    return false;

  setTUINT64(nTime * 1000ULL + pa_unMilliSec);
  return true;
}

struct tm *CIEC_ANY_DATE::getTimeStruct() const {
  TForteUInt64 nToStringBuffer = getTUINT64();
  time_t nTime = static_cast<time_t>(nToStringBuffer / 1000ULL);

  struct tm *pstRetVal;

#if ! defined(WINCE)
  pstRetVal = localtime(&nTime);
#else
  pstRetVal = wceex_localtime(&nTime);
#endif
  return pstRetVal;
}

unsigned int CIEC_ANY_DATE::getMilliSeconds() const{
  return static_cast<unsigned int>(getTUINT64() % 1000ULL);
}
