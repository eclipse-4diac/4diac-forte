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
#include "forte_any_chars_variant.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP

#include "forte_any_chars_variant_gen.cpp"

#endif

DEFINE_FIRMWARE_DATATYPE(ANY_CHARS_VARIANT, g_nStringIdANY_CHARS)

void CIEC_ANY_CHARS_VARIANT::setValue(const CIEC_ANY &paValue) {
  switch (paValue.getDataTypeID()) {
    case e_ANY:
      CIEC_ANY_CHARS_VARIANT::setValue(paValue.unwrap());
      break;
    case e_CHAR:
      operator=(static_cast<const CIEC_CHAR &>(paValue));
      break;
    case e_WCHAR:
      operator=(static_cast<const CIEC_WCHAR &>(paValue));
      break;
    case e_STRING:
      operator=(static_cast<const CIEC_STRING &>(paValue));
      break;
    case e_WSTRING:
      operator=(static_cast<const CIEC_WSTRING &>(paValue));
      break;
    default:
      break;
  }
}

bool CIEC_ANY_CHARS_VARIANT::setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId) {
  switch (paDataTypeId) {
    case e_CHAR:
      operator=(CIEC_CHAR(0));
      return true;
    case e_WCHAR:
      operator=(CIEC_WCHAR(0));
      return true;
    case e_STRING:
      operator=(CIEC_STRING("", 0));
      return true;
    case e_WSTRING:
      operator=(CIEC_WSTRING(""));
      return true;
    default:
      break;
  }
  return false;
}

CIEC_ANY_CHARS &CIEC_ANY_CHARS_VARIANT::unwrap() {
  return std::visit([](auto &&value) -> CIEC_ANY_CHARS & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_CHARS, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, static_cast<CIEC_ANY_CHARS_VARIANT::variant&>(*this));
}

const CIEC_ANY_CHARS &CIEC_ANY_CHARS_VARIANT::unwrap() const {
  return std::visit([](auto &&value) -> const CIEC_ANY_CHARS & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_CHARS, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, static_cast<const CIEC_ANY_CHARS_VARIANT::variant&>(*this));
}

int CIEC_ANY_CHARS_VARIANT::fromString(const char *paValue) {
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

int CIEC_ANY_CHARS_VARIANT::toString(char *paValue, size_t paBufferSize) const {
  int result = -1;
  const CIEC_ANY &value = unwrap();
  switch(value.getDataTypeID()) {
    case e_CHAR:
    case e_WCHAR: // prefix already included
      result = value.toString(paValue, paBufferSize);
      break;
    default:
      const char *typeName = CStringDictionary::get(value.getTypeNameID());
      size_t typeNameLength = strlen(typeName);
      if (paBufferSize > typeNameLength + 2) {
        memcpy(paValue, typeName, typeNameLength);
        paValue[typeNameLength] = '#';
        result = static_cast<int>(typeNameLength) + 1 +
                 value.toString(paValue + typeNameLength + 1, paBufferSize - typeNameLength - 1);
      }
      break;
  }
  return result;
}

size_t CIEC_ANY_CHARS_VARIANT::getToStringBufferSize() const {
  size_t result = 0;
  const CIEC_ANY &value = unwrap();
  switch(value.getDataTypeID()) {
    case e_CHAR:
    case e_WCHAR: // prefix already included
      result = value.getToStringBufferSize();
      break;
    default:
      const char *typeName = CStringDictionary::get(value.getTypeNameID());
      size_t typeNameLength = strlen(typeName);
      result = typeNameLength + 1 + value.getToStringBufferSize();
      break;
  }
  return result;
}

