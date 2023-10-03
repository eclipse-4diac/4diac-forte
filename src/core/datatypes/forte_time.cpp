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
 *    Hesam Rezaee - developing new format for time variables including different time element
 *******************************************************************************/
#include "forte_time.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_time_gen.cpp"
#endif

#include "../../arch/timerha.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "forte_constants.h"
#include "forte_lint.h"
 

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
            nTimeFactor = 24 * 60 * 60 * csmForteTimeBaseUnitsPerSecond;
            break;
          case 'h':
            nTimeFactor = 60 * 60 * csmForteTimeBaseUnitsPerSecond;
            break;
          case 'm':
            if('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = csmForteTimeBaseUnitsPerSecond / forte::core::constants::cMillisecondsPerSecond;
              ++pcEnd;
            } else {
              nTimeFactor = 60 * csmForteTimeBaseUnitsPerSecond;
            }
            break;
          case 'n':
            if('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = csmForteTimeBaseUnitsPerSecond / forte::core::constants::cNanosecondsPerSecond;
              ++pcEnd;
            } else {
              return -1;
            }
            break;
          case 's':
            nTimeFactor = csmForteTimeBaseUnitsPerSecond;
            break;
          case 'u':
            if('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = csmForteTimeBaseUnitsPerSecond / forte::core::constants::cMicrosecondsPerSecond;
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
      } while((!bEnd) && ('\0' != *paValue && ',' != *paValue && ')' != *paValue));

    } else {
      return -1;
    }
  } else {
    nIntVal = forte::core::util::strtol(paValue, &pcEnd, 10);
    nRetVal = static_cast<int>(pcEnd - paValue);
  }

  setLargestInt(nIntVal);
  return nRetVal;

}

int CIEC_TIME::toString(char *paValue, size_t paBufferSize) const
{
  int nSize = 0;
  int nRetVal = -1;
  if(paBufferSize > 4) {
    TValueType timeValNano = static_cast<TValueType>(*this);

    //  generate the correct string for the min value
    if(timeValNano == std::numeric_limits<CIEC_TIME::TValueType>::min()) {
      if(paBufferSize < sizeof(csmMinTimeValue)) {
        return -1;
      }
      strncpy(paValue, csmMinTimeValue, paBufferSize);
      return sizeof(csmMinTimeValue) - 1;
    }

    //  generate the correct string for 0ns
    if(timeValNano == 0) {
      if(paBufferSize < sizeof(csmZeroNanoSecondTimeValue)) {
        return -1;
      }
      strncpy(paValue, csmZeroNanoSecondTimeValue, paBufferSize);
      return sizeof(csmZeroNanoSecondTimeValue) - 1;
    }

    paValue[0] = 'T';
    paValue[1] = '#';
    nRetVal += 3; // Compensate for -1 start value
    if(timeValNano < 0) {
      paValue[2] = '-';
      nRetVal++;
    }
    timeValNano = llabs(timeValNano);

    // process days, hours, minutes, seconds, milliseconds, microseconds and nano seconds
    const int64_t nsPart = timeValNano % 1000;
    const int64_t usPart = timeValNano / csmForteTimeBaseUnitsPerMicroSecond % 1000;
    const int64_t msPart = timeValNano / csmForteTimeBaseUnitsPerMilliSecond % 1000;
    const int64_t secondsPart = timeValNano / csmForteTimeBaseUnitsPerSecond % 60;
    const int64_t minsPart = timeValNano / csmForteTimeBaseUnitsPerMinute % 60;
    const int64_t hoursPart = timeValNano / csmForteTimeBaseUnitsPerHour % 24;
    const int64_t days = timeValNano / csmForteTimeBaseUnitsPerDay;

    // change time elements to string
    nSize = timeElementsToString(days, paValue, nRetVal, paBufferSize, "d");
    if(nSize == -1) {
      return -1;
    } else {
      nRetVal +=nSize;
    }
    nSize = CIEC_ANY_DURATION::timeElementsToString(hoursPart, paValue, nRetVal, paBufferSize, "h");
    if(nSize == -1) {
      return -1;
    } else {
      nRetVal +=nSize;
    }
    nSize = CIEC_ANY_DURATION::timeElementsToString(minsPart, paValue, nRetVal, paBufferSize, "m");
    if(nSize == -1) {
      return -1;
    } else {
      nRetVal +=nSize;
    }
    nSize = CIEC_ANY_DURATION::timeElementsToString(secondsPart, paValue, nRetVal, paBufferSize, "s");
    if(nSize == -1) {
      return -1;
    } else {
      nRetVal +=nSize;
    }
    nSize = CIEC_ANY_DURATION::timeElementsToString(msPart, paValue, nRetVal, paBufferSize, "ms");
    if(nSize == -1) {
      return -1;
    } else {
      nRetVal +=nSize;
    }
    nSize = CIEC_ANY_DURATION::timeElementsToString(usPart, paValue, nRetVal, paBufferSize, "us");
    if(nSize == -1) {
      return -1;
    } else {
      nRetVal +=nSize;
    }
    nSize = CIEC_ANY_DURATION::timeElementsToString(nsPart, paValue, nRetVal, paBufferSize, "ns");
    if(nSize == -1) {
      return -1;
    } else {
      nRetVal +=nSize;
    }
  }

  return nRetVal;
}


CIEC_TIME::TValueType CIEC_TIME::getInDays() const {
  return static_cast<TValueType>(*this) / static_cast<TValueType>(csmForteTimeBaseUnitsPerDay);
}

CIEC_TIME::TValueType CIEC_TIME::getInHours() const {
  return static_cast<TValueType>(*this) / static_cast<TValueType>(csmForteTimeBaseUnitsPerHour);
}

CIEC_TIME::TValueType CIEC_TIME::getInMinutes() const {
  return static_cast<TValueType>(*this) / static_cast<TValueType>(csmForteTimeBaseUnitsPerMinute);
}

CIEC_TIME::TValueType CIEC_TIME::getInSeconds() const {
  return static_cast<TValueType>(*this) / static_cast<TValueType>(csmForteTimeBaseUnitsPerSecond);
}

CIEC_TIME::TValueType CIEC_TIME::getInMilliSeconds() const {
  return
      (forte::core::constants::cMillisecondsPerSecond < csmForteTimeBaseUnitsPerSecond) ?
        static_cast<TValueType>(*this) / (static_cast<TValueType>(csmForteTimeBaseUnitsPerSecond) / forte::core::constants::cMillisecondsPerSecond) :
        static_cast<TValueType>(*this) * (forte::core::constants::cMillisecondsPerSecond / csmForteTimeBaseUnitsPerSecond);
}

CIEC_TIME::TValueType CIEC_TIME::getInMicroSeconds() const {
  return
      (forte::core::constants::cMicrosecondsPerSecond < csmForteTimeBaseUnitsPerSecond) ?
        static_cast<TValueType>(*this) / (static_cast<TValueType>(csmForteTimeBaseUnitsPerSecond) / forte::core::constants::cMicrosecondsPerSecond) :
        static_cast<TValueType>(*this) * (forte::core::constants::cMicrosecondsPerSecond / csmForteTimeBaseUnitsPerSecond);
}

CIEC_TIME::TValueType CIEC_TIME::getInNanoSeconds() const {
  return
      (forte::core::constants::cNanosecondsPerSecond < csmForteTimeBaseUnitsPerSecond) ?
        static_cast<TValueType>(*this) / (static_cast<TValueType>(csmForteTimeBaseUnitsPerSecond) / forte::core::constants::cNanosecondsPerSecond) :
        static_cast<TValueType>(*this) * (forte::core::constants::cNanosecondsPerSecond / csmForteTimeBaseUnitsPerSecond);
}

void CIEC_TIME::setFromSeconds(TValueType paValue) {
  setLargestInt(paValue * csmForteTimeBaseUnitsPerSecond);
}

void CIEC_TIME::setFromMilliSeconds(TValueType paValue) {
  setLargestInt
      ((forte::core::constants::cMillisecondsPerSecond < csmForteTimeBaseUnitsPerSecond) ?
              paValue * (csmForteTimeBaseUnitsPerSecond / forte::core::constants::cMillisecondsPerSecond) :
              paValue / (forte::core::constants::cMillisecondsPerSecond / csmForteTimeBaseUnitsPerSecond));
}

void CIEC_TIME::setFromMicroSeconds(TValueType paValue) {
  setLargestInt(
        (forte::core::constants::cMicrosecondsPerSecond < csmForteTimeBaseUnitsPerSecond) ?
          paValue * (csmForteTimeBaseUnitsPerSecond / forte::core::constants::cMicrosecondsPerSecond) :
          paValue / (forte::core::constants::cMicrosecondsPerSecond / csmForteTimeBaseUnitsPerSecond));
}

void CIEC_TIME::setFromNanoSeconds(TValueType paValue) {
  setLargestInt(
        (forte::core::constants::cNanosecondsPerSecond < csmForteTimeBaseUnitsPerSecond) ?
          paValue * (csmForteTimeBaseUnitsPerSecond / forte::core::constants::cNanosecondsPerSecond) :
          paValue / (forte::core::constants::cNanosecondsPerSecond / csmForteTimeBaseUnitsPerSecond));
}

size_t CIEC_TIME::getToStringBufferSize() const {
  return sizeof("T#-18446744073709551615ns");
}
