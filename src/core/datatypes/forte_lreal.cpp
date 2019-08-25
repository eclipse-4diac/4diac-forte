/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl,
 *    Ingo Hegny, Martin Melik Merkumians, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "forte_lreal.h"

#include "forte_real.h"
#include "forte_string.h"
#include "forte_wstring.h"

#include <forte_printer.h>

DEFINE_FIRMWARE_DATATYPE(LREAL, g_nStringIdLREAL)

int CIEC_LREAL::fromString(const char *paValue){
  char *pcEnd;
  const char *pacRunner = paValue;
  double realval = 0.0;

  if(0 == strncmp(pacRunner, "LREAL#", 6)){
    pacRunner += 6;
  }

  errno = 0;
  realval = strtod(pacRunner, &pcEnd);

  if((errno != 0) || (pacRunner == pcEnd)){
    return -1;
  }
  setTDFLOAT((TForteDFloat) realval);
  return static_cast<int>(pcEnd - paValue);
}

int CIEC_LREAL::toString(char* paValue, size_t paBufferSize) const {
  int nRetVal;
  nRetVal = forte_snprintf(paValue, paBufferSize, "%.*g", 15, getTDFLOAT());
  if((nRetVal < -1) || (nRetVal >= static_cast<int>(paBufferSize))) {
    nRetVal = -1;
  }
  return nRetVal;
}

void CIEC_LREAL::setValue(const CIEC_ANY& paValue){
  EDataTypeID eID = paValue.getDataTypeID();
  switch (eID){
    case e_LREAL:
      setValueSimple(paValue);
      break;
    case e_REAL:
      (*this) = static_cast<const CIEC_REAL &>(paValue).operator TForteFloat();
      break;
    case e_STRING:
      (*this).fromString(reinterpret_cast<const CIEC_STRING&>(paValue).getValue());
      break;
    case e_WSTRING:
      (*this).fromString(reinterpret_cast<const CIEC_WSTRING&>(paValue).getValue());
      break;
    case e_SINT:
    case e_INT:
    case e_DINT:
    case e_LINT:
      (*this) = static_cast<TForteDFloat>(*((CIEC_ANY::TLargestIntValueType *) paValue.getConstDataPtr()));
      break;
    default:
      (*this) =  static_cast<TForteDFloat>(*((CIEC_ANY::TLargestUIntValueType *) paValue.getConstDataPtr()));
      break;
  }
}

void CIEC_LREAL::castLRealData(const CIEC_LREAL &paSrcValue, CIEC_ANY &paDestValue){
  switch (paDestValue.getDataTypeID()){
    case CIEC_ANY::e_REAL:
      static_cast<CIEC_REAL &>(paDestValue) = static_cast<TForteFloat>(paSrcValue);
      break;
    case CIEC_ANY::e_LREAL:
      static_cast<CIEC_LREAL &>(paDestValue) = paSrcValue;
      break;
    case CIEC_ANY::e_BYTE:
    case CIEC_ANY::e_WORD:
    case CIEC_ANY::e_DWORD:
    case CIEC_ANY::e_LWORD:
      // bit string will cast to the binary representation of the real value
      paDestValue.setValue(paSrcValue);
      break;
    case CIEC_ANY::e_BOOL: //bool works better when treated as signed so that negative numbers will be treated as true to
    case CIEC_ANY::e_SINT:
    case CIEC_ANY::e_INT:
    case CIEC_ANY::e_DINT:
    case CIEC_ANY::e_LINT: {
      CIEC_LREAL::TValueType doubleValue = static_cast<CIEC_LREAL::TValueType>(paSrcValue);
      if(0 < doubleValue){
        doubleValue += 0.5;
      }
      if(0 > doubleValue){
        doubleValue -= 0.5;
      }
      *((CIEC_ANY::TLargestIntValueType *) paDestValue.getDataPtr()) = static_cast<CIEC_ANY::TLargestIntValueType>(doubleValue);
    }
      break;
    default: {
      //TODO maybe we should check for destination id to be in valid range (i.e., any_bit, any_unsigned_int, and time)
      //should not be necessary because of connect function, but who knows.
      CIEC_LREAL::TValueType doubleValue = static_cast<CIEC_LREAL::TValueType>(paSrcValue);
      if(0 < doubleValue){
        doubleValue += 0.5;
      }
      if(0 > doubleValue){
        doubleValue -= 0.5;
      }
      *((CIEC_ANY::TLargestUIntValueType *) paDestValue.getDataPtr()) = static_cast<CIEC_ANY::TLargestUIntValueType>(doubleValue);
    }
      break;
  }
}
