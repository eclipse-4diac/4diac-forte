/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH,
 *  2019 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Ingo Hegny,
 *    Martin Melik Merkumians, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - added parsing for signed time literals
 *******************************************************************************/
#include "forte_time.h"

#include "../../arch/timerha.h"
#include <stdlib.h>
#include <ctype.h>

#include "forte_constants.h"

#ifdef FORTE_USE_64BIT_DATATYPES
# include "forte_lint.h"
#else
# include "forte_dint.h"
#endif

DEFINE_FIRMWARE_DATATYPE(TIME, g_nStringIdTIME)

int CIEC_TIME::fromString(const char *paValue) {
  int nRetVal = -1;
  char* pcEnd;

  TValueType nIntVal = 0;

  if('t' == tolower(paValue[0])) {
    nRetVal = 1;
    paValue++;
    if(('i' == tolower(paValue[0])) && ('m' == tolower(paValue[1])) && ('e' == tolower(paValue[2]))) {
      paValue += 3;
      nRetVal += 3;
    }

    if('#' == *paValue) { // the string has to start with T#
      paValue++;
      nRetVal++;
      TValueType nTimeSignFactor = 1;
      if('+' == *paValue || '-' == *paValue) {
        if('-' == *paValue) {
          nTimeSignFactor = -1;
        }
        paValue++;
        nRetVal++;
      }
      TValueType nTimeFactor = 1;
      bool bEnd = false;
      do {
        TValueType nBuf = forte::core::util::strtol(paValue, &pcEnd, 10);
        switch(tolower(*pcEnd)){
          case 'd':
            nTimeFactor = 24 * 60 * 60 * cgForteTimeBaseUnitsPerSecond;
            break;
          case 'h':
            nTimeFactor = 60 * 60 * cgForteTimeBaseUnitsPerSecond;
            break;
          case 'm':
            if('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = cgForteTimeBaseUnitsPerSecond / forte::core::constants::cMillisecondsPerSecond;
              ++pcEnd;
            } else {
              nTimeFactor = 60 * cgForteTimeBaseUnitsPerSecond;
            }
            break;
          case 'n':
            if('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = cgForteTimeBaseUnitsPerSecond / forte::core::constants::cNanosecondsPerSecond;
              ++pcEnd;
            } else {
              return -1;
            }
            break;
          case 's':
            nTimeFactor = cgForteTimeBaseUnitsPerSecond;
            break;
          case 'u':
            if('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = cgForteTimeBaseUnitsPerSecond / forte::core::constants::cMicrosecondsPerSecond;
              ++pcEnd;
            } else {
              return -1;
            }
            break;
          case '_':
            //ignore leading underscores
            break;
          default:
            if((pcEnd != paValue) || (0 == nIntVal)){   //we could not parse anything yet so wrong literal
              //we have a number without unit or it is the first entry which we could not pars then this is an error
              return -1;
            }
             // we are in an array and at the end of the literal
            bEnd = true;
            break;
        }
        nRetVal += static_cast<int>(pcEnd - paValue);
        paValue = pcEnd;
        if(!bEnd) {
          ++nRetVal;
          ++paValue;
        }
        nIntVal += (nBuf * nTimeFactor * nTimeSignFactor);
      } while((!bEnd) && ('\0' != *paValue));

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
  return static_cast<TValueType>(*this) / static_cast<TValueType>(cgForteTimeBaseUnitsPerSecond);
}

CIEC_TIME::TValueType CIEC_TIME::getInMilliSeconds() const {
  return
      (forte::core::constants::cMillisecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ?
        static_cast<TValueType>(*this) / (static_cast<TValueType>(cgForteTimeBaseUnitsPerSecond) / forte::core::constants::cMillisecondsPerSecond) :
        static_cast<TValueType>(*this) * (forte::core::constants::cMillisecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}

CIEC_TIME::TValueType CIEC_TIME::getInMicroSeconds() const {
  return
      (forte::core::constants::cMicrosecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ?
        static_cast<TValueType>(*this) / (static_cast<TValueType>(cgForteTimeBaseUnitsPerSecond) / forte::core::constants::cMicrosecondsPerSecond) :
        static_cast<TValueType>(*this) * (forte::core::constants::cMicrosecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}

CIEC_TIME::TValueType CIEC_TIME::getInNanoSeconds() const {
  return
      (forte::core::constants::cNanosecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ?
        static_cast<TValueType>(*this) / (static_cast<TValueType>(cgForteTimeBaseUnitsPerSecond) / forte::core::constants::cNanosecondsPerSecond) :
        static_cast<TValueType>(*this) * (forte::core::constants::cNanosecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}

void CIEC_TIME::setFromSeconds(TValueType paValue) {
  *this = paValue * cgForteTimeBaseUnitsPerSecond;
}

void CIEC_TIME::setFromMilliSeconds(TValueType paValue) {
  *this =
      (forte::core::constants::cMillisecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ?
        paValue * (cgForteTimeBaseUnitsPerSecond / forte::core::constants::cMillisecondsPerSecond) :
        paValue / (forte::core::constants::cMillisecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}

void CIEC_TIME::setFromMicroSeconds(TValueType paValue) {
  *this =
      (forte::core::constants::cMicrosecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ?
        paValue * (cgForteTimeBaseUnitsPerSecond / forte::core::constants::cMicrosecondsPerSecond) :
        paValue / (forte::core::constants::cMicrosecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}

void CIEC_TIME::setFromNanoSeconds(TValueType paValue) {
  *this =
      (forte::core::constants::cNanosecondsPerSecond < cgForteTimeBaseUnitsPerSecond) ?
        paValue * (cgForteTimeBaseUnitsPerSecond / forte::core::constants::cNanosecondsPerSecond) :
        paValue / (forte::core::constants::cNanosecondsPerSecond / cgForteTimeBaseUnitsPerSecond);
}
