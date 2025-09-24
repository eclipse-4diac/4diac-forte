/*******************************************************************************
 * Copyright (c) 2008, 2025 nxtControl GmbH, ACIN, Profactor GmbH, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Stanislav Meduna, Alois Zoitl, Gerhard Ebenhofer, Martin Melik Merkumians
 *                - initial implementation and rework communication infrastructure
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include <format>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "forte/datatypes/forte_ltime_of_day.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(LTIME_OF_DAY, "LTIME_OF_DAY"_STRID)

  int CIEC_LTIME_OF_DAY::fromString(const char *paValue) {
    // 15:00:00.000
    struct tm tm;
    int msec = 0;
    char *acBuffer = const_cast<char *>(paValue);

    memset(&tm, 0, sizeof(tm));

    if ('l' == tolower(*acBuffer)) {
      if (('t' == tolower(acBuffer[1])) && ('o' == tolower(acBuffer[2])) && ('d' == tolower(acBuffer[3]))) {
        acBuffer += 4;
      } else {
        // TODO maybe allow to turn this check of for small devices
        if ((0 == strncmp("ltime_of_day", acBuffer, 12)) || (0 == strncmp("LTIME_OF_DAY", acBuffer, 12))) {
          acBuffer += 12;
        }
      }

      if ('#' != *acBuffer) {
        return -1;
      }
      acBuffer++;
    }

    if ('\0' != *acBuffer) {
      tm.tm_hour = static_cast<int>(util::strtoul(acBuffer, &acBuffer, 10));
      if (':' == *acBuffer) {
        ++acBuffer;
        tm.tm_min = static_cast<int>(util::strtoul(acBuffer, &acBuffer, 10));
        if (':' == *acBuffer) {
          ++acBuffer;
          tm.tm_sec = static_cast<int>(util::strtoul(acBuffer, &acBuffer, 10));
          if ('.' == *acBuffer) {
            unsigned int nNums = 0;
            ++acBuffer;
            while (isdigit(*acBuffer)) {
              msec = 10 * msec + util::charDigitToInt(*acBuffer);
              ++acBuffer;
              ++nNums;
            }

            if (nNums < 3) {
              for (unsigned int i = nNums; i < 3; ++i) {
                msec *= 10;
              }
            } else {
              for (unsigned int i = 0; i < (nNums - 3); ++i) {
                msec /= 10;
              }
            }
          }
        }
      } else {
        if ('\0' != *acBuffer) {
          return -1;
        }
      }
    } else {
      return -1;
    }

    setTUINT64(((tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec) * 1000ULL + msec) * 1000000ULL);

    return static_cast<int>(acBuffer - paValue);
  }

  void CIEC_LTIME_OF_DAY::toString(std::string &paTargetBuf) const {
    TForteUInt64 ntoStingBuffer = getTUINT64();
    time_t t = static_cast<time_t>(ntoStingBuffer / (1000ULL * 1000000ULL));

    std::format_to(std::back_inserter(paTargetBuf), "LTOD#{:02}:{:02}:{:02}.{:03}", static_cast<int>(t / 3600),
                   static_cast<int>((t % 3600) / 60), static_cast<int>(t % 60),
                   (int) ((ntoStingBuffer / 1000000) % 1000));
  }

  const StringId CDataTypeTrait<CIEC_LTIME_OF_DAY>::scmDataTypeName = "LTIME_OF_DAY"_STRID;
} // namespace forte
