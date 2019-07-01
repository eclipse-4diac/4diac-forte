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
# include "forte_lint.h"
#else
# include "forte_dint.h"
#endif

static const CIEC_TIME::TValueType cMillisecondsPerSecond = 1E3;
static const CIEC_TIME::TValueType cMicrosecondsPerSecond = 1E6;
static const CIEC_TIME::TValueType cNanosecondsPerSecond = 1E9;

DEFINE_FIRMWARE_DATATYPE(TIME, g_nStringIdTIME)

int CIEC_TIME::fromString(const char *paValue) {
  int nRetVal = -1;
  char* pcEnd;

  TValueType nIntVal = 0;

  if((paValue[0] == 'T') || (paValue[0] == 't')) {
    nRetVal = 1;
    paValue++;
    if(('i' == tolower(paValue[0])) && ('m' == tolower(paValue[1])) && ('e' == tolower(paValue[2]))) {
      paValue += 3;
      nRetVal += 3;
    }

    if(*paValue == '#') { // the string has to start with T#
      paValue++;
      nRetVal++;
      TValueType nTimeFactor = 1;
      bool bEnd = false;
      do {
        TValueType nBuf = forte::core::util::strtol(paValue, &pcEnd, 10);
        switch(tolower(*pcEnd)){
          case 'd':
            nTimeFactor = 24 * 60 * 60 * cgForteTimeBaseUnitsPerSecond ;
            break;

          case 'h':
            nTimeFactor = 60 * 60 * cgForteTimeBaseUnitsPerSecond;
            break;

          case 'm':
            if('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = cgForteTimeBaseUnitsPerSecond / cMillisecondsPerSecond;
              ++pcEnd;
            } else {
              nTimeFactor = 60 * cgForteTimeBaseUnitsPerSecond;
            }
            break;
          case 'n':
            if('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = cgForteTimeBaseUnitsPerSecond / cNanosecondsPerSecond;
            } else {
              bEnd = true;
            }
            break;
          case 's':
            nTimeFactor = cgForteTimeBaseUnitsPerSecond;
            break;
          case 'u':
            if('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = cgForteTimeBaseUnitsPerSecond / cMicrosecondsPerSecond;
            } else {
              bEnd = true;
            }
            break;
          case '_':
            //ignore leading underscores
            break;
          default:
            if(paValue == pcEnd) {
              //we couldn't parse anything
              return -1;
            }
            bEnd = true;
            break;
        }
        nRetVal += static_cast<int>(pcEnd - paValue);
        if(!bEnd) {
          ++nRetVal;
        }
        paValue = pcEnd + 1;
        nIntVal += (nBuf * nTimeFactor);
      } while(('\0' != *paValue) && (!bEnd));
    } else {
      return -1;
    }
  } else {
    nIntVal = forte::core::util::strtol(paValue, &pcEnd, 10);
    nRetVal = static_cast<int>(pcEnd - paValue);
  }

  *this = nIntVal;
  return nRetVal;

}

int CIEC_TIME::toString(char* paValue, size_t paBufferSize) const {
  int nRetVal = -1;
  if(paBufferSize > 4) {
#ifdef FORTE_USE_64BIT_DATATYPES
    CIEC_LINT timeVal(getInMilliSeconds());
#else //FORTE_USE_64BIT_DATATYPES
    CIEC_DINT timeVal(getInMilliSeconds());
#endif //FORTE_USE_64BIT_DATATYPES
    nRetVal = timeVal.toString(paValue + 2, paBufferSize - 4);
    if(-1 != nRetVal) {
      paValue[0] = 'T';
      paValue[1] = '#';
      nRetVal += 2;
      timeVal = getInMicroSeconds();
      timeVal = timeVal % 1000; //we only want the microseconds
      if(0 != timeVal) {
        //If we have a microsecond value add it to the literal
        paValue[nRetVal] = '.';
        ++nRetVal;
        if(timeVal < 100) {
          paValue[nRetVal] = '0';
          ++nRetVal;
          if(timeVal < 10) {
            paValue[nRetVal] = '0';
            ++nRetVal;
          }
        }
        int size = timeVal.toString(paValue + nRetVal, paBufferSize - nRetVal);
        if(-1 == size) {
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
  return static_cast<TValueType>(*this) / cgForteTimeBaseUnitsPerSecond;
}

CIEC_TIME::TValueType CIEC_TIME::getInMilliSeconds() const {
  return
      (cMillisecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ? static_cast<TValueType>(*this) / (cgForteTimeBaseUnitsPerSecond / cMillisecondsPerSecond) :
        static_cast<TValueType>(*this) * (cMillisecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}

CIEC_TIME::TValueType CIEC_TIME::getInMicroSeconds() const {
  return
      (cMicrosecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ? static_cast<TValueType>(*this) / (cgForteTimeBaseUnitsPerSecond / cMicrosecondsPerSecond) :
        static_cast<TValueType>(*this) * (cMicrosecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}

CIEC_TIME::TValueType CIEC_TIME::getInNanoSeconds() const {
  return
      (cNanosecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ? static_cast<TValueType>(*this) / (cgForteTimeBaseUnitsPerSecond / cNanosecondsPerSecond) :
        static_cast<TValueType>(*this) * (cNanosecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}

void CIEC_TIME::setFromSeconds(TValueType paValue) {
  *this = paValue * cgForteTimeBaseUnitsPerSecond;
}

void CIEC_TIME::setFromMilliSeconds(TValueType paValue) {
  *this =
      (cMillisecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ? paValue * (cgForteTimeBaseUnitsPerSecond / cMillisecondsPerSecond) :
        paValue / (cMillisecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}

void CIEC_TIME::setFromMicroSeconds(TValueType paValue) {
  *this =
      (cMicrosecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ? paValue * (cgForteTimeBaseUnitsPerSecond / cMicrosecondsPerSecond) :
        paValue / (cMicrosecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}

void CIEC_TIME::setFromNanoSeconds(TValueType paValue) {
  *this =
      (cNanosecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ? paValue * (cgForteTimeBaseUnitsPerSecond / cNanosecondsPerSecond) :
        paValue / (cNanosecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}
