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
#include "forte_ltime_of_day.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_ltime_of_day_gen.cpp"
#endif
#include "../../arch/timerha.h"
#include <forte_printer.h>

DEFINE_FIRMWARE_DATATYPE(LTIME_OF_DAY, g_nStringIdLTIME_OF_DAY)

int CIEC_LTIME_OF_DAY::fromString(const char *paValue){
  // 15:00:00.000
  struct tm tm;
  int msec = 0;
  char *acBuffer = const_cast<char*>(paValue);

  memset(&tm, 0, sizeof(tm));

  if('l' == tolower(*acBuffer) ){
    if (('t' == tolower(acBuffer[1])) && ('o' == tolower(acBuffer[2])) && ('d' == tolower(acBuffer[3])))
    {
      acBuffer += 4;
    }
    else{
      //TODO maybe allow to turn this check of for small devices
      if((0 == strncmp("ltime_of_day", acBuffer, 12)) ||
         (0 == strncmp("LTIME_OF_DAY", acBuffer, 12))){
        acBuffer += 12;
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

  setTUINT64(((tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec) * 1000ULL + msec) * 1000000ULL);

  return static_cast<int>(acBuffer - paValue);
}

int CIEC_LTIME_OF_DAY::toString(char* paValue, size_t paBufferSize) const {
  TForteUInt64 ntoStingBuffer = getTUINT64();
  time_t t = static_cast<time_t>(ntoStingBuffer / (1000ULL * 1000000ULL));

  int nRetVal = forte_snprintf(paValue, paBufferSize, "LTOD#%02d:%02d:%02d.%03d",
      (int) (t / 3600),
      (int) ((t % 3600) / 60),
      (int) (t % 60),
      (int) ((ntoStingBuffer / 1000000ULL) % 1000ULL));
  if((nRetVal < -1) || (nRetVal >= static_cast<int>(paBufferSize))) {
    nRetVal = -1;
  }
  return nRetVal;
}

size_t CIEC_LTIME_OF_DAY::getToStringBufferSize() const {
  return sizeof("LTOD#00:00:00.000");
}
