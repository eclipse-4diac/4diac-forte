/*******************************************************************************
 * Copyright (c) 2008, 2025 nxtControl GmbH, ACIN, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Stanislav Meduna, Alois Zoitl, Martin Melik Merkumians, Monika Wenger
 *               - initial implementation and rework communication infrastructure
 *   Alois Zoitl - migrated data type toString to std::string
 *******************************************************************************/
#include <format>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "forte/datatypes/forte_date_and_time.h"
#include "forte/arch/forte_architecture_time.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(DATE_AND_TIME, "DATE_AND_TIME"_STRID)

  int CIEC_DATE_AND_TIME::fromString(const char *paValue) {
    // 2007-12-21-15:00:00.000
    int nRetVal = -1;
    char *acBuffer = const_cast<char *>(paValue);

    if ('d' == tolower(*acBuffer)) {
      if ('t' == tolower(acBuffer[1])) {
        acBuffer += 2;
      } else {
        // TODO maybe allow to turn this check of for small devices
        if ((0 == strncmp("date_and_time", acBuffer, 13)) || (0 == strncmp("DATE_AND_TIME", acBuffer, 13))) {
          acBuffer += 13;
        }
      }

      if ('#' != *acBuffer) {
        return -1;
      }
      acBuffer++;
    }

    struct tm tm;
    unsigned int msec = 0;

    memset(&tm, 0, sizeof(tm));

    // Duplicate code the same as in forte date
    if ('\0' != *acBuffer) {
      tm.tm_year = static_cast<int>(util::strtoul(acBuffer, &acBuffer, 10) - 1900);
      if ('-' == *acBuffer) {
        ++acBuffer;
        tm.tm_mon = static_cast<int>(util::strtoul(acBuffer, &acBuffer, 10) - 1);
        if ('-' == *acBuffer) {
          ++acBuffer;
          tm.tm_mday = static_cast<int>(util::strtoul(acBuffer, &acBuffer, 10));

          if ('-' == *acBuffer) {
            ++acBuffer;
            // duplicate code this one can be find in time of day
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
                nRetVal = static_cast<int>(acBuffer - paValue);
              }
            }
          }
        }
      }
    }

    if (-1 != nRetVal && !setDateAndTime(tm, msec)) {
      nRetVal = -1;
    }

    return nRetVal;
  }

  void CIEC_DATE_AND_TIME::toString(std::string &paTargetBuf) const {
    tm ptm;
    if (nullptr != getTimeStruct(&ptm)) {
      std::format_to(std::back_inserter(paTargetBuf), "DT#{:04}-{:02}-{:02}-{:02}:{:02}:{:02}.{:03}",
                     1900 + ptm.tm_year, ptm.tm_mon + 1, ptm.tm_mday, ptm.tm_hour, ptm.tm_min, ptm.tm_sec,
                     getMilliSeconds());
    }
  }

  void CIEC_DATE_AND_TIME::toGMTString(std::string &paTargetBuf) const {
    return toString(paTargetBuf);
  }

  const StringId CDataTypeTrait<CIEC_DATE_AND_TIME>::scmDataTypeName = "DATE_AND_TIME"_STRID;
} // namespace forte
