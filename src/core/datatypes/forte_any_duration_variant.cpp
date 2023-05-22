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
#include "forte_any_duration_variant.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP

#include "forte_any_duration_variant_gen.cpp"

#endif

DEFINE_FIRMWARE_DATATYPE(ANY_DURATION_VARIANT, g_nStringIdANY_DURATION)

void CIEC_ANY_DURATION_VARIANT::setValue(const CIEC_ANY &paValue) {
  switch (paValue.getDataTypeID()) {
    case e_ANY:
      CIEC_ANY_DURATION_VARIANT::setValue(paValue.unwrap());
      break;
    case e_TIME:
      operator=(static_cast<const CIEC_TIME &>(paValue));
      break;
    case e_LTIME:
      operator=(static_cast<const CIEC_LTIME &>(paValue));
      break;
    default:
      break;
  }
}

bool CIEC_ANY_DURATION_VARIANT::setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId) {
  switch (paDataTypeId) {
    case e_TIME:
      operator=(CIEC_TIME(0));
      return true;
    case e_LTIME:
      operator=(CIEC_LTIME(0));
      return true;
    default:
      break;
  }
  return false;
}

CIEC_ANY_DURATION &CIEC_ANY_DURATION_VARIANT::unwrap() {
  return std::visit([](auto &&value) -> CIEC_ANY_DURATION & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_DURATION, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, *this);;
}

const CIEC_ANY_DURATION &CIEC_ANY_DURATION_VARIANT::unwrap() const {
  return std::visit([](auto &&value) -> const CIEC_ANY_DURATION & {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_base_of_v<CIEC_ANY_DURATION, T>) {
        return value;
      } else {
        static_assert(always_false_v < T > , "non-exhaustive visitor");
      }
  }, *this);;
}

int CIEC_ANY_DURATION_VARIANT::fromString(const char *paValue) {
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

int CIEC_ANY_DURATION_VARIANT::toString(char *paValue, size_t paBufferSize) const {
  const CIEC_ANY &value = unwrap();
  return value.toString(paValue, paBufferSize);
}

size_t CIEC_ANY_DURATION_VARIANT::getToStringBufferSize() const {
  const CIEC_ANY &value = unwrap();
  return value.getToStringBufferSize();
}

