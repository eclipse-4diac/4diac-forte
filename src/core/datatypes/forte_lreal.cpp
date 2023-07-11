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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_lreal_gen.cpp"
#endif

#include "forte_real.h"
#include "forte_string.h"
#include "forte_wstring.h"
#include "forte_lint.h"
#include "forte_ulint.h"

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

  if(pacRunner == pcEnd || !isfinite(realval) || errno != 0){
    return -1;
  }
  setTDFLOAT((TForteDFloat) realval);
  return static_cast<int>(pcEnd - paValue);
}

int CIEC_LREAL::toString(char* paValue, size_t paBufferSize) const {
  int nRetVal = forte_snprintf(paValue, paBufferSize, "%.*g", std::numeric_limits<CIEC_LREAL::TValueType>::max_digits10, getTDFLOAT());
  if((nRetVal < -1) || (nRetVal >= static_cast<int>(paBufferSize))) {
    return -1;
  }
  return normalizeToStringRepresentation(paValue, paBufferSize, nRetVal);
}

void CIEC_LREAL::setValue(const CIEC_ANY& paValue){
  EDataTypeID eID = paValue.getDataTypeID();
  switch (eID){
    case e_LREAL:
      setValueSimple(paValue);
      break;
    case e_REAL:
      setTDFLOAT(static_cast<TForteFloat>(static_cast<const CIEC_REAL &>(paValue)));
      break;
    case e_STRING:
      (*this).fromString(reinterpret_cast<const CIEC_STRING&>(paValue).getStorage().c_str());
      break;
    case e_WSTRING:
      (*this).fromString(reinterpret_cast<const CIEC_WSTRING&>(paValue).getValue());
      break;
    case e_SINT:
    case e_INT:
    case e_DINT:
    case e_LINT:
      setTDFLOAT(static_cast<CIEC_LREAL::TValueType>(static_cast<TForteInt64>(static_cast<const CIEC_LINT&>(paValue))));
      break;
    case CIEC_ANY::e_BYTE:
    case CIEC_ANY::e_WORD:
    case CIEC_ANY::e_DWORD:
    case CIEC_ANY::e_LWORD:
      // bit string will cast to the binary representation of the real value
      setValueSimple(paValue);
      break;
    default: //UINT types
      setTDFLOAT(static_cast<CIEC_LREAL::TValueType>(static_cast<TForteUInt64>(static_cast<const CIEC_ULINT&>(paValue))));
      break;
  }
}

void CIEC_LREAL::castLRealData(const CIEC_LREAL &paSrcValue, CIEC_ANY &paDestValue){
  switch (paDestValue.getDataTypeID()){
    case CIEC_ANY::e_REAL:
      static_cast<CIEC_REAL &>(paDestValue) = CIEC_REAL(static_cast<TForteFloat>(paSrcValue));
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

size_t CIEC_LREAL::getToStringBufferSize() const {
  return sizeof("-2.2250738585072014E-308"); //Minimal double number, negative for additional sign
}
