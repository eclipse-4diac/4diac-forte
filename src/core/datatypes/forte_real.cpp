/*******************************************************************************
  * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl,
  *    Ingo Hegny, Martin Melik Merkumians, Monika Wenger, Stanislav Meduna,
  *    Matthias Plasch
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "forte_real.h"
#include "forte_lreal.h"

#include <forte_printer.h>

DEFINE_FIRMWARE_DATATYPE(REAL, g_nStringIdREAL)

int CIEC_REAL::fromString(const char *pa_pacValue){
  char *pcEnd;
  const char *pacRunner = pa_pacValue;
  TForteFloat realval = 0.0;
  const TForteFloat TFLOAT_min = 3.4e-38f; // lower precision boundary
  const TForteFloat TFLOAT_max = 3.4e38f; // upper precision boundary

  if(0 == strncmp(pacRunner, "REAL#", 5)){
    pacRunner += 5;
  }

  
  #if defined(WIN32) || defined(__ECOS) || defined(VXWORKS)
  realval = static_cast<TForteFloat>(strtod(pacRunner, &pcEnd));
  #else
  realval = strtof(pacRunner, &pcEnd);
  #endif

  if(((fabs(realval) < TFLOAT_min) && (realval != 0)) || ((fabs(realval) > TFLOAT_max) && (realval != 0)) ||
      (pacRunner == pcEnd))
    return -1;

  setTFLOAT((TForteFloat) realval);
  return static_cast<int>(pcEnd - pa_pacValue);
}

int CIEC_REAL::toString(char* pa_acValue, unsigned int pa_nBufferSize) const{
  int nRetVal;
  nRetVal = forte_snprintf(pa_acValue, pa_nBufferSize, "%g", getTFLOAT());
  if((nRetVal < -1) || (nRetVal >= (int) pa_nBufferSize)){
    nRetVal = -1;  
  }
  return nRetVal;
}

void  CIEC_REAL::setValue(const CIEC_ANY& pa_roValue){
  EDataTypeID eID = pa_roValue.getDataTypeID();
  switch (eID) {
  case e_REAL:
    setValueSimple(pa_roValue);
    break;
#ifdef FORTE_USE_LREAL_DATATYPE
  case e_LREAL:
    (*this)=static_cast<TForteFloat>((CIEC_LREAL&)(pa_roValue));
    break;
#endif
  case e_STRING:
    (*this).fromString(((CIEC_STRING&)pa_roValue).getValue());
    break;
  case e_WSTRING:
    (*this).fromString(((CIEC_WSTRING&)pa_roValue).getValue());
    break;
  case e_SINT:
  case e_INT:
  case e_DINT:
  case e_LINT:
    (*this) = static_cast<TForteFloat>(*((CIEC_ANY::TLargestIntValueType *) pa_roValue.getConstDataPtr()));
    break;
  default:
    (*this) =  static_cast<TForteFloat>(*((CIEC_ANY::TLargestUIntValueType *) pa_roValue.getConstDataPtr()));
    break;
  }
}

void CIEC_REAL::castRealData(const CIEC_REAL &pa_roSrcValue, CIEC_ANY &pa_roDestValue){
  switch(pa_roDestValue.getDataTypeID()){
    case CIEC_ANY::e_REAL:
      static_cast<CIEC_REAL &>(pa_roDestValue) = pa_roSrcValue;
      break;
    case CIEC_ANY::e_LREAL:
#ifdef FORTE_USE_LREAL_DATATYPE
      static_cast<CIEC_LREAL &>(pa_roDestValue) = pa_roSrcValue;
#endif
      break;
    case CIEC_ANY::e_BYTE:
    case CIEC_ANY::e_WORD:
    case CIEC_ANY::e_DWORD:
    case CIEC_ANY::e_LWORD:
      // bit string will cast to the binary representation of the real value
      pa_roDestValue.setValue(pa_roSrcValue);
      break;
    case CIEC_ANY::e_BOOL:    //bool works better when treated as signed so that negative numbers will be treated as true to
    case CIEC_ANY::e_SINT:
    case CIEC_ANY::e_INT:
    case CIEC_ANY::e_DINT:
    case CIEC_ANY::e_LINT: {
      CIEC_REAL::TValueType floatValue = static_cast<CIEC_REAL::TValueType>(pa_roSrcValue);
      if(0 < floatValue){
        floatValue += 0.5f;
      }
      if(0 > floatValue){
        floatValue -= 0.5f;
      }
      *((CIEC_ANY::TLargestIntValueType *) pa_roDestValue.getDataPtr()) = static_cast<CIEC_ANY::TLargestIntValueType>(floatValue);
    }
      break;
    default: {
      //TODO maybe we should check for destination id to be in valid range (i.e., any_bit, any_unsigned_int, and time)
      //should not be necessary because of connect function, but who knows.
      CIEC_REAL::TValueType floatValue = static_cast<CIEC_REAL::TValueType>(pa_roSrcValue);
      if(0 < floatValue){
        floatValue += 0.5f;
      }
      if(0 > floatValue){
        floatValue -= 0.5f;
      }
      *((CIEC_ANY::TLargestUIntValueType *) pa_roDestValue.getDataPtr()) = static_cast<CIEC_ANY::TLargestUIntValueType>(floatValue);
    }
      break;
  }
}
