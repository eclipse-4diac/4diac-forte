/*******************************************************************************
 * Copyright (c) 2007 - 2013 ACIN, nxtcontrol GmbH, Profactor GmbH, fortiss GmbH
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
 *******************************************************************************/
#include "forte_any.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_any_gen.cpp"
#endif
#include "forte_real.h"
#include "forte_lreal.h"
#include <devlog.h>

const CTypeLib::CDataTypeEntry CIEC_ANY::csmFirmwareDataTypeEntry_CIEC_ANY(g_nStringIdANY, CIEC_ANY::createDataType);

const char CIEC_ANY::scmAnyToStringResponse[] = "ND (ANY)";

int CIEC_ANY::dummyInit(){
  return 0;
}

// A way to write binary literals (inspiration: http://www.thescripts.com/forum/thread219656.html)

#define HEX__(n) 0x##n##LU

#define B8__(x) \
 ((x&0x0000000FLU)?1:0) \
|((x&0x000000F0LU)?2:0) \
|((x&0x00000F00LU)?4:0) \
|((x&0x0000F000LU)?8:0) \
|((x&0x000F0000LU)?16:0) \
|((x&0x00F00000LU)?32:0) \
|((x&0x0F000000LU)?64:0) \
|((x&0xF0000000LU)?128:0)

#define B8(d) ((unsigned char)B8__(HEX__(d)))

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
    if(0 != acHashPos){
      CStringDictionary::TStringId nTypeNameId = parseTypeName(pa_pacValue, acHashPos);
      if(CStringDictionary::scm_nInvalidStringId != nTypeNameId && 0 != CTypeLib::createDataTypeInstance(nTypeNameId, (TForteByte *) this)) {
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
  static const TForteByte UpCast[e_WSTRING - e_BOOL + 1][(e_WSTRING - e_BOOL + 8) / 8] = { { B8(11111111), B8(11111000), B8(01100000) }, // e_BOOL to x
      { B8(01111000), B8(01111000), B8(01100000) }, // e_SINT
      { B8(00111000), B8(00111000), B8(01100000) }, // e_INT
      { B8(00011000), B8(00011000), B8(00100000) }, // e_DINT

      { B8(00001000), B8(00001000), B8(10000000) }, // e_LINT
      { B8(00111111), B8(11111000), B8(01100000) }, // e_USINT
      { B8(00011011), B8(10111000), B8(01100000) }, // e_UINT
      { B8(00001001), B8(10011000), B8(00100000) }, // e_UDINT

      { B8(00000000), B8(10001000), B8(00000000) }, // e_ULINT
      { B8(01111111), B8(11111000), B8(00000000) }, // e_BYTE
      { B8(00111011), B8(10111000), B8(00000000) }, // e_WORD
      { B8(00011001), B8(10011000), B8(01000000) }, // e_DWORD

      { B8(00001000), B8(10001000), B8(01100000) }, // e_LWORD
      { B8(00000000), B8(00000100), B8(00000000) }, // e_DATE
      { B8(00000000), B8(00000010), B8(00000000) }, // e_TIME_OF_DAY
      { B8(00000000), B8(00000001), B8(00000000) }, // e_DATE_AND_TIME

      { B8(00001000), B8(00001000), B8(10000000) }, // e_TIME
      { B8(00000000), B8(00011000), B8(01100000) }, // e_REAL
      { B8(00000000), B8(00001000), B8(00100000) }, // e_LREAL
      { B8(00000000), B8(00000000), B8(00010000) }, // e_STRING

      { B8(00000000), B8(00000000), B8(00001000) } // e_WSTRING
  };

  // Downcast: information may get lost (eventually catastrophically)
  static const TForteByte DownCast[e_WSTRING - e_BOOL + 1][(e_WSTRING - e_BOOL + 8) / 8] = { { B8(00000000), B8(00000000), B8(00000000) }, // e_BOOL to x
      { B8(10000111), B8(10000000), B8(00000000) }, // e_SINT
      { B8(11000111), B8(11000000), B8(00000000) }, // e_INT
      { B8(11100111), B8(11100000), B8(01000000) }, // e_DINT

      { B8(11110111), B8(11110000), B8(01100000) }, // e_LINT
      { B8(11000000), B8(00000000), B8(00000000) }, // e_USINT
      { B8(11100100), B8(01000000), B8(00000000) }, // e_UINT
      { B8(11110110), B8(01100000), B8(01000000) }, // e_UDINT

      { B8(11111111), B8(01110000), B8(01100000) }, // e_ULINT
      { B8(10000000), B8(00000000), B8(01100000) }, // e_BYTE
      { B8(11000100), B8(01000000), B8(01100000) }, // e_WORD
      { B8(11100110), B8(01100000), B8(00100000) }, // e_DWORD

      { B8(11110111), B8(01110000), B8(00000000) }, // e_LWORD
      { B8(00000000), B8(00000000), B8(00000000) }, // e_DATE
      { B8(00000000), B8(00000000), B8(00000000) }, // e_TIME_OF_DAY
      { B8(00000000), B8(00000000), B8(00000000) }, // e_DATE_AND_TIME

      { B8(11110111), B8(11110000), B8(01100000) }, // e_TIME
      { B8(11111111), B8(11100000), B8(00000000) }, // e_REAL
      { B8(11111111), B8(11110000), B8(01000000) }, // e_LREAL
      { B8(00000000), B8(00000000), B8(00000000) }, // e_STRING

      { B8(00000000), B8(00000000), B8(00000000) } // e_WSTRING
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
#ifdef FORTE_USE_REAL_DATATYPE
    case CIEC_ANY::e_REAL:
      CIEC_REAL::castRealData(static_cast<const CIEC_REAL &>(pa_roSrcValue), pa_roDstValue);
      break;
#endif
#ifdef FORTE_USE_LREAL_DATATYPE
    case CIEC_ANY::e_LREAL:
      CIEC_LREAL::castLRealData(static_cast<const CIEC_LREAL &>(pa_roSrcValue), pa_roDstValue);
      break;
#endif
    default:
      (void)pa_roDstValue; //to avoid warnings of unused parameter when real types aren't used
      //we should not be here log error
      DEVLOG_ERROR("CIEC_ANY::specialCast: special cast for unsupported source data type requested!\n");
      break;
  }
}

const TForteByte CIEC_ANY::csmStringBufferSize[] = {
         9 /*e_ANY*/,
         6 /*e_BOOL (0, 1)*/,
         5 /*e_SINT (-128, +127)*/,
         7 /*e_INT (-32768, +32767)*/,
        12 /*e_DINT (-2^31, +2^31-1)*/,
        21 /*e_LINT (-2^63. +2^63-1)*/,
         5 /*e_USINT (0, +255)*/,
         7 /*e_UINT (0, +65535)*/,
        12 /*e_UDINT (0, +2^32-1)*/,
        22 /*e_ULINT (0, +2^64-1)*/,
         9 /*e_BYTE (0, 16#FF)*/,
        17 /*e_WORD (0, 16#FFFF)*/,
        33 /*e_DWORD (0, 16#FFFF FFFF)*/,
        65 /*e_LWORD (0, 16#FFFF FFFF FFFF FFFF)*/,
        11 /*e_DATE (d#0001-01-01)*/,
         9 /*e_TIME_OF_DAY (tod#00:00:00)*/,
        24 /*e_DATE_AND_TIME (dt#0001-01-01-00:00:00.000)*/,
        27 /*e_TIME (t#0)*/,
        14 /*e_REAL (32bit = 1bit sign, 8bit exponent, 23bit fraction)*/,
        23 /*e_LREAL (64bit = 1bit sign, 11bit exponent, 52bit fraction)*/,
         8 /*e_STRING multiply with string length +1 for \0*/,
        16 /*e_WSTRING multiply with string length +1 for \0*/,
         0 /*e_DerivedData*/,
         0 /*e_DirectlyDerivedData*/,
         0 /*e_EnumeratedData*/,
         0 /*e_SubrangeData*/,
         0 /*e_ARRAY*/,
         0 /*e_STRUCT*/,
         0 /*e_External*/,
         0 /*e_Max*/
    };

size_t CIEC_ANY::getToStringBufferSize() const {
  return csmStringBufferSize[getDataTypeID()];
}
