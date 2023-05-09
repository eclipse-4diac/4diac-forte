/*******************************************************************************
 * Copyright (c) 2007 - 2013 ACIN, nxtcontrol GmbH, Profactor GmbH, fortiss GmbH
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Martin Melik Merkumians, Stanislav Meduna,
 *    Monika Wenger, Matthias Plasch
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst
 *      - add support for data types with different size
 *******************************************************************************/
#include "forte_any.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_any_gen.cpp"
#endif
#include "forte_real.h"
#include "forte_lreal.h"
#include <devlog.h>

const CTypeLib::CDataTypeEntry CIEC_ANY::csmFirmwareDataTypeEntry_CIEC_ANY(g_nStringIdANY, CIEC_ANY::createDataType, sizeof(CIEC_ANY));

const char CIEC_ANY::scmAnyToStringResponse[] = "ND (ANY)";

int CIEC_ANY::dummyInit(){
  return 0;
}

void CIEC_ANY::saveAssign(const CIEC_ANY &pa_roValue){
  CIEC_ANY::EDataTypeID srcDTID = pa_roValue.getDataTypeID();
  CIEC_ANY::EDataTypeID dstDTID = getDataTypeID();
  if(dstDTID == srcDTID){
    setValue(pa_roValue);
  }
  else{
    if(e_ANY == dstDTID){
      pa_roValue.clone((TForteByte*) this);
    }
    else if(isCastable(srcDTID, dstDTID)){
      if((CIEC_ANY::e_LREAL == srcDTID) || (CIEC_ANY::e_REAL == srcDTID)){
        specialCast(pa_roValue, *this);
      }
      else{
        setValue(pa_roValue);
      }
    }
    //TODO for the else case check for any and maybe clone it
  }

}

int CIEC_ANY::fromString(const char *pa_pacValue){
  int nRetVal = -1;

  if(e_ANY == getDataTypeID()){
    //we should only be here if it is really an unparameterized generic data type
    const char *acHashPos = strchr(pa_pacValue, '#');
    if(nullptr != acHashPos){
      CStringDictionary::TStringId nTypeNameId = parseTypeName(pa_pacValue, acHashPos);
      if(CStringDictionary::scm_nInvalidStringId != nTypeNameId && nullptr != CTypeLib::createDataTypeInstance(nTypeNameId, (TForteByte *) this)) {
        nRetVal = fromString(pa_pacValue); //some of the datatypes require the type upfront for correct parsing e.g., time
        if(0 > nRetVal) {
          //if it didn't work change us back to an any
          CIEC_ANY::createDataType((TForteByte *) this);
        }
      }
    }
  }
  return nRetVal;
}

CStringDictionary::TStringId CIEC_ANY::parseTypeName(const char *pa_pacValue, const char *pa_pacHashPos){
  CStringDictionary::TStringId nRetVal = CStringDictionary::scm_nInvalidStringId;

  int nLen = static_cast<int>(pa_pacHashPos - pa_pacValue);

  if(nLen < scmMaxTypeNameLength){
    char acTypeNameBuf[scmMaxTypeNameLength];
    strncpy(acTypeNameBuf, pa_pacValue, nLen);
    acTypeNameBuf[nLen] = '\0';
    nRetVal = CStringDictionary::getInstance().getId(acTypeNameBuf);
  }
  return nRetVal;
}

int CIEC_ANY::toString(char* paValue, size_t paBufferSize) const {
  int nRetVal = -1;
  if(sizeof(scmAnyToStringResponse) <= paBufferSize) {
     nRetVal = sizeof(scmAnyToStringResponse) - 1;

    //don't use snprintf here since it brings big usage and performance overheads
    memcpy(paValue, scmAnyToStringResponse, nRetVal);
    paValue[nRetVal] = '\0';
  }
  return nRetVal;
}

bool CIEC_ANY::isCastable(EDataTypeID pa_eSource, EDataTypeID pa_eDestination, bool &pa_rbUpCast, bool &pa_rbDownCast){

  pa_rbUpCast = pa_rbDownCast = false;

  // EDataTypeID -> EDataType matrix, e_BOOL to e_WSTRING
  // Watch for ordering - it has to match EDataTypeID

  // Upcast: always possible without any loss of data or precision
  static const TForteByte UpCast[e_WSTRING - e_BOOL + 1][(e_WSTRING - e_BOOL + 8) / 8] = {
      {0b11111111, 0b11111000, 0b00000001, 0b10000000}, // e_BOOL to x
      {0b01111000, 0b01111000, 0b00000001, 0b10000000}, // e_SINT
      {0b00111000, 0b00111000, 0b00000001, 0b10000000}, // e_INT
      {0b00011000, 0b00011000, 0b00000000, 0b10000000}, // e_DINT

      {0b00001000, 0b00001000, 0b00000000, 0b00000000}, // e_LINT
      {0b00111111, 0b11111000, 0b00000001, 0b10000000}, // e_USINT
      {0b00011011, 0b10111000, 0b00000001, 0b10000000}, // e_UINT
      {0b00001001, 0b10011000, 0b00000000, 0b10000000}, // e_UDINT

      {0b00000000, 0b10001000, 0b00000000, 0b00000000}, // e_ULINT
      {0b01111111, 0b11111000, 0b00000000, 0b00000000}, // e_BYTE
      {0b00111011, 0b10111000, 0b00000000, 0b00000000}, // e_WORD
      {0b00011001, 0b10011000, 0b00000001, 0b00000000}, // e_DWORD

      {0b00001000, 0b10001000, 0b00000001, 0b10000000}, // e_LWORD
      {0b00000000, 0b00000100, 0b00010000, 0b00010000}, // e_DATE
      {0b00000000, 0b00000010, 0b00001000, 0b00000000}, // e_TIME_OF_DAY
      {0b00000000, 0b00000001, 0b00000100, 0b00000000}, // e_DATE_AND_TIME

      {0b00000000, 0b00000000, 0b10000010, 0b00000000}, // e_TIME
      {0b00000000, 0b00000000, 0b01000000, 0b01000000}, // e_CHAR
      {0b00000000, 0b00000000, 0b00100000, 0b00100000}, // e_WCHAR
      {0b00000000, 0b00000000, 0b00010000, 0b00010000}, // e_LDATE

      {0b00000000, 0b00000000, 0b00001000, 0b00000000}, // e_LTIME_OF_DAY
      {0b00000000, 0b00000000, 0b00000100, 0b00000000}, // e_LDATE_AND_TIME
      {0b00000000, 0b00000000, 0b00000010, 0b00000000},  // e_LTIME
      {0b00000000, 0b00011000, 0b00000001, 0b10000000}, // e_REAL

      {0b00000000, 0b00001000, 0b00000000, 0b10000000}, // e_LREAL
      {0b00000000, 0b00000000, 0b00000000, 0b01000000}, // e_STRING
      {0b00000000, 0b00000000, 0b00000000, 0b00100000}  // e_WSTRING
  };

  // Downcast: information may get lost (eventually catastrophically)
  static const TForteByte DownCast[e_WSTRING - e_BOOL + 1][(e_WSTRING - e_BOOL + 8) / 8] = {
      {0b00000000, 0b00000000, 0b00000000, 0b00000000}, // e_BOOL to x
      {0b10000111, 0b10000000, 0b00000000, 0b00000000}, // e_SINT
      {0b11000111, 0b11000000, 0b00000000, 0b00000000}, // e_INT
      {0b11100111, 0b11100000, 0b00000001, 0b00000000}, // e_DINT

      {0b11110111, 0b11110000, 0b00000001, 0b10000000}, // e_LINT
      {0b11000000, 0b00000000, 0b00000000, 0b00000000}, // e_USINT
      {0b11100100, 0b01000000, 0b00000000, 0b00000000}, // e_UINT
      {0b11110110, 0b01100000, 0b00000001, 0b00000000}, // e_UDINT

      {0b11111111, 0b01110000, 0b00000001, 0b10000000}, // e_ULINT
      {0b10000000, 0b00000000, 0b00000001, 0b10000000}, // e_BYTE
      {0b11000100, 0b01000000, 0b00000001, 0b10000000}, // e_WORD
      {0b11100110, 0b01100000, 0b00000000, 0b10000000}, // e_DWORD

      {0b11110111, 0b01110000, 0b00000000, 0b00000000}, // e_LWORD
      {0b00000000, 0b00000000, 0b00000000, 0b00000000}, // e_DATE
      {0b00000000, 0b00000000, 0b00000000, 0b00000000}, // e_TIME_OF_DAY
      {0b00000000, 0b00000000, 0b00000000, 0b00000000}, // e_DATE_AND_TIME

      {0b00000000, 0b00000000, 0b00000000, 0b00000000}, // e_TIME
      {0b00000000, 0b00000000, 0b00000000, 0b00000000}, // e_CHAR
      {0b00000000, 0b00000000, 0b00000000, 0b00000000}, // e_WCHAR
      {0b00000000, 0b00000000, 0b00000000, 0b00000000}, // e_LDATE

      {0b00000000, 0b00000000, 0b00000000, 0b00000000}, // e_LTIME_OF_DAY
      {0b00000000, 0b00000000, 0b00000000, 0b00000000}, // e_LDATE_AND_TIME
      {0b00000000, 0b00000000, 0b00000000, 0b00000000},  // e_LTIME
      {0b11111111, 0b11100000, 0b00000000, 0b00000000}, // e_REAL

      {0b11111111, 0b11110000, 0b00000001, 0b00000000}, // e_LREAL
      {0b00000000, 0b00000000, 0b00000000, 0b00000000}, // e_STRING
      {0b00000000, 0b00000000, 0b00000000, 0b00000000} // e_WSTRING
  };

  if((pa_eSource < e_BOOL) || (pa_eSource > e_WSTRING) || (pa_eDestination < e_BOOL) || (pa_eDestination > e_WSTRING)){
    return false;
  }

  pa_rbUpCast = (UpCast[pa_eSource - e_BOOL][(pa_eDestination - e_BOOL) >> 3] & (0x80 >> ((pa_eDestination - e_BOOL) & 0x07))) != 0;
  pa_rbDownCast = (DownCast[pa_eSource - e_BOOL][(pa_eDestination - e_BOOL) >> 3] & (0x80 >> ((pa_eDestination - e_BOOL) & 0x07))) != 0;

  return (pa_rbUpCast | pa_rbDownCast);
}

void CIEC_ANY::specialCast(const CIEC_ANY &pa_roSrcValue, CIEC_ANY &pa_roDstValue){
  switch (pa_roSrcValue.getDataTypeID()){
    case CIEC_ANY::e_REAL:
      CIEC_REAL::castRealData(static_cast<const CIEC_REAL &>(pa_roSrcValue), pa_roDstValue);
      break;
    case CIEC_ANY::e_LREAL:
      CIEC_LREAL::castLRealData(static_cast<const CIEC_LREAL &>(pa_roSrcValue), pa_roDstValue);
      break;
    default:
      (void)pa_roDstValue; //to avoid warnings of unused parameter when real types aren't used
      //we should not be here log error
      DEVLOG_ERROR("CIEC_ANY::specialCast: special cast for unsupported source data type requested!\n");
      break;
  }
}

size_t CIEC_ANY::getToStringBufferSize() const {
  return sizeof(scmAnyToStringResponse);
}
