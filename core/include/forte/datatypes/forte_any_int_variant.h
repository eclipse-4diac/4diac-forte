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
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_lint.h"
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/datatypes/forte_usint.h"

namespace forte {
  using TIecAnyIntVariantType = std::variant<
      // ANY_ELEMENTARY
      //  ANY_MAGNITUDE
      //   ANY_NUM
      //    ANY_INTEGER
      //     ANY_SIGNED
      CIEC_SINT,
      CIEC_INT,
      CIEC_DINT,
      CIEC_LINT,
      //     ANY_UNSINED
      CIEC_USINT,
      CIEC_UINT,
      CIEC_UDINT,
      CIEC_ULINT
      // end
      >;

  class CIEC_ANY_INT_VARIANT : public CIEC_ANY_INT, public TIecAnyIntVariantType {
      DECLARE_FIRMWARE_DATATYPE(ANY_INT_VARIANT)
    public:
      using TIecAnyIntVariantType::variant;
      using TIecAnyIntVariantType::operator=;
      template<class>
      static inline constexpr bool always_false_v = false;

      CIEC_ANY_INT_VARIANT(const CIEC_ANY_INT_VARIANT &paVal) : CIEC_ANY_INT(), variant(paVal) {
      }

      CIEC_ANY_INT_VARIANT(const CIEC_ANY_INT &paVal) {
        setValue(paVal.unwrap());
      }

      CIEC_ANY_INT_VARIANT &operator=(const CIEC_ANY_INT_VARIANT &paOther) {
        variant::operator=(paOther);
        return *this;
      }

      CIEC_ANY_INT_VARIANT &operator=(const CIEC_ANY_INT &paOther) {
        setValue(paOther.unwrap());
        return *this;
      }

      void setValue(const CIEC_ANY &paValue) override;

      void reset() override {
        unwrap().reset();
      }

      bool setDefaultValue(EDataTypeID paDataTypeId);

      [[nodiscard]] CIEC_ANY_INT &unwrap() override;

      [[nodiscard]] const CIEC_ANY_INT &unwrap() const override;

      int fromString(const char *paValue) override;

      void toString(std::string &paTargetBuf) const override;

      [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
        return unwrap().equals(paOther.unwrap());
      }
  };

  static_assert(std::is_copy_constructible_v<CIEC_ANY_INT_VARIANT>);
  static_assert(std::is_move_constructible_v<CIEC_ANY_INT_VARIANT>);
  static_assert(std::is_constructible_v<CIEC_ANY_INT_VARIANT, const CIEC_ANY_INT &>);
  static_assert(std::is_copy_assignable_v<CIEC_ANY_INT_VARIANT>);
  static_assert(std::is_assignable_v<CIEC_ANY_INT_VARIANT, const CIEC_ANY_INT &>);
  static_assert(std::is_destructible_v<CIEC_ANY_INT_VARIANT>);

  template<>
  struct is_generic_datatype<CIEC_ANY_INT_VARIANT> : std::true_type {};
} // namespace forte
