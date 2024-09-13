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
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "forte_real.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_real_gen.cpp"
#endif
#include "forte_lreal.h"
#include "forte_lint.h"
#include "forte_ulint.h"

#include <forte_printer.h>
#include "../../arch/forte_realFunctions.h"

DEFINE_FIRMWARE_DATATYPE(REAL, g_nStringIdREAL)

int CIEC_REAL::fromString(const char *paValue){
  char *pcEnd;
  const char *pacRunner = paValue;
  TForteFloat realval = 0.0;

  if(0 == strncmp(pacRunner, "REAL#", 5)){
    pacRunner += 5;
  }

  errno = 0;
  realval = forte_stringToFloat(pacRunner, &pcEnd);

  if(pacRunner == pcEnd || !std::isfinite(realval) || errno != 0) {
    return -1;
  }

  setTFLOAT((TForteFloat) realval);
  return static_cast<int>(pcEnd - paValue);
}

int CIEC_REAL::toString(char* paValue, size_t paBufferSize) const {
  int nRetVal = forte_snprintf(paValue, paBufferSize, "%.*g", std::numeric_limits<CIEC_REAL::TValueType>::max_digits10, getTFLOAT());
  if((nRetVal < 0) || (nRetVal >= static_cast<int>(paBufferSize))) {
    return -1;
  }
  // Check if there is a trailing decimal fraction or exponent
  return normalizeToStringRepresentation(paValue, paBufferSize, nRetVal);
}

void CIEC_REAL::setValue(const CIEC_ANY& paValue){
  EDataTypeID eID = paValue.getDataTypeID();
  switch (eID) {
  case e_REAL:
    setValueSimple(paValue);
    break;
  case e_LREAL:
    setTFLOAT(static_cast<CIEC_REAL::TValueType>(static_cast<CIEC_LREAL::TValueType>(static_cast<const CIEC_LREAL &>(paValue))));
    break;
  case e_STRING:
    (*this).fromString(((CIEC_STRING&)paValue).getStorage().c_str());
    break;
  case e_WSTRING:
    (*this).fromString(((CIEC_WSTRING&)paValue).getValue());
    break;
  case e_SINT:
  case e_INT:
  case e_DINT:
  case e_LINT:
    setTFLOAT(static_cast<CIEC_REAL::TValueType>(static_cast<CIEC_LINT::TValueType>(static_cast<const CIEC_LINT &>(paValue))));
    break;
  case CIEC_ANY::e_BYTE:
  case CIEC_ANY::e_WORD:
  case CIEC_ANY::e_DWORD:
  case CIEC_ANY::e_LWORD:
    setValueSimple(paValue);
    break;
  default: //UINT types
    setTFLOAT(static_cast<CIEC_REAL::TValueType>(static_cast<CIEC_ULINT::TValueType>(static_cast<const CIEC_ULINT &>(paValue))));
    break;
  }
}

void CIEC_REAL::castRealData(const CIEC_REAL &paSrcValue, CIEC_ANY &paDestValue){
  switch(paDestValue.getDataTypeID()){
    case CIEC_ANY::e_REAL:
      static_cast<CIEC_REAL &>(paDestValue) = paSrcValue;
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
    default: {
      CIEC_REAL::TValueType floatValue = static_cast<CIEC_REAL::TValueType>(paSrcValue);
      *(reinterpret_cast<CIEC_ANY::TLargestUIntValueType*>(paDestValue.getDataPtr())) = static_cast<CIEC_ANY::TLargestUIntValueType>(std::llrint(floatValue));
      break;
    }
  }
}
