/*******************************************************************************
 * Copyright (c) 2023, 2025 Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Erich Jobst - initial implementation
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include "forte/datatypes/forte_any_unsigned_variant.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(ANY_UNSIGNED_VARIANT, "ANY_UNSIGNED"_STRID)

  void CIEC_ANY_UNSIGNED_VARIANT::setValue(const CIEC_ANY &paValue) {
    switch (paValue.getDataTypeID()) {
      case e_ANY: CIEC_ANY_UNSIGNED_VARIANT::setValue(paValue.unwrap()); break;
      case e_USINT: operator=(static_cast<const CIEC_USINT &>(paValue)); break;
      case e_UINT: operator=(static_cast<const CIEC_UINT &>(paValue)); break;
      case e_UDINT: operator=(static_cast<const CIEC_UDINT &>(paValue)); break;
      case e_ULINT: operator=(static_cast<const CIEC_ULINT &>(paValue)); break;
      default: break;
    }
  }

  bool CIEC_ANY_UNSIGNED_VARIANT::setDefaultValue(EDataTypeID paDataTypeId) {
    switch (paDataTypeId) {
      case e_USINT: operator=(CIEC_USINT(0)); return true;
      case e_UINT: operator=(CIEC_UINT(0)); return true;
      case e_UDINT: operator=(CIEC_UDINT(0)); return true;
      case e_ULINT: operator=(CIEC_ULINT(0)); return true;
      default: break;
    }
    return false;
  }

  CIEC_ANY_UNSIGNED &CIEC_ANY_UNSIGNED_VARIANT::unwrap() {
    return std::visit(
        [](auto &&value) -> CIEC_ANY_UNSIGNED & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_base_of_v<CIEC_ANY_UNSIGNED, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<variant &>(*this));
  }

  const CIEC_ANY_UNSIGNED &CIEC_ANY_UNSIGNED_VARIANT::unwrap() const {
    return std::visit(
        [](auto &&value) -> const CIEC_ANY_UNSIGNED & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_base_of_v<CIEC_ANY_UNSIGNED, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<const variant &>(*this));
  }

  int CIEC_ANY_UNSIGNED_VARIANT::fromString(const char *paValue) {
    int nRetVal = -1;
    const char *hashPos = strchr(paValue, '#');
    if (nullptr != hashPos) {
      StringId typeNameId = parseTypeName(paValue, hashPos);
      EDataTypeID dataTypeId = getElementaryDataTypeId(typeNameId);
      if (setDefaultValue(dataTypeId)) {
        CIEC_ANY &value = unwrap();
        nRetVal = value.fromString(paValue);
      }
    }
    return nRetVal;
  }

  void CIEC_ANY_UNSIGNED_VARIANT::toString(std::string &paTargetBuf) const {
    const CIEC_ANY &value = unwrap();
    paTargetBuf += value.getTypeNameID();
    paTargetBuf += '#';
    value.toString(paTargetBuf);
  }
} // namespace forte
