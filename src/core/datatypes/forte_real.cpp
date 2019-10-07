/*******************************************************************************
 * Copyright (c) 2005 - 2019 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl,
 *    Ingo Hegny, Martin Melik Merkumians, Monika Wenger, Stanislav Meduna,
 *    Matthias Plasch
 *      - initial implementation and rework communication infrastructure
 *    Jose Cabral
 *      - Move arch dependant code (strtod) to the arch folder
 *******************************************************************************/
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "forte_real.h"
#include "forte_lreal.h"

#include <forte_printer.h>
#include "../../arch/forte_realFunctions.h"

DEFINE_FIRMWARE_DATATYPE(REAL, g_nStringIdREAL)

int CIEC_REAL::fromString(const char *paValue){
  char *pcEnd;
  const char *pacRunner = paValue;
  TForteFloat realval = 0.0;
  const TForteFloat TFLOAT_min = 3.4e-38F; // lower precision boundary
  const TForteFloat TFLOAT_max = 3.4e38F; // upper precision boundary

  if(0 == strncmp(pacRunner, "REAL#", 5)){
    pacRunner += 5;
  }

  realval = forte_stringToFloat(pacRunner, &pcEnd);

  if(((fabs(realval) < TFLOAT_min) && (realval != 0)) || ((fabs(realval) > TFLOAT_max) && (realval != 0)) ||
      (pacRunner == pcEnd)) {
    return -1;
  }

  setTFLOAT((TForteFloat) realval);
  return static_cast<int>(pcEnd - paValue);
}

int CIEC_REAL::toString(char* paValue, size_t paBufferSize) const {
  int nRetVal;
  nRetVal = forte_snprintf(paValue, paBufferSize, "%g", getTFLOAT());
  if((nRetVal < -1) || (nRetVal >= static_cast<int>(paBufferSize))) {
    nRetVal = -1;
  }
  return nRetVal;
}

void  CIEC_REAL::setValue(const CIEC_ANY& paValue){
  EDataTypeID eID = paValue.getDataTypeID();
  switch (eID) {
  case e_REAL:
    setValueSimple(paValue);
    break;
#ifdef FORTE_USE_LREAL_DATATYPE
  case e_LREAL:
    (*this)=static_cast<TForteFloat>((CIEC_LREAL&)(paValue));
    break;
#endif
  case e_STRING:
    (*this).fromString(((CIEC_STRING&)paValue).getValue());
    break;
  case e_WSTRING:
    (*this).fromString(((CIEC_WSTRING&)paValue).getValue());
    break;
  case e_SINT:
  case e_INT:
  case e_DINT:
  case e_LINT:
    (*this) = static_cast<TForteFloat>(*((CIEC_ANY::TLargestIntValueType *) paValue.getConstDataPtr()));
    break;
  default:
    (*this) =  static_cast<TForteFloat>(*((CIEC_ANY::TLargestUIntValueType *) paValue.getConstDataPtr()));
    break;
  }
}

void CIEC_REAL::castRealData(const CIEC_REAL &paSrcValue, CIEC_ANY &paDestValue){
  switch(paDestValue.getDataTypeID()){
    case CIEC_ANY::e_REAL:
      static_cast<CIEC_REAL &>(paDestValue) = paSrcValue;
      break;
    case CIEC_ANY::e_LREAL:
#ifdef FORTE_USE_LREAL_DATATYPE
      static_cast<CIEC_LREAL &>(paDestValue) = paSrcValue;
#endif
      break;
    case CIEC_ANY::e_BYTE:
    case CIEC_ANY::e_WORD:
    case CIEC_ANY::e_DWORD:
    case CIEC_ANY::e_LWORD:
      // bit string will cast to the binary representation of the real value
      paDestValue.setValue(paSrcValue);
      break;
    case CIEC_ANY::e_BOOL:    //bool works better when treated as signed so that negative numbers will be treated as true to
    case CIEC_ANY::e_SINT:
    case CIEC_ANY::e_INT:
    case CIEC_ANY::e_DINT:
    case CIEC_ANY::e_LINT: {
      CIEC_REAL::TValueType floatValue = static_cast<CIEC_REAL::TValueType>(paSrcValue);
      if(0 < floatValue){
        floatValue += 0.5F;
      }
      if(0 > floatValue){
        floatValue -= 0.5F;
      }
      *((CIEC_ANY::TLargestIntValueType *) paDestValue.getDataPtr()) = static_cast<CIEC_ANY::TLargestIntValueType>(floatValue);
    }
      break;
    default: {
      //TODO maybe we should check for destination id to be in valid range (i.e., any_bit, any_unsigned_int, and time)
      //should not be necessary because of connect function, but who knows.
      CIEC_REAL::TValueType floatValue = static_cast<CIEC_REAL::TValueType>(paSrcValue);
      if(0 < floatValue){
        floatValue += 0.5F;
      }
      if(0 > floatValue){
        floatValue -= 0.5F;
      }
      *(reinterpret_cast<CIEC_ANY::TLargestUIntValueType*>(paDestValue.getDataPtr())) = static_cast<CIEC_ANY::TLargestUIntValueType>(floatValue);
    }
      break;
  }
}
