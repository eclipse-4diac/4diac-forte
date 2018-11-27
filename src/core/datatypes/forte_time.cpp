/*******************************************************************************
  * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Ingo Hegny,
  *    Martin Melik Merkumians, Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include "forte_time.h"

#include "../../arch/timerha.h"
#include <stdlib.h>
#include <ctype.h>

#ifdef FORTE_USE_64BIT_DATATYPES
  #include "forte_lint.h"
#else
  #include "forte_dint.h"
#endif


#define MILISECONDS_PER_SECOND 1000
#define MICROSECONDS_PER_SECOND 1000000

DEFINE_FIRMWARE_DATATYPE(TIME, g_nStringIdTIME)

int CIEC_TIME::fromString(const char *paValue){
  int nRetVal = -1;
  char* pcEnd;

  TValueType nIntVal = 0;

  if((paValue[0] == 'T') || (paValue[0] == 't')){
    nRetVal = 1;
    paValue++;
    if(('i' == tolower(paValue[0])) && ('m' == tolower(paValue[1])) && ('e' == tolower(paValue[2]))){
      paValue += 3;
      nRetVal += 3;
    }

    if(*paValue == '#'){ // the string has to start with T#
      paValue++;
      nRetVal++;
      long nTimeFactor = 1;
      bool bEnd = false;
      do{
        long nBuf = forte::core::util::strtol(paValue, &pcEnd, 10);
        switch (tolower(*pcEnd)){
          case 'd':
            nTimeFactor = 86400000;
            break;

          case 'h':
            nTimeFactor = 3600000;
            break;

          case 'm':
            if('s' == *(pcEnd + 1) || 'S' == *(pcEnd + 1)){
              nTimeFactor = 1;
              ++pcEnd;
            }
            else{
              nTimeFactor = 60000;
            }
            break;

          case 's':
            nTimeFactor = 1000;
            break;
          case '_':
            //ignore leading underscores
            break;
          default:
            if(paValue == pcEnd){
              //we couldn't parse anything
              return -1;
            }
            bEnd = true;
            break;
        }
        nRetVal += static_cast<int>(pcEnd - paValue);
        if(!bEnd){
          ++nRetVal;
        }
        paValue = pcEnd + 1;
        nIntVal += (nBuf * nTimeFactor);
      } while(('\0' != *paValue) && (!bEnd));
    }
    else{
      return -1;
    }
  }
  else{
    nIntVal = forte::core::util::strtol(paValue, &pcEnd, 10);
    nRetVal = static_cast<int>(pcEnd - paValue);
  }

  //the intval is represented in ms
  setFromMiliSeconds(nIntVal);
  return nRetVal;

}

int CIEC_TIME::toString(char* paValue, unsigned int paBufferSize) const{
  int nRetVal = -1;
  if(paBufferSize > 4){
#ifdef FORTE_USE_64BIT_DATATYPES
    CIEC_LINT timeVal(getInMiliSeconds());
#else
    CIEC_DINT timeVal(getInMiliSeconds());
#endif
    nRetVal = timeVal.toString(paValue + 2, paBufferSize - 4);
    if(-1 != nRetVal){
      paValue[0] = 'T';
      paValue[1] = '#';
      nRetVal += 2;
      timeVal = getInMicroSeconds();
      timeVal = timeVal % 1000; //we only want the microseconds
      if(0 != timeVal){
        //If we have a microsecond value add it to the literal
        paValue[nRetVal] = '.';
        ++nRetVal;
        if(timeVal < 100){
          paValue[nRetVal] = '0';
          ++nRetVal;
          if(timeVal < 10){
            paValue[nRetVal] = '0';
            ++nRetVal;
          }
        }
        int size = timeVal.toString(paValue + nRetVal, paBufferSize - nRetVal);
        if(-1 == size){
          return size;
        }
        nRetVal += size;
      }
      paValue[nRetVal] = 'm';
      paValue[nRetVal + 1] = 's';
      paValue[nRetVal + 2] = '\0';
      nRetVal += 2;
    }
  }
  return nRetVal;
}

CIEC_TIME::TValueType CIEC_TIME::getInSeconds() const {
  return (TValueType) *this / FORTE_TIME_BASE_UNITS_PER_SECOND;
}

CIEC_TIME::TValueType CIEC_TIME::getInMiliSeconds() const{
  TValueType nRetVal;
  #if MILISECONDS_PER_SECOND < FORTE_TIME_BASE_UNITS_PER_SECOND
    nRetVal = (TValueType) *this / (FORTE_TIME_BASE_UNITS_PER_SECOND / MILISECONDS_PER_SECOND);
  #else
    nRetVal = (TValueType) *this * (MILISECONDS_PER_SECOND / FORTE_TIME_BASE_UNITS_PER_SECOND);
  #endif
  return nRetVal;
}

CIEC_TIME::TValueType CIEC_TIME::getInMicroSeconds() const{
  TValueType nRetVal;
  #if MICROSECONDS_PER_SECOND < FORTE_TIME_BASE_UNITS_PER_SECOND
    nRetVal = (TValueType) *this / (FORTE_TIME_BASE_UNITS_PER_SECOND / 1000000);
  #else
    nRetVal = (TValueType) *this * (MICROSECONDS_PER_SECOND / FORTE_TIME_BASE_UNITS_PER_SECOND);
  #endif
    //fprintf(stderr,"time us: %lld\n",nRetVal);
  return nRetVal;
}

void CIEC_TIME::setFromSeconds(TValueType paValue){
  *this = paValue * FORTE_TIME_BASE_UNITS_PER_SECOND;
}

void CIEC_TIME::setFromMiliSeconds(TValueType paValue){
  #if MILISECONDS_PER_SECOND < FORTE_TIME_BASE_UNITS_PER_SECOND
    *this = paValue * (FORTE_TIME_BASE_UNITS_PER_SECOND / MILISECONDS_PER_SECOND);
  #else
    *this = paValue / (MILISECONDS_PER_SECOND / FORTE_TIME_BASE_UNITS_PER_SECOND);
  #endif
}

void CIEC_TIME::setFromMicroSeconds(TValueType paValue){
  #if MICROSECONDS_PER_SECOND < FORTE_TIME_BASE_UNITS_PER_SECOND
    *this = paValue * (FORTE_TIME_BASE_UNITS_PER_SECOND / MICROSECONDS_PER_SECOND);
  #else
    *this = paValue / (MICROSECONDS_PER_SECOND / FORTE_TIME_BASE_UNITS_PER_SECOND);
  #endif
}
