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
#include "forte/datatypes/forte_any_char_variant.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(ANY_CHAR_VARIANT, "ANY_CHAR"_STRID)

  void CIEC_ANY_CHAR_VARIANT::setValue(const CIEC_ANY &paValue) {
    switch (paValue.getDataTypeID()) {
      case e_ANY: CIEC_ANY_CHAR_VARIANT::setValue(paValue.unwrap()); break;
      case e_CHAR: operator=(static_cast<const CIEC_CHAR &>(paValue)); break;
      case e_WCHAR: operator=(static_cast<const CIEC_WCHAR &>(paValue)); break;
      default: break;
    }
  }

  bool CIEC_ANY_CHAR_VARIANT::setDefaultValue(EDataTypeID paDataTypeId) {
    switch (paDataTypeId) {
      case e_CHAR: operator=(CIEC_CHAR(0)); return true;
      case e_WCHAR: operator=(CIEC_WCHAR(0)); return true;
      default: break;
    }
    return false;
  }

  CIEC_ANY_CHAR &CIEC_ANY_CHAR_VARIANT::unwrap() {
    return std::visit(
        [](auto &&value) -> CIEC_ANY_CHAR & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_base_of_v<CIEC_ANY_CHAR, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<variant &>(*this));
  }

  const CIEC_ANY_CHAR &CIEC_ANY_CHAR_VARIANT::unwrap() const {
    return std::visit(
        [](auto &&value) -> const CIEC_ANY_CHAR & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_base_of_v<CIEC_ANY_CHAR, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<const variant &>(*this));
  }

  int CIEC_ANY_CHAR_VARIANT::fromString(const char *paValue) {
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

  void CIEC_ANY_CHAR_VARIANT::toString(std::string &paTargetBuf) const {
    unwrap().toString(paTargetBuf);
  }
} // namespace forte
