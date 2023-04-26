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
#include "forte_any_date_variant.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP

#include "forte_any_date_variant_gen.cpp"

#endif

DEFINE_FIRMWARE_DATATYPE(ANY_DATE_VARIANT, g_nStringIdANY_DATE)

void CIEC_ANY_DATE_VARIANT::setValue(const CIEC_ANY &paValue) {
  switch (paValue.getDataTypeID()) {
    case e_ANY:
      CIEC_ANY_DATE_VARIANT::setValue(paValue.unwrap());
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
    case e_LDATE:
      operator=(static_cast<const CIEC_LDATE &>(paValue));
      break;
    case e_LTIME_OF_DAY:
      operator=(static_cast<const CIEC_LTIME_OF_DAY &>(paValue));
      break;
    case e_LDATE_AND_TIME:
      operator=(static_cast<const CIEC_LDATE_AND_TIME &>(paValue));
      break;
    default:
      break;
  }
}

bool CIEC_ANY_DATE_VARIANT::setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId) {
  switch (paDataTypeId) {
    case e_DATE:
      operator=(CIEC_DATE(0));
      return true;
    case e_TIME_OF_DAY:
      operator=(CIEC_TIME_OF_DAY(0));
      return true;
    case e_DATE_AND_TIME:
      operator=(CIEC_DATE_AND_TIME(0));
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
    default:
      break;
  }
  return false;
}

CIEC_ANY_DATE &CIEC_ANY_DATE_VARIANT::unwrap() {
  return std::visit([](auto &&value) -> CIEC_ANY_DATE & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_DATE, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, *this);;
}

const CIEC_ANY_DATE &CIEC_ANY_DATE_VARIANT::unwrap() const {
  return std::visit([](auto &&value) -> const CIEC_ANY_DATE & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_DATE, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, *this);;
}

int CIEC_ANY_DATE_VARIANT::fromString(const char *paValue) {
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

int CIEC_ANY_DATE_VARIANT::toString(char *paValue, size_t paBufferSize) const {
  const CIEC_ANY &value = unwrap();
  return value.toString(paValue, paBufferSize);
}

size_t CIEC_ANY_DATE_VARIANT::getToStringBufferSize() const {
  const CIEC_ANY &value = unwrap();
  return value.getToStringBufferSize();
}

