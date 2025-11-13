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
#pragma once

#include <variant>

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_char.h"
#include "forte/datatypes/forte_wchar.h"

namespace forte {
  using TIecAnyCharVariantType = std::variant<
      // ANY_ELEMENTARY
      //  ANY_CHARS
      //   ANY_CHAR
      CIEC_CHAR,
      CIEC_WCHAR
      // end
      >;

  class CIEC_ANY_CHAR_VARIANT : public CIEC_ANY_CHAR, public TIecAnyCharVariantType {
      DECLARE_FIRMWARE_DATATYPE(ANY_CHAR_VARIANT)
    public:
      using TIecAnyCharVariantType::variant;
      using TIecAnyCharVariantType::operator=;
      template<class>
      static inline constexpr bool always_false_v = false;

      CIEC_ANY_CHAR_VARIANT(const CIEC_ANY_CHAR_VARIANT &paVal) : CIEC_ANY_CHAR(), variant(paVal) {
      }

      CIEC_ANY_CHAR_VARIANT(const CIEC_ANY_CHAR &paVal) {
        setValue(paVal.unwrap());
      }

      CIEC_ANY_CHAR_VARIANT &operator=(const CIEC_ANY_CHAR_VARIANT &paOther) {
        variant::operator=(paOther);
        return *this;
      }

      CIEC_ANY_CHAR_VARIANT &operator=(const CIEC_ANY_CHAR &paOther) {
        setValue(paOther.unwrap());
        return *this;
      }

      void setValue(const CIEC_ANY &paValue) override;

      void reset() override {
        unwrap().reset();
      }

      bool setDefaultValue(EDataTypeID paDataTypeId);

      [[nodiscard]] CIEC_ANY_CHAR &unwrap() override;

      [[nodiscard]] const CIEC_ANY_CHAR &unwrap() const override;

      int fromString(const char *paValue) override;

      void toString(std::string &paTargetBuf) const override;

      [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
        return unwrap().equals(paOther.unwrap());
      }
  };

  static_assert(std::is_copy_constructible_v<CIEC_ANY_CHAR_VARIANT>);
  static_assert(std::is_move_constructible_v<CIEC_ANY_CHAR_VARIANT>);
  static_assert(std::is_constructible_v<CIEC_ANY_CHAR_VARIANT, const CIEC_ANY_CHAR &>);
  static_assert(std::is_copy_assignable_v<CIEC_ANY_CHAR_VARIANT>);
  static_assert(std::is_assignable_v<CIEC_ANY_CHAR_VARIANT, const CIEC_ANY_CHAR &>);
  static_assert(std::is_destructible_v<CIEC_ANY_CHAR_VARIANT>);

  template<>
  struct is_generic_datatype<CIEC_ANY_CHAR_VARIANT> : std::true_type {};
} // namespace forte
