/*******************************************************************************
 * Copyright (c) 2008 - 2015 nxtControl GmbH, ACIN, Profactor GmbH, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Stanislav Meduna, Alois Zoitl, Gerhard Ebenhofer, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "forte_time_of_day.h"
#include "../../arch/timerha.h"
#include <forte_printer.h>

DEFINE_FIRMWARE_DATATYPE(TIME_OF_DAY, g_nStringIdTIME_OF_DAY)

int CIEC_TIME_OF_DAY::fromString(const char *paValue){
  // 15:00:00.000
  struct tm tm;
  int msec = 0;
  char *acBuffer = const_cast<char*>(paValue);

  memset(&tm, 0, sizeof(tm));

  if('t' == tolower(*acBuffer) ){
    if(('o' == tolower(acBuffer[1])) && ('d' == tolower(acBuffer[2]))){
      acBuffer += 3;
    }
    else{
      //TODO maybe allow to turn this check of for small devices
      if((0 == strncmp("time_of_day", acBuffer, 11)) ||
         (0 == strncmp("TIME_OF_DAY", acBuffer, 11))){
        acBuffer += 11;
      }
    }

    if('#' != *acBuffer){
      return -1;
    }
    acBuffer++;
  }

  if('\0' != *acBuffer){
    tm.tm_hour = static_cast<int>(forte::core::util::strtoul(acBuffer, &acBuffer, 10));
    if(':' == *acBuffer){
      ++acBuffer;
      tm.tm_min = static_cast<int>(forte::core::util::strtoul(acBuffer, &acBuffer, 10));
      if(':' == *acBuffer){
        ++acBuffer;
        tm.tm_sec = static_cast<int>(forte::core::util::strtoul(acBuffer, &acBuffer, 10));
        if('.' == *acBuffer){
          unsigned int nNums = 0;
          ++acBuffer;
          while (isdigit(*acBuffer)) {
            msec = 10*msec + forte::core::util::charDigitToInt(*acBuffer);
            ++acBuffer;
            ++nNums;
          }

          if(nNums < 3) {
            for(unsigned int i = nNums; i < 3; ++i) {
              msec *= 10;
            }
          } else {
            for(unsigned int i = 0; i < (nNums - 3); ++i) {
              msec /= 10;
            }
          }
        }
      }
    }
    else{
      if('\0' != *acBuffer){
        return -1;
      }
    }
  }
  else{
    return -1;
  }

  setTUINT64((tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec) * 1000ULL + msec);

  return static_cast<int>(acBuffer - paValue);
}

int CIEC_TIME_OF_DAY::toString(char* paValue, size_t paBufferSize) const {
  TForteUInt64 ntoStingBuffer = getTUINT64();
  time_t t = static_cast<time_t>(ntoStingBuffer / 1000);

  int nRetVal = forte_snprintf(paValue, paBufferSize, "%02d:%02d:%02d.%03d",
      (int) (t / 3600),
      (int) ((t % 3600) / 60),
      (int) (t % 60),
      (int) (ntoStingBuffer % 1000));
  if((nRetVal < -1) || (nRetVal >= static_cast<int>(paBufferSize))) {
    nRetVal = -1;
  }
  return nRetVal;
}

