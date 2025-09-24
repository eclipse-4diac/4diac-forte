/*******************************************************************************
 * Copyright (c) 2008, 2025 nxtControl GmbH, ACIN, fortiss GmbH,
 *                          Primetals Technolgies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Stanislav Meduna, Alois Zoitl, Martin Melik Merkumians
 *                - initial implementation and rework communication infrastructure
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include <format>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "forte/datatypes/forte_ldate.h"
#include "forte/arch/forte_architecture_time.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(LDATE, "LDATE"_STRID)

  int CIEC_LDATE::fromString(const char *paValue) {
    // 2007-12-21
    struct tm tm;
    char *acBuffer = const_cast<char *>(paValue);

    memset(&tm, 0, sizeof(tm));

    if ('l' == tolower(acBuffer[0]) && 'd' == tolower(acBuffer[1])) {
      acBuffer += 2;
      if (('a' == tolower(acBuffer[0])) && ('t' == tolower(acBuffer[1])) && ('e' == tolower(acBuffer[2]))) {
        acBuffer += 3;
      }
      if ('#' != *acBuffer) {
        return -1;
      }
      acBuffer++;
    }

    if ('\0' != *acBuffer) {
      // TODO think of using any elementary fromString function instead of strtoul
      tm.tm_year = static_cast<int>(strtoul(acBuffer, &acBuffer, 10) - 1900);
      if ('-' == *acBuffer) {
        ++acBuffer;
        tm.tm_mon = static_cast<int>(strtoul(acBuffer, &acBuffer, 10) - 1);
        if ('-' == *acBuffer) {
          ++acBuffer;
          tm.tm_mday = static_cast<int>(strtoul(acBuffer, &acBuffer, 10));
        } else {
          return -1;
        }
      } else {
        return -1;
      }
    } else {
      return -1;
    }

    if (!setDateAndTime(tm, 0)) {
      return -1;
    }

    return static_cast<unsigned int>(acBuffer - paValue);
  }

  void CIEC_LDATE::toString(std::string &paTargetBuf) const {
    tm ptm;
    if (nullptr != getTimeStruct(&ptm)) {
      std::format_to(std::back_inserter(paTargetBuf), "LD#{:04}-{:02}-{:02}", 1900 + ptm.tm_year, ptm.tm_mon + 1,
                     ptm.tm_mday);
    }
  }

  const StringId CDataTypeTrait<CIEC_LDATE>::scmDataTypeName = "LDATE"_STRID;
} // namespace forte
