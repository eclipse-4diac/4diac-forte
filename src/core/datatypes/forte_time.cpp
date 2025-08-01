/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Primetals Technologies Austria
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl, Ingo Hegny,
 *     Martin Melik Merkumians, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - added parsing for signed time literals
 *   Hesam Rezaee - developing new format for time variables including different time element
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include "core/datatypes/forte_time.h"
#include <string_view>
#include "arch/timerha.h"
#include "core/util/forte_constants.h"
#include "core/util/string_utils.h"

using namespace forte::core::literals;

using namespace std::literals::string_literals;

DEFINE_FIRMWARE_DATATYPE(TIME, "TIME"_STRID)

int CIEC_TIME::fromString(const char *paValue) {
  int nRetVal = -1;
  char *pcEnd;

  TValueType nIntVal = 0;

  if ('t' == tolower(paValue[0])) {
    nRetVal = 1;
    paValue++;
    if (('i' == tolower(paValue[0])) && ('m' == tolower(paValue[1])) && ('e' == tolower(paValue[2]))) {
      paValue += 3;
      nRetVal += 3;
    }

    if ('#' == *paValue) { // the string has to start with T#
      paValue++;
      nRetVal++;
      TValueType nTimeSignFactor = 1;
      if ('+' == *paValue || '-' == *paValue) {
        if ('-' == *paValue) {
          nTimeSignFactor = -1;
        }
        paValue++;
        nRetVal++;
      }
      TValueType nTimeFactor = 1;
      bool bEnd = false;
      do {
        TValueType nBuf = forte::core::util::strtol(paValue, &pcEnd, 10);
        switch (tolower(*pcEnd)) {
          case 'd': nTimeFactor = 24 * 60 * 60 * csmForteTimeBaseUnitsPerSecond; break;
          case 'h': nTimeFactor = 60 * 60 * csmForteTimeBaseUnitsPerSecond; break;
          case 'm':
            if ('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = csmForteTimeBaseUnitsPerSecond / forte::core::constants::cMillisecondsPerSecond;
              ++pcEnd;
            } else {
              nTimeFactor = 60 * csmForteTimeBaseUnitsPerSecond;
            }
            break;
          case 'n':
            if ('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = csmForteTimeBaseUnitsPerSecond / forte::core::constants::cNanosecondsPerSecond;
              ++pcEnd;
            } else {
              return -1;
            }
            break;
          case 's': nTimeFactor = csmForteTimeBaseUnitsPerSecond; break;
          case 'u':
            if ('s' == tolower(*(pcEnd + 1))) {
              nTimeFactor = csmForteTimeBaseUnitsPerSecond / forte::core::constants::cMicrosecondsPerSecond;
              ++pcEnd;
            } else {
              return -1;
            }
            break;
          case '_':
            // ignore leading underscores
            break;
          default:
            if ((pcEnd != paValue) || (0 == nIntVal)) { // we could not parse anything yet so wrong literal
              // we have a number without unit or it is the first entry which we could not pars then this is an error
              return -1;
            }
            // we are in an array and at the end of the literal
            bEnd = true;
            break;
        }
        nRetVal += static_cast<int>(pcEnd - paValue);
        paValue = pcEnd;
        if (!bEnd) {
          ++nRetVal;
          ++paValue;
        }
        nIntVal += (nBuf * nTimeFactor * nTimeSignFactor);
      } while ((!bEnd) && ('\0' != *paValue && ',' != *paValue && ')' != *paValue));

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

namespace {
  constexpr std::string_view MIN_TIME_VALUE = "T#-106751d23h47m16s854ms775us808ns";
  constexpr std::string_view ZERO_NANO_SECOND_TIME_VALUE = "T#0ns";
} // namespace

void CIEC_TIME::toString(std::string &paValue) const {
  TValueType timeValNano = static_cast<TValueType>(*this);

  //  generate the correct string for the min value
  if (timeValNano == std::numeric_limits<CIEC_TIME::TValueType>::min()) {
    paValue += MIN_TIME_VALUE;
    return;
  }

  //  generate the correct string for 0ns
  if (timeValNano == 0) {
    paValue += ZERO_NANO_SECOND_TIME_VALUE;
    return;
  }

  paValue += "T#"s;
  if (timeValNano < 0) {
    paValue += '-';
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

  timeElementsToString(paValue, days, "d"s);
  timeElementsToString(paValue, hoursPart, "h"s);
  timeElementsToString(paValue, minsPart, "m"s);
  timeElementsToString(paValue, secondsPart, "s"s);
  timeElementsToString(paValue, msPart, "ms"s);
  timeElementsToString(paValue, usPart, "us"s);
  timeElementsToString(paValue, nsPart, "ns"s);
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
  return (forte::core::constants::cMillisecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
             ? static_cast<TValueType>(*this) / (static_cast<TValueType>(csmForteTimeBaseUnitsPerSecond) /
                                                 forte::core::constants::cMillisecondsPerSecond)
             : static_cast<TValueType>(*this) *
                   (forte::core::constants::cMillisecondsPerSecond / csmForteTimeBaseUnitsPerSecond);
}

CIEC_TIME::TValueType CIEC_TIME::getInMicroSeconds() const {
  return (forte::core::constants::cMicrosecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
             ? static_cast<TValueType>(*this) / (static_cast<TValueType>(csmForteTimeBaseUnitsPerSecond) /
                                                 forte::core::constants::cMicrosecondsPerSecond)
             : static_cast<TValueType>(*this) *
                   (forte::core::constants::cMicrosecondsPerSecond / csmForteTimeBaseUnitsPerSecond);
}

CIEC_TIME::TValueType CIEC_TIME::getInNanoSeconds() const {
  return (forte::core::constants::cNanosecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
             ? static_cast<TValueType>(*this) / (static_cast<TValueType>(csmForteTimeBaseUnitsPerSecond) /
                                                 forte::core::constants::cNanosecondsPerSecond)
             : static_cast<TValueType>(*this) *
                   (forte::core::constants::cNanosecondsPerSecond / csmForteTimeBaseUnitsPerSecond);
}

void CIEC_TIME::setFromSeconds(TValueType paValue) {
  setLargestInt(paValue * csmForteTimeBaseUnitsPerSecond);
}

void CIEC_TIME::setFromMilliSeconds(TValueType paValue) {
  setLargestInt((forte::core::constants::cMillisecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
                    ? paValue * (csmForteTimeBaseUnitsPerSecond / forte::core::constants::cMillisecondsPerSecond)
                    : paValue / (forte::core::constants::cMillisecondsPerSecond / csmForteTimeBaseUnitsPerSecond));
}

void CIEC_TIME::setFromMicroSeconds(TValueType paValue) {
  setLargestInt((forte::core::constants::cMicrosecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
                    ? paValue * (csmForteTimeBaseUnitsPerSecond / forte::core::constants::cMicrosecondsPerSecond)
                    : paValue / (forte::core::constants::cMicrosecondsPerSecond / csmForteTimeBaseUnitsPerSecond));
}

void CIEC_TIME::setFromNanoSeconds(TValueType paValue) {
  setLargestInt((forte::core::constants::cNanosecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
                    ? paValue * (csmForteTimeBaseUnitsPerSecond / forte::core::constants::cNanosecondsPerSecond)
                    : paValue / (forte::core::constants::cNanosecondsPerSecond / csmForteTimeBaseUnitsPerSecond));
}

const forte::core::StringId forte::CDataTypeTrait<CIEC_TIME>::scmDataTypeName = "TIME"_STRID;
