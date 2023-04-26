/*******************************************************************************
 * Copyright (c) 2008 - 2015 nxtControl GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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
#include "forte_ldate.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_ldate_gen.cpp"
#endif
#include "../../arch/timerha.h"
#include <forte_printer.h>
#include <forte_architecture_time.h>

DEFINE_FIRMWARE_DATATYPE(LDATE, g_nStringIdLDATE)

int CIEC_LDATE::fromString(const char *paValue){
  // 2007-12-21
  struct tm tm;
  char *acBuffer = const_cast<char*>(paValue);

  memset(&tm, 0, sizeof(tm));

  if('l' == tolower(acBuffer[0]) && 'd' == tolower(acBuffer[1]) ){
    acBuffer += 2;
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

  return static_cast<unsigned int>(acBuffer - paValue);
}

int CIEC_LDATE::toString(char* paValue, size_t paBufferSize) const {
  int nRetVal = -1;
  struct tm ptm;

  if (nullptr != getTimeStruct(&ptm)) {
    nRetVal = forte_snprintf(paValue, paBufferSize, "LD#%04d-%02d-%02d", 1900 + ptm.tm_year, ptm.tm_mon + 1, ptm.tm_mday);
    if((nRetVal < -1) || (nRetVal >= static_cast<int>(paBufferSize))) {
      nRetVal = -1;
    }
  }
  return nRetVal;
}

size_t CIEC_LDATE::getToStringBufferSize() const {
  return sizeof("LD#1970-01-01");
}
