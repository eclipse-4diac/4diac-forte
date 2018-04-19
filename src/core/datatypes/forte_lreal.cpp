/*******************************************************************************
  * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
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

#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_64BIT_DATATYPES

DEFINE_FIRMWARE_DATATYPE(LREAL, g_nStringIdLREAL)

int CIEC_LREAL::fromString(const char *pa_pacValue){
  char *pcEnd;
  const char *pacRunner = pa_pacValue;
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
  return static_cast<int>(pcEnd - pa_pacValue);
}

int CIEC_LREAL::toString(char* pa_pacValue, unsigned int pa_nBufferSize) const{
  int nRetVal;
  nRetVal = forte_snprintf(pa_pacValue, pa_nBufferSize, "%.*g", 15, getTDFLOAT());
  if((nRetVal < -1) || (nRetVal >= (int) pa_nBufferSize)){
    nRetVal = -1;
  }
  return nRetVal;
}

void CIEC_LREAL::setValue(const CIEC_ANY& pa_roValue){
  EDataTypeID eID = pa_roValue.getDataTypeID();
  switch (eID){
    case e_LREAL:
      setValueSimple(pa_roValue);
      break;
    case e_REAL:
      (*this) = (((CIEC_REAL &) pa_roValue)).operator TForteFloat();
      break;
    case e_STRING:
      (*this).fromString(((CIEC_STRING&) pa_roValue).getValue());
      break;
    case e_WSTRING:
      (*this).fromString(((CIEC_WSTRING&) pa_roValue).getValue());
      break;
    case e_SINT:
    case e_INT:
    case e_DINT:
    case e_LINT:
      (*this) = static_cast<TForteDFloat>(*((CIEC_ANY::TLargestIntValueType *) pa_roValue.getConstDataPtr()));
      break;
    default:
      (*this) =  static_cast<TForteDFloat>(*((CIEC_ANY::TLargestUIntValueType *) pa_roValue.getConstDataPtr()));
      break;
  }
}

void CIEC_LREAL::castLRealData(const CIEC_LREAL &pa_roSrcValue, CIEC_ANY &pa_roDestValue){
  switch (pa_roDestValue.getDataTypeID()){
    case CIEC_ANY::e_REAL:
      static_cast<CIEC_REAL &>(pa_roDestValue) = static_cast<TForteFloat>(pa_roSrcValue);
      break;
    case CIEC_ANY::e_LREAL:
      static_cast<CIEC_LREAL &>(pa_roDestValue) = pa_roSrcValue;
      break;
    case CIEC_ANY::e_BYTE:
    case CIEC_ANY::e_WORD:
    case CIEC_ANY::e_DWORD:
    case CIEC_ANY::e_LWORD:
      // bit string will cast to the binary representation of the real value
      pa_roDestValue.setValue(pa_roSrcValue);
      break;
    case CIEC_ANY::e_BOOL: //bool works better when treated as signed so that negative numbers will be treated as true to
    case CIEC_ANY::e_SINT:
    case CIEC_ANY::e_INT:
    case CIEC_ANY::e_DINT:
    case CIEC_ANY::e_LINT:
      *((CIEC_ANY::TLargestIntValueType *) pa_roDestValue.getDataPtr()) = static_cast<CIEC_ANY::TLargestIntValueType>(pa_roSrcValue);
      break;
    default:
      //TODO maybe we should check for destination id to be in valid range (i.e., any_bit, any_unsigned_int, and time)
      //should not be necessary because of connect function, but who knows.
      *((CIEC_ANY::TLargestUIntValueType *) pa_roDestValue.getDataPtr()) = static_cast<CIEC_ANY::TLargestUIntValueType>(pa_roSrcValue);
      break;
  }
}

#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE
