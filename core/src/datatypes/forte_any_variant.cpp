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
#include "forte/datatypes/forte_any_variant.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(ANY_VARIANT, "ANY"_STRID)

  void CIEC_ANY_VARIANT::setValue(const CIEC_ANY &paValue) {
    switch (paValue.getDataTypeID()) {
      case e_ANY: CIEC_ANY_VARIANT::setValue(paValue.unwrap()); break;
      case e_BOOL: operator=(static_cast<const CIEC_BOOL &>(paValue)); break;
      case e_SINT: operator=(static_cast<const CIEC_SINT &>(paValue)); break;
      case e_INT: operator=(static_cast<const CIEC_INT &>(paValue)); break;
      case e_DINT: operator=(static_cast<const CIEC_DINT &>(paValue)); break;
      case e_LINT: operator=(static_cast<const CIEC_LINT &>(paValue)); break;
      case e_USINT: operator=(static_cast<const CIEC_USINT &>(paValue)); break;
      case e_UINT: operator=(static_cast<const CIEC_UINT &>(paValue)); break;
      case e_UDINT: operator=(static_cast<const CIEC_UDINT &>(paValue)); break;
      case e_ULINT: operator=(static_cast<const CIEC_ULINT &>(paValue)); break;
      case e_BYTE: operator=(static_cast<const CIEC_BYTE &>(paValue)); break;
      case e_WORD: operator=(static_cast<const CIEC_WORD &>(paValue)); break;
      case e_DWORD: operator=(static_cast<const CIEC_DWORD &>(paValue)); break;
      case e_LWORD: operator=(static_cast<const CIEC_LWORD &>(paValue)); break;
      case e_DATE: operator=(static_cast<const CIEC_DATE &>(paValue)); break;
      case e_TIME_OF_DAY: operator=(static_cast<const CIEC_TIME_OF_DAY &>(paValue)); break;
      case e_DATE_AND_TIME: operator=(static_cast<const CIEC_DATE_AND_TIME &>(paValue)); break;
      case e_TIME: operator=(static_cast<const CIEC_TIME &>(paValue)); break;
      case e_CHAR: operator=(static_cast<const CIEC_CHAR &>(paValue)); break;
      case e_WCHAR: operator=(static_cast<const CIEC_WCHAR &>(paValue)); break;
      case e_LDATE: operator=(static_cast<const CIEC_LDATE &>(paValue)); break;
      case e_LTIME_OF_DAY: operator=(static_cast<const CIEC_LTIME_OF_DAY &>(paValue)); break;
      case e_LDATE_AND_TIME: operator=(static_cast<const CIEC_LDATE_AND_TIME &>(paValue)); break;
      case e_LTIME: operator=(static_cast<const CIEC_LTIME &>(paValue)); break;
      case e_REAL: operator=(static_cast<const CIEC_REAL &>(paValue)); break;
      case e_LREAL: operator=(static_cast<const CIEC_LREAL &>(paValue)); break;
      case e_STRING: operator=(static_cast<const CIEC_STRING &>(paValue)); break;
      case e_WSTRING: operator=(static_cast<const CIEC_WSTRING &>(paValue)); break;
      case e_ARRAY:
        operator=(CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>(static_cast<CIEC_ARRAY *>(paValue.clone(nullptr))));
        break;
      case e_STRUCT:
        operator=(CIEC_ANY_UNIQUE_PTR<CIEC_STRUCT>(static_cast<CIEC_STRUCT *>(paValue.clone(nullptr))));
        break;
      default: break;
    }
  }

  bool CIEC_ANY_VARIANT::setDefaultValue(EDataTypeID paDataTypeId) {
    switch (paDataTypeId) {
      case e_BOOL: operator=(false_BOOL); return true;
      case e_SINT: operator=(CIEC_SINT(0)); return true;
      case e_INT: operator=(CIEC_INT(0)); return true;
      case e_DINT: operator=(CIEC_DINT(0)); return true;
      case e_LINT: operator=(CIEC_LINT(0)); return true;
      case e_USINT: operator=(CIEC_USINT(0)); return true;
      case e_UINT: operator=(CIEC_UINT(0)); return true;
      case e_UDINT: operator=(CIEC_UDINT(0)); return true;
      case e_ULINT: operator=(CIEC_ULINT(0)); return true;
      case e_BYTE: operator=(CIEC_BYTE(0)); return true;
      case e_WORD: operator=(CIEC_WORD(0)); return true;
      case e_DWORD: operator=(CIEC_DWORD(0)); return true;
      case e_LWORD: operator=(CIEC_LWORD(0)); return true;
      case e_DATE: operator=(CIEC_DATE(0)); return true;
      case e_TIME_OF_DAY: operator=(CIEC_TIME_OF_DAY(0)); return true;
      case e_DATE_AND_TIME: operator=(CIEC_DATE_AND_TIME(0)); return true;
      case e_TIME: operator=(CIEC_TIME(0)); return true;
      case e_CHAR: operator=(CIEC_CHAR(0)); return true;
      case e_WCHAR: operator=(CIEC_WCHAR(0)); return true;
      case e_LDATE: operator=(CIEC_LDATE(0)); return true;
      case e_LTIME_OF_DAY: operator=(CIEC_LTIME_OF_DAY(0)); return true;
      case e_LDATE_AND_TIME: operator=(CIEC_LDATE_AND_TIME(0)); return true;
      case e_LTIME: operator=(CIEC_LTIME(0)); return true;
      case e_REAL: operator=(CIEC_REAL(0.0f)); return true;
      case e_LREAL: operator=(CIEC_LREAL(0.0)); return true;
      case e_STRING: operator=(CIEC_STRING("", 0)); return true;
      case e_WSTRING: operator=(CIEC_WSTRING("")); return true;
      default: break;
    }
    return false;
  }

  CIEC_ANY &CIEC_ANY_VARIANT::unwrap() {
    return std::visit(
        [](auto &&value) -> CIEC_ANY & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_same_v<T, CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>>) {
            return *value;
          } else if constexpr (std::is_same_v<T, CIEC_ANY_UNIQUE_PTR<CIEC_STRUCT>>) {
            return *value;
          } else if constexpr (std::is_base_of_v<CIEC_ANY, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<variant &>(*this));
  }

  const CIEC_ANY &CIEC_ANY_VARIANT::unwrap() const {
    return std::visit(
        [](auto &&value) -> const CIEC_ANY & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_same_v<T, CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>>) {
            return *value;
          } else if constexpr (std::is_same_v<T, CIEC_ANY_UNIQUE_PTR<CIEC_STRUCT>>) {
            return *value;
          } else if constexpr (std::is_base_of_v<CIEC_ANY, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<const variant &>(*this));
  }

  int CIEC_ANY_VARIANT::fromString(const char *paValue) {
    int retVal = -1;
    const char *hashPos = strchr(paValue, '#');
    if (nullptr != hashPos) {
      StringId typeNameId = parseTypeName(paValue, hashPos);
      EDataTypeID dataTypeId = CIEC_ANY_ELEMENTARY::getElementaryDataTypeId(typeNameId);
      if (setDefaultValue(dataTypeId)) {
        CIEC_ANY &value = unwrap();
        retVal = value.fromString(paValue);
      } else {
        CIEC_ANY *value = createDataTypeInstance(typeNameId, nullptr);
        if (value) {
          retVal = value->fromString(hashPos + 1); // start after '#'
          if (retVal < 0) {
            return retVal;
          }
          retVal += static_cast<int>(hashPos - paValue + 1); // add count for type including '#'
          switch (value->getDataTypeID()) {
            case e_ARRAY: operator=(CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>(static_cast<CIEC_ARRAY *>(value))); break;
            case e_STRUCT: operator=(CIEC_ANY_UNIQUE_PTR<CIEC_STRUCT>(static_cast<CIEC_STRUCT *>(value))); break;
            default: setValue(*value); delete value;
          }
        }
      }
    }
    return retVal;
  }

  void CIEC_ANY_VARIANT::toString(std::string &paTargetBuf) const {
    const CIEC_ANY &value = unwrap();
    switch (value.getDataTypeID()) {
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
