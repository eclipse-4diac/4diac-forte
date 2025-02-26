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

int CIEC_ANY::dummyInit(){
  return 0;
}

CStringDictionary::TStringId CIEC_ANY::getTypeNameID() const {
  return g_nStringIdANY;
}

CStringDictionary::TStringId CIEC_ANY::parseTypeName(const char *paValue, const char *paHashPos) {
  return CStringDictionary::getId(paValue, static_cast<size_t>(paHashPos - paValue));
}

bool CIEC_ANY::isCastable(EDataTypeID paSource, EDataTypeID paDestination, bool &paUpCast, bool &paDownCast){

  paUpCast = paDownCast = false;

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

  if((paSource < e_BOOL) || (paSource > e_WSTRING) || (paDestination < e_BOOL) || (paDestination > e_WSTRING)){
    return false;
  }

  paUpCast = (UpCast[paSource - e_BOOL][(paDestination - e_BOOL) >> 3] & (0x80 >> ((paDestination - e_BOOL) & 0x07))) != 0;
  paDownCast = (DownCast[paSource - e_BOOL][(paDestination - e_BOOL) >> 3] & (0x80 >> ((paDestination - e_BOOL) & 0x07))) != 0;

  return (paUpCast | paDownCast);
}

void CIEC_ANY::specialCast(const CIEC_ANY &paSrcValue, CIEC_ANY &paDstValue){
  switch (paSrcValue.getDataTypeID()){
    case CIEC_ANY::e_REAL:
      CIEC_REAL::castRealData(static_cast<const CIEC_REAL &>(paSrcValue), paDstValue);
      break;
    case CIEC_ANY::e_LREAL:
      CIEC_LREAL::castLRealData(static_cast<const CIEC_LREAL &>(paSrcValue), paDstValue);
      break;
    default:
      (void)paDstValue; //to avoid warnings of unused parameter when real types aren't used
      //we should not be here log error
      DEVLOG_ERROR("CIEC_ANY::specialCast: special cast for unsupported source data type requested!\n");
      break;
  }
}

