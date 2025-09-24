/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
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
 *                - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - added parsing for signed time literals
 *   Hesam Rezaee - developing new format for time variables including different
 *                  time element
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include "forte/datatypes/forte_ltime.h"
#include "forte/util/forte_constants.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;
using namespace std::literals::string_literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(LTIME, "LTIME"_STRID)

  int CIEC_LTIME::fromString(const char *paValue) {
    int nRetVal = -1;
    char *pcEnd;

    TValueType nIntVal = 0;

    if (('l' == tolower(paValue[0])) && ('t' == tolower(paValue[1]))) {
      nRetVal = 2;
      paValue += 2;
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
          TValueType nBuf = util::strtol(paValue, &pcEnd, 10);
          switch (tolower(*pcEnd)) {
            case 'd': nTimeFactor = 24 * 60 * 60 * csmForteTimeBaseUnitsPerSecond; break;
            case 'h': nTimeFactor = 60 * 60 * csmForteTimeBaseUnitsPerSecond; break;
            case 'm':
              if ('s' == tolower(*(pcEnd + 1))) {
                nTimeFactor = csmForteTimeBaseUnitsPerSecond / util::constants::cMillisecondsPerSecond;
                ++pcEnd;
              } else {
                nTimeFactor = 60 * csmForteTimeBaseUnitsPerSecond;
              }
              break;
            case 'n':
              if ('s' == tolower(*(pcEnd + 1))) {
                nTimeFactor = csmForteTimeBaseUnitsPerSecond / util::constants::cNanosecondsPerSecond;
                ++pcEnd;
              } else {
                return -1;
              }
              break;
            case 's': nTimeFactor = csmForteTimeBaseUnitsPerSecond; break;
            case 'u':
              if ('s' == tolower(*(pcEnd + 1))) {
                nTimeFactor = csmForteTimeBaseUnitsPerSecond / util::constants::cMicrosecondsPerSecond;
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
        } while ((!bEnd) && ('\0' != *paValue));

      } else {
        return -1;
      }
    } else {
      nIntVal = util::strtol(paValue, &pcEnd, 10);
      nRetVal = static_cast<int>(pcEnd - paValue);
    }

    setLargestInt(nIntVal);
    return nRetVal;
  }

  namespace {
    constexpr std::string_view MIN_TIME_VALUE = "LT#-106751d23h47m16s854ms775us808ns";
    constexpr std::string_view ZERO_NANO_SECOND_TIME_VALUE = "LT#0ns";
  } // namespace

  void CIEC_LTIME::toString(std::string &paTargetBuf) const {
    TValueType timeValNano = static_cast<TValueType>(*this);

    //  generate the correct string for the min value
    if (timeValNano == std::numeric_limits<TValueType>::min()) {
      paTargetBuf += MIN_TIME_VALUE;
      return;
    }

    //  generate the correct string for 0ns
    if (timeValNano == 0) {
      paTargetBuf += ZERO_NANO_SECOND_TIME_VALUE;
      return;
    }

    paTargetBuf += "LT#"s;
    if (timeValNano < 0) {
      paTargetBuf += '-';
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

    timeElementsToString(paTargetBuf, days, "d"s);
    timeElementsToString(paTargetBuf, hoursPart, "h"s);
    timeElementsToString(paTargetBuf, minsPart, "m"s);
    timeElementsToString(paTargetBuf, secondsPart, "s"s);
    timeElementsToString(paTargetBuf, msPart, "ms"s);
    timeElementsToString(paTargetBuf, usPart, "us"s);
    timeElementsToString(paTargetBuf, nsPart, "ns"s);
  }

  CIEC_LTIME::TValueType CIEC_LTIME::getInDays() const {
    return static_cast<TValueType>(*this) / static_cast<TValueType>(csmForteTimeBaseUnitsPerDay);
  }

  CIEC_LTIME::TValueType CIEC_LTIME::getInHours() const {
    return static_cast<TValueType>(*this) / static_cast<TValueType>(csmForteTimeBaseUnitsPerHour);
  }

  CIEC_LTIME::TValueType CIEC_LTIME::getInMinutes() const {
    return static_cast<TValueType>(*this) / static_cast<TValueType>(csmForteTimeBaseUnitsPerMinute);
  }

  CIEC_LTIME::TValueType CIEC_LTIME::getInSeconds() const {
    return static_cast<TValueType>(*this) / static_cast<TValueType>(csmForteTimeBaseUnitsPerSecond);
  }

  CIEC_LTIME::TValueType CIEC_LTIME::getInMilliSeconds() const {
    return (util::constants::cMillisecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
               ? static_cast<TValueType>(*this) /
                     (static_cast<TValueType>(csmForteTimeBaseUnitsPerSecond) / util::constants::cMillisecondsPerSecond)
               : static_cast<TValueType>(*this) *
                     (util::constants::cMillisecondsPerSecond / csmForteTimeBaseUnitsPerSecond);
  }

  CIEC_LTIME::TValueType CIEC_LTIME::getInMicroSeconds() const {
    return (util::constants::cMicrosecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
               ? static_cast<TValueType>(*this) /
                     (static_cast<TValueType>(csmForteTimeBaseUnitsPerSecond) / util::constants::cMicrosecondsPerSecond)
               : static_cast<TValueType>(*this) *
                     (util::constants::cMicrosecondsPerSecond / csmForteTimeBaseUnitsPerSecond);
  }

  CIEC_LTIME::TValueType CIEC_LTIME::getInNanoSeconds() const {
    return (util::constants::cNanosecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
               ? static_cast<TValueType>(*this) /
                     (static_cast<TValueType>(csmForteTimeBaseUnitsPerSecond) / util::constants::cNanosecondsPerSecond)
               : static_cast<TValueType>(*this) *
                     (util::constants::cNanosecondsPerSecond / csmForteTimeBaseUnitsPerSecond);
  }

  void CIEC_LTIME::setFromSeconds(TValueType paValue) {
    setLargestInt(paValue * csmForteTimeBaseUnitsPerSecond);
  }

  void CIEC_LTIME::setFromMilliSeconds(TValueType paValue) {
    setLargestInt((util::constants::cMillisecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
                      ? paValue * (csmForteTimeBaseUnitsPerSecond / util::constants::cMillisecondsPerSecond)
                      : paValue / (util::constants::cMillisecondsPerSecond / csmForteTimeBaseUnitsPerSecond));
  }

  void CIEC_LTIME::setFromMicroSeconds(TValueType paValue) {
    setLargestInt((util::constants::cMicrosecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
                      ? paValue * (csmForteTimeBaseUnitsPerSecond / util::constants::cMicrosecondsPerSecond)
                      : paValue / (util::constants::cMicrosecondsPerSecond / csmForteTimeBaseUnitsPerSecond));
  }

  void CIEC_LTIME::setFromNanoSeconds(TValueType paValue) {
    setLargestInt((util::constants::cNanosecondsPerSecond < csmForteTimeBaseUnitsPerSecond)
                      ? paValue * (csmForteTimeBaseUnitsPerSecond / util::constants::cNanosecondsPerSecond)
                      : paValue / (util::constants::cNanosecondsPerSecond / csmForteTimeBaseUnitsPerSecond));
  }

  const StringId CDataTypeTrait<CIEC_LTIME>::scmDataTypeName = "LTIME"_STRID;
} // namespace forte
