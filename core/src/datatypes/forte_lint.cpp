/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Monika Wenger, Alois Zoitl,
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "forte/datatypes/forte_lint.h"
#include "forte/datatypes/forte_any_bit.h"
#include "forte/datatypes/forte_any_int.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(LINT, "LINT"_STRID);

  void CIEC_LINT::setValue(const CIEC_ANY &paValue) {
    EDataTypeID eID = paValue.getDataTypeID();
    switch (eID) {
      case e_SINT:
      case e_INT:
      case e_DINT:
      case e_LINT:
        setTINT64(static_cast<TValueType>(static_cast<const CIEC_ANY_INT &>(paValue).getSignedValue()));
        break;
      case e_USINT:
      case e_UINT:
      case e_UDINT:
      case e_ULINT:
        setTINT64(static_cast<TValueType>(static_cast<const CIEC_ANY_INT &>(paValue).getUnsignedValue()));
        break;
      case e_BOOL:
      case e_BYTE:
      case e_WORD:
      case e_DWORD:
      case e_LWORD:
        setTINT64(static_cast<TValueType>(static_cast<const CIEC_ANY_BIT &>(paValue).getUnsignedValue()));
        break;
      case e_REAL:
        setTINT64(static_cast<TValueType>(static_cast<TForteFloat>(static_cast<const CIEC_REAL &>(paValue))));
        break;
      case e_LREAL:
        setTINT64(static_cast<TValueType>(static_cast<TForteDFloat>(static_cast<const CIEC_LREAL &>(paValue))));
        break;
      case e_STRING: (*this).fromString(static_cast<const CIEC_STRING &>(paValue).getStorage().c_str()); break;
      case e_WSTRING: (*this).fromString(static_cast<const CIEC_WSTRING &>(paValue).getValue()); break;
      default: break;
    }
  }

  template CIEC_LINT &CIEC_LINT::operator= <>(const CIEC_SINT &paValue);

  template CIEC_LINT &CIEC_LINT::operator= <>(const CIEC_INT &paValue);

  template CIEC_LINT &CIEC_LINT::operator= <>(const CIEC_DINT &paValue);

  template CIEC_LINT &CIEC_LINT::operator= <>(const CIEC_USINT &paValue);

  template CIEC_LINT &CIEC_LINT::operator= <>(const CIEC_UINT &paValue);

  template CIEC_LINT &CIEC_LINT::operator= <>(const CIEC_UDINT &paValue);

  const StringId CDataTypeTrait<CIEC_LINT>::scmDataTypeName = "LINT"_STRID;
} // namespace forte
