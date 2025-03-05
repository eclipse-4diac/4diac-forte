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
#include "forte_any_elementary_variant.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP

#include "forte_any_elementary_variant_gen.cpp"

#endif

DEFINE_FIRMWARE_DATATYPE(ANY_ELEMENTARY_VARIANT, g_nStringIdANY_ELEMENTARY)

void CIEC_ANY_ELEMENTARY_VARIANT::setValue(const CIEC_ANY &paValue) {
  switch (paValue.getDataTypeID()) {
    case e_ANY:
      CIEC_ANY_ELEMENTARY_VARIANT::setValue(paValue.unwrap());
      break;
    case e_BOOL:
      operator=(static_cast<const CIEC_BOOL &>(paValue));
      break;
    case e_SINT:
      operator=(static_cast<const CIEC_SINT &>(paValue));
      break;
    case e_INT:
      operator=(static_cast<const CIEC_INT &>(paValue));
      break;
    case e_DINT:
      operator=(static_cast<const CIEC_DINT &>(paValue));
      break;
    case e_LINT:
      operator=(static_cast<const CIEC_LINT &>(paValue));
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
    case e_DATE:
      operator=(static_cast<const CIEC_DATE &>(paValue));
      break;
    case e_TIME_OF_DAY:
      operator=(static_cast<const CIEC_TIME_OF_DAY &>(paValue));
      break;
    case e_DATE_AND_TIME:
      operator=(static_cast<const CIEC_DATE_AND_TIME &>(paValue));
      break;
    case e_TIME:
      operator=(static_cast<const CIEC_TIME &>(paValue));
      break;
    case e_CHAR:
      operator=(static_cast<const CIEC_CHAR &>(paValue));
      break;
    case e_WCHAR:
      operator=(static_cast<const CIEC_WCHAR &>(paValue));
      break;
    case e_LDATE:
      operator=(static_cast<const CIEC_LDATE &>(paValue));
      break;
    case e_LTIME_OF_DAY:
      operator=(static_cast<const CIEC_LTIME_OF_DAY &>(paValue));
      break;
    case e_LDATE_AND_TIME:
      operator=(static_cast<const CIEC_LDATE_AND_TIME &>(paValue));
      break;
    case e_LTIME:
      operator=(static_cast<const CIEC_LTIME &>(paValue));
      break;
    case e_REAL:
      operator=(static_cast<const CIEC_REAL &>(paValue));
      break;
    case e_LREAL:
      operator=(static_cast<const CIEC_LREAL &>(paValue));
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

bool CIEC_ANY_ELEMENTARY_VARIANT::setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId) {
  switch (paDataTypeId) {
    case e_BOOL:
      operator=(CIEC_BOOL(false));
      return true;
    case e_SINT:
      operator=(CIEC_SINT(0));
      return true;
    case e_INT:
      operator=(CIEC_INT(0));
      return true;
    case e_DINT:
      operator=(CIEC_DINT(0));
      return true;
    case e_LINT:
      operator=(CIEC_LINT(0));
      return true;
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
    case e_DATE:
      operator=(CIEC_DATE(0));
      return true;
    case e_TIME_OF_DAY:
      operator=(CIEC_TIME_OF_DAY(0));
      return true;
    case e_DATE_AND_TIME:
      operator=(CIEC_DATE_AND_TIME(0));
      return true;
    case e_TIME:
      operator=(CIEC_TIME(0));
      return true;
    case e_CHAR:
      operator=(CIEC_CHAR(0));
      return true;
    case e_WCHAR:
      operator=(CIEC_WCHAR(0));
      return true;
    case e_LDATE:
      operator=(CIEC_LDATE(0));
      return true;
    case e_LTIME_OF_DAY:
      operator=(CIEC_LTIME_OF_DAY(0));
      return true;
    case e_LDATE_AND_TIME:
      operator=(CIEC_LDATE_AND_TIME(0));
      return true;
    case e_LTIME:
      operator=(CIEC_LTIME(0));
      return true;
    case e_REAL:
      operator=(CIEC_REAL(0.0f));
      return true;
    case e_LREAL:
      operator=(CIEC_LREAL(0.0));
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

CIEC_ANY_ELEMENTARY &CIEC_ANY_ELEMENTARY_VARIANT::unwrap() {
  return std::visit([](auto &&value) -> CIEC_ANY_ELEMENTARY & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_ELEMENTARY, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, static_cast<CIEC_ANY_ELEMENTARY_VARIANT::variant&>(*this));
}

const CIEC_ANY_ELEMENTARY &CIEC_ANY_ELEMENTARY_VARIANT::unwrap() const {
  return std::visit([](auto &&value) -> const CIEC_ANY_ELEMENTARY & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_ELEMENTARY, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, static_cast<const CIEC_ANY_ELEMENTARY_VARIANT::variant&>(*this));
}

int CIEC_ANY_ELEMENTARY_VARIANT::fromString(const char *paValue) {
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

int CIEC_ANY_ELEMENTARY_VARIANT::toString(char *paValue, size_t paBufferSize) const {
  int result = -1;
  const CIEC_ANY &value = unwrap();
  switch(value.getDataTypeID()) {
    case e_DATE:
    case e_TIME_OF_DAY:
    case e_DATE_AND_TIME:
    case e_TIME:
    case e_LDATE:
    case e_LTIME_OF_DAY:
    case e_LDATE_AND_TIME:
    case e_LTIME:
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

size_t CIEC_ANY_ELEMENTARY_VARIANT::getToStringBufferSize() const {
  size_t result = 0;
  const CIEC_ANY &value = unwrap();
  switch(value.getDataTypeID()) {
    case e_DATE:
    case e_TIME_OF_DAY:
    case e_DATE_AND_TIME:
    case e_TIME:
    case e_LDATE:
    case e_LTIME_OF_DAY:
    case e_LDATE_AND_TIME:
    case e_LTIME:
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

int CIEC_ANY_ELEMENTARY_VARIANT::compare(const CIEC_ANY_ELEMENTARY_VARIANT &paValue,
                                         const CIEC_ANY_ELEMENTARY_VARIANT &paOther) {
  return std::visit([](auto &&value, auto &&other) -> int {
      using T = std::decay_t<decltype(value)>;
      using U = std::decay_t<decltype(other)>;
      using commonType = std::conditional_t<std::is_same_v<T, U>, T, typename forte::core::mpl::get_castable_type<T, U>::type>;
      if constexpr (std::is_base_of_v<CIEC_ANY_STRING, commonType>) {
        if constexpr (std::is_same_v<T,U> && std::is_same_v<T, CIEC_STRING>) {
          return static_cast<CIEC_STRING>(value).compare(static_cast<CIEC_STRING>(other));
        } else if constexpr (std::is_same_v<T,U> && std::is_same_v<T, CIEC_WSTRING>) {
          return strcmp(static_cast<commonType>(value).getValue(), static_cast<commonType>(other).getValue());
        } else {
          DEVLOG_ERROR("Comparing incompatible types %s and %s\n",
                     CStringDictionary::get(value.getTypeNameID()),
                     CStringDictionary::get(other.getTypeNameID()));
          return -1;
        }
      } else if constexpr (!std::is_same_v<commonType, forte::core::mpl::NullType>) {
        auto primitiveValue = static_cast<typename commonType::TValueType>(static_cast<commonType >(value));
        auto primitiveOther = static_cast<typename commonType::TValueType>(static_cast<commonType >(other));
        return (primitiveValue > primitiveOther) - (primitiveValue < primitiveOther);
      } else {
        DEVLOG_ERROR("Comparing incompatible types %s and %s\n",
                     CStringDictionary::get(value.getTypeNameID()),
                     CStringDictionary::get(other.getTypeNameID()));
        return -1;
      }
  }, static_cast<const CIEC_ANY_ELEMENTARY_VARIANT::variant&>(paValue), 
     static_cast<const CIEC_ANY_ELEMENTARY_VARIANT::variant&>(paOther));
}

