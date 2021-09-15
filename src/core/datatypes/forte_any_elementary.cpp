/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, Profactor GmbH, fortiss GmbH, nxtControl GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Monika Wenger, Matthias Plasch, Martin Melik Markumians,
 *    Ingo Hegny, Stanislav Meduna
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte_any_elementary.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_any_elementary_gen.cpp"
#endif
#include <stdlib.h>
#include <errno.h>
#include "forte_sint.h"
#include "forte_int.h"
#include "forte_dint.h"
#include "forte_usint.h"
#include "forte_uint.h"
#include "forte_udint.h"
#include "forte_lint.h"
#include "forte_ulint.h"

DEFINE_FIRMWARE_DATATYPE(ANY_ELEMENTARY, g_nStringIdANY_ELEMENTARY)

const CStringDictionary::TStringId CIEC_ANY_ELEMENTARY::scm_anTypeNameStringIds[] = { g_nStringIdANY, g_nStringIdBOOL, g_nStringIdSINT, g_nStringIdINT, g_nStringIdDINT, g_nStringIdLINT, g_nStringIdUSINT, g_nStringIdUINT, g_nStringIdUDINT, g_nStringIdULINT, g_nStringIdBYTE, g_nStringIdWORD, g_nStringIdDWORD, g_nStringIdLWORD, g_nStringIdDATE, g_nStringIdTIME_OF_DAY, g_nStringIdDATE_AND_TIME, g_nStringIdTIME, //until here simple Datatypes
    g_nStringIdREAL, g_nStringIdLREAL, g_nStringIdSTRING, //e_STRING,
    g_nStringIdWSTRING, //e_WSTRING,
    CStringDictionary::scm_nInvalidStringId, //e_DerivedData,
    CStringDictionary::scm_nInvalidStringId, //e_DirectlyDerivedData,
    CStringDictionary::scm_nInvalidStringId, //e_EnumeratedData,
    CStringDictionary::scm_nInvalidStringId, //e_SubrangeData,
    CStringDictionary::scm_nInvalidStringId, //e_ARRAY,
    CStringDictionary::scm_nInvalidStringId, //e_STRUCT,
    CStringDictionary::scm_nInvalidStringId, //e_External,
    CStringDictionary::scm_nInvalidStringId, //e_Max
    };

int CIEC_ANY_ELEMENTARY::toString(char* paValue, size_t paBufferSize) const {
  int nRetVal = 0;

  TLargestUIntValueType nDivisor = 0;
  TLargestUIntValueType nUBuffer = 0;
  TLargestIntValueType nSBuffer = 0;

  bool bSigned = true;

  switch (getDataTypeID()){
    case e_SINT:
      nSBuffer = getTINT8();
      nDivisor = 100;
      break;
    case e_USINT:
    case e_BYTE:
      nUBuffer = getTUINT8();
      nDivisor = 100;
      bSigned = false;
      break;
    case e_INT:
      nSBuffer = getTINT16();
      nDivisor = 10000;
      break;
    case e_UINT:
    case e_WORD:
      nUBuffer = getTUINT16();
      nDivisor = 10000;
      bSigned = false;
      break;
    case e_DINT:
      nSBuffer = getTINT32();
      nDivisor = 1000000000L;
      break;
    case e_UDINT:
    case e_DWORD:
      nUBuffer = getTUINT32();
      nDivisor = 1000000000L;
      bSigned = false;
      break;
#ifdef FORTE_USE_64BIT_DATATYPES
      case e_LINT:
      nSBuffer = getTINT64();
      nDivisor = 1000000000000000000LL;
      break;
    case e_ULINT:
      case e_LWORD:
      nUBuffer = getTUINT64();
      nDivisor = 10000000000000000000ULL;
      bSigned = false;
      break;
#endif
    default:
      return CIEC_ANY::toString(paValue, paBufferSize);
  }

  if(true == bSigned){
    if(nSBuffer < 0){
      if(nRetVal >= static_cast<int>(paBufferSize)) {
        return -1;
      }
      paValue[nRetVal] = '-';
      nRetVal++;
      nSBuffer *= -1;
    }
    nUBuffer = static_cast<TLargestUIntValueType>(nSBuffer);
  }

  bool bLeadingZeros = true;
  do{
    if((0 == nUBuffer / nDivisor) && (true == bLeadingZeros)){
      nDivisor /= 10;
      continue;
    }
    else{
      bLeadingZeros = false;
    }
    if(nRetVal >= static_cast<int>(paBufferSize)) {
      return -1;
    }

    paValue[nRetVal] = static_cast<char>(static_cast<char>(nUBuffer / nDivisor) + '0');
    nUBuffer = nUBuffer - (paValue[nRetVal] - '0') * nDivisor;
    nDivisor /= 10;
    nRetVal++;
  } while(nDivisor > 1);

  if(nRetVal >= static_cast<int>(paBufferSize)) {
    return -1;
  }
  paValue[nRetVal] = static_cast<char>(static_cast<char>(nUBuffer / nDivisor) + '0');
  nRetVal++;

  if(nRetVal >= static_cast<int>(paBufferSize)) {
    return -1;
  }
  paValue[nRetVal] = '\0';

  return nRetVal;
}

int CIEC_ANY_ELEMENTARY::fromString(const char *pa_pacValue){
  int nRetVal = -1;
  const char *pacRunner = pa_pacValue;

  if((0 == pa_pacValue) || ('\0' == *pa_pacValue)){
    return -1;
  }

  if(e_ANY == getDataTypeID()){
    nRetVal = CIEC_ANY::fromString(pa_pacValue);
    //TODO think of a check if it is really an any elementary that has been created
  }
  else{
    const char *acHashPos = strchr(pa_pacValue, '#');
    int nMultiplier = 10;
    bool bSigned = true;
    if((0 != acHashPos) && (!forte::core::util::isDigit(*pa_pacValue))){
      //if we have a hash and the first character is not a digit it has to be a type identifier
      nRetVal = checkTypeSpec(pa_pacValue, acHashPos);
      if(nRetVal < 0){
        return -1;
      }
      pacRunner += (nRetVal + 1); //put the runner one after the hash
      nRetVal = -1;
    }

    TLargestUIntValueType nUUpperBound = 0;
    TLargestIntValueType nSUpperBound = 0;
    TLargestIntValueType nSLowerBound = 0;

    if('#' == pacRunner[1]){
      switch (pacRunner[0]){
        case '2':
          nMultiplier = 2;
          break;
        case '8':
          nMultiplier = 8;
          break;
        default:
          return -1;
          break;
      }
      pacRunner = pacRunner + 2;
    }
    else if('\0' != pacRunner[1] && '#' == pacRunner[2]){
      if('1' == pacRunner[0]){
        if('6' == pacRunner[1]){
          nMultiplier = 16;
        }
        else{
          if('0' != pacRunner[1]){
            return -1;
          }
        }
      } else {
        return -1;
      }
      pacRunner = pacRunner + 3;
    }

    EDataTypeID eID = getDataTypeID();
    switch (eID){
      case e_SINT:
        nSUpperBound = CIEC_SINT::scm_nMaxVal;
        nSLowerBound = CIEC_SINT::scm_nMinVal;
        break;
      case e_INT:
        nSUpperBound = CIEC_INT::scm_nMaxVal;
        nSLowerBound = CIEC_INT::scm_nMinVal;
        break;
      case e_DINT:
        nSUpperBound = CIEC_DINT::scm_nMaxVal;
        nSLowerBound = CIEC_DINT::scm_nMinVal;
        break;
#ifdef FORTE_USE_64BIT_DATATYPES
        case e_LINT:
        nSUpperBound = CIEC_LINT::scm_nMaxVal;
        nSLowerBound = CIEC_LINT::scm_nMinVal;
        break;
#endif
      case e_USINT:
      case e_BYTE:
        nUUpperBound = CIEC_USINT::scm_nMaxVal;
        bSigned = false;
        break;
      case e_UINT:
      case e_WORD:
        nUUpperBound = CIEC_UINT::scm_nMaxVal;
        bSigned = false;
        break;
      case e_UDINT:
      case e_DWORD:
        nUUpperBound = CIEC_UDINT::scm_nMaxVal;
        bSigned = false;
        break;
#ifdef FORTE_USE_64BIT_DATATYPES
      case e_ULINT:
      case e_LWORD:
        nUUpperBound = CIEC_ULINT::scm_nMaxVal;
        bSigned = false;
        break;
#endif
      default:
        return false;
    }

    if((bSigned) && (10 != nMultiplier)){
      //only decimal numbers have a sign
      bSigned = false;
      nUUpperBound = 2 * nSUpperBound + 1;
    }

    char *pacEndPtr;
    errno = 0; //erno is not cleared by the strto* functions

    if(true == bSigned){
#ifdef FORTE_USE_64BIT_DATATYPES
      TForteInt64 nValue = forte::core::util::strtoll(pacRunner, &pacEndPtr, nMultiplier);
#else
      TForteInt32 nValue = forte::core::util::strtol(pacRunner, &pacEndPtr, nMultiplier);
#endif

      if((ERANGE != errno)&& (nValue <= nSUpperBound) && (nValue
          >= nSLowerBound)){
      setLargestInt(nValue);
      nRetVal = static_cast<int>(pacEndPtr - pa_pacValue);
    }
  }
  else{
    if ('-' != *pacRunner){
      //The strtou* functions will correctly parse also negative numbers and provide their two complement as value
#ifdef FORTE_USE_64BIT_DATATYPES
          TForteUInt64 nValue = forte::core::util::strtoull(pacRunner, &pacEndPtr, nMultiplier);
#else
          TForteUInt32 nValue = forte::core::util::strtoul(pacRunner, &pacEndPtr, nMultiplier);
#endif //FORTE_USE_64BIT_DATATYPES
          if ((ERANGE != errno) && (nValue <= nUUpperBound)){
            setLargestUInt(nValue);
            nRetVal = static_cast<int>(pacEndPtr - pa_pacValue);
          }
        }
      }
    if(0 == nRetVal){
      // at this point a length of zero means broken string so return -1
      nRetVal = -1;
    }
  }
  return nRetVal;
}

int CIEC_ANY_ELEMENTARY::checkTypeSpec(const char* pa_pacValue, const char* pa_pacHashPos) const {
  int nRetVal = -1;

  CStringDictionary::TStringId nTypeNameId = parseTypeName(pa_pacValue, pa_pacHashPos);

  if((CStringDictionary::scm_nInvalidStringId != nTypeNameId) && ((scm_anTypeNameStringIds[getDataTypeID()] == nTypeNameId) || (isCastable(nTypeNameId)) )){
    nRetVal = static_cast<int>(pa_pacHashPos - pa_pacValue);
  }
  return nRetVal;
}

bool CIEC_ANY_ELEMENTARY::isCastable(CStringDictionary::TStringId paTypeNameId) const {
  EDataTypeID literalID = getElementaryDataTypeId(paTypeNameId);
  bool upCast;
  bool downCast;
  return CIEC_ANY::isCastable(literalID, getDataTypeID(), upCast, downCast) && upCast;
}


CIEC_ANY::EDataTypeID CIEC_ANY_ELEMENTARY::getElementaryDataTypeId(CStringDictionary::TStringId paTypeNameId){
  for(size_t i = 0; i <= CIEC_ANY::e_LWORD; i++){
    if(scm_anTypeNameStringIds[i] == paTypeNameId){
      return static_cast<EDataTypeID>(i);
    }
  }
  return CIEC_ANY::e_ANY;
}
