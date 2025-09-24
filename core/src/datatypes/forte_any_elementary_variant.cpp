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
#include "forte/datatypes/forte_any_elementary_variant.h"

using namespace forte::literals;

namespace forte {
  DEFINE_FIRMWARE_DATATYPE(ANY_ELEMENTARY_VARIANT, "ANY_ELEMENTARY"_STRID)

  void CIEC_ANY_ELEMENTARY_VARIANT::setValue(const CIEC_ANY &paValue) {
    switch (paValue.getDataTypeID()) {
      case e_ANY: CIEC_ANY_ELEMENTARY_VARIANT::setValue(paValue.unwrap()); break;
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
      default: break;
    }
  }

  bool CIEC_ANY_ELEMENTARY_VARIANT::setDefaultValue(EDataTypeID paDataTypeId) {
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

  CIEC_ANY_ELEMENTARY &CIEC_ANY_ELEMENTARY_VARIANT::unwrap() {
    return std::visit(
        [](auto &&value) -> CIEC_ANY_ELEMENTARY & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_base_of_v<CIEC_ANY_ELEMENTARY, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<variant &>(*this));
  }

  const CIEC_ANY_ELEMENTARY &CIEC_ANY_ELEMENTARY_VARIANT::unwrap() const {
    return std::visit(
        [](auto &&value) -> const CIEC_ANY_ELEMENTARY & {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_base_of_v<CIEC_ANY_ELEMENTARY, T>) {
            return value;
          } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor");
          }
        },
        static_cast<const variant &>(*this));
  }

  int CIEC_ANY_ELEMENTARY_VARIANT::fromString(const char *paValue) {
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

  void CIEC_ANY_ELEMENTARY_VARIANT::toString(std::string &paTargetBuf) const {
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

  int CIEC_ANY_ELEMENTARY_VARIANT::compare(const CIEC_ANY_ELEMENTARY_VARIANT &paValue,
                                           const CIEC_ANY_ELEMENTARY_VARIANT &paOther) {
    return std::visit(
        [](auto &&value, auto &&other) -> int {
          using T = std::decay_t<decltype(value)>;
          using U = std::decay_t<decltype(other)>;
          using commonType = std::conditional_t<std::is_same_v<T, U>, T, typename mpl::get_castable_type<T, U>::type>;
          if constexpr (std::is_base_of_v<CIEC_ANY_STRING, commonType>) {
            if constexpr (std::is_same_v<T, U> && std::is_same_v<T, CIEC_STRING>) {
              return static_cast<CIEC_STRING>(value).compare(static_cast<CIEC_STRING>(other));
            } else if constexpr (std::is_same_v<T, U> && std::is_same_v<T, CIEC_WSTRING>) {
              return strcmp(static_cast<commonType>(value).getValue(), static_cast<commonType>(other).getValue());
            } else {
              DEVLOG_ERROR("Comparing incompatible types %s and %s\n", value.getTypeNameID().data(),
                           other.getTypeNameID().data());
              return -1;
            }
          } else if constexpr (!std::is_same_v<commonType, mpl::NullType>) {
            auto primitiveValue = static_cast<typename commonType::TValueType>(static_cast<commonType>(value));
            auto primitiveOther = static_cast<typename commonType::TValueType>(static_cast<commonType>(other));
            return (primitiveValue > primitiveOther) - (primitiveValue < primitiveOther);
          } else {
            DEVLOG_ERROR("Comparing incompatible types %s and %s\n", value.getTypeNameID().data(),
                         other.getTypeNameID().data());
            return -1;
          }
        },
        static_cast<const variant &>(paValue), static_cast<const variant &>(paOther));
  }
} // namespace forte
