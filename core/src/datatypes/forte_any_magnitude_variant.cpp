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
#include "forte/datatypes/forte_any_magnitude_variant.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(ANY_MAGNITUDE_VARIANT, "ANY_MAGNITUDE"_STRID)

  void CIEC_ANY_MAGNITUDE_VARIANT::setValue(const CIEC_ANY &paValue) {
    switch (paValue.getDataTypeID()) {
      case e_ANY: CIEC_ANY_MAGNITUDE_VARIANT::setValue(paValue.unwrap()); break;
      case e_SINT: operator=(static_cast<const CIEC_SINT &>(paValue)); break;
      case e_INT: operator=(static_cast<const CIEC_INT &>(paValue)); break;
      case e_DINT: operator=(static_cast<const CIEC_DINT &>(paValue)); break;
      case e_LINT: operator=(static_cast<const CIEC_LINT &>(paValue)); break;
      case e_USINT: operator=(static_cast<const CIEC_USINT &>(paValue)); break;
      case e_UINT: operator=(static_cast<const CIEC_UINT &>(paValue)); break;
      case e_UDINT: operator=(static_cast<const CIEC_UDINT &>(paValue)); break;
      case e_ULINT: operator=(static_cast<const CIEC_ULINT &>(paValue)); break;
      case e_TIME: operator=(static_cast<const CIEC_TIME &>(paValue)); break;
      case e_LTIME: operator=(static_cast<const CIEC_LTIME &>(paValue)); break;
      case e_REAL: operator=(static_cast<const CIEC_REAL &>(paValue)); break;
      case e_LREAL: operator=(static_cast<const CIEC_LREAL &>(paValue)); break;
      default: break;
    }
  }

  bool CIEC_ANY_MAGNITUDE_VARIANT::setDefaultValue(EDataTypeID paDataTypeId) {
    switch (paDataTypeId) {
      case e_SINT: operator=(CIEC_SINT(0)); return true;
      case e_INT: operator=(CIEC_INT(0)); return true;
      case e_DINT: operator=(CIEC_DINT(0)); return true;
      case e_LINT: operator=(CIEC_LINT(0)); return true;
      case e_USINT: operator=(CIEC_USINT(0)); return true;
      case e_UINT: operator=(CIEC_UINT(0)); return true;
      case e_UDINT: operator=(CIEC_UDINT(0)); return true;
      case e_ULINT: operator=(CIEC_ULINT(0)); return true;
      case e_TIME: operator=(CIEC_TIME(0)); return true;
      case e_LTIME: operator=(CIEC_LTIME(0)); return true;
      case e_REAL: operator=(CIEC_REAL(0.0f)); return true;
      case e_LREAL: operator=(CIEC_LREAL(0.0)); return true;
      default: break;
    }
    return false;
  }

  CIEC_ANY_MAGNITUDE &CIEC_ANY_MAGNITUDE_VARIANT::unwrap() {
    return std::visit(
        [](auto &&value) -> CIEC_ANY_MAGNITUDE & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_base_of_v<CIEC_ANY_MAGNITUDE, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<variant &>(*this));
  }

  const CIEC_ANY_MAGNITUDE &CIEC_ANY_MAGNITUDE_VARIANT::unwrap() const {
    return std::visit(
        [](auto &&value) -> const CIEC_ANY_MAGNITUDE & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_base_of_v<CIEC_ANY_MAGNITUDE, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<const variant &>(*this));
  }

  int CIEC_ANY_MAGNITUDE_VARIANT::fromString(const char *paValue) {
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

  void CIEC_ANY_MAGNITUDE_VARIANT::toString(std::string &paTargetBuf) const {
    const CIEC_ANY &value = unwrap();
    switch (value.getDataTypeID()) {
      case e_TIME:
      case e_LTIME: // prefix already included
        value.toString(paTargetBuf);
        break;
      default:
        paTargetBuf += value.getTypeNameID();
        paTargetBuf += '#';
        value.toString(paTargetBuf);
        break;
    }
  }
} // namespace forte
