/*******************************************************************************
 * Copyright (c) 2005 - 2025 Profactor GmbH, ACIN, fortiss GmbH,
 *                           Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl,
 *     Ingo Hegny, Martin Melik Merkumians, Monika Wenger
 *               - initial implementation and rework communication infrastructure
 *   Alois Zoitl - migrated data type toString to std::string
 *******************************************************************************/
#include <cmath>
#include <format>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_lint.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(LREAL, "LREAL"_STRID)

  int CIEC_LREAL::fromString(const char *paValue) {
    char *pcEnd;
    const char *pacRunner = paValue;
    double realval = 0.0;

    if (0 == strncmp(pacRunner, "LREAL#", 6)) {
      pacRunner += 6;
    }

    errno = 0;
    realval = strtod(pacRunner, &pcEnd);

    if (pacRunner == pcEnd || errno != 0) {
      return -1;
    }
    setTDFLOAT((TForteDFloat) realval);
    return static_cast<int>(pcEnd - paValue);
  }

  void CIEC_LREAL::toString(std::string &paTargetBuf) const {
    auto startPos = paTargetBuf.size();
    std::format_to(std::back_inserter(paTargetBuf), "{:.{}g}", getTDFLOAT(),
                   std::numeric_limits<TValueType>::max_digits10);
    normalizeToStringRepresentation(paTargetBuf, startPos);
  }

  void CIEC_LREAL::setValue(const CIEC_ANY &paValue) {
    EDataTypeID eID = paValue.getDataTypeID();
    switch (eID) {
      case e_LREAL: setValueSimple(paValue); break;
      case e_REAL: setTDFLOAT(static_cast<TForteFloat>(static_cast<const CIEC_REAL &>(paValue))); break;
      case e_STRING: (*this).fromString(reinterpret_cast<const CIEC_STRING &>(paValue).getStorage().c_str()); break;
      case e_WSTRING: (*this).fromString(reinterpret_cast<const CIEC_WSTRING &>(paValue).getValue()); break;
      case e_SINT:
      case e_INT:
      case e_DINT:
      case e_LINT:
        setTDFLOAT(static_cast<TValueType>(static_cast<TForteInt64>(static_cast<const CIEC_LINT &>(paValue))));
        break;
      case e_BYTE:
      case e_WORD:
      case e_DWORD:
      case e_LWORD:
        // bit string will cast to the binary representation of the real value
        setValueSimple(paValue);
        break;
      default: // UINT types
        setTDFLOAT(static_cast<TValueType>(static_cast<TForteUInt64>(static_cast<const CIEC_ULINT &>(paValue))));
        break;
    }
  }

  void CIEC_LREAL::castLRealData(const CIEC_LREAL &paSrcValue, CIEC_ANY &paDestValue) {
    switch (paDestValue.getDataTypeID()) {
      case e_REAL:
        static_cast<CIEC_REAL &>(paDestValue) =
            CIEC_REAL(static_cast<CIEC_REAL::TValueType>(static_cast<TValueType>(paSrcValue)));
        break;
      case e_LREAL: static_cast<CIEC_LREAL &>(paDestValue) = paSrcValue; break;
      case e_BYTE:
      case e_WORD:
      case e_DWORD:
      case e_LWORD:
        // bit string will cast to the binary representation of the real value
        paDestValue.setValue(paSrcValue);
        break;
      default: {
        TValueType doubleValue = static_cast<TValueType>(paSrcValue);
        *(reinterpret_cast<TLargestUIntValueType *>(paDestValue.getDataPtr())) =
            static_cast<TLargestUIntValueType>(std::llrint(doubleValue));
        break;
      }
    }
  }

  const StringId CDataTypeTrait<CIEC_LREAL>::scmDataTypeName = "LREAL"_STRID;
} // namespace forte
