/*******************************************************************************
  * Copyright (c) 2008 - 2015 nxtControl GmbH, ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Stanislav Meduna, Alois Zoitl, Martin Melik Merkumians
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "forte_date.h"
#include "../../arch/timerha.h"
#include <forte_printer.h>
#include <forte_architecture_time.h>

DEFINE_FIRMWARE_DATATYPE(DATE, g_nStringIdDATE)

int CIEC_DATE::fromString(const char *pa_pacValue){
  // 2007-12-21
  struct tm tm;
  char *acBuffer = const_cast<char*>(pa_pacValue);

  memset(&tm, 0, sizeof(tm));

  if('d' == tolower(*acBuffer)){
    acBuffer++;
    if(('a' == tolower(acBuffer[0])) && ('t' == tolower(acBuffer[1])) && ('e' == tolower(acBuffer[2]))){
      acBuffer += 3;
    }
    if('#' != *acBuffer){
      return -1;
    }
    acBuffer++;
  }

  if('\0' != *acBuffer){
    //TODO think of using any elementary fromString function instead of strtoul
    tm.tm_year = static_cast<int>(::strtoul(acBuffer, &acBuffer, 10) - 1900);
    if('-' == *acBuffer){
      ++acBuffer;
      tm.tm_mon = static_cast<int>(::strtoul(acBuffer, &acBuffer, 10) - 1);
      if('-' == *acBuffer){
        ++acBuffer;
        tm.tm_mday = static_cast<int>(::strtoul(acBuffer, &acBuffer, 10));
      }
      else{
         return -1;
       }
    }
    else{
       return -1;
     }
  }
  else{
    return -1;
  }

  if(!setDateAndTime(tm, 0)){
    return -1;
  }

  return static_cast<unsigned int>(acBuffer - pa_pacValue);
}

int CIEC_DATE::toString(char* pa_pacValue, unsigned int pa_nBufferSize) const{
  int nRetVal = -1;
  struct tm *ptm = getTimeStruct();

  if (ptm == 0)
    return -1;

  nRetVal = forte_snprintf(pa_pacValue, pa_nBufferSize, "%04d-%02d-%02d", 1900 + ptm->tm_year, ptm->tm_mon + 1, ptm->tm_mday);
  if((nRetVal < -1) || (nRetVal >= (int) pa_nBufferSize)){
    nRetVal = -1;
  }
  return nRetVal;
}
