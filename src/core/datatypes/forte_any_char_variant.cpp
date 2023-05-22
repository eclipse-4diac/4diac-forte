/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/
#include "forte_any_char_variant.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP

#include "forte_any_char_variant_gen.cpp"

#endif

DEFINE_FIRMWARE_DATATYPE(ANY_CHAR_VARIANT, g_nStringIdANY_CHAR)

void CIEC_ANY_CHAR_VARIANT::setValue(const CIEC_ANY &paValue) {
  switch (paValue.getDataTypeID()) {
    case e_ANY:
      CIEC_ANY_CHAR_VARIANT::setValue(paValue.unwrap());
      break;
    case e_CHAR:
      operator=(static_cast<const CIEC_CHAR &>(paValue));
      break;
    case e_WCHAR:
      operator=(static_cast<const CIEC_WCHAR &>(paValue));
      break;
    default:
      break;
  }
}

bool CIEC_ANY_CHAR_VARIANT::setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId) {
  switch (paDataTypeId) {
    case e_CHAR:
      operator=(CIEC_CHAR(0));
      return true;
    case e_WCHAR:
      operator=(CIEC_WCHAR(0));
      return true;
    default:
      break;
  }
  return false;
}

CIEC_ANY_CHAR &CIEC_ANY_CHAR_VARIANT::unwrap() {
  return std::visit([](auto &&value) -> CIEC_ANY_CHAR & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_CHAR, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, *this);;
}

const CIEC_ANY_CHAR &CIEC_ANY_CHAR_VARIANT::unwrap() const {
  return std::visit([](auto &&value) -> const CIEC_ANY_CHAR & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_CHAR, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, *this);;
}

int CIEC_ANY_CHAR_VARIANT::fromString(const char *paValue) {
  int nRetVal = -1;
  const char *hashPos = strchr(paValue, '#');
  if (nullptr != hashPos) {
    CStringDictionary::TStringId typeNameId = parseTypeName(paValue, hashPos);
    CIEC_ANY::EDataTypeID dataTypeId = CIEC_ANY_ELEMENTARY::getElementaryDataTypeId(typeNameId);
    if (setDefaultValue(dataTypeId)) {
      CIEC_ANY &value = unwrap();
      nRetVal = value.fromString(paValue);
    }
  }
  return nRetVal;
}

int CIEC_ANY_CHAR_VARIANT::toString(char *paValue, size_t paBufferSize) const {
  const CIEC_ANY &value = unwrap();
  return value.toString(paValue, paBufferSize);
}

size_t CIEC_ANY_CHAR_VARIANT::getToStringBufferSize() const {
  const CIEC_ANY &value = unwrap();
  return value.getToStringBufferSize();
}

