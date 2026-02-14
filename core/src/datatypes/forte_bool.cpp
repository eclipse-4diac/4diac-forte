/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Primetals Technologies Ausria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl, Rene Smodic,
 *     Gerhard Ebenhofer, Ingo Hegny, Monika Wenger
 *               - initial implementation and rework communication infrastructure
 *   Alois Zoitl - migrated data type toString to std::string
 *******************************************************************************/
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_any_bit.h"
#include "forte/datatypes/forte_any_int.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

using namespace forte::literals;
using namespace std::literals::string_literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(BOOL, "BOOL"_STRID)

  void CIEC_BOOL::setValue(const CIEC_ANY &paValue) {
    EDataTypeID eID = paValue.getDataTypeID();
    switch (eID) {
      case e_BOOL:
      case e_BYTE:
      case e_WORD:
      case e_DWORD:
      case e_LWORD: setTBOOL8(0 != static_cast<const CIEC_ANY_BIT &>(paValue).getUnsignedValue()); break;
      case e_SINT:
      case e_INT:
      case e_DINT:
      case e_LINT: setTBOOL8(0 != static_cast<const CIEC_ANY_INT &>(paValue).getSignedValue()); break;
      case e_USINT:
      case e_UINT:
      case e_UDINT:
      case e_ULINT: setTBOOL8(0 != static_cast<const CIEC_ANY_INT &>(paValue).getUnsignedValue()); break;
      case e_REAL: setTBOOL8(0.0f != static_cast<TForteFloat>(static_cast<const CIEC_REAL &>(paValue))); break;
      case e_LREAL: setTBOOL8(0.0 != static_cast<TForteDFloat>(static_cast<const CIEC_LREAL &>(paValue))); break;
      case e_STRING: (*this).fromString(static_cast<const CIEC_STRING &>(paValue).getStorage().c_str()); break;
      case e_WSTRING: (*this).fromString(static_cast<const CIEC_WSTRING &>(paValue).getValue()); break;
      default: break;
    }
  }

  int CIEC_BOOL::fromString(const char *paValue) {
    int nRetVal = 0;

    if (0 == strncmp(paValue, "BOOL#", 5)) {
      paValue += 5;
      nRetVal += 5;
    }

    if (paValue[0] == '1') {
      setTBOOL8(true);
      nRetVal += 1;
    } else {
      if (paValue[0] == '0') {
        setTBOOL8(false);
        nRetVal += 1;
      } else {
        if (((paValue[0] == 'T') || (paValue[0] == 't')) && ((paValue[1] == 'R') || (paValue[1] == 'r')) &&
            ((paValue[2] == 'U') || (paValue[2] == 'u')) && ((paValue[3] == 'E') || (paValue[3] == 'e'))) {
          setTBOOL8(true);
          nRetVal += 4;
        } else {
          if (((paValue[0] == 'F') || (paValue[0] == 'f')) && ((paValue[1] == 'A') || (paValue[1] == 'a')) &&
              ((paValue[2] == 'L') || (paValue[2] == 'l')) && ((paValue[3] == 'S') || (paValue[3] == 's')) &&
              ((paValue[4] == 'E') || (paValue[4] == 'e'))) {
            setTBOOL8(false);
            nRetVal += 5;
          } else {
            nRetVal = -1;
          }
        }
      }
    }
    return nRetVal;
  }

  void CIEC_BOOL::toString(std::string &paTargetBuf) const {
    paTargetBuf += getTBOOL8() ? "TRUE"s : "FALSE"s;
  }

  const StringId CDataTypeTrait<CIEC_BOOL>::scmDataTypeName = "BOOL"_STRID;
} // namespace forte
