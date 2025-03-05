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
#include "forte_any_bit_variant.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP

#include "forte_any_bit_variant_gen.cpp"

#endif

DEFINE_FIRMWARE_DATATYPE(ANY_BIT_VARIANT, g_nStringIdANY_BIT)

void CIEC_ANY_BIT_VARIANT::setValue(const CIEC_ANY &paValue) {
  switch (paValue.getDataTypeID()) {
    case e_ANY:
      CIEC_ANY_BIT_VARIANT::setValue(paValue.unwrap());
      break;
    case e_BOOL:
      operator=(static_cast<const CIEC_BOOL &>(paValue));
      break;
    case e_BYTE:
      operator=(static_cast<const CIEC_BYTE &>(paValue));
      break;
    case e_WORD:
      operator=(static_cast<const CIEC_WORD &>(paValue));
      break;
    case e_DWORD:
      operator=(static_cast<const CIEC_DWORD &>(paValue));
      break;
    case e_LWORD:
      operator=(static_cast<const CIEC_LWORD &>(paValue));
      break;
    default:
      break;
  }
}

bool CIEC_ANY_BIT_VARIANT::setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId) {
  switch (paDataTypeId) {
    case e_BOOL:
      operator=(CIEC_BOOL(false));
      return true;
    case e_BYTE:
      operator=(CIEC_BYTE(0));
      return true;
    case e_WORD:
      operator=(CIEC_WORD(0));
      return true;
    case e_DWORD:
      operator=(CIEC_DWORD(0));
      return true;
    case e_LWORD:
      operator=(CIEC_LWORD(0));
      return true;
    default:
      break;
  }
  return false;
}

CIEC_ANY_BIT &CIEC_ANY_BIT_VARIANT::unwrap() {
  return std::visit([](auto &&value) -> CIEC_ANY_BIT & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_BIT, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, static_cast<CIEC_ANY_BIT_VARIANT::variant&>(*this));
}

const CIEC_ANY_BIT &CIEC_ANY_BIT_VARIANT::unwrap() const {
  return std::visit([](auto &&value) -> const CIEC_ANY_BIT & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_BIT, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, static_cast<const CIEC_ANY_BIT_VARIANT::variant&>(*this));
}

int CIEC_ANY_BIT_VARIANT::fromString(const char *paValue) {
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

int CIEC_ANY_BIT_VARIANT::toString(char *paValue, size_t paBufferSize) const {
  int result = -1;
  const CIEC_ANY &value = unwrap();
  const char *typeName = CStringDictionary::get(value.getTypeNameID());
  size_t typeNameLength = strlen(typeName);
  if (paBufferSize > typeNameLength + 2) {
    memcpy(paValue, typeName, typeNameLength);
    paValue[typeNameLength] = '#';
    result = static_cast<int>(typeNameLength) + 1 +
             value.toString(paValue + typeNameLength + 1, paBufferSize - typeNameLength - 1);
  }
  return result;
}

size_t CIEC_ANY_BIT_VARIANT::getToStringBufferSize() const {
  const CIEC_ANY &value = unwrap();
  const char *typeName = CStringDictionary::get(value.getTypeNameID());
  size_t typeNameLength = strlen(typeName);
  return typeNameLength + 1 + value.getToStringBufferSize();
}

