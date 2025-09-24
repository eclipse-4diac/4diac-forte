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
#include "forte/datatypes/forte_any_bit_variant.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(ANY_BIT_VARIANT, "ANY_BIT"_STRID)

  void CIEC_ANY_BIT_VARIANT::setValue(const CIEC_ANY &paValue) {
    switch (paValue.getDataTypeID()) {
      case e_ANY: CIEC_ANY_BIT_VARIANT::setValue(paValue.unwrap()); break;
      case e_BOOL: operator=(static_cast<const CIEC_BOOL &>(paValue)); break;
      case e_BYTE: operator=(static_cast<const CIEC_BYTE &>(paValue)); break;
      case e_WORD: operator=(static_cast<const CIEC_WORD &>(paValue)); break;
      case e_DWORD: operator=(static_cast<const CIEC_DWORD &>(paValue)); break;
      case e_LWORD: operator=(static_cast<const CIEC_LWORD &>(paValue)); break;
      default: break;
    }
  }

  bool CIEC_ANY_BIT_VARIANT::setDefaultValue(EDataTypeID paDataTypeId) {
    switch (paDataTypeId) {
      case e_BOOL: operator=(false_BOOL); return true;
      case e_BYTE: operator=(CIEC_BYTE(0)); return true;
      case e_WORD: operator=(CIEC_WORD(0)); return true;
      case e_DWORD: operator=(CIEC_DWORD(0)); return true;
      case e_LWORD: operator=(CIEC_LWORD(0)); return true;
      default: break;
    }
    return false;
  }

  CIEC_ANY_BIT &CIEC_ANY_BIT_VARIANT::unwrap() {
    return std::visit(
        [](auto &&value) -> CIEC_ANY_BIT & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_base_of_v<CIEC_ANY_BIT, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<variant &>(*this));
  }

  const CIEC_ANY_BIT &CIEC_ANY_BIT_VARIANT::unwrap() const {
    return std::visit(
        [](auto &&value) -> const CIEC_ANY_BIT & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_base_of_v<CIEC_ANY_BIT, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<const variant &>(*this));
  }

  int CIEC_ANY_BIT_VARIANT::fromString(const char *paValue) {
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

  void CIEC_ANY_BIT_VARIANT::toString(std::string &paTargetBuf) const {
    const CIEC_ANY &value = unwrap();
    paTargetBuf += value.getTypeNameID();
    paTargetBuf += '#';
    value.toString(paTargetBuf);
  }
} // namespace forte
