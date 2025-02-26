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
#include "forte_any_unsigned_variant.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "forte_any_unsigned_variant_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(ANY_UNSIGNED_VARIANT, g_nStringIdANY_UNSIGNED)

void CIEC_ANY_UNSIGNED_VARIANT::setValue(const CIEC_ANY &paValue) {
  switch (paValue.getDataTypeID()) {
    case e_ANY:
      CIEC_ANY_UNSIGNED_VARIANT::setValue(paValue.unwrap());
      break;
    case e_USINT:
      operator=(static_cast<const CIEC_USINT &>(paValue));
      break;
    case e_UINT:
      operator=(static_cast<const CIEC_UINT &>(paValue));
      break;
    case e_UDINT:
      operator=(static_cast<const CIEC_UDINT &>(paValue));
      break;
    case e_ULINT:
      operator=(static_cast<const CIEC_ULINT &>(paValue));
      break;
    default:
      break;
  }
}

bool CIEC_ANY_UNSIGNED_VARIANT::setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId) {
  switch (paDataTypeId) {
    case e_USINT:
      operator=(CIEC_USINT(0));
      return true;
    case e_UINT:
      operator=(CIEC_UINT(0));
      return true;
    case e_UDINT:
      operator=(CIEC_UDINT(0));
      return true;
    case e_ULINT:
      operator=(CIEC_ULINT(0));
      return true;
    default:
      break;
  }
  return false;
}

CIEC_ANY_UNSIGNED &CIEC_ANY_UNSIGNED_VARIANT::unwrap() {
  return std::visit([](auto &&value) -> CIEC_ANY_UNSIGNED & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_UNSIGNED, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, static_cast<CIEC_ANY_UNSIGNED_VARIANT::variant&>(*this));
}

const CIEC_ANY_UNSIGNED &CIEC_ANY_UNSIGNED_VARIANT::unwrap() const {
  return std::visit([](auto &&value) -> const CIEC_ANY_UNSIGNED & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_UNSIGNED, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, static_cast<const CIEC_ANY_UNSIGNED_VARIANT::variant&>(*this));
}

int CIEC_ANY_UNSIGNED_VARIANT::fromString(const char *paValue) {
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

int CIEC_ANY_UNSIGNED_VARIANT::toString(char *paValue, size_t paBufferSize) const {
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

size_t CIEC_ANY_UNSIGNED_VARIANT::getToStringBufferSize() const {
  const CIEC_ANY &value = unwrap();
  const char *typeName = CStringDictionary::get(value.getTypeNameID());
  size_t typeNameLength = strlen(typeName);
  return typeNameLength + 1 + value.getToStringBufferSize();
}

