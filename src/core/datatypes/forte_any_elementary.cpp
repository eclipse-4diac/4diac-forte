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

const std::map<CStringDictionary::TStringId, CIEC_ANY::EDataTypeID> CIEC_ANY_ELEMENTARY::scm_StringToTypeId = {
    {g_nStringIdANY, CIEC_ANY::e_ANY},
    {g_nStringIdBOOL, CIEC_ANY::e_BOOL},
    {g_nStringIdSINT, CIEC_ANY::e_SINT},
    {g_nStringIdINT, CIEC_ANY::e_INT},
    {g_nStringIdDINT, CIEC_ANY::e_DINT},
    {g_nStringIdLINT, CIEC_ANY::e_LINT},
    {g_nStringIdUSINT, CIEC_ANY::e_USINT},
    {g_nStringIdUINT, CIEC_ANY::e_UINT},
    {g_nStringIdUDINT, CIEC_ANY::e_UDINT},
    {g_nStringIdULINT, CIEC_ANY::e_ULINT},
    {g_nStringIdBYTE, CIEC_ANY::e_BYTE},
    {g_nStringIdWORD, CIEC_ANY::e_WORD},
    {g_nStringIdDWORD, CIEC_ANY::e_DWORD},
    {g_nStringIdLWORD, CIEC_ANY::e_LWORD},
    {g_nStringIdT, CIEC_ANY::e_TIME},
    {g_nStringIdTIME, CIEC_ANY::e_TIME},
    {g_nStringIdD, CIEC_ANY::e_DATE},
    {g_nStringIdDATE, CIEC_ANY::e_DATE},
    {g_nStringIdTOD, CIEC_ANY::e_TIME_OF_DAY},
    {g_nStringIdTIME_OF_DAY, CIEC_ANY::e_TIME_OF_DAY},
    {g_nStringIdDT, CIEC_ANY::e_DATE_AND_TIME},
    {g_nStringIdDATE_AND_TIME, CIEC_ANY::e_DATE_AND_TIME},
    {g_nStringIdLT, CIEC_ANY::e_LTIME},
    {g_nStringIdLTIME, CIEC_ANY::e_LTIME},
    {g_nStringIdLD, CIEC_ANY::e_LDATE},
    {g_nStringIdLDATE, CIEC_ANY::e_LDATE},
    {g_nStringIdLTOD, CIEC_ANY::e_LTIME_OF_DAY},
    {g_nStringIdLTIME_OF_DAY, CIEC_ANY::e_LTIME_OF_DAY},
    {g_nStringIdLDT, CIEC_ANY::e_LDATE_AND_TIME},
    {g_nStringIdLDATE_AND_TIME, CIEC_ANY::e_LDATE_AND_TIME},
    {g_nStringIdCHAR, CIEC_ANY::e_CHAR},
    {g_nStringIdWCHAR, CIEC_ANY::e_WCHAR},
    {g_nStringIdREAL, CIEC_ANY::e_REAL},
    {g_nStringIdLREAL, CIEC_ANY::e_LREAL},
    {g_nStringIdSTRING, CIEC_ANY::e_STRING},
    {g_nStringIdWSTRING, CIEC_ANY::e_WSTRING}};

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
    default:
      DEVLOG_ERROR("Attempt to call CIEC_ANY::toString in CIEC_ANY_ELEMENTARY\n");
      return -1;
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

int CIEC_ANY_ELEMENTARY::fromString(const char *paValue){
  int nRetVal = -1;
  const char *pacRunner = paValue;

  if((nullptr == paValue) || ('\0' == *paValue)){
    return -1;
  }

  if(e_ANY == getDataTypeID()){
    DEVLOG_ERROR("Attempt to call CIEC_ANY::fromString in CIEC_ANY_ELEMENTARY\n");
    return -1;
    //TODO think of a check if it is really an any elementary that has been created
  }
  else{
    const char *acHashPos = strchr(paValue, '#');
    int nMultiplier = 10;
    bool bSigned = true;
    if((nullptr != acHashPos) && (!forte::core::util::isDigit(*paValue))){
      //if we have a hash and the first character is not a digit it has to be a type identifier
      if(!isTypeSpecifier(paValue, acHashPos)){
        return -1;
      }
      pacRunner = acHashPos + 1; //put the runner one after the hash
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
        nSUpperBound = CIEC_SINT::scmMaxVal;
        nSLowerBound = CIEC_SINT::scmMinVal;
        break;
      case e_INT:
        nSUpperBound = CIEC_INT::scmMaxVal;
        nSLowerBound = CIEC_INT::scmMinVal;
        break;
      case e_DINT:
        nSUpperBound = CIEC_DINT::scmMaxVal;
        nSLowerBound = CIEC_DINT::scmMinVal;
        break;
        case e_LINT:
        nSUpperBound = CIEC_LINT::scmMaxVal;
        nSLowerBound = CIEC_LINT::scmMinVal;
        break;
      case e_USINT:
      case e_BYTE:
        nUUpperBound = CIEC_USINT::scmMaxVal;
        bSigned = false;
        break;
      case e_UINT:
      case e_WORD:
        nUUpperBound = CIEC_UINT::scmMaxVal;
        bSigned = false;
        break;
      case e_UDINT:
      case e_DWORD:
        nUUpperBound = CIEC_UDINT::scmMaxVal;
        bSigned = false;
        break;
      case e_ULINT:
      case e_LWORD:
        nUUpperBound = CIEC_ULINT::scmMaxVal;
        bSigned = false;
        break;
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
      TForteInt64 nValue = forte::core::util::strtoll(pacRunner, &pacEndPtr, nMultiplier);
      if((ERANGE != errno)&& (nValue <= nSUpperBound) && (nValue
          >= nSLowerBound)){
      setLargestInt(nValue);
      nRetVal = static_cast<int>(pacEndPtr - paValue);
    }
  }
  else{
    if ('-' != *pacRunner){
      //The strtou* functions will correctly parse also negative numbers and provide their two complement as value
          TForteUInt64 nValue = forte::core::util::strtoull(pacRunner, &pacEndPtr, nMultiplier);
          if ((ERANGE != errno) && (nValue <= nUUpperBound)){
            setLargestUInt(nValue);
            nRetVal = static_cast<int>(pacEndPtr - paValue);
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

bool CIEC_ANY_ELEMENTARY::isTypeSpecifier(const char* paValue, const char* paHashPosition) const {
  CStringDictionary::TStringId nTypeNameId = parseTypeName(paValue, paHashPosition);

  if ((CStringDictionary::scmInvalidStringId != nTypeNameId) && ((scm_StringToTypeId.find(nTypeNameId) != scm_StringToTypeId.end()) || (isCastable(nTypeNameId))))
  {
    return true;
  }
  return false;
}

bool CIEC_ANY_ELEMENTARY::isCastable(CStringDictionary::TStringId paTypeNameId) const {
  EDataTypeID literalID = getElementaryDataTypeId(paTypeNameId);
  bool upCast;
  bool downCast;
  return CIEC_ANY::isCastable(literalID, getDataTypeID(), upCast, downCast) && upCast;
}


CIEC_ANY::EDataTypeID CIEC_ANY_ELEMENTARY::getElementaryDataTypeId(const CStringDictionary::TStringId paTypeNameId){
    if(scm_StringToTypeId.find(paTypeNameId) != scm_StringToTypeId.end()){
      return scm_StringToTypeId.at(paTypeNameId);
    }
  return CIEC_ANY::e_ANY;
}
