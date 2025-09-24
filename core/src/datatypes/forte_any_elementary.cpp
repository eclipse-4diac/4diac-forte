/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN, Profactor GmbH, fortiss GmbH, nxtControl GmbH,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Monika Wenger, Matthias Plasch, Martin Melik Markumians,
 *     Ingo Hegny, Stanislav Meduna
 *          - initial implementation and rework communication infrastructure
 *   Markus Meingast, Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include "forte/datatypes/forte_any_elementary.h"
#include "forte/util/string_utils.h"
#include <stdlib.h>
#include <errno.h>
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_lint.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/util/devlog.h"
#include <map>

using namespace forte::literals;

namespace forte {
  namespace {
    const std::map<StringId, CIEC_ANY::EDataTypeID> scm_StringToTypeId = {
        {"ANY"_STRID, CIEC_ANY::e_ANY},
        {"BOOL"_STRID, CIEC_ANY::e_BOOL},
        {"SINT"_STRID, CIEC_ANY::e_SINT},
        {"INT"_STRID, CIEC_ANY::e_INT},
        {"DINT"_STRID, CIEC_ANY::e_DINT},
        {"LINT"_STRID, CIEC_ANY::e_LINT},
        {"USINT"_STRID, CIEC_ANY::e_USINT},
        {"UINT"_STRID, CIEC_ANY::e_UINT},
        {"UDINT"_STRID, CIEC_ANY::e_UDINT},
        {"ULINT"_STRID, CIEC_ANY::e_ULINT},
        {"BYTE"_STRID, CIEC_ANY::e_BYTE},
        {"WORD"_STRID, CIEC_ANY::e_WORD},
        {"DWORD"_STRID, CIEC_ANY::e_DWORD},
        {"LWORD"_STRID, CIEC_ANY::e_LWORD},
        {"T"_STRID, CIEC_ANY::e_TIME},
        {"TIME"_STRID, CIEC_ANY::e_TIME},
        {"D"_STRID, CIEC_ANY::e_DATE},
        {"DATE"_STRID, CIEC_ANY::e_DATE},
        {"TOD"_STRID, CIEC_ANY::e_TIME_OF_DAY},
        {"TIME_OF_DAY"_STRID, CIEC_ANY::e_TIME_OF_DAY},
        {"DT"_STRID, CIEC_ANY::e_DATE_AND_TIME},
        {"DATE_AND_TIME"_STRID, CIEC_ANY::e_DATE_AND_TIME},
        {"LT"_STRID, CIEC_ANY::e_LTIME},
        {"LTIME"_STRID, CIEC_ANY::e_LTIME},
        {"LD"_STRID, CIEC_ANY::e_LDATE},
        {"LDATE"_STRID, CIEC_ANY::e_LDATE},
        {"LTOD"_STRID, CIEC_ANY::e_LTIME_OF_DAY},
        {"LTIME_OF_DAY"_STRID, CIEC_ANY::e_LTIME_OF_DAY},
        {"LDT"_STRID, CIEC_ANY::e_LDATE_AND_TIME},
        {"LDATE_AND_TIME"_STRID, CIEC_ANY::e_LDATE_AND_TIME},
        {"CHAR"_STRID, CIEC_ANY::e_CHAR},
        {"WCHAR"_STRID, CIEC_ANY::e_WCHAR},
        {"REAL"_STRID, CIEC_ANY::e_REAL},
        {"LREAL"_STRID, CIEC_ANY::e_LREAL},
        {"STRING"_STRID, CIEC_ANY::e_STRING},
        {"WSTRING"_STRID, CIEC_ANY::e_WSTRING}};
  }

  void CIEC_ANY_ELEMENTARY::toString(std::string &paTargetBuf) const {
    TLargestUIntValueType divisor = 0;
    TLargestUIntValueType unsignedValBuf = 0;
    TLargestIntValueType signedValBuf = 0;

    bool isSigned = true;

    switch (getDataTypeID()) {
      case e_SINT:
        signedValBuf = getTINT8();
        divisor = 100;
        break;
      case e_USINT:
      case e_BYTE:
        unsignedValBuf = getTUINT8();
        divisor = 100;
        isSigned = false;
        break;
      case e_INT:
        signedValBuf = getTINT16();
        divisor = 10000;
        break;
      case e_UINT:
      case e_WORD:
        unsignedValBuf = getTUINT16();
        divisor = 10000;
        isSigned = false;
        break;
      case e_DINT:
        signedValBuf = getTINT32();
        divisor = 1000000000L;
        break;
      case e_UDINT:
      case e_DWORD:
        unsignedValBuf = getTUINT32();
        divisor = 1000000000L;
        isSigned = false;
        break;
      case e_LINT:
        signedValBuf = getTINT64();
        divisor = 1000000000000000000LL;
        break;
      case e_ULINT:
      case e_LWORD:
        unsignedValBuf = getTUINT64();
        divisor = 10000000000000000000ULL;
        isSigned = false;
        break;
      default: DEVLOG_ERROR("Attempt to call CIEC_ANY::toString in CIEC_ANY_ELEMENTARY\n"); return;
    }

    if (true == isSigned) {
      if (signedValBuf < 0) {
        paTargetBuf += '-';
        signedValBuf *= -1;
      }
      unsignedValBuf = static_cast<TLargestUIntValueType>(signedValBuf);
    }

    bool leadingZeros = true;
    do {
      if ((0 == unsignedValBuf / divisor) && (true == leadingZeros)) {
        divisor /= 10;
        continue;
      } else {
        leadingZeros = false;
      }
      paTargetBuf += static_cast<char>(static_cast<char>(unsignedValBuf / divisor) + '0');
      unsignedValBuf = unsignedValBuf - (paTargetBuf.back() - '0') * divisor;
      divisor /= 10;
    } while (divisor > 1);

    paTargetBuf += static_cast<char>(static_cast<char>(unsignedValBuf / divisor) + '0');
  }

  int CIEC_ANY_ELEMENTARY::fromString(const char *paValue) {
    int nRetVal = -1;
    const char *pacRunner = paValue;

    if ((nullptr == paValue) || ('\0' == *paValue)) {
      return -1;
    }

    if (e_ANY == getDataTypeID()) {
      DEVLOG_ERROR("Attempt to call CIEC_ANY::fromString in CIEC_ANY_ELEMENTARY\n");
      return -1;
      // TODO think of a check if it is really an any elementary that has been created
    } else {
      const char *acHashPos = strchr(paValue, '#');
      int nMultiplier = 10;
      bool bSigned = true;
      if ((nullptr != acHashPos) && (!util::isDigit(*paValue))) {
        // if we have a hash and the first character is not a digit it has to be a type identifier
        if (!isTypeSpecifier(paValue, acHashPos)) {
          return -1;
        }
        pacRunner = acHashPos + 1; // put the runner one after the hash
        nRetVal = -1;
      }

      TLargestUIntValueType nUUpperBound = 0;
      TLargestIntValueType nSUpperBound = 0;
      TLargestIntValueType nSLowerBound = 0;

      if ('#' == pacRunner[1]) {
        switch (pacRunner[0]) {
          case '2': nMultiplier = 2; break;
          case '8': nMultiplier = 8; break;
          default: return -1; break;
        }
        pacRunner = pacRunner + 2;
      } else if ('\0' != pacRunner[1] && '#' == pacRunner[2]) {
        if ('1' == pacRunner[0]) {
          if ('6' == pacRunner[1]) {
            nMultiplier = 16;
          } else {
            if ('0' != pacRunner[1]) {
              return -1;
            }
          }
        } else {
          return -1;
        }
        pacRunner = pacRunner + 3;
      }

      EDataTypeID eID = getDataTypeID();
      switch (eID) {
        case e_SINT:
          nSUpperBound = std::numeric_limits<CIEC_SINT::TValueType>::max();
          nSLowerBound = std::numeric_limits<CIEC_SINT::TValueType>::min();
          break;
        case e_INT:
          nSUpperBound = std::numeric_limits<CIEC_INT::TValueType>::max();
          nSLowerBound = std::numeric_limits<CIEC_INT::TValueType>::min();
          break;
        case e_DINT:
          nSUpperBound = std::numeric_limits<CIEC_DINT::TValueType>::max();
          nSLowerBound = std::numeric_limits<CIEC_DINT::TValueType>::min();
          break;
        case e_LINT:
          nSUpperBound = std::numeric_limits<CIEC_LINT::TValueType>::max();
          nSLowerBound = std::numeric_limits<CIEC_LINT::TValueType>::min();
          break;
        case e_USINT:
        case e_BYTE:
          nUUpperBound = std::numeric_limits<CIEC_USINT::TValueType>::max();
          bSigned = false;
          break;
        case e_UINT:
        case e_WORD:
          nUUpperBound = std::numeric_limits<CIEC_UINT::TValueType>::max();
          bSigned = false;
          break;
        case e_UDINT:
        case e_DWORD:
          nUUpperBound = std::numeric_limits<CIEC_UDINT::TValueType>::max();
          bSigned = false;
          break;
        case e_ULINT:
        case e_LWORD:
          nUUpperBound = std::numeric_limits<CIEC_ULINT::TValueType>::max();
          bSigned = false;
          break;
        default: return false;
      }

      if ((bSigned) && (10 != nMultiplier)) {
        // only decimal numbers have a sign
        bSigned = false;
        nUUpperBound = 2 * nSUpperBound + 1;
      }

      char *pacEndPtr;
      errno = 0; // erno is not cleared by the strto* functions

      if (true == bSigned) {
        TForteInt64 nValue = util::strtoll(pacRunner, &pacEndPtr, nMultiplier);
        if ((ERANGE != errno) && (nValue <= nSUpperBound) && (nValue >= nSLowerBound)) {
          setLargestInt(nValue);
          nRetVal = static_cast<int>(pacEndPtr - paValue);
        }
      } else {
        if ('-' != *pacRunner) {
          // The strtou* functions will correctly parse also negative numbers and provide their two complement as value
          TForteUInt64 nValue = util::strtoull(pacRunner, &pacEndPtr, nMultiplier);
          if ((ERANGE != errno) && (nValue <= nUUpperBound)) {
            setLargestUInt(nValue);
            nRetVal = static_cast<int>(pacEndPtr - paValue);
          }
        }
      }
      if (0 == nRetVal) {
        // at this point a length of zero means broken string so return -1
        nRetVal = -1;
      }
    }
    return nRetVal;
  }

  bool CIEC_ANY_ELEMENTARY::isTypeSpecifier(const char *paValue, const char *paHashPosition) const {
    StringId nTypeNameId = parseTypeName(paValue, paHashPosition);

    if (nTypeNameId &&
        ((scm_StringToTypeId.find(nTypeNameId) != scm_StringToTypeId.end()) || (isCastable(nTypeNameId)))) {
      return true;
    }
    return false;
  }

  bool CIEC_ANY_ELEMENTARY::isCastable(StringId paTypeNameId) const {
    EDataTypeID literalID = getElementaryDataTypeId(paTypeNameId);
    bool upCast;
    bool downCast;
    return CIEC_ANY::isCastable(literalID, getDataTypeID(), upCast, downCast) && upCast;
  }

  CIEC_ANY::EDataTypeID CIEC_ANY_ELEMENTARY::getElementaryDataTypeId(const StringId paTypeNameId) {
    if (scm_StringToTypeId.find(paTypeNameId) != scm_StringToTypeId.end()) {
      return scm_StringToTypeId.at(paTypeNameId);
    }
    return e_ANY;
  }
} // namespace forte
